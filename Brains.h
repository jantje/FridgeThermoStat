/*
 * Brains.h
 *
 *  Created on: 5 apr. 2022
 *      Author: jan
 */

#include "Arduino.h"
#include "TempMeterInterface.h"
extern uint32_t loopMillis;

class Brains
	{
	private:
		uint8_t myRelaisPin;
		uint8_t myMotorState;
		uint32_t myLastSwitch;
		uint16_t myDesiredcentiTemp;
		TempMeterInterface& myTempSensor;

public:
		Brains (uint8_t relaisPin,uint16_t desiredcentiTemp,TempMeterInterface &tempSensor);
		void setup();
		void loop();
		boolean isFridgeOn();
	};

#pragma once
