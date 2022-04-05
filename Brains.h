/*
 * Brains.h
 *
 *  Created on: 5 apr. 2022
 *      Author: jan
 */

#include "Arduino.h"

class Brains
	{
	private:
		uint8_t myRelaisPin;
		uint8_t myMotorState;
		uint32_t myLastSwitch;
		uint16_t myDesiredcentiTemp;

public:
		Brains (uint8_t relaisPin,uint16_t desiredcentiTemp);
		void setup();
		void loop();
	};

#pragma once
