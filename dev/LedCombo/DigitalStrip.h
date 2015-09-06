// DigitalStrip.h

#ifndef _DIGITALSTRIP_h
#define _DIGITALSTRIP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class DigitalStripClass
{
 protected:


 public:
	void init();
};

extern DigitalStripClass DigitalStrip;

#endif

