#ifndef HEATING_CONTROLLER_H_
#define HEATING_CONTROLLER_H_

#include <Module.h>
#include <Prefs.h>
#include <ArduinoLog.h>

class HeatingController : public Module, public PrefsClient {

    public:
        HeatingController();

        const char *getName();
        void setup();
        void everySecond();

        void configUpdate(const char *id, const char *value);

    private:
        Prefs *prefs = Prefs::getInstance();
        int atFuehlerPin = 34;
};

#endif