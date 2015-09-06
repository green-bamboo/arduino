#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

#define VERSION			"SoftwareSerialTerminal, v1.0"

#define PIN_LED			13

#define MAX				30

char blueToothVal;           //value sent over via bluetooth
char lastValue;              //stores last state of device (on/off)
unsigned long currentMillis = 0;
unsigned long previousLED = 0;
int ledCounter = 0;


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

	mySerial.begin(9600); // 57600

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
		else
		{
			Serial.println(received);
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