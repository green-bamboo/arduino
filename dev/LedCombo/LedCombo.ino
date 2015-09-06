#include "DigitalEffect.h"
#include <Adafruit_NeoPixel.h>

#define PIN_NEO1	 3
#define PIN_NEO2	 3
#define PIN_NEO3	 3
#define PIN_NEO4	 3
#define PIN_NEO5	 3

#define PIN_NEO16	 8
#define PIN_NEO50	 3
#define PIN_NEO150	 4
#define PIN_RED		 9
#define PIN_GREEN	10
#define PIN_BLUE	11
#define PIN_LED		13
#define PIN_BUZZER  32


#define WAIT		110

#define ANALOG_MIN		0
#define ANALOG_MAX		255
#define DIGITAL_MIN		0
#define DIGITAL_MAX		60


// #define DIGITAL_COUNT 16

Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(1, PIN_NEO16, NEO_GRB + NEO_KHZ400);
//DigitalEffect strip16 = DigitalEffect(16, PIN_NEO16, NEO_GRB + NEO_KHZ800);
DigitalEffect strip50 = DigitalEffect(50, PIN_NEO50, NEO_RGB + NEO_KHZ800);
//DigitalEffect strip150 = DigitalEffect(150, PIN_NEO150, NEO_GRB + NEO_KHZ800);

DigitalEffect strip1 = DigitalEffect(150, PIN_NEO1, NEO_GRB + NEO_KHZ800);
DigitalEffect strip2 = DigitalEffect(150, PIN_NEO2, NEO_GRB + NEO_KHZ800);
DigitalEffect strip3 = DigitalEffect(150, PIN_NEO3, NEO_GRB + NEO_KHZ800);
DigitalEffect strip4 = DigitalEffect(150, PIN_NEO4, NEO_GRB + NEO_KHZ800);
DigitalEffect strip5 = DigitalEffect(150, PIN_NEO5, NEO_GRB + NEO_KHZ800);

/*
Adafruit_NeoPixel strip16 = Adafruit_NeoPixel(16, PIN_NEO16, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip50 = Adafruit_NeoPixel(50, PIN_NEO50, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip300 = Adafruit_NeoPixel(300, PIN_NEO300, NEO_GRB + NEO_KHZ800);

DigitalEffectClass digital16 =  DigitalEffectClass(16, PIN_NEO16, NEO_GRB + NEO_KHZ800);
*/

unsigned long currentMillis = 0;
unsigned long bigTimer = 0;

bool ledOn = false;
int led = 0;
unsigned long previousLED = 0;

/*int digitalProgram = 1;
unsigned long previousDigital = 0;
int digitalStep = 0;
int digital = 0;
int pixel = 0;
int pixelStep = 1;*/

int analogProgram = 0;
unsigned long previousAnalog = 0;
int analogColour = 0;
int analogStep = 0;


void setup()
{

	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_RED, OUTPUT);
	pinMode(PIN_GREEN, OUTPUT);
	pinMode(PIN_BLUE, OUTPUT);

	pinMode(PIN_BUZZER, OUTPUT);

	//strip16.Init();
	//strip16.program = 1;

	strip50.Init();
	strip50.program = 1;

	//strip150.Init();
	//strip150.program = 1;

	strip1.Init();
	strip1.program = 1;

	strip2.Init();
	strip2.program = 2;

	strip3.Init();
	strip3.program = 3;

	strip4.Init();
	strip4.program = 4;

	strip5.Init();
	strip5.program = 5;

	SetAnalog(0, 0, 0);

	Serial.begin(9600);

	randomSeed(analogRead(2) + millis());

	strip1.ShowAll(0, 0, 0);
	strip2.ShowAll(0, 0, 0);
	strip3.ShowAll(0, 0, 0);
	strip4.ShowAll(0, 0, 0);
	strip5.ShowAll(0, 0, 0);
	strip50.ShowAll(0, 0, 0);

	bigTimer = millis();

} // setup ****************************************************************************************************************************************************************************************************************************


