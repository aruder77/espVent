#ifndef PWM_CONTROLLER_H_
#define PWM_CONTROLLER_H_

#include <Arduino.h>
#include <Prefs.h>
#include <ArduinoLog.h>
#include <esp32-hal-ledc.h>

class PWMController : public Module {
    public:
        PWMController();

        const char *getName();

        void setup();
        void everySecond();

        void configUpdate(const char *key, const char *value);

    private:
        Prefs *prefs = Prefs::getInstance();
};


#endif