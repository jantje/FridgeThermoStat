/*
 * WebServer.cpp
 *
 *  Created on: 2022 Apr 21
 *      Author: jan
 */
#include "WebServer.h"
#include "TempMeter18b20.h"
#include <ESP8266mDNS.h>

extern TempMeter18b20 myTempSensor;
extern WebServer theWebserver;
extern Brains myBrains;

const char index_html[] PROGMEM
        = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Fridge manager</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    <span class="ds-labels">Temperature Celsius</span>
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-repeat fa-1" aria-hidden="true"></i>
    <span class="ds-labels">Cooling</span>
    <span id="cooling">%COOLING%</span>
  </p>
  <p>
    <span class="ds-labels">On time </span>
    <span id="OnTime">%ONTIME%</span>
    <sup class="units">HHHH:MM:SS</sup>
  </p>
  <p>
    <span class="ds-labels">Running Time </span>
    <span id="RunTime">%RUNTIME%</span>
    <sup class="units">HHHH:MM:SS</sup>
  </p>
</body>
<script>
setInterval(update, 10000) ;

function update(  ) {
    updateValue("temperature","/temperature")
    updateValue("cooling","/cooling")
    updateValue("OnTime","/OnTime")
    updateValue("RunTime","/RunTime")
}


function updateValue( field,url ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById(field).innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", url, true);
  xhttp.send();
}
</script>
</html>)rawliteral";

void proxyMainPage() {
    theWebserver.sendMainPage();
}
void proxyTemperature() {
    theWebserver.sendTemperature();
}
void proxyOnTime() {
    theWebserver.sendOnTime();
}
void proxyRunTime() {
    theWebserver.sendRunTime();
}
void proxyCooling() {
    theWebserver.sendCooling();
}
void proxyHandleNotFound() {
    Serial.println("Recieved proxyHandleNotFound");
    theWebserver.sendHandleNotFound();
}


WebServer::WebServer() :
        myServer(80) {
}

void WebServer::setup() {
    if (MDNS.begin("frigo")) {
        Serial.println("MDNS responder started");
    }

    myServer.on("/", proxyMainPage);
    myServer.on("/temperature", proxyTemperature);
    myServer.on("/cooling", proxyCooling);
    myServer.on("/OnTime", proxyOnTime);
    myServer.on("/RunTime", proxyRunTime);

    myServer.onNotFound(proxyHandleNotFound);

    myServer.begin();

}

void WebServer::loop() {
    myServer.handleClient();
}

void WebServer::sendHandleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += myServer.uri();
    message += "\nMethod: ";
    message += (myServer.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += myServer.args();
    message += "\n";

    for (uint8_t i = 0; i < myServer.args(); i++) {
        message += " " + myServer.argName(i) + ": " + myServer.arg(i) + "\n";
    }

    myServer.send(404, "text/plain", message);
}

void WebServer::sendMainPage() {
    myServer.send(200, "text/html", index_html);
    sendTemperature();
}

void WebServer::sendTemperature() {
    myServer.send(200, "text/plain", String( myTempSensor.getTemp()));
}

void WebServer::sendCooling() {
    if (myBrains.isFridgeOn()) {
        myServer.send(200, "text/plain", "ON");
    } else {
        myServer.send(200, "text/plain", "OFF");
    }
}

char * TimeToString(uint32_t tm)
{
  static char str[12];
  uint32_t t=tm/1000;
  long h = t / 3600;
  t = t % 3600;
  int m = t / 60;
  int s = t % 60;
  sprintf(str, "%04ld:%02d:%02d", h, m, s);
  return str;
}

void WebServer::sendRunTime() {
    myServer.send(200, "text/plain", String( TimeToString(loopMillis)));
}

void WebServer::sendOnTime() {
    myServer.send(200, "text/plain", String( TimeToString(myBrains.getOnTime())));
}
