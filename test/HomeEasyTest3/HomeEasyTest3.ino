/*
* HomeEasyAdv library demo program to show how to receive and process instructions
* from the HE300 remote control
* This shows how to call different functions depending on the action required.
* In this example it is just turning on and off a bunch of LEDs
* Written by Andrew Lindsay
*/

#include <HomeEasyAdv.h>

// Set the TX and RX pins you are using
#define TXPIN 3
#define RXPIN 2

//This can be the same as an existing remote control, makes setting up the HE302S sockets easier
// use the receiver test program from the playground to determine yours.
#define REMOTE_ADDRESS 1010123

// Define where the LEDs are connected
#define LEDPIN1 5
#define LEDPIN2 6
#define LEDPIN3 7
#define LEDPIN4 8

// For receive only, the remote address is ignored
HomeEasyAdv hea = HomeEasyAdv(RXPIN, TXPIN, REMOTE_ADDRESS);

// 4 buttons so 4 action to implement
#define NUM_ACTIONS 4

// Define the functions to go with the ON action
void(*action_on[NUM_ACTIONS])(void) = {
	action_on_1,
	action_on_2,
	action_on_3,
	action_on_4
};

// Define the functions to go with the OFF action
void(*action_off[NUM_ACTIONS])(void) = {
	action_off_1,
	action_off_2,
	action_off_3,
	action_off_4
};

// Setup the test LEDs
void setup()
{
	Serial.begin(9600);
	Serial.println("Ready");

	pinMode(LEDPIN1, OUTPUT);
	digitalWrite(LEDPIN1, LOW);
	pinMode(LEDPIN2, OUTPUT);
	digitalWrite(LEDPIN2, LOW);
	pinMode(LEDPIN3, OUTPUT);
	digitalWrite(LEDPIN4, LOW);
	pinMode(LEDPIN4, OUTPUT);
	digitalWrite(LEDPIN4, LOW);
}


// Sit and wait for commands to be recieved.
void loop()
{
	if (hea.waitForRxData() > 0) {
		// We have received a commmand from remote
		// Use this line to include debug output
		printResult(hea.sender, hea.groupFlag, hea.onFlag, hea.recipient);
		// Switch things on and off, controlled by HE300 remote
		//performAction(hea.sender, hea.groupFlag, hea.onFlag, hea.recipient);
		
	}
}

// Simple debug outpt - can be used to determine your HE300 remote control ID
void printResult(unsigned long sender, boolean groupFlag, boolean onFlag, unsigned int recipient)
{
	Serial.print("sendinf from remote ");
	Serial.print(sender);

	Serial.print(" to recipient ");
	Serial.println(recipient);

	Serial.println(groupFlag ? "Group command" : "No group");
	Serial.println(onFlag ? "ON" : "OFF");

	Serial.println();
}

// Process received command and call correct action function
void performAction(unsigned long sender, boolean group, boolean on, unsigned int recipient) {
	if (recipient >= 0 && recipient < NUM_ACTIONS && !group) {
		Serial.print(recipient, DEC);

		if (on) {
			(*action_on[recipient])();
			Serial.println(" ON");
		}
		else {
			(*action_off[recipient])();
			Serial.println(" OFF");
		}
	}
}


// A dummy function in case we dont want to use a particular input
void dummy_action(void) {
	// Does nothing 
	Serial.println("Dummy");
}

// Action for button 1 ON
void action_on_1(void) {
	digitalWrite(LEDPIN1, HIGH);
}

// Action for button 1 OFF
void action_off_1(void) {
	digitalWrite(LEDPIN1, LOW);
}

void action_on_2(void) {
	digitalWrite(LEDPIN2, HIGH);
}

void action_off_2(void) {
	digitalWrite(LEDPIN2, LOW);
}

void action_on_3(void) {
	digitalWrite(LEDPIN3, HIGH);
}

void action_off_3(void) {
	digitalWrite(LEDPIN3, LOW);
}

void action_on_4(void) {
	digitalWrite(LEDPIN4, HIGH);
}

void action_off_4(void) {
	digitalWrite(LEDPIN4, LOW);
}
