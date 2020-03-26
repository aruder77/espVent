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

#ifndef DISPLAYCONTROL_H_
#define DISPLAYCONTROL_H_

#include <SPI.h>
#include <Module.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <Homie.h>

class DisplayControl : public Module {

    public:
	    virtual ~DisplayControl();
        static DisplayControl* getInstance();        

        const char *getName();
        void setup();    

        void displayFlowTemperature(double flowTemperature);
        void displayReturnTemperature(double returnTemperature);
        void displayOutsideTemperature(double outsideTemperature);
        void displayValvePosition(int valvePosition);
        void displayPumpState(bool valveState);
        void displayVersion(char *version);
        void displayIPAddress(char *ipAddress);

    private:
	    static DisplayControl* instance;
        TFT_eSPI tft = TFT_eSPI(); 
};

#endif