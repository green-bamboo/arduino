// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>
//#include <Wiring.h>

#define PIN_LIGHTS		7
#define PIN_LED			13
#define MAX				20


unsigned long currentMillis = 0;
unsigned long previousLED = 0;
int ledCounter = 0;


void setup()
{
	Serial.begin(9600);	// Debugging only
	Serial.println("Test_VirtualWire, v1.1");

	pinMode(PIN_LED, OUTPUT);

	// Initialise the IO and ISR
	vw_set_ptt_inverted(true); // Required for DR3100

	vw_set_rx_pin(2);

	vw_setup(64);	 // Bits per sec

	vw_rx_start();       // Start the receiver PLL running

}

void loop()
{
	uint8_t buf[VW_MAX_MESSAGE_LEN];
	uint8_t buflen = VW_MAX_MESSAGE_LEN;

	//

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
			digitalWrite(PIN_LED, HIGH);
		else
			digitalWrite(PIN_LED, LOW);

	}

	//

	if (vw_get_message(buf, &buflen)) // Non-blocking
	{
		int i;

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