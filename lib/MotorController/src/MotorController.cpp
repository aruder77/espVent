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

void MotorController::setStartIndex(uint8_t currentValue) {
	int i = 0;
	while (curve[i] >= currentValue || i == 125) {
		i++;
	}
	currentIndex = i;
}


void MotorController::adjust() {
	if (this->targetPwmSignal > this->currentPwmSignal) {
		uint8_t newValue = curve[currentIndex--];
		if (newValue > this->targetPwmSignal) {
			newValue = this->targetPwmSignal;
		} 
		setCurrentPwmSignal(newValue);
	} else if (this->targetPwmSignal < this->currentPwmSignal) {
		uint8_t newValue = curve[currentIndex++];
		if (newValue < this->targetPwmSignal) {
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
	setStartIndex(this->currentPwmSignal);
	char speedLog[20];
	char targetPwmLog[25];
	snprintf(speedLog, sizeof(speedLog), "new speed: %d\n", targetSpeed);
	snprintf(targetPwmLog, sizeof(targetPwmLog), "new target pwm: %d\n", this->targetPwmSignal);
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
	char targetPwmLog[25];
	snprintf(targetPwmLog, sizeof(targetPwmLog), "new target pwm: %d\n", this->targetPwmSignal);
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

	uint8_t pwmDiff = speed/100.0f * PWM_RANGE;

	if (realDirection) {
		pwmSpeed = 125 - pwmDiff;
	} else {
		pwmSpeed = 125 + pwmDiff;
	}

	return pwmSpeed;
}

uint8_t MotorController::convertToSpeed(uint8_t pwmSignal) {
	return abs(pwmSignal - 125) / PWM_RANGE * 100;
}

void MotorController::setCurrentPwmSignal(uint8_t currentPwmSignal) {
	this->currentPwmSignal = currentPwmSignal;
	char targetPwmLog[25];
	snprintf(targetPwmLog, sizeof(targetPwmLog), " new current pwm: %d\n", this->currentPwmSignal);
	ledcWrite(pwmChannel, this->currentPwmSignal);
}

bool MotorController::isDirectionForward(uint8_t pwmSignal) {
	bool directionForward = pwmSignal < 128;
	return inverseDirection ? !directionForward : directionForward;
}
