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

#include "DisplayControl.h"

DisplayControl *DisplayControl::instance = 0;

DisplayControl::~DisplayControl() {
}

DisplayControl *DisplayControl::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	if (!DisplayControl::instance)
	{
		DisplayControl::instance = new DisplayControl();
	}
	return DisplayControl::instance;
}

void DisplayControl::setup() { 
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK); // Note: the new fonts do not draw the background colour
  tft.drawString("Heatmaster 1.0", 0, 0, 2);
  tft.drawRightString("IP: 192.168.178.45", 320, 0, 2);

  tft.drawCentreString("Valve: 25 %", 160, 120 - tft.fontHeight(4) - 5, 4);
  tft.drawCentreString("Pump: ON", 160, 120 + 5, 4);

  tft.drawString("AF: 17.0° C", 0, 240 - tft.fontHeight(2), 2);
  tft.drawCentreString("VF: 39.0° C", 160, 240 - tft.fontHeight(2), 2);
  tft.drawRightString("RueF: 56.0° C", 320, 240 - tft.fontHeight(2), 2);

  //tft.drawCentreString("Updating firmware...", 160, 160, 2);
  //tft.drawRect(0, 180, 320, 10, TFT_WHITE);

  tft.setTextColor(TFT_RED, TFT_BLACK); // Note: the new fonts do not draw the background colour
  tft.drawCentreString("Configuration Mode", 160, 160, 2);
  tft.drawCentreString("SSID: heatmaster, http://192.168.4.1", 160, 180, 2);
}

const char *DisplayControl::getName() {
    return "DisplayControl";
}

void DisplayControl::displayFlowTemperature(double flowTemperature) {
    char buffer[100];
    sprintf(buffer, "VF: %.1f° C", flowTemperature);
    tft.drawCentreString(buffer, 160, 240 - tft.fontHeight(2), 2);
}

void DisplayControl::displayReturnTemperature(double returnTemperature) {
    char buffer[100];
    sprintf(buffer, "RueF: %.1f° C", returnTemperature);
    tft.drawRightString(buffer, 320, 240 - tft.fontHeight(2), 2);
}

void DisplayControl::displayOutsideTemperature(double outsideTemperature) {
    char buffer[100];
    sprintf(buffer, "AF: %.1f° C", outsideTemperature);
    tft.drawString(buffer, 0, 240 - tft.fontHeight(2), 2);  
}

void DisplayControl::displayValvePosition(int valvePosition) {
    char buffer[100];
    sprintf(buffer, "Ventil: %d %", valvePosition);
    tft.drawCentreString(buffer, 160, 120 - tft.fontHeight(4) - 5, 4);
}

void DisplayControl::displayPumpState(bool valveState) {
    if (valveState) {
      tft.drawCentreString("Pump: AN", 160, 120 + 5, 4);
    } else {
      tft.drawCentreString("Pump: AUS", 160, 120 + 5, 4);
    }
}

void DisplayControl::displayVersion(char *version) {
    char buffer[100];
    sprintf(buffer, "Heatmaster %s", version);
    tft.drawString(buffer, 0, 0, 2);
}

void DisplayControl::displayIPAddress(char *ipAddress) {
    char buffer[100];
    sprintf(buffer, "IP: %s", ipAddress);
    tft.drawRightString(buffer, 320, 0, 2);
}





