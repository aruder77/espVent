#include <PWMController.h>


PWMController::PWMController() {
}

const char *PWMController::getName() {
     return "PWMController";
}


void PWMController::setup() {
    Log.notice("setting up PWM signal!\n");
    ledcSetup(1, 4000, 8);
    ledcAttachPin(27, 1);
    ledcWrite(1, 50);
}

void PWMController::everySecond() {

}


void PWMController::configUpdate(const char *key, const char *value) {
    Log.notice("PWMController config update: %s\n", key);
}