/*
 * MotorControl.cpp
 *
 *  Created on: 22 Jun 2017
 *      Author: axel
 */

#include "MotorController.h"




MotorController::MotorController(uint8_t pwmPin, uint8_t pwmChannel, bool inverseDirection) : pwmPin(pwmPin), pwmChannel(pwmChannel), inverseDirection(inverseDirection) {
	// initialize digital pin LED_BUILTIN as an output.
	pinMode(pwmPin, OUTPUT);

    ledcSetup(pwmChannel, 4000, 8);
    ledcAttachPin(pwmPin, pwmChannel);
}


MotorController::~MotorController() {
}


uint8_t MotorController::decreaseValue(uint8_t currentValue) {
	int i = 126;
	while (curve[i] > currentValue || i == 0) {
		i--;
	}
	return curve[i];
}

uint8_t MotorController::increaseValue(uint8_t currentValue) {
	int i = 0;
	while (curve[i] < currentValue || i == 126) {
		i++;
	}
	return curve[i];

}


void MotorController::adjust() {
	if (this->targetPwmSignal > this->currentPwmSignal) {
		uint8_t newValue = decreaseValue(this->currentPwmSignal);
		if (newValue < this->currentPwmSignal) {
			newValue = this->targetPwmSignal;
		} 
		setCurrentPwmSignal(newValue);
	} else if (this->targetPwmSignal < this->currentPwmSignal) {
		uint8_t newValue = increaseValue(this->currentPwmSignal);
		if (newValue > this->currentPwmSignal) {
			newValue = this->targetPwmSignal;
		}
		setCurrentPwmSignal(newValue);
	}
}

bool MotorController::isInverseDirection() const {
	return inverseDirection;
}

void MotorController::setInverseDirection(bool inverseDirection) {
	this->inverseDirection = inverseDirection;
}


uint8_t MotorController::getTargetSpeed() const {
	return this->targetSpeed;
}

void MotorController::setTargetSpeed(uint8_t targetSpeed) {
	this->targetSpeed = targetSpeed;
	this->targetPwmSignal = convertToPwmSignal(this->targetDirection, targetSpeed);
	char speedLog[20];
	char targetPwmLog[25];
	snprintf(speedLog, sizeof(speedLog), "new speed: %d\n", targetSpeed);
	Log.notice(speedLog);
	snprintf(targetPwmLog, sizeof(targetPwmLog), "new target pwm: %d\n", this->targetPwmSignal);
	Log.notice(targetPwmLog);
}

uint8_t MotorController::getCurrentSpeed() {
	return convertToSpeed(currentPwmSignal);
}

uint8_t MotorController::getCurrentPwmValue() {
	return currentPwmSignal;
}

bool MotorController::isTargetDirection() const {
	return this->targetDirection;
}

void MotorController::setTargetDirection(bool targetDirection) {
	this->targetDirection = targetDirection;
	this->targetPwmSignal = convertToPwmSignal(targetDirection, this->targetSpeed);
	Log.notice("changing direction\n");
	char targetPwmLog[25];
	snprintf(targetPwmLog, sizeof(targetPwmLog), "new target pwm: %d\n", this->targetPwmSignal);
	Log.notice(targetPwmLog);
}

bool MotorController::isCurrentDirection() {
	return isDirectionForward(this->currentPwmSignal);
}

bool MotorController::isFlowDirectionIn() {
	return this->inverseDirection ? this->isCurrentDirection() : !this->isCurrentDirection();
}

uint8_t MotorController::convertToPwmSignal(bool direction, uint8_t speed) {
	int pwmSpeed = 0;
	bool realDirection = direction;
	if (inverseDirection) {
		realDirection = !direction;
	}
	// calculate scaled speed (considering min and max speed) unless 0
	float scaledSpeed = 0;
	if (speed > 1) {
		scaledSpeed = MIN_SPEED + speed * (MAX_SPEED - MIN_SPEED) / 100.0;
	}

	if (realDirection) {
		pwmSpeed = (50 - (scaledSpeed / 2.0)) * 255.0 / 100.0;
	} else {
		pwmSpeed = (50 + (scaledSpeed / 2.0)) * 255.0 / 100.0;
	}

	// minimum
	if (pwmSpeed < 30) {
		pwmSpeed = 30;
	}
	// maximum
	if (pwmSpeed >= 224) {
		pwmSpeed = 255;
	}
	return pwmSpeed;
}

uint8_t MotorController::convertToSpeed(uint8_t pwmSignal) {
	uint8_t scaledSpeed = 0;
	if (isDirectionForward(pwmSignal)) {
		scaledSpeed = 2.0 * ( 50 - pwmSignal * 100.0 / 255.0);
	} else {
		scaledSpeed = 2.0 * ( pwmSignal * 100.0 / 255.0 - 50);
	}
	// convert from scaled speed (considering min and max speed) to percent speed (0-100)
	uint8_t speed = 0;
	if (scaledSpeed > 1) {
		speed = (scaledSpeed - MIN_SPEED) * 100.0 / (MAX_SPEED - MIN_SPEED);
	}
	return speed;
}

void MotorController::setCurrentPwmSignal(uint8_t currentPwmSignal) {
	this->currentPwmSignal = currentPwmSignal;
	char targetPwmLog[25];
	snprintf(targetPwmLog, sizeof(targetPwmLog), " new current pwm: %d\n", this->currentPwmSignal);
	Log.notice(targetPwmLog);
	ledcWrite(pwmChannel, this->currentPwmSignal);
}

bool MotorController::isDirectionForward(uint8_t pwmSignal) {
	bool directionForward = pwmSignal < 128;
	return inverseDirection ? !directionForward : directionForward;
}