void loop()
{

	currentMillis = millis();

	// Simple half second for LED

	if (currentMillis - previousLED >= 500)
	{
		Serial.println("LED");
		previousLED = currentMillis;

		if (ledOn)
			digitalWrite(PIN_LED, LOW);
		else
			digitalWrite(PIN_LED, HIGH);

		ledOn = !ledOn;
	}

	//strip16.Process();
	//strip50.Process();
	//strip150.Process();

	if (PIN_RED > 0)
	{
		// Analog, 3 channel RGB

		if (analogProgram == 1)
		{
			//Serial.println("AnalogProgram1");
			AnalogProgram1();
		}
		else if (analogProgram == 2)
		{
			//Serial.println("AnalogProgram2");
			AnalogProgram2();
			analogProgram = 19;
		}
		else if (analogProgram == 3)
		{
			//Serial.println("AnalogProgram3");
			AnalogProgram3();
		}
		else if (analogProgram == 4)
		{
			Serial.println("AnalogProgram4");
			AnalogProgram4();
		}
		else if (analogProgram == 5)
		{
			Serial.println("AnalogProgram5");
			AnalogProgram6();
		}
		else if (analogProgram == 6)
		{
			analogProgram++;
			//Serial.println("AnalogProgram6");
			//AnalogProgram6();
		}
		else if (analogProgram == 7)
		{
			analogProgram++;
			SetAnalog(255, 255, 255);
			delay(2000);
			//Serial.println("AnalogProgram7");
			//AnalogProgram7();
		}
		else if (analogProgram == 8)
		{
			analogProgram++;
			SetAnalog(0, 0, 0);
			delay(2000);
			//Serial.println("AnalogProgram8");
			//AnalogProgram8();
		}
		else
		{
			analogProgram = 1;
			analogColour = 0;
			analogStep = 0;
		}
	}

} // loop ****************************************************************************************************************************************************************************************************************************


void loop12()
{
	TestPrimaryColours();
	//TestStarBurst();

	/*
	strip1.ShowAll(20, 20, 20);
	strip2.ShowAll(20, 20, 20);
	strip3.ShowAll(20, 20, 20);
	strip4.ShowAll(20, 20, 20);
	*/

	//TestSparkle();

	//TestDisco();

	//TestSlider();

	//TestLedOnly();
}


void TestLedOnly()
{
	currentMillis = millis();

	// Simple half second for LED

	if (currentMillis - previousLED >= 100)
	{
		previousLED = currentMillis;

		if (++led >= 10)
			led = 0;

		if (led == 1 || led == 3)
			digitalWrite(PIN_LED, HIGH);
		else
			digitalWrite(PIN_LED, LOW);

	}
}



void loop44()
{

	digitalWrite(PIN_LED, HIGH);
	int a = 20;
	int b = 80;
	int c = 30;

	for (int i = 0; i < strip1.numPixels(); i++)
	{
		strip1.setPixelColor(i, a, a, a);
		strip1.show();
		delay(50);

		strip1.setPixelColor(i, 0, 0, 0);
		strip1.show();
		delay(50);

		strip1.setPixelColor(i, b, b, b);
		strip1.show();
		delay(50);

		strip1.setPixelColor(i, 0, 20, 0);
		strip1.show();
		delay(10);
	}

	//strip1.setPixelColor(10, c,c,c);
	//strip1.show();
	//delay(100);

	//strip1.setPixelColor(10, 0, 0, 0);
	//strip1.show();
	//delay(100);

	digitalWrite(PIN_LED, LOW);

	delay(3000);
}



