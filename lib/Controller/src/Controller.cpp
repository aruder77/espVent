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



Controller::Controller()
{

  static Controller* controller = this;

  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);

  // create all modules 
  modules.addModule(VentilationController::getInstance());

  Homie_setFirmware("espVent", "1.0.0");
  Homie_setBrand("espVent");  
  Homie.setLoopFunction([]() { controller->workLoop(); } );  
  Homie.setSetupFunction([]() { controller->setup(); });

  Homie.setup();  
}

Controller::~Controller() {
}

void Controller::setup() {
  for (int i = 0; i < modules.count(); i++) {
    modules.getAt(i)->setup();
  }

  for (int i = 0; i < modules.count(); i++) {
    modules.getAt(i)->afterSetup();
  }
}


void Controller::loop()
{
  Homie.loop();
}

void Controller::workLoop() {
  unsigned long currentMillis = millis();

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

      loopCounter++;
  }    
}
