// DigitalEffect.h

#ifndef _DIGITALEFFECT_h
#define _DIGITALEFFECT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_NeoPixel.h>

class DigitalEffect : public Adafruit_NeoPixel
{
private:
	void NextProgram();
	void DigitalProgram1();
	void DigitalProgram2();
	void DigitalProgram3();
	void DigitalProgram4();
	void DigitalProgram5();
	void DigitalProgram6();
	void DigitalProgram7();
	void DigitalProgram8();
	void DigitalProgram9();
	void SetBar(int i, uint32_t c);
	void Clash(int index1, int index2, uint32_t colour);

protected:

public:
	unsigned long currentMillis;
	int program;
	unsigned long previous;
	int step;
	int pixel;
	int pixel1;
	int pixel2;
	int pixelStep;

	DigitalEffect();
	DigitalEffect(uint16_t n, uint8_t p, uint8_t t);
	~DigitalEffect();
	void Init();
	void Process();
	void SetAll(int r, int g, int b);
	void ShowAll(int r, int g, int b);
};

// extern DigitalEffectClass DigitalEffect;

#endif

