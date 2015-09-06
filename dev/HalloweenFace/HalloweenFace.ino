#define PIN_SMILE_RED		 3
#define PIN_SMILE_GREEN		 5
#define PIN_SMILE_BLUE		 6
#define PIN_GROWL_RED		 9
#define PIN_GROWL_GREEN		10
#define PIN_GROWL_BLUE		11
#define PIN_LED				13

#define ANALOG_MIN			 0
#define ANALOG_MAX		   255

String mode = "";
unsigned long current = 0;
unsigned long ledPrevious = 0;
unsigned long growlPrevious = 0;
int led = 0;
int incomingByte;      // a variable to read incoming serial data into
bool growl = false;
bool red = true;

void setup()
{
	Serial.begin(9600);

	pinMode(PIN_LED, OUTPUT);

	pinMode(PIN_SMILE_RED, OUTPUT);
	pinMode(PIN_SMILE_GREEN, OUTPUT);
	pinMode(PIN_SMILE_BLUE, OUTPUT);

	pinMode(PIN_GROWL_RED, OUTPUT);
	pinMode(PIN_GROWL_GREEN, OUTPUT);
	pinMode(PIN_GROWL_BLUE, OUTPUT);

	Off();
	mode = "O";

} // setup ****************************************************************************************************************************************************************************************************************************

void loop2()
{
	Off();
	mode = "O";

	delay(5000); // 5 seconds
	delay(5000); // 5 seconds

	//

	mode = "W";
	Serial.println("WhiteFace");
	WhiteFace();

	delay(5000); // 5 seconds

	//

	mode = "T";
	Serial.println("Transmute");
	Transmute();

	delay(2000); // 2 seconds

	Transmute();

	delay(2000); // 2 seconds

	Transmute();

	delay(2000); // 2 seconds

	Transmute();

	delay(2000); // 2 seconds

	//

	for (int i = 0; i < 20; i++)
	{
		Timer();
		delay(100);
	}

	//

	//mode = "R";
	//Serial.println("Rainbow");
	//Rainbow();

	//

}

void loop()
{

	if (Serial.available() > 0)
	{
		// read the oldest byte in the serial buffer:
		incomingByte = Serial.read();

		if (incomingByte == 'O')
		{
			mode = "O";
			Serial.println("Off");
			Off();
		}
		else if (incomingByte == 'W')
		{
			mode = "W";
			Serial.println("WhiteFace");
			WhiteFace();
		}
		else if (incomingByte == 'G')
		{
			mode = "G";
			Serial.println("GrowlFace");
			GrowlFace();
		}
		else if (incomingByte == 'F')
		{
			mode = "F";
			Serial.println("FlashFaces");
			FlashFaces();
		}
		else if (incomingByte == 'T')
		{
			mode = "T";
			Serial.println("Transmute");
			Transmute();
		}
	}

	Timer();

} // loop ****************************************************************************************************************************************************************************************************************************


void Timer()
{
	//

	current = millis();

	// Simple half second for LED

	if (current - ledPrevious >= 200)
	{
		ledPrevious = current;

		led++;
		//Serial.println("LED");

		uint8_t state = LOW;

		if (mode == "O") // off, once
		{
			if (led == 1 || led == 6)
				state = HIGH;
		}
		else if (mode == "T") // trasmuted, 4
		{
			if (led == 1 || led == 3 || led == 5 || led == 7)
				state = HIGH;
		}
		else if (mode == "W") // white smile , 2
		{
			if (led == 1 || led == 3)
				state = HIGH;
		}
		else if (mode != "W") // any other, 3
		{
			if (led == 1 || led == 3 || led == 5)
				state = HIGH;
		}

		digitalWrite(PIN_LED, state);

		if (led >= 12)
			led = 0;
	}

	// 2 second timer to swap red/green growl face

	if (growl && current - growlPrevious >= 2000)
	{
		growlPrevious = current;

		if (red)
		{
			Serial.println("Growl RED");
			SetGrowl(ANALOG_MAX, ANALOG_MIN, ANALOG_MIN);
		}
		else
		{
			Serial.println("Growl GREEN");
			SetGrowl(ANALOG_MIN, ANALOG_MAX, ANALOG_MIN);
		}

		red = !red;
	}

} // loop ****************************************************************************************************************************************************************************************************************************


