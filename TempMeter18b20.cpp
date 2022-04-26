/*
 * TemPemetds18b20.cpp
 *
 *  Created on: 2022 Apr 21
 *      Author: jan
 */

#include "TempMeter18b20.h"
#include "DallasTemperature.h"
extern uint32_t loopMillis;
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature *mySensors=NULL;

TempMeter18b20::TempMeter18b20(OneWire *oneWire) {
    myOneWire=oneWire;
    myTemp=0;
    myLastAction=0;
    if (mySensors==NULL){
        mySensors=new DallasTemperature(oneWire);
    }
}

void TempMeter18b20::setup() {
    mySensors->begin();
}

void TempMeter18b20::loop() {
    if (loopMillis - myLastAction > 10000) {
        myLastAction = loopMillis;
        uint32_t startTime =millis();
        mySensors->requestTemperatures(); // Send the command to get temperature readings
            myTemp = mySensors->getTempCByIndex(0);
            myCentiCelsius=myTemp*100;

            Serial.print("Reading temp sensor took ");
            Serial.println(millis()-startTime);
    }
}
