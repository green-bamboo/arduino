#include <RCSwitch.h>

#define ACTOFF		14614743
#define ACT101		14614751

#define PIN_RED		 9
#define PIN_GREEN	10
#define PIN_BLUE	11
#define PIN_LED		13

#define ANALOG_MIN	 0
#define ANALOG_MAX	120
#define FADEUP		180

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int ID = 0;
int ledCounter = 0;
unsigned long previousLED = 0;
int currentAct = 101;
int previousAct = -1;
int currentScene = 0;
int previousScene = -1;
int stage = 0;
int counter = 0;
int select = 0;

RCSwitch mySwitch = RCSwitch();

void setup()
{
	Serial.begin(9600);
	pinMode(PIN_LED, OUTPUT);
	//pinMode(PIN_RED,OUTPUT);
	//pinMode(PIN_GREEN, OUTPUT);
	//pinMode(PIN_BLUE, OUTPUT);
	mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2

	if (analogRead(A1) > 200 && analogRead(A2) < 100)
		ID = 1;
	else if (analogRead(A1) < 100 && analogRead(A2) > 200)
		ID = 2;

	Serial.print("This ID = ");
	Serial.println(ID);
}


void loop()
{
	currentMillis = millis();

	// Heart beat LED

	if (currentMillis - previousLED >= 100)
	{
		previousLED = currentMillis;

		if (++ledCounter >= 10)
			ledCounter = 0;

		if (currentAct <= 0)
		{
			if (ledCounter == 1)
				digitalWrite(PIN_LED, HIGH);
			else
				digitalWrite(PIN_LED, LOW);
		}
		else if (currentAct <= 100)
		{
			if (ledCounter == 1 || ledCounter == 3)
				digitalWrite(PIN_LED, HIGH);
			else
				digitalWrite(PIN_LED, LOW);

		}
		else
		{
			if (ledCounter == 1 || ledCounter == 3 || ledCounter == 5)
				digitalWrite(PIN_LED, HIGH);
			else
				digitalWrite(PIN_LED, LOW);
		}

	}

	// check for serial comms

	if (Serial.available() > 0)
	{
		int input = Serial.parseInt();

		if (input >= 0 && input < 1001)
		{
			currentAct = input;
		}
	}

	// check for radio comms

	if (mySwitch.available())
	{

		int value = mySwitch.getReceivedValue();

		if (value == 0)
		{
			// Serial.print("Unknown encoding");
		}
		else
		{
			unsigned long received = mySwitch.getReceivedValue();

			//Serial.print("Received ");
			//Serial.print(received);
			//Serial.print(" / ");
			//Serial.print(mySwitch.getReceivedBitlength());
			//Serial.print("bit ");
			//Serial.print("Protocol: ");
			//Serial.println(mySwitch.getReceivedProtocol());

			if (received == ACT101) // On
				currentAct = 101;
			else if (received == ACTOFF) // Off
				currentAct = 0;

		}

		mySwitch.resetAvailable();
	}

	//

	if (currentAct > 0 && currentAct < 100)
		SimpleAct();
	else if (currentAct == 101)
		Act101();
	else if (currentAct == 102)
		Act102();
	else if (currentAct == 103)
		Act103();
	else
		currentAct = 0;

	previousAct = currentAct;

	//

} // loop ***************************************************************************************************************************************************************************************************************************************


void SetStrip(int red, int green, int blue)
{
	if (red < ANALOG_MIN)
		red = ANALOG_MIN;

	if (red > ANALOG_MAX)
		red = ANALOG_MAX;

	if (green < ANALOG_MIN)
		green = ANALOG_MIN;

	if (green > ANALOG_MAX)
		green = ANALOG_MAX;

	if (blue < ANALOG_MIN)
		blue = ANALOG_MIN;

	if (blue > ANALOG_MAX)
		blue = ANALOG_MAX;

	analogWrite(PIN_RED, 255 - red);
	analogWrite(PIN_GREEN, 255 - green);
	analogWrite(PIN_BLUE, 255 - blue);

	//analogWrite(PIN_RED, red);
	//analogWrite(PIN_GREEN, green);
	//analogWrite(PIN_BLUE, blue);
} // SetStrip ****************************************************************************************************************************************************************************************************************************



void SimpleAct()
{
	if (currentAct == previousAct)
		return;

	//

	int inc = 10;

	if (currentAct == 0) // all off
	{
		SetStrip(0, 0, 0);
	}
	else if (currentAct >= 1 && currentAct <= 10) // white
	{
		int level = currentAct * inc;
		SetStrip(level, level, level);
	}
	else if (currentAct >= 21 && currentAct <= 30) // red
	{
		int level = (currentAct - 20) * inc;
		SetStrip(level, 0, 0);
	}
	else if (currentAct >= 31 && currentAct <= 40) // green
	{
		int level = (currentAct - 30) * inc;
		SetStrip(0, level, 0);
	}
	else if (currentAct >= 41 && currentAct <= 50) // blue
	{
		int level = (currentAct - 40) * inc;
		SetStrip(0, 0, level);
	}

	//

	previousAct = currentAct;

} // SimpleAct ***************************************************************************************************************************************************************************************************************************************


