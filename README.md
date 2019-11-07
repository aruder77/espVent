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

- download and install PlatformIO Core to your computer (PC, Mac, Linux supported)
- clone this EspVent Git Repository to your computer
- change into the espVent working directory 
- execute
``pio run´´

### Usage

## EspVent Hardware

The hardware consists of 
* one power-distribution- and fan-connector-board
* one controller board which holds the NodeMCU Esp32 microcontroller board


PIN assignments.
* LED: 2
* BUTTON: 13
* ACTORS
** PUMP: 4
** VALVE_OPEN: 26
** VALVE_CLOSE: 27
* SENSORS
** FLOW_TEMP: 32
** RETURN_TEMP: 35
** OUTSIDE_TEMP: 34
* TFT
 ** TFT_MISO 25
 ** TFT_MOSI 23
 ** TFT_SCLK 19 -
 ** TFT_CS 22
 ** TFT_DC 21
 ** TFT_RST 18 -
 ** TFT_BL 5 -
 
