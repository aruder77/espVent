/*
 * VentilationControl.h
 *
 *  Created on: 22 Jun 2017
 *      Author: axel
 */

#ifndef VENTILATIONCONTROLLER_H_
#define VENTILATIONCONTROLLER_H_

#include <MotorController.h>
#include <Arduino.h>
#include <Module.h>
#include <Homie.h>

#define MOTOR_NUMBER = 2

class VentilationController : public Module {
public:
	static VentilationController* getInstance(); 	
	virtual ~VentilationController();

	virtual const char *getName();

	virtual void setup();
	void every100Milliseconds();

	bool isDirection() const;
	void setDirection(bool direction = true);
	void setDirection(uint8_t motorNumber, bool direction = true);

	int getMode() const;
	void setMode(int mode = 0);

	uint8_t getSpeed() const;
	void setSpeed(uint8_t speed = 0);
	void setSpeed(uint8_t motorNumber, uint8_t speed);

	bool isPowerOn() const;
	void setPowerOn(bool powerOn = true);

	void commandReceived(const char *command, const char *payload);
	void getTelemetryData(char *targetBuffer);

	void setCycleTime(int seconds);


private:
	static const uint8_t MOTOR_COUNT = 8;

	// digital pins for power
	static const uint8_t POWER_PIN = 9;

	static VentilationController *instance;
	VentilationController();

	HomieNode *memNode;

	HomieNode *motorsNode;
	HomieNode *motorNode[8];

  	HomieSetting<bool> *inverseMotor1;
  	HomieSetting<bool> *inverseMotor2;
  	HomieSetting<bool> *inverseMotor3;
  	HomieSetting<bool> *inverseMotor4;
  	HomieSetting<bool> *inverseMotor5;
  	HomieSetting<bool> *inverseMotor6;
  	HomieSetting<bool> *inverseMotor7;
  	HomieSetting<bool> *inverseMotor8;

	bool powerOn = true;
	uint8_t speed = 25;
	int mode = 0;
	bool direction = true;
	long directionChangeLoopCount = 0;
	int loopCounter = 0;
	int cycleTime = 75;

	// these are backups for status updates
	bool powerOnLast = powerOn;
	uint8_t speedLast = speed;
	int modeLast = mode;
	bool directionLast = direction;
	uint8_t motorSpeedLast[MOTOR_COUNT];
	uint8_t motorPwmLast[MOTOR_COUNT];
	bool motorDirectionLast[MOTOR_COUNT];

	uint32_t maxMillis = 0;
	uint32_t lastMillis = 0;

	// keep last speed before powerOff
	uint8_t speedBeforePowerOff = 25;

	MotorController* motors[MOTOR_COUNT];

	void setPowerPin(bool powerOn);

	char* substr(const char *buff, uint8_t start,uint8_t len, char* substr);
};

#endif /* VENTILATIONCONTROL_H_ */
