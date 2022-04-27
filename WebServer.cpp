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


void proxyMainPage() {
    theWebserver.sendHTML(index_html);
}
void proxyTemperature() {
    static char strTemp[7];
    float temp=myTempSensor.getTemp();
    dtostrf(temp, 6, 2, strTemp);
    theWebserver.sendPlain( strTemp);
}

void proxyOnTime() {
    theWebserver.sendPlain( TimeToString(myBrains.getOnTime()));
}
void proxyRunTime() {
    theWebserver.sendPlain( TimeToString(loopMillis));
}
void proxyCooling() {
    if (myBrains.isFridgeOn()) {
        theWebserver.sendPlain( "ON");
    } else {
        theWebserver.sendPlain( "OFF");
    }
}
void proxyHandleNotFound() {
//    String message = "File Not Found\n\n";
//    message += "URI: ";
//    message += myServer.uri();
//    message += "\nMethod: ";
//    message += (myServer.method() == HTTP_GET) ? "GET" : "POST";
//    message += "\nArguments: ";
//    message += myServer.args();
//    message += "\n";
//
//    for (uint8_t i = 0; i < myServer.args(); i++) {
//        message += " " + myServer.argName(i) + ": " + myServer.arg(i) + "\n";
//    }
//
//    myServer.send(404, "text/plain", message);
    const char * message = "File Not Found";
    theWebserver.sendError(message);
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






void WebServer::sendHTML(const char * text) {
    myServer.send(200, "text/html", text);
}

void WebServer::sendPlain(const char *text) {
    myServer.send(200, "text/plain",text);
}

void WebServer::sendError(const char * text){
    myServer.send(404, "text/plain",text);
}