void loop5()
{
	currentMillis = millis();

	// Simple half second for LED

	if (currentMillis - previousLED >= 100)
	{
		previousLED = currentMillis;

		if (++led >= 10)
			led = 0;

		if (led == 1 || led == 3)
			digitalWrite(PIN_LED, HIGH);
		else
			digitalWrite(PIN_LED, LOW);

	}

	//

	//strip150.Process();
	//strip50.Process();
	//strip1.Process();
	//strip2.Process();
	//strip3.Process();
	//strip4.Process();
	//strip5.Process();

	//


	//if (currentMillis - bigTimer >= 20000) // 20 seconds
	//{
	//bigTimer = currentMillis;
	Special();
	//	bigTimer = currentMillis;
	//}


}


void loop3()
{

	SetAnalog(0, 0, 0);

	delay(1000);

	//digitalWrite(PIN_LED, HIGH);
	//digitalWrite(PIN_BUZZER, HIGH);

	SetAnalog(255, 0, 0);

	delay(1000);

	//digitalWrite(PIN_LED, LOW);
	//digitalWrite(PIN_BUZZER, LOW);

	SetAnalog(0, 255, 0);

	delay(1000);

	SetAnalog(0, 0, 255);

	delay(1000);

	SetAnalog(255, 255, 255);

	delay(1000);

	for (int i = 0; i < 255; i += 10)
	{
		SetAnalog(i, 0, 0);
		delay(WAIT);
	}

	delay(1000);

	for (int i = 0; i < 255; i += 10)
	{
		SetAnalog(0, i, 0);
		delay(WAIT);
	}
	delay(1000);

	for (int i = 0; i < 255; i += 10)
	{
		SetAnalog(0, 0, i);
		delay(WAIT);
	}

	delay(1000);

}



void TestDisco()
{
	for (uint16_t i = 0; i < strip1.numPixels(); i++)
	{
		uint32_t colour = strip1.Color(random(DIGITAL_MIN, DIGITAL_MAX), random(DIGITAL_MIN, DIGITAL_MAX), random(DIGITAL_MIN, DIGITAL_MAX));
		strip1.setPixelColor(i, colour);
	}

	strip1.show();

}



void TestSlider()
{


	strip1.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip2.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip3.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip4.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip5.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip50.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);

	delay(2000);

	Slider(DIGITAL_MAX, 0, 0);
	Slider(0, DIGITAL_MAX, 0);
	Slider(0, 0, DIGITAL_MAX);

	Slider(255,20,147); // pink
	Slider(125,38,205); // purple
	Slider(0,205,205); // cyan
	Slider(238,201,0); // yellow gold
	Slider(255,128,0); // orange

	//Slider(DIGITAL_MAX, 0, 0);


}


void Slider(int r, int g, int b)
{
	for (int i = 0; i < 75; i++)
	{
		uint32_t colour = strip1.Color(r, g, b);
		strip1.setPixelColor(75 - i, colour);
		strip1.setPixelColor(75 + i, colour);
		strip2.setPixelColor(75 - i, colour);
		strip2.setPixelColor(75 + i, colour);
		strip3.setPixelColor(75 - i, colour);
		strip3.setPixelColor(75 + i, colour);
		strip4.setPixelColor(75 - i, colour);
		strip4.setPixelColor(75 + i, colour);

		strip1.show();
		strip2.show();
		strip3.show();
		strip4.show();

		delay(4);
	}

}



