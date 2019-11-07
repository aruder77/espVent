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


#include <Controller.h>

void progress(DlState state, int percent)
{
  Log.notice("state = %d - percent = %d\n", state, percent);
}

void error(char *message) {
  Log.error(message);
}

void startDl(void) {
}

void endDl(void) {
}

void click() {
  Log.notice("Klick!");
  Controller::getInstance()->buttonClick();
}

void doubleClick() {
  Log.notice("DoubleKlick!");
  Controller::getInstance()->buttonDoubleClick();
}

void longPressed() {
  Log.notice("LongPress!");
  Controller::getInstance()->buttonLongPressed();
}

void Modules::addModule(Module *module) {
  modules[length++] = module;
}

uint8_t Modules::count() {
  return length;
}

Module *Modules::getAt(uint8_t index) {
  return modules[index];
}

void Modules::setup() {
  for (int i = 0; i < length; i++) {
    modules[i]->setup();
  }  
};

void Modules::afterSetup() {
  for (int i = 0; i < length; i++) {
    modules[i]->afterSetup();
  }  
};

void Modules::loop() {
  for (int i = 0; i < length; i++) {
    modules[i]->loop();
  }  
};

void Modules::every10Milliseconds() {
  for (int i = 0; i < length; i++) {
    modules[i]->every10Milliseconds();
  }  
};

void Modules::every50Milliseconds() {
  for (int i = 0; i < length; i++) {
    modules[i]->every50Milliseconds();
  }  
};

void Modules::every100Milliseconds() {
  for (int i = 0; i < length; i++) {
    modules[i]->every100Milliseconds();
  }  
};

void Modules::everySecond() {
  for (int i = 0; i < length; i++) {
    modules[i]->everySecond();
  }  
};

void Modules::getTelemetryData(char *targetBuffer) {
  char telemetryBuffer[1024] = {0};
  strcpy(telemetryBuffer, "{");
  int currentLength = 1;
  for (int i = 0; i < length; i++) {
    strcpy(telemetryBuffer + currentLength, "\"");
    currentLength += 1;

    const char *moduleName = modules[i]->getName();
    strcpy(telemetryBuffer + currentLength, moduleName);
    currentLength += strlen(moduleName);

    strcpy(telemetryBuffer + currentLength, "\":");
    currentLength += 2;

    char moduleBuffer[100] = {0};
    modules[i]->getTelemetryData(moduleBuffer);
    strcpy(telemetryBuffer + currentLength, moduleBuffer);
    currentLength += strlen(moduleBuffer);

    if (i != length-1) {
      strcpy(telemetryBuffer + currentLength, ",");
      currentLength += 1;
    }
  }
  strcpy(telemetryBuffer + currentLength, "}");
  strcpy(targetBuffer, telemetryBuffer);
};

Controller *Controller::instance = 0;

Controller *Controller::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	if (!Controller::instance)
	{
		Controller::instance = new Controller();
	}
	return Controller::instance;
}



void Controller::commandReceived(const char *command, const char *payload) {
  if (strcmp(OTA_TOPIC, command) == 0) {
    memset(url, 0, 100);
    memset(md5_check, 0, 50);
    char *tmp = strstr(payload, "url:");
    char *tmp1 = strstr(payload, ",");
    if (!tmp || !tmp1) {
      Log.warning("unsupported payload format!\n");
      return;
    }
    memcpy(url, tmp + strlen("url:"), tmp1 - (tmp + strlen("url:")));

    char *tmp2 = strstr(payload, "md5:");
    int length = strlen(payload);
    memcpy(md5_check, tmp2 + strlen("md5:"), length - (tmp2 + strlen("md5:") - &payload[0]));

    Log.notice("started fota url: %s\n", url);
    Log.notice("started fota md5: %s\n", md5_check);
    state = Fota_e;
  }
}


