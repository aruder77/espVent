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

private:
	static const uint8_t MOTOR_COUNT = 8;

	// digital pins for power
	static const uint8_t POWER_PIN = 9;

	// time interval between direction changes in mode 0 in seconds
	static const int DIRECTION_CHANGE_INTERVAL = 75;

	static VentilationController *instance;
	VentilationController();

	HomieNode *motorsNode;
	HomieNode *motorNode[8];

	bool powerOn = true;
	uint8_t speed = 25;
	int mode = 0;
	bool direction = true;
	long directionChangeLoopCount = 0;
	int loopCounter = 0;

	// these are backups for status updates
	bool powerOnLast = powerOn;
	uint8_t speedLast = speed;
	int modeLast = mode;
	bool directionLast = direction;
	uint8_t motorSpeedLast[MOTOR_COUNT];
	uint8_t motorPwmLast[MOTOR_COUNT];
	bool motorDirectionLast[MOTOR_COUNT];

	// keep last speed before powerOff
	uint8_t speedBeforePowerOff = 25;

	MotorController* motors[MOTOR_COUNT];

	void setPowerPin(bool powerOn);

	char* substr(const char *buff, uint8_t start,uint8_t len, char* substr);
};

#endif /* VENTILATIONCONTROL_H_ */