void TestPrimaryColours()
{

	strip1.ShowAll(0, 0, 0);
	strip2.ShowAll(0, 0, 0);
	strip3.ShowAll(0, 0, 0);
	strip4.ShowAll(0, 0, 0);
	strip5.ShowAll(0, 0, 0);
	strip50.ShowAll(0, 0, 0);

	delay(2000);

	strip1.ShowAll(DIGITAL_MAX, 0, 0);
	strip2.ShowAll(DIGITAL_MAX, 0, 0);
	strip3.ShowAll(DIGITAL_MAX, 0, 0);
	strip4.ShowAll(DIGITAL_MAX, 0, 0);
	strip5.ShowAll(DIGITAL_MAX, 0, 0);
	strip50.ShowAll(DIGITAL_MAX, 0, 0);

	delay(9000);

	strip1.ShowAll(0, DIGITAL_MAX, 0);
	strip2.ShowAll(0, DIGITAL_MAX, 0);
	strip3.ShowAll(0, DIGITAL_MAX, 0);
	strip4.ShowAll(0, DIGITAL_MAX, 0);
	strip5.ShowAll(0, DIGITAL_MAX, 0);
	strip50.ShowAll(0, DIGITAL_MAX, 0);

	delay(9000);


	strip1.ShowAll(0, 0, DIGITAL_MAX);
	strip2.ShowAll(0, 0, DIGITAL_MAX);
	strip3.ShowAll(0, 0, DIGITAL_MAX);
	strip4.ShowAll(0, 0, DIGITAL_MAX);
	strip5.ShowAll(0, 0, DIGITAL_MAX);
	strip50.ShowAll(0, 0, DIGITAL_MAX);

	delay(9000);

	strip1.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip2.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip3.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip4.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip5.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip50.ShowAll(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);

	delay(9000);

} // TestPrimaryColours ****************************************************************************************************************************************************************************************************************************


