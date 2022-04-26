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
		uint8_t myRelaisPin;//the actual pin the relais is connected to
		uint8_t myMotorState; // the state of the pin the relais is connected to
		uint32_t myLastSwitch;  //the last time the relais pin has changed state
		uint16_t myDesiredcentiTemp;  //the desired temperature (cooling assumed)
		TempMeterInterface& myTempSensor;  //the sensor
		uint32_t myTotalOnTime;//the total time the relais was on

public:
		Brains (uint8_t relaisPin,uint16_t desiredcentiTemp,TempMeterInterface &tempSensor);
		void setup();
		void loop();
		boolean isFridgeOn();
		uint32_t getOnTime(){return myTotalOnTime;}
	};

#pragma once
