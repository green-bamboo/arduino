#include <Adafruit_NeoPixel.h>

#define PIN_NEO16	 4
#define PIN_NEO50	 5
#define PIN_NEO150	 6
#define PIN_LED				13

#define MAX		   55

Adafruit_NeoPixel heart = Adafruit_NeoPixel(16, PIN_NEO16, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel body = Adafruit_NeoPixel(50, PIN_NEO50, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel fence = Adafruit_NeoPixel(300, PIN_NEO150, NEO_GRB + NEO_KHZ800);

unsigned long current = 0;
unsigned long ledPrevious = 0;
unsigned long previous = 0;
int led = 0;
int incomingByte;      // a variable to read incoming serial data into
String mode = "off";

void setup()
{
	Serial.begin(9600);

	heart.begin();
	body.begin();
	fence.begin();

	//heart.setBrightness(255);
	//heart.show();

	//body.setBrightness(255);
	//body.show();

	//fence.setBrightness(255);
	//fence.show();

	AllOff();

	mode = "eyes";
} // setup ****************************************************************************************************************************************************************************************************************************


void loop()
{

	if (Serial.available() > 0)
	{
		// read the oldest byte in the serial buffer:
		incomingByte = Serial.read();

		if (incomingByte == 'O')
		{
			mode = "off";
			Serial.println("AllOff");
			AllOff();
		}
		else if (incomingByte == 'A')
		{
			mode = "on";
			Serial.println("AllOn");
			AllOn();
		}
		else if (incomingByte == 'B')
		{
			mode = "body";
			Serial.println("Body");
			Body();
		}
		else if (incomingByte == 'E')
		{
			mode = "eyes";
			led = 0;
			Serial.println("Eyes");
			AllOff();
			Eyes();
		}
		else if (incomingByte == 'M')
		{
			mode = "mouth";
			led = 0;
			Serial.println("Mouth");
			AllOff();
			Mouth();
		}
		else if (incomingByte == 'G')
		{
			//Serial.println("GrowlFace");
			//GrowlFace();
		}
		else if (incomingByte == 'F')
		{
			//Serial.println("FlashFaces");
			//FlashFaces();
		}
		else if (incomingByte == 'T')
		{
			//Serial.println("Transmute");
			//Transmute();
		}
	}

	//

	current = millis();

	// Simple half second for LED

	if (current - ledPrevious >= 500)
	{
		ledPrevious = current;

		led++;
		Serial.println("LED");

		if (led == 1 || led == 3)
			digitalWrite(PIN_LED, HIGH);
		else
			digitalWrite(PIN_LED, LOW);

		if (led >= 7)
			led = 0;
	}

	//

	if (mode == "eyes")
	{
		Eyes();
	}
	else if (mode == "mouth")
	{
		Mouth();
	}

	//

} // loop ****************************************************************************************************************************************************************************************************************************


void AllOff()
{
	SetColour(&heart, 0, 0, 0);
	SetColour(&body, 0, 0, 0);
	SetColour(&fence, 0, 0, 0);
} // AllOff ****************************************************************************************************************************************************************************************************************************


void AllOn()
{
	SetColour(&heart, MAX, MAX, MAX);
	SetColour(&body, MAX, MAX, MAX);
	SetColour(&fence, MAX, MAX, MAX);
} // AllOn ****************************************************************************************************************************************************************************************************************************


void Body()
{
	SetColour(&heart, 0, 0, 0);
	SetColour(&body, MAX, MAX, MAX);
	SetColour(&fence, 0, 0, 0);

	uint32_t black = body.Color(0, 0, 0);

	body.setPixelColor(25, black);

	for (int i = 26; i < 38; i++)
		body.setPixelColor(i, black);

	body.show();


} // Body ****************************************************************************************************************************************************************************************************************************


void SetColour(Adafruit_NeoPixel *strip, int red, int green, int blue)
{

	uint32_t colour = strip->Color(red, green, blue);

	for (int i = 0; i < strip->numPixels(); i++)
	{
		strip->setPixelColor(i, colour);
	}

	strip->show();
}


void Eyes()
{

	if (current - ledPrevious >= 200)
	{
		ledPrevious = current;

		uint32_t colour = body.Color(0, 1, 0);
		int red = 0;

		led++;
		if (led >= 21)
			led = 0;

		if (led < 10)
			red = led;
		else
			red = 20 - led;

		// 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
		// 1 2 3 4 5 6 7 8 9 10  9  8  7  6  5  4  3  2  1  0

		colour = body.Color(red * 4, 0, 0);

		body.setPixelColor(27, colour);
		body.setPixelColor(30, colour);

		body.show();
	}
} // Eyes ****************************************************************************************************************************************************************************************************************************


void Mouth()
{
	if (current - ledPrevious >= 100)
	{
		ledPrevious = current;

		uint32_t colour = body.Color(0, MAX, 0);;

		body.setPixelColor(20, colour);
		body.setPixelColor(21, colour);
		body.setPixelColor(22, colour);
		body.setPixelColor(23, colour);
		body.setPixelColor(24, colour);

		body.show();
	}
} // Mouth ****************************************************************************************************************************************************************************************************************************


void WhiteFace()
{
	//growl = false;
} // WhiteFace ****************************************************************************************************************************************************************************************************************************


void GrowlFace()
{
	//growl = false;
} // GrowlFace ****************************************************************************************************************************************************************************************************************************


void FlashFaces()
{
	//red = true;

	for (int i = 0; i < 6; i++)
	{
		//SetSmile(ANALOG_MAX, ANALOG_MAX, ANALOG_MAX);
		//SetGrowl(ANALOG_MIN, ANALOG_MIN, ANALOG_MIN);

		//delay(200);

		//SetSmile(ANALOG_MIN, ANALOG_MIN, ANALOG_MIN);

		//	if (red)
		//	SetChannel(3, ANALOG_MAX);
		//SetGrowl(ANALOG_MAX, ANALOG_MIN, ANALOG_MIN);
		//else
		//SetChannel(4, ANALOG_MAX);
		//SetGrowl(ANALOG_MIN, ANALOG_MAX, ANALOG_MIN)//;

		//		red = !red;

		//	delay(200);
	}

} // FlashFaces ****************************************************************************************************************************************************************************************************************************


void Transmute()
{
	// wait for sound effects to finish

	//delay(3000);

	//WhiteFace();

	//FlashFaces();

	//GrowlFace();

	// growl = true;

} // Transmute ****************************************************************************************************************************************************************************************************************************