void TestStarBurst()
{
	delay(2000);

	int t1 = 15;
	int t2 = 25;

	int c = random(10, 60);

	strip1.setPixelColor(c, 200, 200, 200);
	strip1.show();
	delay(t1);

	strip1.setPixelColor(c - 1, 200, 200, 200);
	strip1.setPixelColor(c, 40, 40, 40);
	strip1.setPixelColor(c + 1, 200, 200, 200);
	strip1.show();
	delay(t1);

	strip1.setPixelColor(c - 2, 200, 200, 200);
	strip1.setPixelColor(c - 1, 40, 40, 40);
	strip1.setPixelColor(c, 40, 40, 40);
	strip1.setPixelColor(c + 1, 40, 40, 40);
	strip1.setPixelColor(c + 2, 200, 200, 200);
	strip1.show();
	delay(t1);

	strip1.setPixelColor(c - 3, 200, 200, 200);
	strip1.setPixelColor(c - 2, 40, 40, 40);
	strip1.setPixelColor(c - 1, 40, 40, 40);
	strip1.setPixelColor(c, 40, 40, 40);
	strip1.setPixelColor(c + 1, 40, 40, 40);
	strip1.setPixelColor(c + 2, 40, 40, 40);
	strip1.setPixelColor(c + 3, 200, 200, 200);
	strip1.show();
	delay(t1);

	strip1.setPixelColor(c - 4, 200, 200, 200);
	strip1.setPixelColor(c - 3, 40, 40, 40);
	strip1.setPixelColor(c - 2, 40, 40, 40);
	strip1.setPixelColor(c - 1, 40, 40, 40);
	strip1.setPixelColor(c, 40, 40, 40);
	strip1.setPixelColor(c + 1, 40, 40, 40);
	strip1.setPixelColor(c + 2, 40, 40, 40);
	strip1.setPixelColor(c + 3, 40, 40, 40);
	strip1.setPixelColor(c + 4, 200, 200, 200);
	strip1.show();
	delay(t1);

	strip1.setPixelColor(c - 5, 200, 200, 200);
	strip1.setPixelColor(c - 4, 40, 40, 40);
	strip1.setPixelColor(c - 3, 40, 40, 40);
	strip1.setPixelColor(c - 2, 40, 40, 40);
	strip1.setPixelColor(c - 1, 40, 40, 40);
	strip1.setPixelColor(c, 40, 40, 40);
	strip1.setPixelColor(c + 1, 40, 40, 40);
	strip1.setPixelColor(c + 2, 40, 40, 40);
	strip1.setPixelColor(c + 3, 40, 40, 40);
	strip1.setPixelColor(c + 4, 40, 40, 40);
	strip1.setPixelColor(c + 5, 200, 200, 200);
	strip1.show();
	delay(t1);


	strip1.setPixelColor(c - 6, 200, 200, 200); // 5
	strip1.setPixelColor(c - 5, 40, 40, 40);    // 2
	strip1.setPixelColor(c - 4, 40, 40, 40);    // 7
	strip1.setPixelColor(c - 3, 40, 40, 40);    // 12
	strip1.setPixelColor(c - 2, 40, 40, 40);    // 9
	strip1.setPixelColor(c - 1, 40, 40, 40);    // 11
	strip1.setPixelColor(c, 40, 40, 40);        // 13
	strip1.setPixelColor(c + 1, 40, 40, 40);    // 3
	strip1.setPixelColor(c + 2, 40, 40, 40);    // 6
	strip1.setPixelColor(c + 3, 40, 40, 40);    // 1
	strip1.setPixelColor(c + 4, 40, 40, 40);    // 4
	strip1.setPixelColor(c + 5, 40, 40, 40);    // 8
	strip1.setPixelColor(c + 6, 200, 200, 200); // 10
	strip1.show();
	delay(t1);

	strip1.setPixelColor(c - 6, 40, 40, 40);
	strip1.setPixelColor(c + 6, 40, 40, 40);
	strip1.show();
	delay(t1);

	int list[] = { +3, -5, +1, +4, -6, +2, -4, +5, -2, +6, -1, -3, 0 };

	int last = 0;
	for (int i = 0; i < 13; i++)
	{
		strip1.setPixelColor(c + last, 0, 0, 0);
		strip1.setPixelColor(c + list[i], 200, 200, 200);
		strip1.show();
		last = list[i];
		delay(t2);
	}

	for (int i = c - 6; i < c + 6; i++)
	{
		strip1.setPixelColor(i, 0, 0, 0);
	}

	strip1.show();

	/*
	strip1.setPixelColor(c + 3, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c + 3, 0, 0, 0);
	strip1.setPixelColor(c - 5, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c - 5, 0, 0, 0);
	strip1.setPixelColor(c + 1, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c + 1, 0, 0, 0);
	strip1.setPixelColor(c + 4, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c + 4, 0, 0, 0);
	strip1.setPixelColor(c - 6, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c - 6, 0, 0, 0);
	strip1.setPixelColor(c + 2, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c + 2, 0, 0, 0);
	strip1.setPixelColor(c - 4, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c - 4, 0, 0, 0);
	strip1.setPixelColor(c + 5, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c + 5, 0, 0, 0);
	strip1.setPixelColor(c - 2, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c - 2, 0, 0, 0);
	strip1.setPixelColor(c + 6, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c + 6, 0, 0, 0);
	strip1.setPixelColor(c - 1, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c - 1, 0, 0, 0);
	strip1.setPixelColor(c - 3, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c - 3, 0, 0, 0);
	strip1.setPixelColor(c, 200, 200, 200);
	strip1.show();
	delay(5);

	strip1.setPixelColor(c, 0, 0, 0);
	strip1.show();
	delay(5);
	*/

} // TestStarBurst ****************************************************************************************************************************************************************************************************************************


void TestSparkle()
{
	delay(20);
	Adafruit_NeoPixel *strip;

	for (int i = 0; i < 50; i++)
	{
		int s = random(1, 4);
		int on = random(1, 150);
		int off = random(1, 150);

		if (s == 1)
			strip = &strip1;
		else if (s == 2)
			strip = &strip2;
		else if (s == 3)
			strip = &strip3;
		else if (s == 4)
			strip = &strip4;

		strip->setPixelColor(off, 40, 40, 40);
		strip->setPixelColor(on, 200, 200, 200);
		strip->show();
	}

} // TestSparkle ****************************************************************************************************************************************************************************************************************************