void Act101()
{
	if (previousAct != currentAct)
	{
		//previousAct = currentAct;
		previousScene = -1;
		currentScene = 1;
		stage = 0;
		counter = 0;
	}
	else if (currentScene == 1)
	{
		FadeUpWhite();
	}
	else if (currentScene == 2)
	{
		FadeDownWhite();
	}
	else if (currentScene == 3)
	{
		Rainbow1();
	}
	else
	{
		StartScene();
	}
} // Act101 ***************************************************************************************************************************************************************************************************************************************


void Act102()
{
	if (previousAct != currentAct)
	{
		previousAct = currentAct;
		previousScene = -1;
		currentScene = 1;
		stage = 0;
		counter = 0;
	}
	else if (currentScene == 1)
	{
		Action4();
	}
	else if (currentScene == 2)
	{
		Action5();
	}
	else if (currentScene == 3)
	{
		Action6();
	}
	else if (currentScene == 4)
	{
		Action7();
	}
	else if (currentScene == 5)
	{
		Action8();
	}
	else if (currentScene == 6)
	{
		Action9();
	}
	else if (currentScene == 7)
	{
		FadeUpWhite();
	}
	else if (currentScene == 8)
	{
		FadeDownWhite();
	}
	else if (currentScene == 9)
	{
		Flash1();
	}
	else if (currentScene == 10)
	{
		FlashRed();
	}
	else if (currentScene == 11)
	{
		FlashGreen();
	}
	else if (currentScene == 12)
	{
		FlashBlue();
	}
	else if (currentScene == 13)
	{
		FlashWhite();
	}
	else if (currentScene == 14)
	{
		Rainbow6();
	}
	else
	{
		StartScene();
	}

} // Act102 ***************************************************************************************************************************************************************************************************************************************


void Act103()
{
} // Act103 ***************************************************************************************************************************************************************************************************************************************


void StartScene()
{
	previousScene = currentScene;
	currentScene = 1;
	stage = 0;
	counter = 0;
	select = 0;
} // StartScene ***************************************************************************************************************************************************************************************************************************************


void NextScene()
{
	previousScene = currentScene;
	currentScene++;
	stage = 0;
	counter = 0;
	select = 0;
} // NextScene ***************************************************************************************************************************************************************************************************************************************


void Action4()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (++counter > FADEUP)
		{
			NextScene();
		}
		else
		{
			int level = counter;
			SetStrip(level, 0, 0);
		}
	}

} // Action4 ***************************************************************************************************************************************************************************************************************************************


void Action5()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (++counter > FADEUP)
		{
			NextScene();
		}
		else
		{
			int level = FADEUP - counter;
			SetStrip(level, 0, 0);
		}
	}

} // Action5 ***************************************************************************************************************************************************************************************************************************************


void Action6()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (++counter > FADEUP)
		{
			NextScene();
		}
		else
		{
			int level = counter;
			SetStrip(0, level, 0);
		}
	}

} // Action6 ***************************************************************************************************************************************************************************************************************************************


void Action7()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (++counter > FADEUP)
		{
			NextScene();
		}
		else
		{
			int level = FADEUP - counter;
			SetStrip(0, level, 0);
		}
	}

} // Action7 ***************************************************************************************************************************************************************************************************************************************


void Action8()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (++counter > FADEUP)
		{
			NextScene();
		}
		else
		{
			int level = counter;
			SetStrip(0, 0, level);
		}
	}

} // Action8 ***************************************************************************************************************************************************************************************************************************************


void Action9()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (++counter > FADEUP)
		{
			NextScene();
		}
		else
		{
			int level = FADEUP - counter;
			SetStrip(0, 0, level);
		}
	}

} // Action9 ***************************************************************************************************************************************************************************************************************************************



void FadeUpWhite()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (++counter > FADEUP)
		{
			NextScene();
		}
		else
		{
			int level = counter;
			SetStrip(level, level, level);
		}
	}

} // FadeUpWhite ***************************************************************************************************************************************************************************************************************************************


void FadeDownWhite()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (++counter > FADEUP)
		{
			NextScene();
		}
		else
		{
			int level = FADEUP - counter;
			SetStrip(level, level, level);
		}
	}

} // FadeDownWhite ***************************************************************************************************************************************************************************************************************************************


