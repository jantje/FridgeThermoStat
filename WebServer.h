#pragma once
#include "Arduino.h"
#include <ESP8266WebServer.h>
#include "Brains.h"

class WebServer {
public:
    WebServer();
    void setup();
    void loop();

    void sendTemperature();
    void sendCooling();
    void sendHandleNotFound();
    void sendOnTime();
    void sendRunTime();
    void sendMainPage();
private:
    ESP8266WebServer myServer;
    float myLastTemp=0;
};