void Special()
{
	strip1.ShowAll(0, 0, 0);
	strip2.ShowAll(0, 0, 0);
	strip3.ShowAll(0, 0, 0);
	strip4.ShowAll(0, 0, 0);
	strip5.ShowAll(0, 0, 0);
	strip50.ShowAll(0, 0, 0);

	delay(1000);

	// red
	for (int i = 0; i < DIGITAL_MAX; i++)
	{
		strip1.ShowAll(i, 0, 0);
		//strip2.ShowAll(i, 0, 0);
		//strip3.ShowAll(i, 0, 0);
		//strip4.ShowAll(i, 0, 0);
		//strip5.ShowAll(i, 0, 0);
		//strip50.ShowAll(i, 0, 0);
		delay(20);
	}

	// green
	for (int i = 0; i < DIGITAL_MAX; i++)
	{
		strip1.ShowAll(0, i, 0);
		//strip2.ShowAll(0, i, 0);
		//strip3.ShowAll(0, i, 0);
		//strip4.ShowAll(0, i, 0);
		//strip5.ShowAll(0, i, 0);
		//strip50.ShowAll(0, i, 0);
		delay(20);
	}

	// blue
	for (int i = 0; i < DIGITAL_MAX; i++)
	{
		strip1.ShowAll(0, 0, i);
		//strip2.ShowAll(0, 0, i);
		//strip3.ShowAll(0, 0, i);
		//strip4.ShowAll(0, 0, i);
		//strip5.ShowAll(0, 0, i);
		//strip50.ShowAll(0, 0, i);
		delay(20);
	}

	// white
	for (int i = 0; i < DIGITAL_MAX; i++)
	{
		strip1.ShowAll(i, i, i);
		//strip2.ShowAll(i, i, i);
		//strip3.ShowAll(i, i, i);
		//strip4.ShowAll(i, i, i);
		//strip5.ShowAll(i, i, i);
		//strip50.ShowAll(i, i, i);
		delay(20);
	}

	delay(1000);

	//

	strip1.ShowAll(0, 0, 0);
	strip2.ShowAll(0, 0, 0);
	strip3.ShowAll(0, 0, 0);
	strip4.ShowAll(0, 0, 0);
	strip5.ShowAll(0, 0, 0);
	strip50.ShowAll(0, 0, 0);

	delay(1000);

	//

	strip1.SetAll(0, 0, 0);
	strip2.SetAll(0, 0, 0);
	strip3.SetAll(0, 0, 0);
	strip4.SetAll(0, 0, 0);
	strip5.SetAll(0, 0, 0);
	strip50.SetAll(0, 0, 0);

	for (int i = 0; i < strip1.numPixels(); i++)
	{
		strip1.setPixelColor(i, DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
		strip1.show();
		//delay(2);
	}

	delay(2000);
	/*
	for (int i = 0; i < 810; i++)
	{

	if (i >= 0 & i <= 150) // 1
	{
	int pixel = i;
	strip1.setPixelColor(pixel, DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip1.show();
	delay(2);
	}
	else if (i >= 151 & i <= 300) // 2
	{
	int pixel = i - 151;
	pixel = 150 - pixel;
	strip2.setPixelColor(pixel, DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip2.show();
	delay(2);
	}
	else if (i >= 301 & i <= 450) // 3
	{
	int pixel = i - 301;
	strip3.setPixelColor(pixel, DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip3.show();
	delay(2);
	}
	else if (i >= 451 & i <= 600) // 4
	{
	int pixel = i - 451;
	pixel = 150 - pixel;
	strip4.setPixelColor(pixel, DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip4.show();
	delay(2);
	}
	else if (i >= 601 & i <= 750) // 5
	{
	int pixel = i - 601;
	strip5.setPixelColor(pixel, DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip5.show();
	delay(2);
	}
	else if (i >= 751 & i <= 800) // 6
	{
	int pixel = i - 751;
	strip50.setPixelColor(pixel, DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);
	strip50.show();
	delay(6);
	}


	//delay(1);
	}
	*/

	//

}



void AnalogProgram1() // Red
{

	if (currentMillis - previousAnalog >= 50)
	{
		previousAnalog = currentMillis;

		analogColour += 1;

		Serial.println(analogColour);

		if (analogColour >= ANALOG_MAX)
		{
			analogColour = ANALOG_MIN;
			analogProgram++;
		}
		else
		{
			if (analogColour <= ANALOG_MIN)
				analogColour = ANALOG_MIN;

			SetAnalog(analogColour, 0, 0);
		}
	}

} // AnalogProgram1 ****************************************************************************************************************************************************************************************************************************


void AnalogProgram2() // Green
{
	if (currentMillis - previousAnalog >= 200)
	{
		previousAnalog = currentMillis;

		analogColour += 10;

		Serial.println(analogColour);

		if (analogColour >= ANALOG_MAX)
		{
			analogColour = ANALOG_MIN;
			analogProgram++;
		}
		else
		{
			if (analogColour <= ANALOG_MIN)
				analogColour = ANALOG_MIN;

			SetAnalog(0, analogColour, 0);
		}
	}
} // AnalogProgram2 ****************************************************************************************************************************************************************************************************************************


void AnalogProgram3() // Blue
{
	if (currentMillis - previousAnalog >= 50)
	{
		previousAnalog = currentMillis;

		analogColour += 1;

		if (analogColour >= ANALOG_MAX)
		{
			analogColour = ANALOG_MIN;
			analogProgram++;
		}
		else
		{
			if (analogColour <= ANALOG_MIN)
				analogColour = ANALOG_MIN;

			SetAnalog(0, 0, analogColour);
		}
	}
} // AnalogProgram3 ****************************************************************************************************************************************************************************************************************************


void AnalogProgram4() // White
{
	if (currentMillis - previousAnalog >= 200)
	{
		previousAnalog = currentMillis;

		analogColour += 1;

		if (analogColour >= ANALOG_MAX)
		{
			analogColour = ANALOG_MIN;
			analogProgram++;
		}
		else
		{
			if (analogColour <= ANALOG_MIN)
				analogColour = ANALOG_MIN;

			SetAnalog(analogColour, analogColour, analogColour);
		}
	}
} // AnalogProgram4 ****************************************************************************************************************************************************************************************************************************


void AnalogProgram5() // 3 Step, RGB
{

	if (currentMillis - previousAnalog >= 500)
	{
		previousAnalog = currentMillis;

		analogStep += 1;

		if (analogStep > 8)
		{
			analogStep = 0;
			analogColour = ANALOG_MIN;
			analogProgram++;
		}
		else
		{
			if (analogStep == 0)
				SetAnalog(ANALOG_MAX, 0, 0);
			else if (analogStep == 1)
				SetAnalog(0, ANALOG_MAX, 0);
			else if (analogStep == 2)
				SetAnalog(0, 0, ANALOG_MAX);
			else if (analogStep == 3)
				SetAnalog(ANALOG_MAX, 0, 0);
			else if (analogStep == 4)
				SetAnalog(0, ANALOG_MAX, 0);
			else if (analogStep == 5)
				SetAnalog(0, 0, ANALOG_MAX);
			else if (analogStep == 6)
				SetAnalog(ANALOG_MAX, 0, 0);
			else if (analogStep == 7)
				SetAnalog(0, ANALOG_MAX, 0);
			else if (analogStep == 8)
				SetAnalog(0, 0, ANALOG_MAX);
		}
	}
} // AnalogProgram5 ****************************************************************************************************************************************************************************************************************************


void AnalogProgram6() // 8 Step
{
	if (currentMillis - previousAnalog >= 600)
	{
		previousAnalog = currentMillis;

		analogStep += 1;

		if (analogStep > 8)
		{
			analogStep = 0;
			analogColour = ANALOG_MIN;
			analogProgram++;
		}
		else
		{
			if (analogStep == 0)
				SetAnalog(ANALOG_MAX, 0, 0);
			else if (analogStep == 1)
				SetAnalog(0, ANALOG_MAX, 0);
			else if (analogStep == 2)
				SetAnalog(0, 0, ANALOG_MAX);
			else if (analogStep == 3)
				SetAnalog(ANALOG_MAX, ANALOG_MAX, 0);
			else if (analogStep == 4)
				SetAnalog(ANALOG_MAX, 0, ANALOG_MAX);
			else if (analogStep == 5)
				SetAnalog(0, ANALOG_MAX, ANALOG_MAX);
			else if (analogStep == 6)
				SetAnalog(100, 150, ANALOG_MAX);
			else if (analogStep == 7)
				SetAnalog(ANALOG_MAX, 150, 100);
			else if (analogStep == 8)
				SetAnalog(ANALOG_MAX, ANALOG_MAX, ANALOG_MAX);
		}
	}
} // AnalogProgram6 ****************************************************************************************************************************************************************************************************************************


void AnalogProgram7()
{
	analogStep = 0;
	analogColour = ANALOG_MIN;
	analogProgram++;
} // AnalogProgram7 ****************************************************************************************************************************************************************************************************************************


void AnalogProgram8()
{
	analogStep = 0;
	analogColour = ANALOG_MIN;
	analogProgram++;
} // AnalogProgram8 ****************************************************************************************************************************************************************************************************************************


void SetAnalog(int red, int green, int blue)
{
	//analogWrite(PIN_RED, 255 - red);
	//analogWrite(PIN_GREEN, 255 - green);
	//analogWrite(PIN_BLUE, 255 - blue);

	analogWrite(PIN_RED, red);
	analogWrite(PIN_GREEN, green);
	analogWrite(PIN_BLUE, blue);
} // SetAnalog ****************************************************************************************************************************************************************************************************************************



// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
	for (uint16_t i = 0; i < strip0.numPixels(); i++)
	{
		strip0.setPixelColor(i, c);
		strip0.show();
		delay(wait);
	}
}