Controller::Controller()
{
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  Log.begin(LOG_LEVEL_TRACE, &Serial);

  prefs = Prefs::getInstance();
  Log.notice("Controller Prefs %d\n", prefs);
  ledController = LedController::getInstance();

  // register prefs
  prefs->registerConfigParam("ConfigPortalPin", "Config-Portal Pin", "15", 3, this);

  // short press -> reset
  // long press 10 sec -> complete reset
  // double press -> config mode

  // create all modules 
  networkControl = NetworkControl::getInstance();
  displayControl = DisplayControl::getInstance();
  modules.addModule(networkControl);
  modules.addModule(displayControl);
  modules.addModule(new VentilationController());

  setup();
}

Controller::~Controller() {
}

void Controller::setup() {
  enterConfigPortalPin = prefs->getInt("ConfigPortalPin");
  /* oneButton = new OneButton(enterConfigPortalPin, true);
  oneButton->setPressTicks(LONG_PRESS_TIME);
  oneButton->attachClick(click);
  oneButton->attachDoubleClick(doubleClick);
  oneButton->attachLongPressStop(longPressed);*/
  
  networkControl->subscribeToCommand(OTA_TOPIC, this);

  for (int i = 0; i < modules.count(); i++) {
    modules.getAt(i)->setup();
  }

  for (int i = 0; i < modules.count(); i++) {
    modules.getAt(i)->afterSetup();
  }

}


void Controller::loop()
{
  unsigned long currentMillis = millis();

  //oneButton->tick();

  switch (state)
  {
  case Runnning_e:

    // is configuration portal requested?
    if ( wasClicked ) {
      wasClicked = false;
      Log.notice("Entering config portal...\n");
      networkControl->enterConfigPortal();

      //if you get here you have connected to the WiFi
      Log.notice("connected...yeey :)\n");
    } 

    // reset per double click requested
    if (wasDoubleClicked) {
      wasDoubleClicked = false;
      Log.notice("Resetting controller...\n");
      ESP.restart();
    }

    // reset completely as requested per long click
    if (wasLongClicked) {
      wasLongClicked = false;
      Log.notice("Resetting and erasing all configuration...\n");
      prefs->clear();
      networkControl->reset();
      delay(1000);
      ESP.restart();
    }

    modules.loop();

    if (currentMillis > timer) {
        timer = timer + LOOP_INTERVAL_IN_MS;
        modules.every10Milliseconds();

        // every 50 milliseconds
        if (loopCounter % 5 == 0) {
          modules.every50Milliseconds();
        } 

        // every 100 milliseconds
        if (loopCounter % 10 == 0) {
          modules.every100Milliseconds();
        }

        // every second
        if (loopCounter % 100 == 0) {
          modules.everySecond();
        }

        if (loopCounter % 1000 == 0) {
          char telemetryData[1024];
          modules.getTelemetryData(telemetryData);
          networkControl->sendTelemetry(telemetryData);
        }

        loopCounter++;
    }    

    break;

  case Fota_e:
    ledController->blinkSlow();

    DlInfo info;
    char urlStr[100];
    strcpy(urlStr, url);
    info.url = urlStr;
    char md5Str[50];
    strcpy(md5Str, md5_check);
    info.md5 = md5Str;
    info.startDownloadCallback = startDl;
    info.endDownloadCallback = endDl;
    info.progressCallback = progress;
    info.errorCallback = error;
    info.caCert = NULL;
    Log.notice("starting ota update!\n");
    Log.notice("URL: %s\n", urlStr);
    Log.notice("md5: %s\n", md5Str);
    networkControl->sendStat(OTA_TOPIC, "starting ota update!");
    httpFOTA.start(info);

    networkControl->sendStat(OTA_TOPIC, "ok");
    break;
  default:
    break;
  }
}

void Controller::buttonClick() {
  wasClicked = true;
}

void Controller::buttonDoubleClick() {
  wasDoubleClicked = true;
}

void Controller::buttonLongPressed() {
  wasLongClicked = true;
}

void Controller::configUpdate(const char *id, const char *value) {
    enterConfigPortalPin = atoi(value);
}

void Controller::getTelemetryData(char *targetBuffer) {
  sprintf(targetBuffer, "\"\"");
}