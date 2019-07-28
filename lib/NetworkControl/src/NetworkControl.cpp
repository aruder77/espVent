/*
  Copyright (C) 2019 Axel Ruder

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <NetworkControl.h>
#include <Prefs.h>

NetworkControl *NetworkControl::instance = 0;

bool NetworkControl::saveConfig = false;

void callback(char *topic, byte *payload, unsigned int length)
{
	char data[length + 1];
	for (unsigned int i = 0; i < length; i++)
	{
		data[i] = (char)payload[i];
	}
	data[length] = 0;
	char topicCopy[strlen(topic) + 1];
	strcpy(topicCopy, topic);
	NetworkControl::getInstance()->messageReceived(topicCopy, data);
}

NetworkControl *NetworkControl::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	if (!NetworkControl::instance)
	{
		NetworkControl::instance = new NetworkControl();
	}
	return NetworkControl::instance;
}

NetworkControl::NetworkControl() {
	Log.notice("Initializing NetworkControl\n");

	WiFiClient *espClient = new WiFiClient();
	mqttClient = new PubSubClient(*espClient);

	prefs = Prefs::getInstance();
	ledController = LedController::getInstance();

	// initialize prefs
	prefs->registerConfigParam("clientId", "Client-ID", "espVent", 100, this);
	prefs->registerConfigParam("wpaKey", "WPA-Key", "~CM3CDUx", 100, this);
	prefs->registerConfigParam("mqtt-server", "MQTT-Server", "192.168.178.27", 100, this);

	mqttClient->setCallback(callback);

	prefs->get("mqtt-server", mqtt_server);
	Log.notice("loaded mqtt-server from eeprom: %s\n", mqtt_server);
	mqttClient->setServer(mqtt_server, 1883);

	prefs->get("clientId", clientId);
	Log.notice("loaded clientId from eeprom: %s\n", clientId);
}

NetworkControl::~NetworkControl()
{
}

const char *NetworkControl::getName() {
	return "network";
}

uint8_t NetworkControl::waitForConnectResult() {
    unsigned long start = millis();
    boolean keepConnecting = true;
    uint8_t status;
    while (keepConnecting) {
      status = WiFi.status();
      if (millis() > start + WIFI_CONNECT_TIMEOUT) {
        keepConnecting = false;
      }
      if (status == WL_CONNECTED || status == WL_CONNECT_FAILED) {
        keepConnecting = false;
      }
      delay(100);
    }
    return status;
}


void NetworkControl::afterSetup() {

	ledController->blinkFast();
	WiFi.begin();
	if (waitForConnectResult() == WL_CONNECTED)
	{
		//keep LED on
		ledController->on();
	} else {
		enterConfigPortal();
	}

	// Allow the hardware to sort itself out
	delay(1500);
	Log.notice("finished initializing NetworkControl\n");
	reconnect();
}

void NetworkControl::subscribeTopic(const char *clientId) {
	char cmndTopic[100];
	sprintf(cmndTopic, "cmnd/%s/#", clientId);
	mqttClient->subscribe(cmndTopic);

	char configTopic[100];
	sprintf(configTopic, "config/%s/#", clientId);
	mqttClient->subscribe(configTopic);

	char statusTopic[100];
	sprintf(statusTopic, "tele/%s/hello", clientId);
	send(statusTopic, "Hello, world!");	
}

void NetworkControl::unsubscribeTopic(const char *clientId) {
	char cmndTopic[100];
	sprintf(cmndTopic, "cmnd/%s/#", clientId);
	mqttClient->unsubscribe(cmndTopic);

	char configTopic[100];
	sprintf(configTopic, "config/%s/#", clientId);
	mqttClient->unsubscribe(configTopic);

	char statusTopic[100];
	sprintf(statusTopic, "tele/%s/status", clientId);
	send(statusTopic, "Bye bye!");	
}

void NetworkControl::reconnect()
{
	// Loop until we're reconnected
	if (!mqttClient->connected())
	{
		Log.notice("Attempting MQTT connection...\n");
		// Attempt to connect
		if (mqttClient->connect(clientId))	
		{
			Log.notice("connected\n");
			subscribeTopic(clientId);
		}
		else
		{
			Log.error("failed, rc=%d try again in 5 minutes\n", mqttClient->state());
		}
	}
}

void NetworkControl::loop()
{
	mqttClient->loop();
}

void NetworkControl::everySecond() {
	reconnect();
}

char *substr(const char *buff, uint8_t start, uint8_t len, char *substr)
{
	strncpy(substr, buff + start, len);
	substr[len] = 0;
	return substr;
}

void NetworkControl::messageReceived(const char *topic, const char *message)
{
	messageDispatcher->messageReceived(topic, message);
}

void NetworkControl::subscribeToCommand(const char *command, NetworkModule *networkModule)
{
	messageDispatcher->subscribeToCommand(command, networkModule);
}

void NetworkControl::send(const char *topic, const char *message)
{
	Log.notice("MQTT send topic %s: %s\n", topic, message);
	mqttClient->publish(topic, message);
}

void NetworkControl::sendTelemetry(const char *data) {
	char teleTopic[100] = {0};
	sprintf(teleTopic, "tele/%s/status", clientId);
	Log.notice("sending telemetry topic %s with data %s\n", teleTopic, data);
	send(teleTopic, data);
}

void NetworkControl::sendStat(const char *subTopic, const char *message)
{
	char statTopic[100] = {0};
	sprintf(statTopic, "stat/%s/%s", clientId, subTopic);
	Log.notice("MQTT send topic %s: %s\n", statTopic, message);
	mqttClient->publish(statTopic, message);
}

bool NetworkControl::exists()
{
	return instance != 0;
}

bool NetworkControl::isConnected()
{
	return mqttClient->connected();
}

//gets called when WiFiManager enters configuration mode
void NetworkControl::configModeCallback(WiFiManager *myWiFiManager)
{
	Log.notice("Entered config mode %s\n", WiFi.softAPIP().toString());
	//if you used auto generated SSID, print it
	Log.notice("%s\n", myWiFiManager->getConfigPortalSSID());
	//entered config mode, make led blink
	LedController::getInstance()->blink();
}

//gets called when WiFiManager saves config
void NetworkControl::saveConfigCallback() {
	saveConfig = true;
}


void NetworkControl::enterConfigPortal()
{
	WiFiManager wifiManager;

	Log.notice("entering config portal mode...\n");

		//set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
	wifiManager.setAPCallback(configModeCallback);
	wifiManager.setSaveConfigCallback(saveConfigCallback);

	PrefsItems *prefsItems = prefs->getPrefsItems();

	Log.notice("number of config items: %d\n", prefsItems->length);
	int wifiParamCount = prefsItems->length;
	WiFiManagerParameter **params = new WiFiManagerParameter*[wifiParamCount];

	for (int i = 0; i < wifiParamCount; i++)
	{
		Log.notice("adding config item %s\n", prefsItems->prefsItems[i]->id);
		char currentValue[50];
		prefs->get(prefsItems->prefsItems[i]->id, currentValue);
		params[i] = new WiFiManagerParameter(prefsItems->prefsItems[i]->id, prefsItems->prefsItems[i]->prompt, currentValue, prefsItems->prefsItems[i]->length);
		wifiManager.addParameter(params[i]);
	}

	ledController->blink();

	char wpaKey[100];
	prefs->get("wpaKey", wpaKey);
	if (!wifiManager.startConfigPortal(clientId, wpaKey))
	{
		Log.warning("failed to connect and hit timeout\n");
		delay(3000);
		//reset and try again, or maybe put it to deep sleep
		ESP.restart();
		delay(5000);
	}

	if (saveConfig) {
		saveConfig = false;
		for (int i = 0; i < wifiParamCount; i++) {
			prefs->set(params[i]->getID(), params[i]->getValue());
		}
	}

	char buffer[100];
	prefs->get("mqtt-server", buffer);
	mqttClient->setServer(buffer, 1883);

	Log.notice("saved mqtt-server to eeprom: %s\n", buffer);

	ESP.restart();
}

void NetworkControl::configUpdate(const char *id, const char *value) {
	Log.notice("NetworkControl: config update received: %s, %s\n", id, value);
	if (strcmp(id, "clientId") == 0) {
		unsubscribeTopic(clientId);
		strcpy(clientId, value);
		subscribeTopic(clientId);
	}
}

void NetworkControl::reset() {
	WiFi.disconnect(false, true);
}

