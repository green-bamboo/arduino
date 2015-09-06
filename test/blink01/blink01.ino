#include <LowPower.h>
/*
Blink
Turns on an LED on for one second, then off for one second, repeatedly.

This example code is in the public domain.
*/

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int mins = 1;

// the setup routine runs once when you press reset:
void setup() {
	Serial.begin(9600);
	// initialize the digital pin as an output.
	pinMode(led, OUTPUT);
	digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
}

// the loop routine runs over and over again forever:
void loop() {
	Serial.println("Loop");

	digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
	//LowPower.powerDown(SLEEP_500MS, ADC_OFF, BOD_OFF);
	delay(500);

	digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
	//LowPower.powerDown(SLEEP_500MS, ADC_OFF, BOD_OFF);
	delay(500);

	digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
	//LowPower.powerDown(SLEEP_500MS, ADC_OFF, BOD_OFF);
	delay(500);

	digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW

	for (int waiting = 0; waiting < (mins * 15); waiting++)
		LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
}