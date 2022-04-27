#pragma once
#include "Arduino.h"
#include <ESP8266WebServer.h>
#include "Brains.h"

class WebServer {
public:
    WebServer();
    void setup();
    void loop();

    void sendPlain(const char * text);
    void sendHTML(const char * text);
    void sendError(const char * text);

private:
    ESP8266WebServer myServer;
    float myLastTemp=0;
};
