#include <Adafruit_NeoPixel.h>
#include <RCSwitch.h>

#define PIN_LIGHTS		7
#define PIN_LED			13
#define MAX				20

unsigned long currentMillis = 0;
unsigned long previousLED = 0;
unsigned long treeOn = 0;
unsigned long treeOff = 0;
unsigned long pipeOn = 0;
unsigned long pipeOff = 0;
unsigned long sideOn = 0;
unsigned long sideOff = 0;
int ledCounter = 0;
int outdoor1 = 0;
int outdoor2 = 0;
int outdoor3 = 0;
int outdoor = 0;

Adafruit_NeoPixel lights = Adafruit_NeoPixel(6, PIN_LIGHTS, NEO_RGB + NEO_KHZ800);
RCSwitch mySwitch = RCSwitch();



void setup()
{
	Serial.begin(9600);

	pinMode(PIN_LED, OUTPUT);

	//mySwitch.setProtocol(2);
	//mySwitch.setPulseLength();
	mySwitch.setReceiveTolerance(30);
	mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2

	/*
		protocol 2
		NOT receive wall switches, Hall Light, Bedroom lights, they are Lighting 2, AC, On/Off

		DOES work -
		GardenFloodLight, Lighting 1, ARC, On/Off
		Sockets, Lighting 1, Energenie, On/Off
		Socket4_On  = 14635231
		Socket4_Off = 14635223

		6ch HAndheld
		1 5575939
		2 5575948
		3 5575951
		4 5575984
		5 5575987
		6 5575996
		*/

	//mySwitch.setProtocol()

	lights.begin();

	lights.setPixelColor(0, 0, 0, 0);
	lights.setPixelColor(1, 0, 0, 0);
	lights.setPixelColor(2, 0, 0, 0);
	lights.setPixelColor(3, 0, 0, 0);
	lights.setPixelColor(4, 0, 0, 0);
	lights.setPixelColor(5, 0, 0, 0);
	lights.show();

	Serial.println("Ready, 433Mhz, v1.4");

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
			//Serial.println("Tick");

			Check(1, treeOn);
			Check(2, pipeOn);
			Check(3, sideOn);
		}

		if (ledCounter == 1 || ledCounter == 2 || ledCounter == 4 || ledCounter == 5)
			digitalWrite(PIN_LED, HIGH);
		else
			digitalWrite(PIN_LED, LOW);

	}

	//

	if (mySwitch.available())
	{

		int value = -10; // mySwitch.getReceivedRawdata();

		//int value = mySwitch.getReceivedValue();

		output(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(), mySwitch.getReceivedProtocol());

		if (value == 0)
		{
			Serial.print("Unknown encoding");
		}
		else // if (value != 0)
		{
			unsigned long received = mySwitch.getReceivedValue();

			Serial.print("Received ");
			Serial.println(received);

			if (received == 10367240) // All Off
			{
				outdoor1 = 0;
				outdoor2 = 0;
				outdoor3 = 0;

				lights.setPixelColor(0, 0, 0, 0);
				lights.setPixelColor(1, 0, 0, 0);
				lights.setPixelColor(2, 0, 0, 0);
				lights.setPixelColor(3, 0, 0, 0);
				lights.setPixelColor(4, 0, 0, 0);
				lights.setPixelColor(5, 0, 0, 0);
				lights.show();
			}
			else if (received == 1048597 || received == 10367247) // Tree On + 1on
			{
				treeOn = currentMillis;
				outdoor1 = 2;
				TurnedOn(1);
			}
			else if (received == 1048596 || received == 10367239) // Tree Off + 1off
			{
				treeOff = currentMillis;
				outdoor1 = 1;
				TurnedOff(1);
			}
			else if (received == 1064981 || received == 10367243) // Pipe On + 2on
			{
				pipeOn = currentMillis;
				outdoor2 = 2;
				TurnedOn(2);
			}
			else if (received == 1064980 || received == 10367235) // Pipe Off + 2off
			{
				pipeOff = currentMillis;
				outdoor2 = 1;
				TurnedOff(2);
			}
			else if (received == 1052693 || received == 10367245) // Sideway On + 3on
			{
				sideOn = currentMillis;
				outdoor3 = 2;
				TurnedOn(3);
			}
			else if (received == 1052692 || received == 10367237) // Sideway Off + 3off
			{
				sideOff = currentMillis;
				outdoor3 = 1;
				TurnedOff(3);
			}
			else if (received == 4457493 || received == 10367246) // Garden Flood Light On
			{
				lights.setPixelColor(0, 30, 0, 0);
				lights.show();
			}
			else if (received == 4457492 || received == 10367238) // Garden Flood Light Off
			{
				lights.setPixelColor(0, 0, 30, 0);
				lights.show();
			}
		}

		mySwitch.resetAvailable();
	}

	//

}


