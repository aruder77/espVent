/*
 * VentilationControl.cpp
 *
 *  Created on: 22 Jun 2017
 *      Author: axel
 */

#include "VentilationController.h"

bool speedHandler(const HomieRange& range, const String& value) {
	long intValue = value.toInt();
	if (intValue < 0 || intValue > 100) {
		return false;
	}

	VentilationController::getInstance()->setSpeed(intValue);
  	return true;
}

bool modeHandler(const HomieRange& range, const String& value) {
	long intValue = value.toInt();
	if (intValue < 0 || intValue > 3) {
		return false;
	}

	VentilationController::getInstance()->setMode(intValue);
  	return true;
}

bool directionHandler(const HomieRange& range, const String& value) {
	bool boolValue = value.equalsIgnoreCase("in");

	VentilationController::getInstance()->setDirection(boolValue);
  	return true;
}

bool cycleTimeHandler(const HomieRange& range, const String& value) {
	long intValue = value.toInt();
	VentilationController::getInstance()->setCycleTime(intValue);
	return true;
}

bool setMotorSpeed(const uint8_t motorNumber, const String& value) {
	long intValue = value.toInt();
	if (intValue < 0 || intValue > 100) {
		return false;
	}

	VentilationController::getInstance()->setSpeed(motorNumber, intValue);
  	return true;
}

bool motorSpeedHandler0(const HomieRange& range, const String& value) {
	return setMotorSpeed(0, value);
}

bool motorSpeedHandler1(const HomieRange& range, const String& value) {
	return setMotorSpeed(1, value);
}

bool motorSpeedHandler2(const HomieRange& range, const String& value) {
	return setMotorSpeed(2, value);
}

bool motorSpeedHandler3(const HomieRange& range, const String& value) {
	return setMotorSpeed(3, value);
}

bool motorSpeedHandler4(const HomieRange& range, const String& value) {
	return setMotorSpeed(4, value);
}

bool motorSpeedHandler5(const HomieRange& range, const String& value) {
	return setMotorSpeed(5, value);
}

bool motorSpeedHandler6(const HomieRange& range, const String& value) {
	return setMotorSpeed(6, value);
}

bool motorSpeedHandler7(const HomieRange& range, const String& value) {
	return setMotorSpeed(7, value);
}


bool setMotorDirection(const uint8_t motorNumber, const String& value) {
	bool boolValue = value.equalsIgnoreCase("in");

	VentilationController::getInstance()->setDirection(motorNumber, boolValue);
  	return true;
}

bool motorDirectionHandler0(const HomieRange& range, const String& value) {
	return setMotorDirection(0, value);
}

bool motorDirectionHandler1(const HomieRange& range, const String& value) {
	return setMotorDirection(1, value);
}

bool motorDirectionHandler2(const HomieRange& range, const String& value) {
	return setMotorDirection(2, value);
}

bool motorDirectionHandler3(const HomieRange& range, const String& value) {
	return setMotorDirection(3, value);
}

bool motorDirectionHandler4(const HomieRange& range, const String& value) {
	return setMotorDirection(4, value);
}

bool motorDirectionHandler5(const HomieRange& range, const String& value) {
	return setMotorDirection(5, value);
}

bool motorDirectionHandler6(const HomieRange& range, const String& value) {
	return setMotorDirection(6, value);
}

bool motorDirectionHandler7(const HomieRange& range, const String& value) {
	return setMotorDirection(7, value);
}

VentilationController *VentilationController::instance = 0;

VentilationController *VentilationController::getInstance()
{
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	if (!VentilationController::instance)
	{
		VentilationController::instance = new VentilationController();
	}
	return VentilationController::instance;
}

