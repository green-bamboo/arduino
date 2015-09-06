#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define VERSION			"Test OneWire , v1.2"

#define PIN_LED				13
#define PIN_BUS				8
#define WAIT_TIME			10 // wait time in seconds
#define SENSORS				2

char blueToothVal;           //value sent over via bluetooth
char lastValue;              //stores last state of device (on/off)
unsigned long currentMillis = 0;
unsigned long previousLED = 0;
unsigned long previousCheck = 0;
int ledCounter = 0;
int waitTime = WAIT_TIME * 1000;

OneWire oneWire(PIN_BUS);
DallasTemperature sensors(&oneWire);

// Test Colour Receiver

void setup()
{
	Serial.begin(9600);
	Serial.println(VERSION);

	pinMode(PIN_LED, OUTPUT);

	sensors.begin();

	previousLED = millis();
}


void loop()
{

	currentMillis = millis();

	if (currentMillis - previousLED > 100)
	{
		previousLED = currentMillis;

		if (++ledCounter > 10)
		{
			ledCounter = 0;
			Serial.println("Tick");
		}

		if (ledCounter == 1 || ledCounter == 2 || ledCounter == 4 || ledCounter == 5)
		{
			digitalWrite(PIN_LED, HIGH);
		}
		else
		{
			digitalWrite(PIN_LED, LOW);
		}

	}

	//

	if (currentMillis - previousCheck > waitTime)
	{
		previousCheck = currentMillis;

		Serial.println("Request");
		sensors.requestTemperatures();

		for (int i = 0; i < SENSORS; i++)
		{
			String output = "Sensor ";
			output += i;

			float tempC = sensors.getTempCByIndex(i);

			output += ", C=";
			output += tempC;

			float tempFvalue = DallasTemperature::toFahrenheit(tempC);
			char buffer[10];
			String tempF = dtostrf(tempFvalue, 4, 1, buffer);

			output += ", F=";
			output += tempF;

			Serial.println(output);
		}
	}

	//


}

