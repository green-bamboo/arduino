#define PIN_LED			13
#define PIN_RELAY1		9
#define PIN_RELAY2		10
#define PIN_RELAY3		11
//#define PIN_RELAY4		8

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int ledCounter = 0;
unsigned long previousLED = 0;

bool on1 = false;
bool on2 = false;
bool on3 = false;

void setup()
{
	Serial.begin(9600);
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_RELAY1, OUTPUT);
	pinMode(PIN_RELAY2, OUTPUT);
	pinMode(PIN_RELAY3, OUTPUT);
	//	pinMode(PIN_RELAY4, OUTPUT);

	Out(PIN_RELAY1, 0);
	Out(PIN_RELAY2, 0);
	Out(PIN_RELAY3, 0);

}

void loop()
{

	currentMillis = millis();

	// Heart beat LED

	if (currentMillis - previousLED >= 100)
	{
		previousLED = currentMillis;

		if (++ledCounter >= 12)
			ledCounter = 0;

		if (ledCounter == 1)
			digitalWrite(PIN_LED, HIGH);
		else if (ledCounter == 4 && on1)
			digitalWrite(PIN_LED, HIGH);
		else if (ledCounter == 6 && on2)
			digitalWrite(PIN_LED, HIGH);
		else if (ledCounter == 8 && on3)
			digitalWrite(PIN_LED, HIGH);
		else
			digitalWrite(PIN_LED, LOW);
	}

	// check for serial comms

	if (Serial.available() > 0)
	{
		int input = Serial.parseInt();

		if (input == 0)
		{
			Out(PIN_RELAY1, 0);
			Out(PIN_RELAY2, 0);
			Out(PIN_RELAY3, 0);
		}
		else if (input == 1)
		{
			Out(PIN_RELAY1, 255);
		}
		else if (input == 2)
		{
			Out(PIN_RELAY2, 255);
		}
		else if (input == 3)
		{
			Out(PIN_RELAY3, 255);
		}
		else if (input >= 1000 && input < 2000)
		{
			Out(PIN_RELAY1, input - 1000);
		}
		else if (input >= 2000 && input < 3000)
		{
			Out(PIN_RELAY2, input - 2000);
		}
		else if (input >= 3000 && input < 4000)
		{
			Out(PIN_RELAY3, input - 3000);
		}
	}


} // ************************************************************************************************************************************************************************************************************************************************************



void Out(int pin, int pwm)
{
	if (pwm < 0)
		pwm = 0;

	if (pwm > 255)
		pwm = 255;

	bool on = false;

	if (pwm == 0)
	{
		if (pin == PIN_RELAY1)
			on1 = false;
		else if (pin == PIN_RELAY2)
			on2 = false;
		else if (pin == PIN_RELAY3)
			on3 = false;
	}
	else
	{
		on = true;
		if (pin == PIN_RELAY1)
			on1 = true;
		else if (pin == PIN_RELAY2)
			on2 = true;
		else if (pin == PIN_RELAY3)
			on3 = true;
	}

	//analogWrite(pin, pwm);
	if (on)
		digitalWrite(pin, LOW);
	else
		digitalWrite(pin, HIGH);

} // ************************************************************************************************************************************************************************************************************************************************************


