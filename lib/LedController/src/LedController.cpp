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

#include <LedController.h>

LedController *LedController::instance = 0;

LedController* LedController::getInstance()
{
    // The only instance
    // Guaranteed to be lazy initialized
    // Guaranteed that it will be destroyed correctly
	if (!LedController::instance) {
		LedController::instance = new LedController();
	}
	return LedController::instance;
}

LedController::LedController() {
    //set led pin as output
    pinMode(statusLedPin, OUTPUT);
}

void LedController::toggle()
{
    //toggle state
    int pin = LedController::getInstance()->statusLedPin;
    int state = digitalRead(pin); // get the current state of GPIO1 pin
    digitalWrite(pin, !state);    // set pin to the opposite state
}

void LedController::blink() {
    ticker.attach(0.6, toggle);    
}

void LedController::blinkFast() {
    ticker.attach(0.1, toggle);
}

void LedController::blinkSlow() {
    ticker.attach(1.5, toggle);
}

void LedController::on() {        
    ticker.detach();
    digitalWrite(statusLedPin, HIGH);  
}

void LedController::off() {
    ticker.detach();
    digitalWrite(statusLedPin, LOW);  
}

void LedController::configUpdate(const char *key, const char *value) {
    Log.notice("LedController config update: %s\n", key);
    statusLedPin = atoi(value);
}