VentilationController::VentilationController() {
	memNode = new HomieNode("controller", "Controller", "controller");
	motorsNode = new HomieNode("motors", "Motors", "ventilation");
	motorNode[0] = new HomieNode("motor0", "Motor 0", "ventilation");
	motorNode[1] = new HomieNode("motor1", "Motor 1", "ventilation");
	motorNode[2] = new HomieNode("motor2", "Motor 2", "ventilation");
	motorNode[3] = new HomieNode("motor3", "Motor 3", "ventilation");
	motorNode[4] = new HomieNode("motor4", "Motor 4", "ventilation");
	motorNode[5] = new HomieNode("motor5", "Motor 5", "ventilation");
	motorNode[6] = new HomieNode("motor6", "Motor 6", "ventilation");
	motorNode[7] = new HomieNode("motor7", "Motor 7", "ventilation");

	memNode->advertise("freeHeap")
			.setName("free heap")
			.setDatatype("integer");

	memNode->advertise("maxTimeBetweenCycles")
			.setName("max time between cycles")
			.setDatatype("integer");

	motorsNode->advertise("speed")
			.setName("speed")
			.setDatatype("integer")
			.setUnit("%")
			.settable(speedHandler);
	motorsNode->advertise("mode")
			.setName("mode")
			.setDatatype("integer")
			.setFormat("0:3")
			.settable(modeHandler);
	motorsNode->advertise("direction")
			.setName("direction")
			.setDatatype("enum")
			.setFormat("in,out")
			.settable(directionHandler);
	motorsNode->advertise("cycleTime")
			.setName("cycleTime")
			.setDatatype("integer")
			.settable(cycleTimeHandler);

	motorNode[0]->advertise("speed")
			.setName("speed")
			.setDatatype("integer")
			.setUnit("%")
			.settable(motorSpeedHandler0);
	motorNode[0]->advertise("direction")
			.setName("direction")
			.setDatatype("enum")
			.setFormat("in,out")
			.settable(motorDirectionHandler0);

	motorNode[1]->advertise("speed")
			.setName("speed")
			.setDatatype("integer")
			.setUnit("%")
			.settable(motorSpeedHandler1);
	motorNode[1]->advertise("direction")
			.setName("direction")
			.setDatatype("enum")
			.setFormat("in,out")
			.settable(motorDirectionHandler1);

	motorNode[2]->advertise("speed")
			.setName("speed")
			.setDatatype("integer")
			.setUnit("%")
			.settable(motorSpeedHandler2);
	motorNode[2]->advertise("direction")
			.setName("direction")
			.setDatatype("enum")
			.setFormat("in,out")
			.settable(motorDirectionHandler3);

	motorNode[3]->advertise("speed")
			.setName("speed")
			.setDatatype("integer")
			.setUnit("%")
			.settable(motorSpeedHandler3);
	motorNode[3]->advertise("direction")
			.setName("direction")
			.setDatatype("enum")
			.setFormat("in,out")
			.settable(motorDirectionHandler3);

	motorNode[4]->advertise("speed")
			.setName("speed")
			.setDatatype("integer")
			.setUnit("%")
			.settable(motorSpeedHandler4);
	motorNode[4]->advertise("direction")
			.setName("direction")
			.setDatatype("enum")
			.setFormat("in,out")
			.settable(motorDirectionHandler4);

	motorNode[5]->advertise("speed")
			.setName("speed")
			.setDatatype("integer")
			.setUnit("%")
			.settable(motorSpeedHandler5);
	motorNode[5]->advertise("direction")
			.setName("direction")
			.setDatatype("enum")
			.setFormat("in,out")
			.settable(motorDirectionHandler5);

	motorNode[6]->advertise("speed")
			.setName("speed")
			.setDatatype("integer")
			.setUnit("%")
			.settable(motorSpeedHandler6);
	motorNode[6]->advertise("direction")
			.setName("direction")
			.setDatatype("enum")
			.setFormat("in,out")
			.settable(motorDirectionHandler6);

	motorNode[7]->advertise("speed")
			.setName("speed")
			.setDatatype("integer")
			.setUnit("%")
			.settable(motorSpeedHandler7);
	motorNode[7]->advertise("direction")
			.setName("direction")
			.setDatatype("enum")
			.setFormat("in,out")
			.settable(motorDirectionHandler7);

  	inverseMotor1 = new HomieSetting<bool>("inverseMotor1", "Inverse rotation direction of motor 1.");
  	inverseMotor2 = new HomieSetting<bool>("inverseMotor2", "Inverse rotation direction of motor 2.");
  	inverseMotor3 = new HomieSetting<bool>("inverseMotor3", "Inverse rotation direction of motor 3.");
  	inverseMotor4 = new HomieSetting<bool>("inverseMotor4", "Inverse rotation direction of motor 4.");
  	inverseMotor5 = new HomieSetting<bool>("inverseMotor5", "Inverse rotation direction of motor 5.");
  	inverseMotor6 = new HomieSetting<bool>("inverseMotor6", "Inverse rotation direction of motor 6.");
  	inverseMotor7 = new HomieSetting<bool>("inverseMotor7", "Inverse rotation direction of motor 7.");
  	inverseMotor8 = new HomieSetting<bool>("inverseMotor8", "Inverse rotation direction of motor 8.");
	inverseMotor1->setDefaultValue(false);
	inverseMotor2->setDefaultValue(true);
	inverseMotor3->setDefaultValue(false);
	inverseMotor4->setDefaultValue(true);
	inverseMotor5->setDefaultValue(false);
	inverseMotor6->setDefaultValue(true);
	inverseMotor7->setDefaultValue(false);
	inverseMotor8->setDefaultValue(true);

	// initialize motors
	motors[0] = new MotorController(4 , 1, false);		// Flur EG
	motors[1] = new MotorController(13, 2, true);		// Wohnzimmer EG
	motors[2] = new MotorController(14, 3, false);		// Schlafzimmer
	motors[3] = new MotorController(27, 4, true);		// Flur OG
	motors[4] = new MotorController(26, 5, false);		// Kind 1
	motors[5] = new MotorController(25, 6, true);		// Kind 2
	motors[6] = new MotorController(33, 7, false);
	motors[7] = new MotorController(32, 8, true);
	// calculate directionChangeLoops
	directionChangeLoopCount = cycleTime * 1000.0 / 100;

	setPowerOn(true);
	setSpeed(20);	
}