void rainbow(uint8_t wait)
{
	uint16_t i, j;

	for (j = 0; j < 256; j++)
	{
		for (i = 0; i < strip0.numPixels(); i++)
		{
			strip0.setPixelColor(i, Wheel((i + j) & 255));
		}
		strip0.show();
		delay(wait);
	}
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
	uint16_t i, j;

	for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
		for (i = 0; i < strip0.numPixels(); i++)
		{
			strip0.setPixelColor(i, Wheel(((i * 256 / strip0.numPixels()) + j) & 255));
		}
		strip0.show();
		delay(wait);
	}
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
	for (int j = 0; j < 10; j++)
	{  //do 10 cycles of chasing
		for (int q = 0; q < 3; q++)
		{
			for (int i = 0; i < strip0.numPixels(); i = i + 3) {
				strip0.setPixelColor(i + q, c);    //turn every third pixel on
			}
			strip0.show();

			delay(wait);

			for (int i = 0; i < strip0.numPixels(); i = i + 3)
			{
				strip0.setPixelColor(i + q, 0);        //turn every third pixel off
			}
		}
	}
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
	for (int j = 0; j < 256; j++)
	{     // cycle all 256 colors in the wheel
		for (int q = 0; q < 3; q++)
		{
			for (int i = 0; i < strip0.numPixels(); i = i + 3) {
				strip0.setPixelColor(i + q, Wheel((i + j) % 255));    //turn every third pixel on
			}
			strip0.show();

			delay(wait);

			for (int i = 0; i < strip0.numPixels(); i = i + 3) {
				strip0.setPixelColor(i + q, 0);        //turn every third pixel off
			}
		}
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return strip0.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	else if (WheelPos < 170)
	{
		WheelPos -= 85;
		return strip0.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	else
	{
		WheelPos -= 170;
		return strip0.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
}


// ****************************************************************************************************************************************************************************************************************************



// ****************************************************************************************************************************************************************************************************************************
// ****************************************************************************************************************************************************************************************************************************