void Check(int pixel, unsigned long timeOn)
{
	int seconds = (currentMillis - timeOn) / 1000;

	if (seconds > 60 && seconds < 240)
	{
		lights.setPixelColor(pixel, MAX, 0, MAX); // magenta
		lights.show();
	}
	else if (seconds > 240)
	{
		lights.setPixelColor(pixel, 0, 0, 0); // off
		lights.show();
	}

}


void TurnedOn(int pixel)
{
	lights.setPixelColor(pixel, MAX, 0, 0); // red
	lights.show();

}


void TurnedOff(int pixel)
{
	lights.setPixelColor(pixel, 0, 0, MAX); // blue
	lights.show();

}


/*
1048597 oPIR1 Tree
1064981 oPIR2 Pipe
1052693 oPIR2 Sideway

GardenFloodLight
On   4457493
Off  4457492

*/



void output(unsigned long decimal, unsigned int length, unsigned int delay, unsigned int* raw, unsigned int protocol) {

	if (decimal == 0) {
		Serial.print("Unknown encoding.");
	}
	else {
		char* b = dec2binWzerofill(decimal, length);
		Serial.print("Decimal: ");
		Serial.print(decimal);
		Serial.print(" (");
		Serial.print(length);
		Serial.print("Bit) Binary: ");
		Serial.print(b);
		Serial.print(" Tri-State: ");
		Serial.print(bin2tristate(b));
		Serial.print(" PulseLength: ");
		Serial.print(delay);
		Serial.print(" microseconds");
		Serial.print(" Protocol: ");
		Serial.println(protocol);
	}

	Serial.print("Raw data: ");
	for (int i = 0; i <= length * 2; i++) {
		Serial.print(raw[i]);
		Serial.print(",");
	}
	Serial.println();
	Serial.println();
}


static char* bin2tristate(char* bin) {
	char returnValue[50];
	int pos = 0;
	int pos2 = 0;
	while (bin[pos] != '\0' && bin[pos + 1] != '\0') {
		if (bin[pos] == '0' && bin[pos + 1] == '0') {
			returnValue[pos2] = '0';
		}
		else if (bin[pos] == '1' && bin[pos + 1] == '1') {
			returnValue[pos2] = '1';
		}
		else if (bin[pos] == '0' && bin[pos + 1] == '1') {
			returnValue[pos2] = 'F';
		}
		else {
			return "not applicable";
		}
		pos = pos + 2;
		pos2++;
	}
	returnValue[pos2] = '\0';
	return returnValue;
}


static char * dec2binWzerofill(unsigned long Dec, unsigned int bitLength){
	static char bin[64];
	unsigned int i = 0;

	while (Dec > 0) {
		bin[32 + i++] = (Dec & 1 > 0) ? '1' : '0';
		Dec = Dec >> 1;
	}

	for (unsigned int j = 0; j < bitLength; j++) {
		if (j >= bitLength - i) {
			bin[j] = bin[31 + i - (j - (bitLength - i))];
		}
		else {
			bin[j] = '0';
		}
	}
	bin[bitLength] = '\0';

	return bin;
}
