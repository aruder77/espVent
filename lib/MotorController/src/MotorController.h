/*
 * MotorControl.h
 *
 *  Created on: 22 Jun 2017
 *      Author: axel
 */

#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

#include <Arduino.h>
#include <Module.h>
#include <esp32-hal-ledc.h>
#include <Homie.h>


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
	static const uint8_t MAX_PWM = 220;
	static const uint8_t MIN_PWM = 30;
	static const uint8_t PWM_RANGE = (MAX_PWM - MIN_PWM) / 2;


	const uint8_t curve[126] { 255,253,253,252,252,252,251,249,247,243,238,233,228,223,218,213,208,203,199,195,192,189,186,184,181,179,177,176,174,173,172,170,168,167,165,164,162,161,160,159,158,158,157,157,156,156,155,154,153,152,151,150,150,149,148,148,147,146,145,145,144,143,142,141,140,140,139,138,138,137,136,135,134,134,133,133,132,131,130,130,129,128,127,127,126,125,124,123,122,121,120,119,118,117,116,114,113,112,110,108,107,105,101,98,95,90,86,82,77,73,69,64,59,54,48,42,36,28,20,14,8,4,2,1,0,0 };
	

	uint8_t pwmPin;
	uint8_t pwmChannel;
	bool inverseDirection;

	uint8_t targetSpeed = 20;
	bool targetDirection = true;

	uint8_t currentPwmSignal = 128;
	uint8_t targetPwmSignal = 0;

	uint8_t currentIndex = 0;

	void setCurrentPwmSignal(uint8_t currentPwmSignal);

	uint8_t convertToPwmSignal(bool direction, uint8_t speed);
	uint8_t convertToSpeed(uint8_t pwmSignal);
	bool isDirectionForward(uint8_t pwmSignal);

	uint8_t decreaseValue(uint8_t currentValue);
	uint8_t increaseValue(uint8_t currentValue);
	void setStartIndex(uint8_t currentValue);
};

#endif /* MOTORCONTROL_H_ */
