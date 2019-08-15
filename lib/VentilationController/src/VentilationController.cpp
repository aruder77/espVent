/*
 * VentilationControl.cpp
 *
 *  Created on: 22 Jun 2017
 *      Author: axel
 */

#include "VentilationController.h"

VentilationController::VentilationController() {
	networkControl = NetworkControl::getInstance();
	prefs = Prefs::getInstance();

	// initialize motors
	motors[0] = new MotorController(32, 1, false);		// Flur EG
	motors[1] = new MotorController(33, 2, true);		// Wohnzimmer EG
	motors[2] = new MotorController(25, 3, false);		// Schlafzimmer
	motors[3] = new MotorController(26, 4, true);		// Flur OG
	motors[4] = new MotorController(27, 5, false);		// Kind 1
	motors[5] = new MotorController(14, 6, true);		// Kind 2
	motors[6] = new MotorController(12, 7, false);
	motors[7] = new MotorController(13, 8, true);

	// calculate directionChangeLoops
	directionChangeLoopCount = DIRECTION_CHANGE_INTERVAL * 1000.0 / 100;

	setPowerOn(true);
	setSpeed(20);

	networkControl->subscribeToCommand("speed", this);
	networkControl->subscribeToCommand("mode", this);
	networkControl->subscribeToCommand("direction", this);
}

VentilationController::~VentilationController() {
}

const char *VentilationController::getName() {
	return "VentilationController";
}

void VentilationController::every100Milliseconds() {
	// after DIRECTION_CHANGE_INTERVAL, change direction
	if (loopCounter == directionChangeLoopCount) {
		// only in mode 0, change direction
		if (mode == 0) {
			Log.notice("changing direction...");
			this->setDirection(!this->isDirection());
		}
		loopCounter = 0;
	}

	// adjust all motors speed and direction slowly in every loop
	for (int i = 0; i < MOTOR_COUNT; i++) {
		motors[i]->adjust();
	}

	// send status updates to mqtt
	if (powerOn != powerOnLast) {
		networkControl->sendStat("powerOn", powerOn ? "true" : "false");
		powerOnLast = powerOn;
	}
	if (speed != speedLast) {
		char speedStr[5];
		snprintf(speedStr, 5, "%d", speed);
		networkControl->sendStat("speed", speedStr);
		speedLast = speed;
	}
	if (mode != modeLast) {
		char modeStr[5];
		snprintf(modeStr, 5, "%d", mode);
		networkControl->sendStat("mode", modeStr);
		modeLast = mode;
	}
	if (direction != directionLast) {
		networkControl->sendStat("direction", direction ? "true" : "false");
		directionLast = direction; 
	}
	for (int i = 0; i < MOTOR_COUNT; i++) {
		uint8_t currentSpeed = motors[i]->getTargetSpeed();
		if (motorSpeedLast[i] != currentSpeed) {
			char networkStr[100];
			snprintf(networkStr, sizeof(networkStr), "motors/%d/speed", i);
			char speedStr[5];
			snprintf(speedStr, sizeof(speedStr), "%d", currentSpeed);
			networkControl->sendStat(networkStr, speedStr);
			motorSpeedLast[i] = currentSpeed;
		}
		uint8_t currentPwm = motors[i]->getCurrentPwmValue();
		if (motorPwmLast[i] != currentPwm) {
			char topicStr[100];
			snprintf(topicStr, sizeof(topicStr), "motors/%d/pwm", i);
			char currentPwmStr[5];
			snprintf(currentPwmStr, sizeof(currentPwmStr), "%d", currentPwm);
			networkControl->sendStat(topicStr, currentPwmStr);
			motorPwmLast[i] = currentPwm;
		}
		bool currentDirection = motors[i]->isTargetDirection();
		if (motorDirectionLast[i] != currentDirection) {
			char topicStr[100];
			snprintf(topicStr, sizeof(topicStr), "motors/%d/flowDirection", i);			
			networkControl->sendStat(topicStr, currentDirection ? "in" : "out");
			motorDirectionLast[i] = currentDirection;
		}
	}

	loopCounter++;
}

bool VentilationController::isDirection() const {
	return direction;
}

void VentilationController::setDirection(bool direction) {
	this->direction = direction;
	for (int i = 0; i < MOTOR_COUNT; i++) {
		motors[i]->setTargetDirection(direction);
	}
}

int VentilationController::getMode() const {
	return mode;
}

void VentilationController::setMode(int mode) {
	char logBuffer[50];
	snprintf(logBuffer, sizeof(logBuffer), "Mode: %d", mode);
	Log.notice(logBuffer);
	this->mode = mode;
}

uint8_t VentilationController::getSpeed() const {
	return speed;
}

void VentilationController::setSpeed(uint8_t speed) {
	this->speed = speed;
	for (int i = 0; i < MOTOR_COUNT; i++) {
		motors[i]->setTargetSpeed(speed);
	}
	if (speed == 0) {
		setPowerPin(false);
	} else if (!this->powerOn) {
		setPowerPin(true);
	}
}

void VentilationController::setSpeed(uint8_t motorNumber, uint8_t speed) {
	if (this->mode == 1) {
		// in mode 0, speed can only be adjusted for all motors
		setSpeed(speed);
	} else if (this->mode == 0) {
		motors[motorNumber]->setTargetSpeed(speed);
	}
}

bool VentilationController::isPowerOn() const {
	return powerOn;
}

void VentilationController::setPowerOn(bool powerOn) {
	if (powerOn) {
		setSpeed(speedBeforePowerOff);
	} else {
		speedBeforePowerOff = speed;
		setSpeed(0);
	}
}

void VentilationController::setPowerPin(bool powerOn) {
	this->powerOn = powerOn;
	if (powerOn) {
		digitalWrite(POWER_PIN, HIGH);
	} else {
		digitalWrite(POWER_PIN, LOW);
	}
	char logBuffer[20];
	snprintf(logBuffer, sizeof(logBuffer), "Power: %s", powerOn ? "true" : "false");
	Log.notice(logBuffer);
}

void VentilationController::commandReceived(const char *command, const char *payload) {
	Log.notice("received command %s", command);
	if (strcmp(command, "speed") == 0) {
		setSpeed(atoi(payload));
	} else if (strcmp(command, "mode") == 0) {
		setMode(atoi(payload));
	} else if (strcmp(command, "direction") == 0) {
		setDirection(strcmp(payload, "true"));
	} else if (strncasecmp(command, "motors/", 7)) {
		char motorNumberStr[2];
		substr(command, 7, 1, motorNumberStr);
		int motorNumber = atoi(motorNumberStr);
		if (strncmp("speed", command, strlen("speed")) == 0) {
			setSpeed(motorNumber, atoi(payload));
		}
 	}
}

void VentilationController::getTelemetryData(char *targetBuffer) {
}
