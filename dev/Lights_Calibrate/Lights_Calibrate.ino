#include <Adafruit_NeoPixel.h>
#include <RCSwitch.h>

#define BUTTONOFF			10367240 
#define BUTTON1ON			10367247 
#define BUTTON1OFF			10367239 
#define BUTTON2ON			10367243 
#define BUTTON2OFF			10367235 
#define BUTTON3ON			10367245 
#define BUTTON3OFF			10367237 
#define BUTTON4ON			10367246 
#define BUTTON4OFF			10367238 

#define PIN_NEO			27
#define PIN_LED			13

#define MIN				0
#define LOW				20
#define MAX				170
#define STARBURST_MAX	30

#define TREE_PIXELS		100
#define GROUND_PIXELS	50
#define LEFT_PIXELS		150
#define TOP_PIXELS		300
#define RIGHT_PIXELS	150
#define PORCH_PIXELS	300
#define NEO_PIXELS		450

Adafruit_NeoPixel neo = Adafruit_NeoPixel(NEO_PIXELS, PIN_NEO, NEO_RGB + NEO_KHZ800);

Adafruit_NeoPixel tree = Adafruit_NeoPixel(TREE_PIXELS, 3, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel porch = Adafruit_NeoPixel(PORCH_PIXELS, 3, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel ground = Adafruit_NeoPixel(GROUND_PIXELS, 3, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel left = Adafruit_NeoPixel(LEFT_PIXELS, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel right = Adafruit_NeoPixel(RIGHT_PIXELS, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel top = Adafruit_NeoPixel(TOP_PIXELS, 3, NEO_GRB + NEO_KHZ800);


unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int ledCounter = 0;
unsigned long previousLED = 0;
int currentAct = 109;
int previousAct = -1;
int currentScene = 0;
int previousScene = -1;
int sceneDir = 0;
int stage = 0;
int position = 0;
int counter = 0;
int last = 0;

int leftpos = 0;
int rightpos = 0;
int toppos = 0;
int ppos = 0;


RCSwitch mySwitch = RCSwitch();


void setup()
{
	Serial.begin(9600);
	pinMode(PIN_LED, OUTPUT);

	mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2

	neo.begin();

	Serial.println("Ready");

	SetStrips(0, 0, 0);
	ShowStrips();
} // setup ****************************************************************************************************************************************************************************************************************************



void loop()
{

	currentMillis = millis();

	// Heart beat LED

	if (currentMillis - previousLED > 100)
	{
		previousLED = currentMillis;

		if (++ledCounter >= 9)
			ledCounter = 0;

		if (ledCounter == 1 || ledCounter == 3)
			digitalWrite(PIN_LED, HIGH);
		else
			digitalWrite(PIN_LED, LOW);

	}

	// check for serial comms

	if (Serial.available() > 0)
	{

		int input = Serial.parseInt();

		if (input >= 0 && input < 1001)
		{
			currentAct = input;
		}
	}

	// check for radio comms

	if (mySwitch.available())
	{

		int value = mySwitch.getReceivedValue();

		if (value == 0)
		{
			Serial.print("Unknown encoding");
		}
		else
		{
			unsigned long received = mySwitch.getReceivedValue();

			Serial.print("Received ");
			Serial.println(received);

			if (received == BUTTONOFF)
			{
				leftpos = 0;
				toppos = 0;
				rightpos = 0;
				ppos = 0;
			}

			else if (received == BUTTON1ON)
				leftpos = CALIBMIN(leftpos);
			else if (received == BUTTON1OFF)
				leftpos = CALIBMAX(leftpos, NEO_PIXELS);

			else if (received == BUTTON2ON)
			{
				leftpos -= 10;
				leftpos = CALIBMIN(leftpos);
			}
			else if (received == BUTTON2OFF)
			{
				leftpos += 10;
				leftpos = CALIBMAX(leftpos, NEO_PIXELS);
			}

			Serial.print("Pixel ");
			Serial.println(leftpos);

		}

		mySwitch.resetAvailable();
	}

	//

	Calibrate();

	//

} // loop ***************************************************************************************************************************************************************************************************************************************


int CALIBMIN(int pos)
{
	pos--;

	if (pos < 0)
		pos = 0;

	return pos;
} // CALIBMIN ***************************************************************************************************************************************************************************************************************************************


int CALIBMAX(int pos, int max)
{
	pos++;

	if (pos > max)
		pos = max;

	return pos;
} // CALIBMAX ***************************************************************************************************************************************************************************************************************************************


void Calibrate()
{
	if (currentMillis - previousMillis > 300)
	{
		previousMillis = currentMillis;

		SetStrips(0, 0, 0);

		for (int i = 0; i < leftpos; i++)
			neo.setPixelColor(i, MAX, MAX, MAX);


		neo.show();
	}

} // Calibrate ***************************************************************************************************************************************************************************************************************************************



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
	SetColour(&neo, red, green, blue);
} // SetStrips ****************************************************************************************************************************************************************************************************************************


void SetStripsBrightness(int level)
{
	neo.setBrightness(counter);
} // SetStripsBrightness ****************************************************************************************************************************************************************************************************************************


void ShowStrips()
{
	neo.show();
} // ShowStrips ****************************************************************************************************************************************************************************************************************************



