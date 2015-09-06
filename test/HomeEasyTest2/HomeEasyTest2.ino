/*
Simple example for receiving

http://code.google.com/p/rc-switch/
*/

#include <RCSwitch.h>

#define PIN_LED		13

unsigned long currentMillis = 0;

bool ledActive = false;
int ledCounter = 0;
unsigned long previousLED = 0;

RCSwitch mySwitch = RCSwitch();

void setup() {
	pinMode(PIN_LED, OUTPUT);
	Serial.begin(9600);
	mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2
	Serial.println("Ready");
}

void loop() {

	currentMillis = millis();

	// Simple half second for LED

	if (currentMillis - previousLED >= 100)
	{
		//if (mySwitch.available())
		//	Serial.println("LED_A");
		//else
		//	Serial.println("LED_x");

		previousLED = currentMillis;

		if (++ledCounter >= 10)
			ledCounter = 0;

		if (ledActive)
		{
			if (ledCounter == 1 || ledCounter == 3 || ledCounter == 5)
				digitalWrite(PIN_LED, HIGH);
			else
				digitalWrite(PIN_LED, LOW);
		}
		else
		{
			if (ledCounter == 1 )
				digitalWrite(PIN_LED, HIGH);
			else
				digitalWrite(PIN_LED, LOW);
		}


	}


	if (mySwitch.available()) {

		int value = mySwitch.getReceivedValue();

		if (value == 0) {
			Serial.print("Unknown encoding");
		}
		else {
			unsigned long received = mySwitch.getReceivedValue();

			Serial.print("Received ");
			Serial.print(received);
			Serial.print(" / ");
			Serial.print(mySwitch.getReceivedBitlength());
			Serial.print("bit ");
			Serial.print("Protocol: ");
			Serial.println(mySwitch.getReceivedProtocol());

			if (received == 14614751) // On
				ledActive = true;
			else if (received == 14614743) // Off
				ledActive = false;

		}

		mySwitch.resetAvailable();
	}
}