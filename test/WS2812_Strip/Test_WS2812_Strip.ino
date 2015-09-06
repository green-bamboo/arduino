#include <Adafruit_NeoPixel.h>

#define PIXELS	150
#define MAX		150
#define STEP	2

Adafruit_NeoPixel test = Adafruit_NeoPixel(PIXELS, 8, NEO_GRB + NEO_KHZ800);

int i;


void setup()
{
	Serial.begin(9600);

	Serial.println("Ready, v1.0");

	test.begin();

	SetStrips(0, 0, 0);
	ShowStrips();

}


void loop()
{

	// red

	for (i = 0; i < MAX; i++)
	{
		SetStrips(i, 0, 0);
		ShowStrips();
		delay(STEP);
	}

	for (i = MAX; i > 0; i--)
	{
		SetStrips(i, 0, 0);
		ShowStrips();
		delay(STEP);
	}

	// green

	for (i = 0; i < MAX; i++)
	{
		SetStrips(0, i, 0);
		ShowStrips();
		delay(STEP);
	}

	for (i = MAX; i > 0; i--)
	{
		SetStrips(0, i, 0);
		ShowStrips();
		delay(STEP);
	}

	// blue

	for (i = 0; i < MAX; i++)
	{
		SetStrips(0, 0, i);
		ShowStrips();
		delay(STEP);
	}

	for (i = MAX; i > 0; i--)
	{
		SetStrips(0, 0, i);
		ShowStrips();
		delay(STEP);
	}

	// white

	for (i = 0; i < MAX; i++)
	{
		SetStrips(i, i, i);
		ShowStrips();
		delay(STEP);
	}

	for (i = MAX; i > 0; i--)
	{
		SetStrips(i, i, i);
		ShowStrips();
		delay(STEP);
	}

	//

	//Pixel(31);
	//Pixel(35);

}


void Pixel(int pixel)
{

	SetStrips(0, 0, 0);

	// red

	for (i = 0; i < MAX; i++)
	{
		test.setPixelColor(pixel, i, 0, 0);
		ShowStrips();
		delay(STEP);
	}

	for (i = MAX; i > 0; i--)
	{
		test.setPixelColor(pixel, i, 0, 0);
		ShowStrips();
		delay(STEP);
	}

	// green

	for (i = 0; i < MAX; i++)
	{
		test.setPixelColor(pixel, 0, i, 0);
		ShowStrips();
		delay(STEP);
	}

	for (i = MAX; i > 0; i--)
	{
		test.setPixelColor(pixel, 0, i, 0);
		ShowStrips();
		delay(STEP);
	}

	// blue

	for (i = 0; i < MAX; i++)
	{
		test.setPixelColor(pixel, 0, 0, i);
		ShowStrips();
		delay(STEP);
	}

	for (i = MAX; i > 0; i--)
	{
		test.setPixelColor(pixel, 0, 0, i);
		ShowStrips();
		delay(STEP);
	}

	// white

	for (i = 0; i < MAX; i++)
	{
		test.setPixelColor(pixel, i, i, i);
		ShowStrips();
		delay(STEP);
	}

	for (i = MAX; i > 0; i--)
	{
		test.setPixelColor(pixel, i, i, i);
		ShowStrips();
		delay(STEP);
	}

	//

}



void ShowStrips()
{
	test.show();
} // ShowStrips ****************************************************************************************************************************************************************************************************************************


void SetColour(Adafruit_NeoPixel *strip, int red, int green, int blue)
{
	uint32_t colour = strip->Color(red, green, blue);

	for (int i = 0; i < strip->numPixels(); i++)
	{
		strip->setPixelColor(i, colour);
	}

} // SetColour ****************************************************************************************************************************************************************************************************************************


void SetColour(Adafruit_NeoPixel *strip, uint32_t colour)
{
	for (int i = 0; i < strip->numPixels(); i++)
	{
		strip->setPixelColor(i, colour);
	}

} // SetColour ****************************************************************************************************************************************************************************************************************************


void SetStrips(int red, int green, int blue)
{
	SetColour(&test, red, green, blue);
} // SetStrips ****************************************************************************************************************************************************************************************************************************