void Flash1()
{
	if (currentMillis - previousMillis > 200)
	{
		previousMillis = currentMillis;

		if (++select > 4)
			select = 1;

		if (++stage > 8)
			stage = 1;

		if (++counter > 200)
		{
			NextScene();
		}
		else
		{
			if (stage == 1 || stage == 2 || stage == 4 || stage == 5)
			{
				if (select == 1)
					SetStrip(ANALOG_MAX, 0, 0);
				else if (select == 2)
					SetStrip(0, ANALOG_MAX, 0);
				else if (select == 3)
					SetStrip(0, 0, ANALOG_MAX);
				else if (select == 4)
					SetStrip(ANALOG_MAX, ANALOG_MAX, ANALOG_MAX);
			}
			else
				SetStrip(0, 0, 0);
		}
	}

} // Flash1 ***************************************************************************************************************************************************************************************************************************************


void FlashRed()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;


		if (++stage > 8)
		{
			stage = 1;
			select++;
		}

		if (++counter > 70)
		{
			NextScene();
		}
		else
		{
			if (stage == 1 || stage == 2 || stage == 4 || stage == 5)
				SetStrip(ANALOG_MAX, 0, 0);
			else
				SetStrip(0, 0, 0);
		}
	}

} // FlashRed ***************************************************************************************************************************************************************************************************************************************


void FlashGreen()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;


		if (++stage > 8)
		{
			stage = 1;
			select++;
		}

		if (++counter > 70)
		{
			NextScene();
		}
		else
		{
			if (stage == 1 || stage == 2 || stage == 4 || stage == 5)
				SetStrip(0, ANALOG_MAX, 0);
			else
				SetStrip(0, 0, 0);
		}
	}

} // FlashGreen ***************************************************************************************************************************************************************************************************************************************


void FlashBlue()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;


		if (++stage > 8)
		{
			stage = 1;
			select++;
		}

		if (++counter > 70)
		{
			NextScene();
		}
		else
		{
			if (stage == 1 || stage == 2 || stage == 4 || stage == 5)
				SetStrip(0, 0, ANALOG_MAX);
			else
				SetStrip(0, 0, 0);
		}
	}

} // FlashBlue ***************************************************************************************************************************************************************************************************************************************


void FlashWhite()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;


		if (++stage > 8)
		{
			stage = 1;
			select++;
		}

		if (++counter > 70)
		{
			NextScene();
		}
		else
		{
			if (stage == 1 || stage == 2 || stage == 4 || stage == 5)
				SetStrip(ANALOG_MAX, ANALOG_MAX, ANALOG_MAX);
			else
				SetStrip(0, 0, 0);
		}
	}

} // FlashWhite ***************************************************************************************************************************************************************************************************************************************


int rred;
int rgreen;
int rblue;
void Rainbow6()
{
	if (currentMillis - previousMillis > 250)
	{
		previousMillis = currentMillis;

		if (counter == 0)
		{
			rred = 100;
			rgreen = 30;
			rblue = 200;
		}

		if (++counter > 100)
		{
			NextScene();
		}
		else
		{
			rred += 20;
			rgreen += 7;
			rblue += 44;

			if (rred > 255)
				rred = 20;

			if (rgreen > 250)
				rgreen = 10;

			if (rblue > 255)
				rblue = 50;

			SetStrip(rred, rgreen, rblue);
		}
	}

} // Rainbow ***************************************************************************************************************************************************************************************************************************************


void Pause2Seconds()
{
	if (currentMillis - previousMillis > 5000)
	{
		previousMillis = currentMillis;

		if (++counter >= 4)
		{
			NextScene();
		}
	}

} // Pause2Seconds ***************************************************************************************************************************************************************************************************************************************


void Rainbow1()
{
	if (currentMillis - previousMillis > 50)
	{
		previousMillis = currentMillis;


		if (++counter > 256)
		{
			NextScene();
		}
		else
		{
			int red, green, blue;

			Wheel(counter, &red, &green, &blue);

			SetStrip(red, green, blue);
		}
	}

} // Rainbow1 ***************************************************************************************************************************************************************************************************************************************



void Wheel(byte WheelPos, int *red, int *green, int *blue)
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		*red = 255 - WheelPos * 3;
		*green = 0;
		*blue = WheelPos * 3;
	}
	else if (WheelPos < 170)
	{
		WheelPos -= 85;
		*red = 0;
		*green = WheelPos * 3;
		*blue = 255 - WheelPos * 3;
	}
	else
	{
		WheelPos -= 170;
		*red = WheelPos * 3;
		*green = 255 - WheelPos * 3;
		*blue = 0;
	}

}  // Wheel ***************************************************************************************************************************************************************************************************************************************
