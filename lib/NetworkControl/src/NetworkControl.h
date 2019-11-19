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

#ifndef NETWORKCONTROL_H_
#define NETWORKCONTROL_H_

#include <SPI.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <ArduinoLog.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <NetworkModule.h>
#include <LedController.h>
#include <Prefs.h>
#include <MessageDispatcher.h>

#define RECONNECT_LOOP_COUNT 100
#define WIFI_CONNECT_TIMEOUT 2000


class NetworkControl : public Module, public PrefsClient {
public:
	virtual ~NetworkControl();
    static NetworkControl* getInstance();
    static bool exists();

	const char *getName();
	void afterSetup();
	void loop();
	void everySecond();	
	
	void messageReceived(const char *topic, const char *message);
	void send(const char *topic, const char *message);
	void sendTelemetry(const char *data);
	void sendStat(const char *subTopic, const char *message);
	bool isConnected();

	void subscribeToCommand(const char *command, NetworkModule *networkModule);

	void enterConfigPortal();
    void configUpdate(const char *id, const char *value);

	void reset();

private:
	static NetworkControl* instance;

	PubSubClient* mqttClient;

	Prefs* prefs;
	LedController* ledController;
	MessageDispatcher* messageDispatcher = new MessageDispatcher();

	int loop_counter = 0;
	char mqtt_server[100] = {0};
	char mqtt_user[100] = {0};
	char mqtt_password[100] = {0};
	char clientId[20] = {0};
	static bool saveConfig;

	NetworkControl();
	void reconnect();
	void subscribeTopic(const char *clientId);
	void unsubscribeTopic(const char *clientId);

	void addWifiManagerParameter();
	void configureConfigPortal(WiFiManager *wifiManager);
	uint8_t waitForConnectResult();

	static void configModeCallback(WiFiManager *myWiFiManager);
	static void saveConfigCallback();
};

#endif /* NETWORKCONTROL_H_ */