VentilationController::~VentilationController() {
}

const char *VentilationController::getName() {
	return "VentilationController";
}

void VentilationController::setup() {
	motors[0]->setInverseDirection(inverseMotor1->get());
	motors[1]->setInverseDirection(inverseMotor2->get());
	motors[2]->setInverseDirection(inverseMotor3->get());
	motors[3]->setInverseDirection(inverseMotor4->get());
	motors[4]->setInverseDirection(inverseMotor5->get());
	motors[5]->setInverseDirection(inverseMotor6->get());
	motors[6]->setInverseDirection(inverseMotor7->get());
	motors[7]->setInverseDirection(inverseMotor8->get());
}

void VentilationController::every100Milliseconds() {
	uint32_t currentMillis = millis();
	uint32_t diff = currentMillis - lastMillis;
	lastMillis = currentMillis;
	if (diff > maxMillis && diff < 1000) {
		maxMillis = diff;
	} 

	// after DIRECTION_CHANGE_INTERVAL, change direction
	if (loopCounter == directionChangeLoopCount) {
		// only in mode 0, change direction
		if (mode == 0) {
			this->setDirection(!this->isDirection());
		}

		// publish free memory
		memNode->setProperty("freeHeap").send(String(ESP.getFreeHeap()));
		Homie.getLogger() << "free heap: " << ESP.getFreeHeap() << endl;

		memNode->setProperty("maxTimeBetweenCycles").send(String(maxMillis));
		maxMillis = 0;

		loopCounter = 0;
	}

	// adjust all motors speed and direction slowly in every loop
	for (int i = 0; i < MOTOR_COUNT; i++) {
		motors[i]->adjust();
	}

	loopCounter++;
}

