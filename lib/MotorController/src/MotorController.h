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
	static const int DECREASE_PWM = 1;
	static const uint8_t MAX_SPEED = 77;
	static const uint8_t MIN_SPEED = 10;

	const uint8_t curve[126] { 225,223,222,222,222,221,220,219,217,214,209,205,201,196,191,187,183,179,175,171,168,166,163,161,160,158,156,155,153,152,151,149,148,146,145,144,143,142,141,140,139,139,138,138,137,137,136,135,134,134,133,132,132,131,130,130,129,129,128,127,126,126,125,124,123,123,122,122,121,120,119,119,118,118,117,116,116,115,115,114,113,113,112,111,110,110,109,108,107,106,106,105,104,103,102,101,99,98,97,95,94,92,89,87,83,79,76,72,68,64,60,56,52,48,43,37,31,24,18,12,7,4,2,1,0,0 };

	

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

	uint8_t decreaseValue(uint8_t currentValue);
	uint8_t increaseValue(uint8_t currentValue);
};

#endif /* MOTORCONTROL_H_ */
