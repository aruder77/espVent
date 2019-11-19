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

#ifndef LED_CONTROLLER_H_
#define LED_CONTROLLER_H_

#include <Arduino.h>
#include <Ticker.h>
#include <Prefs.h>
#include <ArduinoLog.h>

class LedController : public PrefsClient {
    public:
        static LedController *getInstance();

        void blink();
        void blinkFast();
        void blinkSlow();
        void on();
        void off();

        void configUpdate(const char *key, const char *value);

    private:
        static LedController *instance;

        LedController();

        Ticker ticker;
        Prefs *prefs = Prefs::getInstance();
        uint8_t statusLedPin = 2;

        static void toggle();
};


#endif