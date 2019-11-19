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

The EspVent Firmware is based on my generic firmware for the Esp32 [espGeneric](https://github.com/aruder77/esp32Generic). 

### Installation instructions

- download and install [PlatformIO](https://platformio.org) to your computer (PC, Mac, Linux supported)
- clone this EspVent Git Repository to your computer
- change into the espVent working directory 
- execute
``pio run --target upload´´

### Usage

## Initial configuration

After successful upload, the esp will reboot and go into configuration portal mode. This is indicated by the flashing blue LED.
In this mode, the microcontroller will span a new WiFi network 'espVent'. 

- connecto to the WiFi with SSID 'espVent' using the password '~CM3CDUx'
- Open http://192.168.4.1 with your browser. A configuration page should appear.
- Click 'Configure WiFi'
- Select your Wifi network and enter your WPA2 key (first two fields)
- you can usually leave the next two fields as-is (2 and 15)
- if you want a different id for your controller than 'espVent', enter it in the field 'Client-ID'. This id will be used for all communication with the device.
- you can choose a different configuration portal WPA2 key in the next field. Replace '~CM3CDUx' with your key. Please make sure that it is a valid WPA2 key, there is no validation!
- Enter your MQTT server settings in the next three fields: hostname/IP-address, username, password.
- Click 'Save'

This is it. Your device should reboot and connect to your WiFi and MQTT server. The blue LED should be on. On successful connect, the controller sends a hello message to MQTT topic tele/<clientId>/hello. The client id is 'espVent' or whatever you entered instead during initial configuration.

## MQTT communication

## Button usage

Use the external button to trigger basic functions like

|single press|enter configuration portal|
|double press|reset(reboot) controller  |
|long press (> 10 seconds)|reset controller to firmware defaults|


### Over-the-air firmware updates

The firmware supports over-the-air updates. Just send the following command via MQTT:
topic: `cmnd/<client-id>/ota`
payload: `url:<url-to-firmware.bin>,md5:<md5-hash-of-firmware.bin`


## EspVent Hardware

The DIY hardware consists of 
* one power-distribution- and fan-connector-board
* one controller board which holds the NodeMCU Esp32 microcontroller board

Additionally, you need a Esp32 NodeMCU Dev board like [this one](https://www.amazon.de/gp/product/B07PG923ZK?th=1). Make sure that the distance between the two pin rows is 22.86mm, otherwise the board will not fit into the connectors on the controller board.

The other two-pin connector on the controller board may be used to add a button for things like reset, enter config mode, reset to defaults (see Button usage).

Optionally, you can add a 2.8" SPI TFT display like [this one](https://www.amazon.de/gp/product/B07DPN9LQV/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1 ). The controller board already has a connector for it but at the moment the firmware does not support it yet.


### Circuit Boards

The boards have been designed using EasyEDA, the sources are available at https://easyeda.com/axel.ruder/espvent. Feel free to clone and modify.

You can also find the Gerber files for the two boards so that you can order the PCBs online and PDFs if you want to etch the boards yourself in the download area.


### GPIO pin usage

For reference here is how the GPIO pins of the esp32 are used:

* LED: 2
* BUTTON: 15
* FANS
** FAN 1: 4
** FAN 2: 13
** FAN 3: 14
** FAN 4: 27
** FAN 5: 26
** FAN 6: 25
** FAN 7: 33
** FAN 8: 32
* TFT Display
** CS: 23
** RST: 22
** DC: 21
** MOSI: 19
** SCLK: 18
** BL: 5
** MISO: 17


