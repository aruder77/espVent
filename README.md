# EspVent

EspVent is a project including a firmware for the Esp32 Microcontroller and an according hardware board in order to build a controller for a decentral ventilation system like my LIG Activar.
The original LIG Activar controller board only allows four discrete fan speed levels and has no means to connect to my home automation system. 

EspVent is a complete substitiution for the LIG Activar (or similar) controller board and has the following features:
* easy drop-in replacement of the original controller board
* very cheap hardware cost of around 15€
* supports up to 8 fans
* linear speed control from 0% to 100%
* individual speed control of every single fan
* completely controllable via WiFi and MQTT 
* supports over-the-air firmware updates
* Web-UI for easy initial configuration

## EspVent Firmware

The EspVent Firmware is based on the great framework homie-esp8266 https://github.com/homieiot/homie-esp8266. Documentation can be found at https://homieiot.github.io/homie-esp8266/docs/stable/ 

### Installation instructions

- download and install [PlatformIO](https://platformio.org) to your computer (PC, Mac, Linux supported)
- clone or download this EspVent Git Repository to your computer
- change into the espVent working directory 
- execute
```pio run```
- copy the file User_Setup.h to .pio/libdeps/TFT_eSPI_ID1559 (the last part may be different), overwriting the existing User_Setup.h. If you use a different SPI TFT display, adopt this file according to your display.
- connect your esp32 board via USB and execute 
```pio device list``` to list the USB serial ports
- find the USB serial port device for your board and adopt the monitor_port and upload_port setting in platformio.ini accordingly
- execute
```pio run --target upload```

### Usage

#### Initial configuration

The device needs to be configured so that it can connect to your WiFi network and MQTT server. The configuration is done by editing and uploading
the file data/homie/config.json. 

- set wifi.ssid and wifi.password
- set mqtt settings
- execute 
```pio run --target uploadfs``` to upload the file to the device


#### Ventilation modes

The controller supports three different modes for the ventilation system. The first two modes are also supported by the original LIG Activar controller, the third mode is only supported by espVent.

| Ventilation Mode | Description |
|------------------|-------------|
| 0                | Normal ventilation mode. The fans are blowing 75 seconds in one direction and then 75 seconds in the other direction. |
| 1                | "hot summer mode". Half of the fans are always blowing in, the other half are always blowing out. |
| 2                | All fans are blowing in. |
| 3                | Manual mode. Speed and direction can be set individually for each fan. |

Individual fan speed control is only supported in mode 0 for security reasons.

#### MQTT communication

The communication with the espVent controller follows the homie communication scheme.

| node       | property  | settable | datatype   | unit    | description                                  |
|------------|-----------|----------|------------|---------|----------------------------------------------|
| controller | freeHeap  | no       | integer    | byte    | the available heap space in bytes            |
| motors     | mode      | yes      | 0, 1, 2, 3 | enum    | the ventilation mode as described above      |
|            | speed     | yes      | 0..100     | percent | the ventilation speed for all motors         |
|            | direction | yes      | in, out    | enum    | the ventilation direction                    |
|            | cycleTime | yes      | integer    | seconds | the time between direction changes in mode 0 |
| motor0..7  | speed     | yes      | 0..100     | percent | the ventilation speed for motor x            |
| motor0..7  | direction | yes      | in, out    | enum    | the ventilation direction for motor x        |

The homie root topic is 'devices', the device name is 'espVent'.

The device reports its property values with the topic: 'devices/espVent/<node>/<property>'. 
The property can be set by posting the new value to the setter topic 'devices/espVent/<node>/<property>/set'.


#### Button usage

**Unfortunately the button does not work currently since the gpio port 15 is somehow used by something else.**

Use the external button to trigger basic functions like

|button press             |result                               |
|-------------------------|-------------------------------------|
|single press             |enter configuration portal           |
|double press             |reset(reboot) controller             |
|long press (> 10 seconds)|reset controller to firmware defaults|


#### Over-the-air firmware updates

The firmware supports over-the-air updates. You need the the ota_update.py script from homie-esp8266 to upload a new firmware version to the device over the air.

ota_updater.py -l <mqttServer> -u <mqttUsername> -d <mqttPassword> -t "devices/" -i "espVent" .pio/build/nodemcu-32s/firmware.bin

## EspVent Hardware

The hardware is very simple since the only real component is the ESP32 nodeMCU board. Everything else is just connectors and power distribution. The L7805CV provides the 5V for the ESP since the input voltage is 12V DC.

![alt text](https://github.com/aruder77/espVent/raw/master/images/assembled.jpeg "Assembled Controller")

The hardware consists of 
* one power-distribution- and fan-connector-board
  ![alt text](https://github.com/aruder77/espVent/raw/master/images/connectors.jpeg "Connectors board")

* and a controller board which sits on top and holds the NodeMCU Esp32 microcontroller board. It also has a connector for a button and a display.

  ![alt text](https://github.com/aruder77/espVent/raw/master/images/espBoard.jpeg "Controller board")


You need a Esp32 NodeMCU Dev board like [this one](https://www.amazon.de/gp/product/B07PG923ZK?th=1). Make sure that the distance between the two pin rows is 22.86mm, otherwise the board will not fit into the connectors on the controller board.

The other two-pin connector on the controller board may be used to add a button for things like reset, enter config mode, reset to defaults (see Button usage).

Optionally, you can add a 2.8" SPI TFT display like [this one](https://www.amazon.de/gp/product/B07DPN9LQV/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1 ). The controller board already has a connector for it but at the moment the firmware does not support it yet.


### Circuit Boards

The boards have been designed using EasyEDA, the sources are available at https://easyeda.com/axel.ruder/espvent. Feel free to clone and modify.

You can also find the Gerber files for the two boards so that you can order the PCBs online and PDFs if you want to etch the boards yourself in the pcb folder.


### GPIO pin usage

For reference here is how the GPIO pins of the esp32 are used:

* LED: 2
* BUTTON: 16 (15/16/34/35/36/39 possible)
* FANS
  * FAN 1: 4
  * FAN 2: 13
  * FAN 3: 14
  * FAN 4: 27
  * FAN 5: 26
  * FAN 6: 25
  * FAN 7: 33
  * FAN 8: 32
* TFT Display
  * CS: 23
  * RST: 22
  * DC: 21
  * MOSI: 19
  * SCLK: 18
  * BL: 5
  * MISO: 17


## Contributing

### Bug Reports & Feature Requests

Please use the [issue tracker](https://github.com/aruder77/espVent/issues) to report any bugs or file feature requests.

### Developing

PRs are welcome. We follow the typical "fork-and-pull" Git workflow.

 1. **Fork** the repo on GitHub
 2. **Clone** the project to your own machine
 3. **Commit** changes to your own branch
 4. **Push** your work back up to your fork
 5. Submit a **Pull Request** so that we can review your changes

**NOTE:** Be sure to merge the latest changes from "upstream" before making a pull request!


## Copyright

Copyright © 2019 Axel Ruder



## License 

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

GNU GENERAL PUBLIC LICENSE  
Version 3, 29 June 2007

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

### Homie-esp8266 License

The software is based on the homie-esp8266 license which has a MIT license:

The MIT License (MIT)

Copyright (c) 2015 Marvin Roger

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.