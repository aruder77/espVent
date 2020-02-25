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

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <Arduino.h>
#include <ArduinoLog.h>
#include <DisplayControl.h>
#include <Module.h>
#include <Homie.h>


class Modules {
    private:
        Module *modules[10];
        uint8_t length = 0;
    
    public:
        void addModule(Module *module);
        Module *getAt(uint8_t index);
        uint8_t count();

        virtual void setup();
        virtual void afterSetup();
        virtual void loop();
        virtual void every10Milliseconds();
        virtual void every50Milliseconds();
        virtual void every100Milliseconds();
        virtual void everySecond();
};

class Controller {
    public:
        virtual ~Controller();

        static Controller* getInstance();           

        void setup();
        void loop();
        void workLoop();

    private:
        static const int BAUD_RATE = 115200;
        static const int LOOP_INTERVAL_IN_MS = 10;

        static Controller *instance;

        Controller();

        DisplayControl* displayControl;

        Modules modules;

        long timer = 0;
        int loopCounter = 0;

};

#endif