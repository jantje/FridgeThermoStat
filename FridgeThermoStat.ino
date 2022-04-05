#include "Arduino.h"
#include "TempMeterTemperatureSensetiveResistor.h"
#include "Brains.h"

#define tempPin 100
#define relaisPin 101
#define tempMultiplyer 200
#define tempOfset 300
#define DESIRED_CENTI_TEMP 400

uint32_t loopMillis;


TempMeterTemperatureSensetiveResistor myTempSensor= TempMeterTemperatureSensetiveResistor(tempPin,tempMultiplyer,tempOfset);
Brains myBrains=Brains(relaisPin,DESIRED_CENTI_TEMP);


void setup()
{
		loopMillis=millis();
		myTempSensor.setup();
		myBrains.setup();
}

// The loop function is called in an endless loop
void loop()
{
		loopMillis=millis();
		myTempSensor.loop();
		myBrains.loop();
}
