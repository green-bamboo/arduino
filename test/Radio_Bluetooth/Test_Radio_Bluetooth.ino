#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

SoftwareSerial mySerial(4, 5); // RX, TX

#define VERSION			"Test_Radio_Bluetooth, v1.9"

#define PIN_LED			13
#define PIN_LIGHTS		8

#define MAX				30

char blueToothVal;           //value sent over via bluetooth
char lastValue;              //stores last state of device (on/off)
unsigned long currentMillis = 0;
unsigned long previousLED = 0;
int ledCounter = 0;

Adafruit_NeoPixel lights = Adafruit_NeoPixel(6, PIN_LIGHTS, NEO_RGB + NEO_KHZ800);


// Simple Terminal
/*
void setup()
{
Serial.begin(9600);
pinMode(LED, OUTPUT);

Serial.println("Test_Radio_Bluetooth, Terminal, v1.4");

mySerial.begin(9600); // 9600, 34800,57600
}


void loop()
{
if (Serial.available())
{
char letter = Serial.read(); //read it
Serial.print(letter);

if (letter == 'Z')
{
mySerial.println("Hello There");
}
else if (letter == '#')
{
mySerial.print("AT+VERSION");
}
else
{
mySerial.print(letter);
}
}

if (mySerial.available())
{
blueToothVal = mySerial.read(); //read it
Serial.print(blueToothVal);
}

//delay(100);
}
*/



// Test Colour Receiver

void setup()
{
	Serial.begin(9600);
	pinMode(PIN_LED, OUTPUT);

	Serial.println(VERSION);
	Serial.println(", ColourReceiver");

	mySerial.begin(9600); // 57600

	lights.begin();

	lights.setPixelColor(0, 0);
	lights.setPixelColor(1, 0);
	lights.setPixelColor(2, 0);
	lights.setPixelColor(3, 0);
	lights.setPixelColor(4, 0);
	lights.setPixelColor(5, 0);
	lights.show();

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
			lights.setPixelColor(0, 0, 0, MAX);
		}
		else
		{
			digitalWrite(PIN_LED, LOW);
			lights.setPixelColor(0, 0, 0, 0);
		}

		lights.show();
	}

	//

	bool didReceive = false;
	String received = "";

	while (mySerial.available())
	{
		didReceive = true;

		int value = mySerial.read();
		char letter = value;
		received.concat(letter);

		//Serial.print("Received ");
		//Serial.println(value);
		//Serial.print(", [");
		//Serial.print(blueToothVal);
		//Serial.println("]");
	}

	if (didReceive)
	{

		if (received.indexOf("VERSION") >= 0)
		{
			Serial.println(VERSION);
		}
		else if (received.startsWith("00"))
		{
			lights.setPixelColor(0, 0, MAX, 0);
			lights.setPixelColor(1, 0);
			lights.setPixelColor(2, 0);
			lights.setPixelColor(3, 0);
			lights.setPixelColor(4, 0);
			lights.setPixelColor(5, 0);
			lights.show();
		}
		else
		{
			Serial.print("Received=");
			Serial.println(received);

			// 01234567890123
			// 1,rrr,ggg,bbb
			lights.setPixelColor(0, MAX, 0, 0);

			int pixel = -1;
			uint32_t colour = 0;

			if (received.length() == 13)
			{
				//Serial.println(received.substring(0, 1));
				//Serial.println(received.substring(2, 5));
				//Serial.println(received.substring(6, 9));
				//Serial.println(received.substring(10, 13));

				pixel = received.substring(0, 1).toInt();
				int red = received.substring(2, 5).toInt();
				int green = received.substring(6, 9).toInt();
				int blue = received.substring(10, 13).toInt();

				colour = lights.Color(red, green, blue);
				//lights.setPixelColor(pixel,)
			}
			else
			{
				if (received.startsWith("1"))
					pixel = 1;
				else if (received.startsWith("2"))
					pixel = 2;
				else if (received.startsWith("3"))
					pixel = 3;
				else if (received.startsWith("4"))
					pixel = 4;
				else if (received.startsWith("5"))
					pixel = 5;
				else if (received.startsWith("6"))
					pixel = 6;
				else if (received.startsWith("7"))
					pixel = 7;

				if (received.indexOf("r") >= 0 || received.indexOf("R") >= 0)
					colour = lights.Color(MAX, 0, 0);
				else if (received.indexOf("g") >= 0 || received.indexOf("G") >= 0)
					colour = lights.Color(0, MAX, 0);
				else if (received.indexOf("b") >= 0 || received.indexOf("B") >= 0)
					colour = lights.Color(0, 0, MAX);
			}

			//Serial.print("Pixel=");
			//Serial.print(pixel);
			//Serial.print("  Colour=");
			//Serial.println(colour);

			if (pixel >= 1 && pixel <= 5)
				lights.setPixelColor(pixel, colour);

			lights.show();
		}
	}

}



// Simple Test receiver
/*
void setup()
{
Serial.begin(9600);
pinMode(LED, OUTPUT);

Serial.println("Test_Radio_Bluetooth, Receiver, v1.3");

mySerial.begin(9600); // 57600
}


void loop()
{
if (mySerial.available())
{//if there is data being recieved
int value = mySerial.read(); //read it
blueToothVal = value;
Serial.print("Received ");
Serial.println(value);
Serial.print(", [");
Serial.print(blueToothVal);
Serial.println("]");
}

if (blueToothVal == 'n')
{//if value from bluetooth serial is n
digitalWrite(LED, HIGH);            //switch on LED
if (lastValue != 'n')
Serial.println(F("LED is on")); //print LED is on
lastValue = blueToothVal;
}
else if (blueToothVal == 'f')
{//if value from bluetooth serial is n
digitalWrite(LED, LOW);             //turn off LED
if (lastValue != 'f')
Serial.println(F("LED is off")); //print LED is on
lastValue = blueToothVal;
}

//delay(1000);
}
*/


/*
void setup()
{
Serial.begin(9600);
pinMode(LED, OUTPUT);
}


void loop()
{
if (Serial.available())
{//if there is data being recieved
blueToothVal = Serial.read(); //read it
}

if (blueToothVal == 'n')
{//if value from bluetooth serial is n
digitalWrite(LED, HIGH);            //switch on LED
if (lastValue != 'n')
Serial.println(F("LED is on")); //print LED is on
lastValue = blueToothVal;
}
else if (blueToothVal == 'f')
{//if value from bluetooth serial is n
digitalWrite(LED, LOW);             //turn off LED
if (lastValue != 'f')
Serial.println(F("LED is off")); //print LED is on
lastValue = blueToothVal;
}
delay(1000);
}

*/