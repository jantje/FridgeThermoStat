#include "Arduino.h"
#include "Brains.h"
#include "TempMeter18b20.h"

//OTA stuff
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "private.h" //file containing your ssid password and OTApassword
//end of OTA stuff

#include "Webserver.h"

WebServer theWebserver;
#define relaisPin D8
//the 2 values below should give us a temperature between 4° and 3,5° celcius
#define DESIRED_CENTI_TEMP 375
#define HYSTERESIS_CENTI_TEMP 25
// Data wire is plugged into pin 2
#define ONE_WIRE_BUS D2

uint32_t loopMillis;

OneWire oneWire(ONE_WIRE_BUS);
TempMeter18b20 myTempSensor = TempMeter18b20(&oneWire);
Brains myBrains = Brains(relaisPin, DESIRED_CENTI_TEMP,HYSTERESIS_CENTI_TEMP,myTempSensor);



void setup() {

    Serial.begin(115200);
    delay(2000); //wait so serial monitor can catch up
    Serial.println("starting fridge app");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("Connection Failed! Rebooting...");
        delay(5000);
        ESP.restart();
    }
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Port defaults to 8266
    ArduinoOTA.setPort(8266);

    // Hostname defaults to esp8266-[ChipID]
    // ArduinoOTA.setHostname("frigo");

    // No authentication by default
    ArduinoOTA.setPassword("frig0Werk");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else
            // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
            Serial.println("End Failed");
    });
    ArduinoOTA.begin();

    theWebserver.setup();


    loopMillis = millis();

    myTempSensor.setup();
    myBrains.setup();
}

// The loop function is called in an endless loop
void loop() {
    loopMillis = millis();
    ArduinoOTA.handle();
    theWebserver.loop();
    myTempSensor.loop();
    myBrains.loop();
    static uint32_t last_log = 0;
    if (loopMillis - last_log > 10000) {
        last_log = loopMillis;
        Serial.print("millis: ");
        Serial.print(loopMillis);
        Serial.print("; temp ");
        Serial.print(myTempSensor.getCentiCelsius());
        Serial.print(" centi celcius; fridge 4 is ");
        if (myBrains.isFridgeOn()) {
            Serial.println("on.");
        } else {
            Serial.println("off.");
        }
    }

}
