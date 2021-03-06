/*
 * TemPemetds18b20.h
 *
 *  Created on: 2022 Apr 21
 *      Author: jan
 */
#pragma once
#include "TempMeterInterface.h"
#include <OneWire.h>


class TempMeter18b20: public TempMeterInterface {
public:
    TempMeter18b20(OneWire *oneWire);
    void setup();
    void loop();
    float getTemp(){return myTemp;};
private:
    OneWire *myOneWire;
    uint32_t myLastAction;
    float myTemp;
};