bool VentilationController::isDirection() const {
	return direction;
}

void VentilationController::setDirection(bool direction) {
	if (mode != 2) {
		this->direction = direction;
		motorsNode->setProperty("direction").send(direction ? "in" : "out");
		Homie.getLogger() << "switching direction to " << (direction ? "in" : "out") << endl;
		for (int i = 0; i < MOTOR_COUNT; i++) {
			motors[i]->setTargetDirection(direction);
			motorNode[i]->setProperty("direction").send((motors[i]->isInverseDirection() ? !direction : direction) ? "in" : "out");
		}
	}
}

void VentilationController::setDirection(uint8_t motorNumber, bool direction) {
	if (mode == 3) {
		motors[motorNumber]->setTargetDirection(direction);
		motorNode[motorNumber]->setProperty("direction").send(direction ? "in" : "out");
	}
}

void VentilationController::setCycleTime(int seconds) {
	this->cycleTime = seconds;
	motorsNode->setProperty("cycleTime").send(String(cycleTime));
}

int VentilationController::getMode() const {
	return mode;
}

void VentilationController::setMode(int mode) {
	char logBuffer[50];
	snprintf(logBuffer, sizeof(logBuffer), "Mode: %d\n", mode);
	Homie.getLogger() << logBuffer;
	this->mode = mode;
	motorsNode->setProperty("mode").send(String(mode));

	if (mode == 0 || mode == 1) {
		// regular switching mode
		for (int i = 0; i < MOTOR_COUNT; i++) {
			motors[i]->setInverseDirection(i % 2 != 0);
			motors[i]->setTargetDirection(direction);
			motorNode[i]->setProperty("direction").send((motors[i]->isInverseDirection() ? !direction : direction) ? "in" : "out");
		}
	} else if (mode == 2) {
		// all in, no inverse direction
		for (int i = 0; i < MOTOR_COUNT; i++) {
			motors[i]->setInverseDirection(false);
			motors[i]->setTargetDirection(true);
			motorNode[i]->setProperty("direction").send("in");
		}
	} else if (mode == 3) {
		// manual mode, no inverse direction
		for (int i = 0; i < MOTOR_COUNT; i++) {
			motors[i]->setInverseDirection(false);
			motors[i]->setTargetDirection(direction);
			motorNode[i]->setProperty("direction").send(direction ? "in" : "out");
		}
	}
}

uint8_t VentilationController::getSpeed() const {
	return speed;
}

void VentilationController::setSpeed(uint8_t speed) {
	this->speed = speed;
	for (int i = 0; i < MOTOR_COUNT; i++) {
		motors[i]->setTargetSpeed(speed);
		motorNode[i]->setProperty("speed").send(String(speed));
	}

	if (speed == 0) {
		setPowerPin(false);
	} else if (!this->powerOn) {
		setPowerPin(true);
	}
	Homie.getLogger() << "Speed is " << speed << endl;
	motorsNode->setProperty("speed").send(String(speed));
}

void VentilationController::setSpeed(uint8_t motorNumber, uint8_t speed) {
	if (this->mode == 1) {
		// in mode 0, speed can only be adjusted for all motors
		setSpeed(speed);
	} else {
		motors[motorNumber]->setTargetSpeed(speed);
		motorNode[motorNumber]->setProperty("speed").send(String(speed));
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
	snprintf(logBuffer, sizeof(logBuffer), "Power: %s\n", powerOn ? "true" : "false");
	Homie.getLogger() << logBuffer;
}

char* VentilationController::substr(const char *buff, uint8_t start,uint8_t len, char* substr)
{
    strncpy(substr, buff+start, len);
    substr[len] = 0;
    return substr;
}

