/*
 * MotorControl.h
 *
 *  Created on: 22 Jun 2017
 *      Author: axel
 */

#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

#include <Arduino.h>
#include <ArduinoLog.h>
#include <Module.h>
#include <esp32-hal-ledc.h>


class MotorController {
public:
	MotorController(uint8_t, uint8_t, bool);
	virtual ~MotorController();

	uint8_t getTargetSpeed() const;
	void setTargetSpeed(uint8_t targetSpeed);

	uint8_t getCurrentSpeed();
	uint8_t getCurrentPwmValue();

	bool isTargetDirection() const;
	void setTargetDirection(bool direction);

	bool isCurrentDirection();

	bool isInverseDirection() const;
	void setInverseDirection(bool inverseDirection);

	bool isFlowDirectionIn();

	void adjust();

private:
	static const int DECREASE_PWM = 3;
	static const uint8_t MAX_SPEED = 77;
	static const uint8_t MIN_SPEED = 10;

	uint8_t pwmPin;
	uint8_t pwmChannel;
	bool inverseDirection;

	uint8_t targetSpeed = 20;
	bool targetDirection = true;

	uint8_t currentPwmSignal = 128;
	uint8_t targetPwmSignal = 0;

	void setCurrentPwmSignal(uint8_t currentPwmSignal);

	uint8_t convertToPwmSignal(bool direction, uint8_t speed);
	uint8_t convertToSpeed(uint8_t pwmSignal);
	bool isDirectionForward(uint8_t pwmSignal);
};

#endif /* MOTORCONTROL_H_ */
