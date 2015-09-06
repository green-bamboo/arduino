// ask_receiver.pde
// -*- mode: C++ -*-
// Simple example of how to use RadioHead to receive messages
// with a simple ASK transmitter in a very simple way.
// Implements a simplex (one-way) receiver with an Rx-B1 module

#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

#define PIN_LED			13
#define PIN_RX			2
#define PIN_TX			11

RH_ASK driver;


void setup()
{
	Serial.begin(9600);	// Debugging only

	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, false);

	Serial.println("Test_RadioHead, v1.0");

	//driver.
	if (driver.init())
		Serial.println("init ok");
	else
		Serial.println("init failed");
}


void loop()
{
	uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
	uint8_t buflen = sizeof(buf);

	if (driver.recv(buf, &buflen)) // Non-blocking
	{
		int i;

		// Message with a good checksum received, dump it.
		// driver.printBuffer("Got:", buf, buflen);

		digitalWrite(PIN_LED, true); // Flash a light to show received good message

		// Message with a good checksum received, dump it.
		Serial.print("Got: [");

		for (i = 0; i < buflen; i++)
		{
			Serial.print(buf[i], HEX);
			Serial.print(" ");
		}
		Serial.println("]");

		digitalWrite(PIN_LED, false);

	}
}