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


const char index_html[] PROGMEM = R"rawliteral(
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
  <h2>ESP DS18B20 Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    <span class="ds-labels">Temperature Celsius</span>
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
    <span class="ds-labels">Temperature Fahrenheit</span>
    <span id="temperaturef">%TEMPERATUREF%</span>
    <sup class="units">&deg;F</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturef").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturef", true);
  xhttp.send();
}, 10000) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DS18B20 values
void processorProxy(){
    Serial.println("Recieved processorProxy");
    theWebserver.processor("TEMPERATUREC");
}
void proxyTemperatureC(){
    Serial.println("Recieved proxyTemperatureC");
    theWebserver.temperatureC();
}
void proxyTemperatureF(){
    Serial.println("Recieved proxyTemperatureF");
    theWebserver.temperatureF();
}
void proxyHandleNotFound() {
    Serial.println("Recieved proxyHandleNotFound");
        theWebserver.handleNotFound();
}
void WebServer::handleNotFound() {
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

void WebServer::processor(const String& var){
    myServer.send(200, "text/html", index_html);
//  //Serial.println(var);
//  if(var == "TEMPERATUREC"){
//      myServer.send(200, "text/html", getTemperatureC());
//  }
//  else if(var == "TEMPERATUREF"){
//      myServer.send(200, "text/html", getTemperatureF());
//  }
//  myServer.send(200, "text/html", "unsupported temp format");
}



WebServer::WebServer():myServer(80) {
}

void WebServer::setup() {
    if (MDNS.begin("frigo")) {
      Serial.println("MDNS responder started");
    }

    myServer.on("/", processorProxy);
    myServer.on("/temperaturec", proxyTemperatureC);
    myServer.on("/temperaturef", proxyTemperatureF);
    myServer.onNotFound(proxyHandleNotFound);

    myServer.begin();

}

void WebServer::loop() {
    myServer.handleClient();
    if(myLastTempC!=myTempSensor.getTempC()){
        myLastTempC=myTempSensor.getTempC();
        myTemperatureC=String(myLastTempC);
        myTemperatureF=String(myTempSensor.getTempF());
    }
}

void WebServer::temperatureC() {
    myServer.send(200, "text/plain", getTemperatureC());
}

void WebServer::temperatureF() {
    myServer.send(200, "text/plain", getTemperatureF());
}