void Rainbow()
{

	for (int analogStep = 0; analogStep < 9; analogStep++)
	{
		if (analogStep == 0)
			SetAnalog(ANALOG_MAX, 0, 0);
		else if (analogStep == 1)
			SetAnalog(0, ANALOG_MAX, 0);
		else if (analogStep == 2)
			SetAnalog(0, 0, ANALOG_MAX);
		else if (analogStep == 3)
			SetAnalog(ANALOG_MAX, ANALOG_MAX, 0);
		else if (analogStep == 4)
			SetAnalog(ANALOG_MAX, 0, ANALOG_MAX);
		else if (analogStep == 5)
			SetAnalog(0, ANALOG_MAX, ANALOG_MAX);
		else if (analogStep == 6)
			SetAnalog(100, 150, ANALOG_MAX);
		else if (analogStep == 7)
			SetAnalog(ANALOG_MAX, 150, 100);
		else if (analogStep == 8)
			SetAnalog(ANALOG_MAX, ANALOG_MAX, ANALOG_MAX);

		delay(500);
	}

}


void SetSmile(int red, int green, int blue)
{
	analogWrite(PIN_SMILE_RED, ANALOG_MAX - red);
	analogWrite(PIN_SMILE_GREEN, ANALOG_MAX - green);
	analogWrite(PIN_SMILE_BLUE, ANALOG_MAX - blue);
} // SetAnalog ****************************************************************************************************************************************************************************************************************************


void SetGrowl(int red, int green, int blue)
{
	analogWrite(PIN_GROWL_RED, ANALOG_MAX - red);
	analogWrite(PIN_GROWL_GREEN, ANALOG_MAX - green);
	analogWrite(PIN_GROWL_BLUE, ANALOG_MAX - blue);
} // SetAnalog ****************************************************************************************************************************************************************************************************************************


void SetAnalog(int red, int green, int blue)
{
	analogWrite(PIN_SMILE_RED, ANALOG_MAX - red);
	analogWrite(PIN_SMILE_GREEN, ANALOG_MAX - green);
	analogWrite(PIN_SMILE_BLUE, ANALOG_MAX - blue);

	analogWrite(PIN_GROWL_RED, ANALOG_MAX - red);
	analogWrite(PIN_GROWL_GREEN, ANALOG_MAX - green);
	analogWrite(PIN_GROWL_BLUE, ANALOG_MAX - blue);
} // SetAnalog ****************************************************************************************************************************************************************************************************************************


void Off()
{
	growl = false;
	SetSmile(ANALOG_MIN, ANALOG_MIN, ANALOG_MIN);
	SetGrowl(ANALOG_MIN, ANALOG_MIN, ANALOG_MIN);
} // Off ****************************************************************************************************************************************************************************************************************************


void WhiteFace()
{
	growl = false;
	SetSmile(ANALOG_MAX, ANALOG_MAX, ANALOG_MAX);
	SetGrowl(ANALOG_MIN, ANALOG_MIN, ANALOG_MIN);
} // WhiteFace ****************************************************************************************************************************************************************************************************************************


void GrowlFace()
{
	growl = false;
	SetSmile(ANALOG_MIN, ANALOG_MIN, ANALOG_MIN);
	SetGrowl(ANALOG_MIN, ANALOG_MAX, ANALOG_MIN);
} // GrowlFace ****************************************************************************************************************************************************************************************************************************


void FlashFaces()
{
	red = true;

	for (int i = 0; i < 6; i++)
	{
		SetSmile(ANALOG_MAX, ANALOG_MAX, ANALOG_MAX);
		SetGrowl(ANALOG_MIN, ANALOG_MIN, ANALOG_MIN);

		delay(200);

		SetSmile(ANALOG_MIN, ANALOG_MIN, ANALOG_MIN);

		if (red)
			SetGrowl(ANALOG_MAX, ANALOG_MIN, ANALOG_MIN);
		else
			SetGrowl(ANALOG_MIN, ANALOG_MAX, ANALOG_MIN);

		red = !red;

		delay(200);
	}

} // FlashFaces ****************************************************************************************************************************************************************************************************************************


void Transmute()
{
	// wait for sound effects to finish

	delay(3000);

	WhiteFace();

	FlashFaces();

	GrowlFace();

	growl = true;

} // Transmute ****************************************************************************************************************************************************************************************************************************


