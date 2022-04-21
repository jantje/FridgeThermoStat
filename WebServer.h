#pragma once
#include "Arduino.h"
#include <ESP8266WebServer.h>

class WebServer {
public:
    WebServer();
    void setup();
    void loop();
    String getTemperatureC() {
        return myTemperatureC;
    }
    String getTemperatureF() {
        return myTemperatureF;
    }
    void processor(const String& var);
    void temperatureC();
    void temperatureF();
    void handleNotFound() ;
private:
    String myTemperatureC;
    String myTemperatureF;

    ESP8266WebServer myServer;
    float myLastTempC;
};
