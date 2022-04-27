/*
 * Brains.cpp
 *
 *  Created on: 5 apr. 2022
 *      Author: jan
 */

#include "Brains.h"
#include "TempMeterInterface.h"

Brains::Brains(uint8_t relaisPin,uint16_t desiredcentiTemp,uint16_t hysteresiscentiTemp,TempMeterInterface &tempSensor):
        myTempSensor(tempSensor) {
    myRelaisPin = relaisPin;
    myMotorState = LOW;
    myLastSwitch = 0;
    myDesiredcentiTemp = desiredcentiTemp;
    myHysteresiscentiTemp= hysteresiscentiTemp;
}

void Brains::setup() {
    myLastSwitch = loopMillis;
    pinMode(myRelaisPin, OUTPUT);
    digitalWrite(myRelaisPin, myMotorState);
}

void Brains::loop() {
    uint8_t newMotorState = LOW;

    if (myTempSensor.getCentiCelsius() > myDesiredcentiTemp) {
        newMotorState = HIGH;
    }
    if(abs((int)myTempSensor.getCentiCelsius() - (int)myDesiredcentiTemp)<=myHysteresiscentiTemp){
        newMotorState=myMotorState;
    }
    if (newMotorState != myMotorState) {
        if ((loopMillis - myLastSwitch > (1000 * 60 * 3)) || (myLastSwitch == 0)) {
            if (myMotorState == HIGH) {
                myTotalOnTime += (loopMillis - myLastSwitch);
            }
            //Never switch the motor in less than 3 minutes
            myLastSwitch = loopMillis;
            myMotorState = newMotorState;
            digitalWrite(myRelaisPin, myMotorState);
        }
    }
}

uint32_t Brains::getOnTime() {
    if (myMotorState == HIGH) {
        return myTotalOnTime + (loopMillis - myLastSwitch);
    } else {
        return myTotalOnTime;
    }
}

boolean Brains::isFridgeOn() {
    return (myMotorState == HIGH);
}
