#include <Adafruit_NeoPixel.h>
#include <RCSwitch.h>

#define FULLHOUSE
//#define TEST1

#define ACTOFF			14636499 
#define ACT101			14632415 
#define ACT102			14632407 
#define ACT103			14632411 
#define ACT104			14632403 
#define ACT105			14614555
#define ACT106			14614666
#define ACT107			14614777
#define ACT108			14614888
#define ACT109			14614999
#define ACT999			14614751
#define BUTTONOFF		10367240 
#define BUTTON101		10367247 
#define BUTTON102		10367239 
#define BUTTON103		10367243 
#define BUTTON104		10367235 
#define BUTTONNEXTACT		10367237
#define BUTTONPREVACT		10367245
#define BUTTONNEXTSCENE		10367238 
#define BUTTONPREVSCENE		10367246 
#define BUTTON1ON			10367247 
#define BUTTON1OFF			10367247 
#define BUTTON2ON			10367247 
#define BUTTON2OFF			10367247 
#define BUTTON3ON			10367247 
#define BUTTON3OFF			10367247 
#define BUTTON4ON			10367247 
#define BUTTON4OFF			10367247 

#define PIN_NEOTREE		23
#define PIN_NEOGROUND	25
#define PIN_NEOLEFT		27
#define PIN_NEOTOP		29
#define PIN_NEORIGHT	31
#define PIN_NEOPORCH	33
#define PIN_LED			13
#define PIN_NET1		22
#define PIN_NET2		22
#define PIN_NET3		26
#define PIN_NET4		28
#define PIN_NET5		30
#define PIN_NET6		32

#define MIN				0
#define LOW				20
#define MAX				200
#define STARBURST_MAX	30

#ifdef FULLHOUSE
#define TREE_PIXELS		100
#define GROUND_PIXELS	50
#define LEFT_PIXELS		450
#define TOP_PIXELS		300
#define RIGHT_PIXELS	150
#define PORCH_PIXELS	300
#define HOUSE_PIXELS	600
#define ALL_PIXELS		1050
#endif

#ifdef TEST1
#define TREE_PIXELS		50
#define GROUND_PIXELS	50
#define LEFT_PIXELS		150
#define TOP_PIXELS		150
#define RIGHT_PIXELS	150
#define PORCH_PIXELS	50
#define HOUSE_PIXELS	450
#define ALL_PIXELS		600
#endif


Adafruit_NeoPixel tree = Adafruit_NeoPixel(TREE_PIXELS, PIN_NEOTREE, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel porch = Adafruit_NeoPixel(PORCH_PIXELS, PIN_NEOPORCH, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel ground = Adafruit_NeoPixel(GROUND_PIXELS, PIN_NEOGROUND, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel left = Adafruit_NeoPixel(LEFT_PIXELS, PIN_NEOLEFT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel right = Adafruit_NeoPixel(RIGHT_PIXELS, PIN_NEORIGHT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel top = Adafruit_NeoPixel(TOP_PIXELS, PIN_NEOTOP, NEO_GRB + NEO_KHZ800);


unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousRadio = 0;
unsigned long previousTree = 0;
unsigned long previousPorch = 0;
unsigned long sceneMillis = 0;
unsigned long SceneLength = 20 * 1000; // 20 seconds for scene

int ledCounter = 0;
unsigned long previousLED = 0;
int currentAct = 105;
int previousAct = -1;
int currentScene = 0;
int previousScene = -1;
int sceneDir = 0;
int stage = 0;
int position = 0;
int counter = 0;
int last = 0;

int treeCounter = 0;
int treeDir = 0;
int treeStage = 0;

int porchCounter = 0;
int porchDir = 0;

int porchStars[8][10] =
{
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }
};

int porchHorzs[8][10] =
{
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }
};

int porchVerts[8][10] =
{
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }
};

int porchBoxes[8][10] =
{
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }
};

int treeHorzs[8][10] =
{
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }
};

int treeVerts[8][10] =
{
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }
};


uint32_t black;
uint32_t white;

RCSwitch mySwitch = RCSwitch();


void setup()
{
	Serial.begin(9600);
	pinMode(PIN_LED, OUTPUT);
	pinMode(PIN_NET1, OUTPUT);
	pinMode(PIN_NET2, OUTPUT);
	pinMode(PIN_NET3, OUTPUT);
	pinMode(PIN_NET4, OUTPUT);
	pinMode(PIN_NET5, OUTPUT);
	pinMode(PIN_NET6, OUTPUT);

	mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2

	tree.begin();
	ground.begin();
	porch.begin();
	left.begin();
	top.begin();
	right.begin();

	black = tree.Color(0, 0, 0);
	white = tree.Color(MAX, MAX, MAX);
	//red = tree.Color(0, 0, 0);
	//green = tree.Color(0, 0, 0);
	//blue = tree.Color(0, 0, 0);

	randomSeed(analogRead(2) + analogRead(4) + millis());

	Serial.println("Ready");

	SetStrips(0, 0, 0);
	ShowStrips();
} // setup ****************************************************************************************************************************************************************************************************************************



void loop()
{

	currentMillis = millis();

	// Heart beat LED

	if (currentMillis - previousLED >= 100)
	{
		previousLED = currentMillis;

		if (++ledCounter >= 13)
			ledCounter = 0;

		if (ledCounter == 1)
			digitalWrite(PIN_LED, HIGH);

		/*if (currentAct > 0 && currentAct < 100 && (ledCounter == 3 || ledCounter == 4 || ledCounter == 5))
			digitalWrite(PIN_LED, HIGH);
			else if (currentAct == 101 && (ledCounter == 4))
			digitalWrite(PIN_LED, HIGH);
			else if (currentAct == 102 && (ledCounter == 4 || ledCounter == 6))
			digitalWrite(PIN_LED, HIGH);
			else if (currentAct == 103 && (ledCounter == 4 || ledCounter == 6 || ledCounter == 8))
			digitalWrite(PIN_LED, HIGH);
			else if (currentAct == 104 && (ledCounter == 4 || ledCounter == 6 || ledCounter == 8 || ledCounter == 10))
			digitalWrite(PIN_LED, HIGH);
			else if (currentAct > 200 && (ledCounter == 4 || ledCounter == 5 || ledCounter == 6 || ledCounter == 7 || ledCounter == 8))
			digitalWrite(PIN_LED, HIGH);
			else */
		digitalWrite(PIN_LED, LOW);

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
			Serial.print("Unknown encoding");
		}
		else
		{
			unsigned long received = mySwitch.getReceivedValue();

			Serial.print("Received ");
			Serial.println(received);

			if (received == ACTOFF || received == BUTTONOFF) // Off
				currentAct = 0;
			else 	if (received == ACT101 || received == BUTTON101) // On
				currentAct = 101;
			else 	if (received == ACT102 || received == BUTTON102)
			{
				currentAct = 102;
				StartScene();
			}
			else 	if (received == ACT103 || received == BUTTON103)
			{
				currentAct = 103;
				StartScene();
			}
			else 	if (received == ACT104 || received == BUTTON104)
			{
				currentAct = 104;
				StartScene();
			}
			else 	if (received == ACT105)
				currentAct = 105;
			else 	if (received == ACT106)
				currentAct = 106;
			else 	if (received == ACT107)
				currentAct = 107;
			else 	if (received == ACT108)
				currentAct = 108;
			else 	if (received == ACT109)
				currentAct = 109;
			else 	if (received == ACT999)
				currentAct = 999;
			else 	if (received == BUTTONPREVACT)
			{
				currentAct--;
				StartScene();
			}
			else if (received == BUTTONNEXTACT)
			{
				currentAct++;
				StartScene();
			}
			else 	if (received == BUTTONPREVSCENE)
			{
				currentAct = 999;
				currentScene--;
			}
			else 	if (received == BUTTONNEXTSCENE)
			{
				currentAct = 999;
				currentScene++;
			}
		}

		mySwitch.resetAvailable();
	}

	//

	//	if (currentMillis - previousRadio < 3)
	//		return;

	//	previousRadio = currentMillis;

	//

	if (currentAct >= 0 && currentAct < 100)
		SimpleAct();
	else if (currentAct == 101) // Main show
		Act101();
	else if (currentAct == 102) // Dull only
		Act102();
	else if (currentAct == 103)
		Act103();
	else if (currentAct == 104)
		Act104();
	else if (currentAct == 105) // Fade all colours
		Act105();
	else if (currentAct == 109) // Tree Only
		Act109();
	else if (currentAct == 999)
		ManualControl();
	else
		currentAct = 0;

	previousAct = currentAct;

	//

} // loop ***************************************************************************************************************************************************************************************************************************************


void SetColour(Adafruit_NeoPixel *strip, int red, int green, int blue)
{
	uint32_t colour = strip->Color(red, green, blue);

	for (int i = 0; i < strip->numPixels(); i++)
	{
		strip->setPixelColor(i, colour);
	}

} // SetColour ****************************************************************************************************************************************************************************************************************************


void SetColour(Adafruit_NeoPixel *strip, uint32_t colour)
{
	for (int i = 0; i < strip->numPixels(); i++)
	{
		strip->setPixelColor(i, colour);
	}

} // SetColour ****************************************************************************************************************************************************************************************************************************


void SetStrips(int red, int green, int blue)
{
	SetColour(&tree, red, green, blue);
	SetColour(&ground, red, green, blue);
	SetColour(&porch, red, green, blue);
	SetColour(&left, red, green, blue);
	SetColour(&right, red, green, blue);
	SetColour(&top, red, green, blue);
} // SetStrips ****************************************************************************************************************************************************************************************************************************


void SetStripsBrightness(int level)
{
	tree.setBrightness(counter);
	ground.setBrightness(counter);
	left.setBrightness(counter);
	top.setBrightness(counter);
	right.setBrightness(counter);
	porch.setBrightness(counter);
} // SetStripsBrightness ****************************************************************************************************************************************************************************************************************************


void SetHouse(int red, int green, int blue)
{
	SetColour(&left, red, green, blue);
	SetColour(&right, red, green, blue);
	SetColour(&top, red, green, blue);
} // SetStrips ****************************************************************************************************************************************************************************************************************************


void ShowStrips()
{
	// blank out the pixels from the porch that do not matchthe nice pattern
	porch.setPixelColor(51, black);
	porch.setPixelColor(71, black);
	porch.setPixelColor(75, black);

	tree.show();
	ground.show();
	porch.show();
	left.show();
	right.show();
	top.show();
} // ShowStrips ****************************************************************************************************************************************************************************************************************************


void ShowHouse()
{
	left.show();
	right.show();
	top.show();
} // ShowHouse ****************************************************************************************************************************************************************************************************************************


void SimpleAct()
{
	if (currentAct == previousAct)
		return;

	//

	int inc = 10;

	Serial.println(currentAct);

	if (currentAct == 0) // all off
	{
		SetStrips(0, 0, 0);
		ShowStrips();
	}
	else if (currentAct >= 1 && currentAct <= 10) // white
	{
		int level = currentAct * inc;
		SetStrips(level, level, level);
		ShowStrips();
	}
	else if (currentAct >= 21 && currentAct <= 30) // red
	{
		int level = (currentAct - 20) * inc;
		SetStrips(level, 0, 0);
		ShowStrips();
	}
	else if (currentAct >= 31 && currentAct <= 40) // green
	{
		int level = (currentAct - 30) * inc;
		SetStrips(0, level, 0);
		ShowStrips();
	}
	else if (currentAct >= 41 && currentAct <= 50) // blue
	{
		int level = (currentAct - 40) * inc;
		SetStrips(0, 0, level);
		ShowStrips();
	}

	//

	previousAct = currentAct;

} // SimpleAct ***************************************************************************************************************************************************************************************************************************************


void Act101()
{
	if (previousAct != currentAct)
	{
		Serial.println("Start 101");
		//previousAct = currentAct;
		previousScene = -1;
		currentScene = 1;
		stage = 0;
		counter = 0;
	}
	else
	{
		// change scene every 20 seconds
		if (currentMillis - sceneMillis > SceneLength)
		{
			NextScene();
		}

		if (currentScene != previousScene)
		{
			Serial.print("Scene ");
			Serial.print(currentScene);
			Serial.print(", (");
			Serial.print(previousScene);
			Serial.println(")");
		}

		if (currentScene == 1)
		{
			Glimmer(); // All over
		}
		else if (currentScene == 2)
		{
			Sparkle();
			TreeSparkle();
			PorchSparkle();
		}
		else if (currentScene == 3)
		{
			FadeDownWhite();
		}
		else if (currentScene == 4)
		{
			Star3();
		}
		else if (currentScene == 5)
		{
			Sparkle();
			TreeRocket();
			PorchFader();
		}
		else if (currentScene == 6)
		{
			FillUpWhite();
			TreeBerries();
			//PorchFillUp();
		}
		else if (currentScene == 7)
		{
			Twinkle(); // House
			TreeVert();
			PorchSparkle();
		}
		else if (currentScene == 8)
		{
			Sparkle();
			TreeVert();
			PorchRainbowAll();
		}
		else if (currentScene == 9)
		{
			Dull(); // All over
		}
		else if (currentScene == 10)
		{
			Sparkle();
			TreeRainbowAll();
			PorchStarTurn();
		}
		else if (currentScene == 11)
		{
			StarBurst();
			TreeSparkle();
			PorchStarTurn();
		}
		else if (currentScene == 12)
		{
			Divider();
			TreeHorz();
			PorchRainbowAll();
		}
		else if (currentScene == 13)
		{
			DomeBar();
			TreeRainbowAll();
		}
		else if (currentScene == 14)
		{
			FadeOutAll();
		}
		else if (currentScene == 15)
		{
			Star1();
		}
		else if (currentScene == 16)
		{
			Sparkle();
			TreeVert();
			PorchFader();
		}
		else if (currentScene == 17)
		{
			Star2();
		}
		else if (currentScene == 18)
		{
			HouseAntiClockWise();
			TreeBerries();
			PorchStarTurn();
		}
		else if (currentScene == 19)
		{
			HouseClockWise();
			TreeBerries();
			PorchStarTurn();
		}
		else if (currentScene == 19)
		{
			FillUpRed();
			TreeSpiral();
			PorchSparkle();
		}
		else if (currentScene == 20)
		{
			RainbowHousePixel(); // House
			TreeSpiral();
			PorchSparkle();
		}
		else if (currentScene == 21)
		{
			Disco(); // All over
		}
		else if (currentScene == 22)
		{
			DomeTrail();
			TreeSparkle();
			PorchFader();
		}
		else if (currentScene == 23)
		{
			Mash();
			TreeFader();
			PorchSparkle();
		}
		else if (currentScene == 24)
		{
			//RainbowHouseAll(); // House
			RainbowAll(); // All
		}
		//else if (currentScene == 25)
		//{
		//	Snow();
		//	TreeBerries();
		//}
		else if (currentScene == 25)
		{
			Sparkle();
			TreeRainbowAll();
			PorchRainbowAll();
		}
		else if (currentScene == 26)
		{
			RainbowHouseTwinkle();
			TreeSparkle();
			PorchSparkle();
		}
		/*
		else if (currentScene == 14)
		{
		Cylon1();
		}
		*/
		else
		{
			StartScene();
		}
	}
} // Act101 ***************************************************************************************************************************************************************************************************************************************


void Act102()
{
	if (previousAct != currentAct)
	{
		Serial.println("Start 102");
		previousAct = currentAct;
		previousScene = -1;
		currentScene = 1;
		stage = 0;
		counter = 0;
	}
	else
	{
		Dull();
	}

} // Act102 ***************************************************************************************************************************************************************************************************************************************


void Act103()
{
	if (previousAct != currentAct)
	{
		Serial.println("Start 103");
		previousAct = currentAct;
		previousScene = -1;
		currentScene = 1;
		stage = 0;
		counter = 0;
		sceneMillis = 0;
	}
	else
	{
		if (currentScene == 1)
		{
			FadeUpWhite();
		}
		else if (currentScene == 2)
		{
			FadeDownWhite();
		}
		else
		{
			StartScene();
		}
	}
} // Act103 ***************************************************************************************************************************************************************************************************************************************


void Act104()
{
	if (previousAct != currentAct)
	{
		Serial.println("Start 104");
		previousAct = currentAct;
		previousScene = -1;
		currentScene = 1;
		stage = 0;
		counter = 0;
		sceneMillis = 0;
	}
	else
	{
		// change scene every 20 seconds
		if (currentMillis - sceneMillis > SceneLength)
		{
			NextScene();
		}

		if (currentScene != previousScene)
		{
			Serial.print("Scene ");
			Serial.print(currentScene);
			Serial.print(", (");
			Serial.print(previousScene);
			Serial.println(")");
		}

		if (currentScene == 1)
		{
			Glimmer();
		}
		else if (currentScene == 2)
		{
			Sparkle();
		}
		else
		{
			StartScene();
		}
	}
} // Act104 ***************************************************************************************************************************************************************************************************************************************


void Act105()
{
	if (previousAct != currentAct)
	{
		Serial.println("Start 102");
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
	else
	{
		StartScene();
	}

} // Act105 ***************************************************************************************************************************************************************************************************************************************


void Act109()
{
	if (previousAct != currentAct)
	{
		Serial.println("Start 109, Tree Only");
		//previousAct = currentAct;
		previousScene = -1;
		currentScene = 1;
		stage = 0;
		counter = 0;
	}
	else
	{
		// change scene every 20 seconds
		if (currentMillis - sceneMillis > SceneLength)
		{
			NextScene();
		}

		if (currentScene != previousScene)
		{
			Serial.print("Scene ");
			Serial.print(currentScene);
			Serial.print(", (");
			Serial.print(previousScene);
			Serial.println(")");
		}

		if (currentScene == 1)
		{
			Glimmer(); // All over
		}
		else if (currentScene == 2)
		{
			TreeSparkle();
		}
		else if (currentScene == 3)
		{
			TreeBerries();
		}
		else if (currentScene == 4)
		{
			TreeFader();
		}
		else if (currentScene == 5)
		{
			TreeRocket();
		}
		else if (currentScene == 6)
		{
			TreeBerries();
		}
		else if (currentScene == 7)
		{
			TreeRainbowAll();
		}
		else if (currentScene == 8)
		{
			TreeRainbowPixel();
		}
		else if (currentScene == 9)
		{
			TreeSpiral();
		}
		else if (currentScene == 10)
		{
			TreeSparkle();
		}
		else if (currentScene == 11)
		{
			Disco(); // All over
		}
		else if (currentScene == 12)
		{
			TreeMash();
		}
		else if (currentScene == 13)
		{
			TreeDomeTrail();
		}
		else
		{
			StartScene();
		}
	}
} // Act109 ***************************************************************************************************************************************************************************************************************************************


void ManualControl()
{
	if (previousAct != currentAct)
	{
		Serial.println("Manual Control");
		//previousAct = currentAct;
		//previousScene = -1;
		//currentScene = 3;
		//stage = 0;
		//counter = 0;
		//sceneMillis = 0;
	}
	else
	{

		if (currentScene != previousScene)
		{
			Serial.print("Scene ");
			Serial.print(currentScene);
			Serial.print(", (");
			Serial.print(previousScene);
			Serial.println(")");

			SetStrips(0, 0, 0);
			Nets(0, 0, 0, 0, 0, 0);
		}

		if (currentScene == 1)
		{
			FadeUpWhite();
		}
		else if (currentScene == 2)
		{
			FadeDownWhite();
		}
		else if (currentScene == 3)
		{
			Glimmer();
		}
		else if (currentScene == 4)
		{
			FillUpWhite();
		}
		else if (currentScene == 5)
		{
			Divider();
		}
		else if (currentScene == 6)
		{
			StarBurst();
		}
		else if (currentScene == 7)
		{
			Sparkle();
		}
		else if (currentScene == 8)
		{
			Star1();
		}
		else if (currentScene == 9)
		{
			Star2();
		}
		else if (currentScene == 10)
		{
			Star3();
		}
		else if (currentScene == 11)
		{
			Disco();
		}
		else if (currentScene == 12)
		{
			DomeBar();
		}
		else if (currentScene == 13)
		{
			DomeTrail();
		}
		else if (currentScene == 14)
		{
			HouseClockWise();
		}
		else if (currentScene == 15)
		{
			HouseAntiClockWise();
		}
		else if (currentScene == 16)
		{
			Cylon1();
		}
		else if (currentScene == 17)
		{
			RainbowHouseAll();
		}
		else if (currentScene == 18)
		{
			TreeSpiral();
		}
		else if (currentScene == 19)
		{
			TreeHorz();
		}
		else if (currentScene == 20)
		{
			TreeVert();
		}
		else if (currentScene == 21)
		{
			TreeSparkle();
		}
		else if (currentScene == 22)
		{
			TreeRocket();
		}
		else if (currentScene == 23)
		{
			TreeBerries();
		}
		else if (currentScene == 24)
		{
			Mash();
		}
		else
		{
			StartScene();
		}
	}
} // ManualControl ***************************************************************************************************************************************************************************************************************************************


void StartScene()
{
	previousScene = currentScene;
	currentScene = 1;
	stage = 0;
	counter = 0;
	treeCounter = 0;
	porchCounter = 0;

	SetColour(&ground, 0, 0, 0);
	ground.show();
} // StartScene ***************************************************************************************************************************************************************************************************************************************


void NextScene()
{
	previousScene = currentScene;
	currentScene++;
	sceneDir = 1;
	stage = 0;
	counter = 0;
	treeCounter = 0;
	treeDir = 1;
	porchCounter = 0;
	porchDir = 1;
	sceneMillis = currentMillis;
	previousTree = currentMillis;

	SetColour(&ground, 0, 0, 0);
	ground.show();
} // NextScene ***************************************************************************************************************************************************************************************************************************************


bool NewScene()
{
	if (previousScene == currentScene)
		return false;

	previousScene = currentScene;

	return true;
} // NewScene ***************************************************************************************************************************************************************************************************************************************


void Action4()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		counter += 8;

		if (counter > MAX)
		{
			NextScene();
		}
		else
		{
			int level = counter;
			SetStrips(level, 0, 0);
			ShowStrips();
		}
	}

} // Action4 ***************************************************************************************************************************************************************************************************************************************


void Action5()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		counter += 8;

		if (counter > MAX)
		{
			NextScene();
		}
		else
		{
			int level = MAX - counter;
			SetStrips(level, 0, 0);
			ShowStrips();
		}
	}

} // Action5 ***************************************************************************************************************************************************************************************************************************************


void Action6()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		counter += 8;

		if (++counter > MAX)
		{
			NextScene();
		}
		else
		{
			int level = counter;
			SetStrips(0, level, 0);
			ShowStrips();
		}
	}

} // Action6 ***************************************************************************************************************************************************************************************************************************************


void Action7()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		counter += 8;

		if (++counter > MAX)
		{
			NextScene();
		}
		else
		{
			int level = MAX - counter;
			SetStrips(0, level, 0);
			ShowStrips();
		}
	}

} // Action7 ***************************************************************************************************************************************************************************************************************************************


void Action8()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		counter += 8;

		if (++counter > MAX)
		{
			NextScene();
		}
		else
		{
			int level = counter;
			SetStrips(0, 0, level);
			ShowStrips();
		}
	}

} // Action8 ***************************************************************************************************************************************************************************************************************************************


void Action9()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		counter += 8;

		if (++counter > MAX)
		{
			NextScene();
		}
		else
		{
			int level = MAX - counter;
			SetStrips(0, 0, level);
			ShowStrips();
		}
	}

} // Action9 ***************************************************************************************************************************************************************************************************************************************


void FadeUpWhite()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetStrips(0, 0, 0);
			ShowStrips();
			Nets(0, 0, 0, 0, 0, 0);
		}

		counter += 8;

		if (++counter > MAX)
		{
			NextScene();
		}
		else
		{
			if (counter > 130)
				Nets(1, 1, 1, 1, 1, 1);
			else if (counter > 100)
				Nets(1, 0, 1, 0, 1, 0);

			int level = counter;
			SetStrips(level, level, level);
			ShowStrips();
		}
	}

} // FadeUpWhite ***************************************************************************************************************************************************************************************************************************************


void FadeDownWhite()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetStrips(MAX, MAX, MAX);
			ShowStrips();
			Nets(1, 1, 1, 1, 1, 1);
		}

		counter += 8;

		if (++counter > MAX)
		{
			NextScene();
		}
		else
		{
			if (counter < 100)
				Nets(0, 0, 0, 0, 0, 0);
			else if (counter < 130)
				Nets(1, 0, 1, 0, 1, 0);

			int level = MAX - counter;
			SetStrips(level, level, level);
			ShowStrips();
		}
	}

} // FadeDownWhite ***************************************************************************************************************************************************************************************************************************************


void FadeOutAll()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			counter = 255;
		}

		counter -= 10;

		if (counter <= 0)
			counter = 0;

		SetStripsBrightness(counter);

		ShowStrips();

		if (counter <= 0)
		{
			SetStrips(0, 0, 0);
			SetStripsBrightness(255);
			ShowStrips();

			NextScene();
		}
	}

} // FadeOutAll ***************************************************************************************************************************************************************************************************************************************


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


void Nets(int net1, int net2, int net3, int net4, int net5, int net6)
{
	digitalWrite(PIN_NET1, net1);
	digitalWrite(PIN_NET2, net2);
	digitalWrite(PIN_NET3, net3);
	digitalWrite(PIN_NET4, net4);
	digitalWrite(PIN_NET5, net5);
	digitalWrite(PIN_NET6, net6);
} // Nets ***************************************************************************************************************************************************************************************************************************************


void SetBar(Adafruit_NeoPixel *strip, int pixelIndex, uint32_t colour)
{
	int index;

	index = pixelIndex - 2;
	if (index >= 0 && index < strip->numPixels())
		strip->setPixelColor(index, colour);

	index = pixelIndex - 1;
	if (index >= 0 && index < strip->numPixels())
		strip->setPixelColor(index, colour);

	index = pixelIndex;
	if (index >= 0 && index < strip->numPixels())
		strip->setPixelColor(index, colour);

	index = pixelIndex + 1;
	if (index >= 0 && index < strip->numPixels())
		strip->setPixelColor(index, colour);

	index = pixelIndex + 2;
	if (index >= 0 && index < strip->numPixels())
		strip->setPixelColor(index, colour);

} // SetBar ****************************************************************************************************************************************************************************************************************************


void SetHouseBar(int pixelIndex, int red, int green, int blue)
{

	SetHousePixel(pixelIndex - 2, red, green, blue);
	SetHousePixel(pixelIndex - 1, red, green, blue);
	SetHousePixel(pixelIndex + 0, red, green, blue);
	SetHousePixel(pixelIndex + 1, red, green, blue);
	SetHousePixel(pixelIndex + 2, red, green, blue);

} // SetHouseBar ****************************************************************************************************************************************************************************************************************************



void SetPixel(int pixelIndex, int red, int green, int blue)
{
	SetPixel(pixelIndex, red, green, blue, true);
} // SetBar ****************************************************************************************************************************************************************************************************************************


void SetPixel(int pixelIndex, int red, int green, int blue, bool show)
{

	if (pixelIndex >= 0 & pixelIndex < 100) // 1 tree
	{
		int pixel = 100 - pixelIndex;
		tree.setPixelColor(pixel, red, green, blue);

		if (show)
			tree.show();
	}
	else if (pixelIndex >= 100 && pixelIndex < 150) // 2 ground
	{
		int pixel = pixelIndex - 100;
		ground.setPixelColor(pixel, red, green, blue);

		if (show)
			ground.show();
	}
	else if (pixelIndex >= 150 && pixelIndex < 300) // 3 left
	{
		int pixel = pixelIndex - 150;
		left.setPixelColor(pixel, red, green, blue);

		if (show)
			left.show();
	}
	else if (pixelIndex >= 300 && pixelIndex < 450) // 4 top
	{
		int pixel = pixelIndex - 300;
		top.setPixelColor(pixel, red, green, blue);

		if (show)
			top.show();
	}
	else if (pixelIndex >= 450 && pixelIndex < 600) // 5 right
	{
		int pixel = pixelIndex - 450;
		pixel = 150 - pixel;
		right.setPixelColor(pixel, red, green, blue);

		if (show)
			right.show();
	}
	else if (pixelIndex >= 751 && pixelIndex <= 800) // 6 porch
	{
		int pixel = pixelIndex - 751;
		porch.setPixelColor(pixel, red, green, blue);

		if (show)
			porch.show();
	}

} // SetPixel ****************************************************************************************************************************************************************************************************************************


void SetHousePixel(int pixelIndex, int red, int green, int blue)
{
	SetHousePixel(pixelIndex, red, green, blue, true);
} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


void SetHousePixel(int pixelIndex, int red, int green, int blue, bool show)
{
	uint32_t colour = left.Color(red, green, blue);
	SetHousePixel(pixelIndex, colour, show);
} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


void SetHousePixel(int pixelIndex, uint32_t colour, bool show)
{

	if (pixelIndex >= 0 && pixelIndex < 150) // 1 left
	{
		int pixel = pixelIndex;
		left.setPixelColor(pixel, colour);

		if (show)
			left.show();
	}
	else if (pixelIndex >= 150 && pixelIndex < 300) // 2 top
	{
		int pixel = pixelIndex - 150;
		top.setPixelColor(pixel, colour);

		if (show)
			top.show();
	}
	else if (pixelIndex >= 300 && pixelIndex < 450) // 3 right
	{
		int pixel = pixelIndex - 300;
		pixel = 150 - pixel;
		right.setPixelColor(pixel, colour);

		if (show)
			right.show();
	}

} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


void FillUpWhite()
{
	if (currentMillis - previousMillis > 60)
	{
		previousMillis = currentMillis;

		NewScene();

		if (counter == 0)
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetHouse(0, 0, 0);
			ShowHouse();
		}

		if (counter <= 150)
		{
			int h = 60;

			//for (int i = 1; i < counter; i++)
			//{
			left.setPixelColor(counter, 60, 60, 60);
			right.setPixelColor(counter, 60, 60, 60);
			//}

			int silver = 0;
			if (counter == 0)
				SetColour(&top, 0, 0, 0);
			else if (counter == 40)
				Nets(1, 0, 0, 0, 0, 0);
			else if (counter == 60)
				Nets(1, 1, 0, 0, 0, 0);
			else if (counter == 80)
				Nets(1, 1, 1, 0, 0, 0);
			else if (counter == 100)
				Nets(1, 1, 1, 1, 0, 0);
			else if (counter == 120)
				Nets(1, 1, 1, 1, 1, 0);
			else if (counter == 140)
				Nets(1, 1, 1, 1, 1, 1);
			//else if (counter == 136 || counter == 142)
			//	SetColour(&top, h, h, h);
			//else if (counter == 138 || counter == 144)
			//	SetColour(&top, h, h, h);
			//else if (counter == 140 || counter == 146)
			//	SetColour(&top, h, h, h);
			else if (counter == 148 || counter == 150)
				SetColour(&top, 60, 60, 60);
			else if (counter > 135)
				SetColour(&top, 0, 0, 0);

			left.show();
			right.show();
			top.show();
		}

		if (++counter > 180)
			counter = 0;
	}
} // FillUpWhite ****************************************************************************************************************************************************************************************************************************


void FillUpRed()
{
	if (currentMillis - previousMillis > 60)
	{
		previousMillis = currentMillis;

		NewScene();

		if (counter == 0)
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetHouse(0, 10, 0);
			ShowHouse();
		}


		if (counter <= 150)
		{
			int h = 60;

			//for (int i = 1; i < counter; i++)
			//{
			left.setPixelColor(counter, 80, 0, 0);
			right.setPixelColor(counter, 80, 0, 0);
			//}

			int silver = 0;
			if (counter == 0)
				SetColour(&top, 0, 0, 0);
			else if (counter == 40)
				Nets(1, 0, 0, 0, 0, 0);
			else if (counter == 60)
				Nets(1, 1, 0, 0, 0, 0);
			else if (counter == 80)
				Nets(1, 1, 1, 0, 0, 0);
			else if (counter == 100)
				Nets(1, 1, 1, 1, 0, 0);
			else if (counter == 120)
				Nets(1, 1, 1, 1, 1, 0);
			else if (counter == 140)
				Nets(1, 1, 1, 1, 1, 1);
			//else if (counter == 136 || counter == 142)
			//	SetColour(&top, h, h, h);
			//else if (counter == 138 || counter == 144)
			//	SetColour(&top, h, h, h);
			//else if (counter == 140 || counter == 146)
			//	SetColour(&top, h, h, h);
			else if (counter == 148 || counter == 150)
				SetColour(&top, h, 0, 0);
			else if (counter > 135)
				SetColour(&top, 0, 0, 0);

			left.show();
			right.show();
			top.show();
		}

		if (++counter > 180)
			counter = 0;
	}
} // FillUpRed ****************************************************************************************************************************************************************************************************************************


void Disco()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetStrips(0, 0, 0); // off
			ShowStrips();
		}

		for (uint16_t i = 0; i < tree.numPixels(); i++)
		{
			uint32_t colour = tree.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			tree.setPixelColor(i, colour);
		}

		for (uint16_t i = 0; i < ground.numPixels(); i++)
		{
			uint32_t colour = ground.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			ground.setPixelColor(i, colour);
		}

		for (uint16_t i = 0; i < left.numPixels(); i++)
		{
			uint32_t colour = left.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			left.setPixelColor(i, colour);
		}

		for (uint16_t i = 0; i < top.numPixels(); i++)
		{
			uint32_t colour = top.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			top.setPixelColor(i, colour);
		}

		for (uint16_t i = 0; i < right.numPixels(); i++)
		{
			uint32_t colour = right.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			right.setPixelColor(i, colour);
		}

		for (uint16_t i = 0; i < porch.numPixels(); i++)
		{
			uint32_t colour = porch.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			porch.setPixelColor(i, colour);
		}

		ShowStrips();
	}
} // Disco ****************************************************************************************************************************************************************************************************************************


void Divider()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetHouse(MAX, MAX, MAX); // white
			ShowHouse();
		}

		if (stage == 0)
			Divider(MAX, 0, 0); // red
		else if (stage == 1)
			Divider(125, 38, 205); // purple
		else if (stage == 2)
			Divider(0, MAX, 0); // green
		else if (stage == 3)
			Divider(0, 0, MAX); // blue
		else if (stage == 4)
			Divider(238, 201, 0); // yellow gold
		else if (stage == 5)
			Divider(0, 205, 205); // cyan
		else if (stage == 6)
			Divider(255, 20, 147); // pink
		else if (stage == 7)
			Divider(255, 128, 0); // orange
		else if (stage == 8)
			Divider(150, 150, 150); // white

		counter++;
		if (counter > 90)
		{
			counter = 0;

			stage++;
			if (stage > 8)
				stage = 0;
		}
	}
} // Divider ****************************************************************************************************************************************************************************************************************************




void Divider(int r, int g, int b)
{
	uint32_t colour = left.Color(r, g, b);

	int middle = LEFT_PIXELS / 2;
	left.setPixelColor(middle - counter, colour);
	left.setPixelColor(middle + counter, colour);

	middle = TOP_PIXELS / 4;
	top.setPixelColor(middle - counter, colour);
	top.setPixelColor(middle + counter, colour);

	middle = (TOP_PIXELS / 4) * 3;
	top.setPixelColor(middle - counter, colour);
	top.setPixelColor(middle + counter, colour);

	middle = RIGHT_PIXELS / 2;
	right.setPixelColor(middle - counter, colour);
	right.setPixelColor(middle + counter, colour);

	left.show();
	top.show();
	right.show();
} // Divider


void RainbowHousePixel()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			counter = random(10, 250);
		}

		if (counter++ > 256)
			counter = 0;

		for (int i = 0; i < HOUSE_PIXELS; i++)
		{
			SetHousePixel(i, Wheel(&left, (i + counter) & 255), false);
		}

		ShowHouse();
	}
} // RainbowHousePixel ****************************************************************************************************************************************************************************************************************************


void RainbowHouseAll()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			counter = random(25, 233);
		}

		if (counter++ > 256)
			counter = 0;

		uint32_t colour = Wheel(&left, (counter)& 255);

		SetColour(&left, colour);
		SetColour(&right, colour);
		SetColour(&top, colour);

		ShowHouse();

	}
} // RainbowHouseAll ****************************************************************************************************************************************************************************************************************************


void RainbowHouseTwinkle()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			counter = random(10, 250);
		}

		if (counter++ > 256)
			counter = 0;

		for (int i = 0; i < HOUSE_PIXELS; i++)
		{
			SetHousePixel(i, Wheel(&left, (i + counter) & 255), false);
		}

		int l = 70;
		int h = 180;

		for (int i = 0; i < 10; i++)
		{
			StripTwinkle(&left, LEFT_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&right, RIGHT_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&top, TOP_PIXELS, l, l, l, h, h, h);
		}

		ShowHouse();
	}
} // RainbowHouseTwinkle ****************************************************************************************************************************************************************************************************************************


void RainbowAll()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			counter = random(25, 233);
		}

		if (counter++ > 256)
			counter = 0;

		int red, green, blue;

		Wheel(counter, &red, &green, &blue);

		SetColour(&left, red, green, blue);
		SetColour(&right, red, green, blue);
		SetColour(&top, red, green, blue);
		SetColour(&tree, red, green, blue);
		SetColour(&ground, red, green, blue);
		SetColour(&porch, red, green, blue);

		ShowStrips();

	}
} // RainbowAll ****************************************************************************************************************************************************************************************************************************


void HouseClockWise()
{

	if (currentMillis - previousMillis > 80)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
		}

		for (int i = counter; i < HOUSE_PIXELS; i += 4)
		{
			SetHousePixel(i + 0, 10, 10, 10, false);
			SetHousePixel(i + 1, 20, 20, 20, false);
			SetHousePixel(i + 2, 40, 40, 40, false);
			SetHousePixel(i + 3, MAX, MAX, MAX, false);
		}

		ShowHouse();

		counter++;
		if (counter > 3)
			counter = 0;
	}
} // HouseClockWise ****************************************************************************************************************************************************************************************************************************


void HouseAntiClockWise()
{

	if (currentMillis - previousMillis > 80)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
		}

		for (int i = counter; i < HOUSE_PIXELS; i += 4)
		{
			SetHousePixel(HOUSE_PIXELS - (i + 0), 10, 10, 10, false);
			SetHousePixel(HOUSE_PIXELS - (i + 1), 20, 20, 20, false);
			SetHousePixel(HOUSE_PIXELS - (i + 2), 40, 40, 40, false);
			SetHousePixel(HOUSE_PIXELS - (i + 3), MAX, MAX, MAX, false);
		}

		ShowHouse();

		counter++;
		if (counter > 3)
			counter = 0;
	}

} // HouseAntiClockWise ****************************************************************************************************************************************************************************************************************************

int sideRedPos = 0;
int sideRedDir = 2;
int sideGreenPos = 0;
int sideGreenDir = 1;

int topRedPos = 0;
int topRedDir = 2;
int topGreenPos = 0;
int topGreenDir = 1;

int allWhitePos = 0;
int allWhiteDir = 1;

void Cylon1()
{

	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;

		uint32_t l = left.Color(40, 40, 40);
		uint32_t h = left.Color(200, 200, 200);
		uint32_t red = left.Color(100, 0, 0);
		uint32_t green = left.Color(0, 100, 0);

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetHouse(40, 40, 40);

			sideRedPos = 1;
			sideRedDir = 2;
			sideGreenPos = LEFT_PIXELS - 1;
			sideGreenDir = -1;

			topRedPos = 100;
			topRedDir = 2;
			topGreenPos = TOP_PIXELS - 1;
			topGreenDir = -1;

			allWhitePos = 300;
			allWhiteDir = 1;
		}

		SetHouse(0, 0, 0);

		SetBar(&left, sideRedPos, red);
		SetBar(&left, sideGreenPos, green);

		SetBar(&top, topRedPos, red);
		SetBar(&top, topGreenPos, green);

		SetBar(&right, sideRedPos, red);
		SetBar(&right, sideGreenPos, green);

		ShowHouse();

		//SetBar(&left, allWhitePos, white);

		sideRedPos += sideRedDir;
		sideGreenPos += sideGreenDir;
		topRedPos += topRedDir;
		topGreenPos += topGreenDir;
		allWhitePos += allWhiteDir;

		if (sideRedDir > 0)
		{
			if (sideRedPos >= LEFT_PIXELS)
				sideRedDir = -2;
		}
		else
		{
			if (sideRedPos <= 0)
				sideRedDir = +2;
		}

		if (sideGreenDir > 0)
		{
			if (sideGreenPos >= LEFT_PIXELS)
				sideGreenDir = -1;
		}
		else
		{
			if (sideGreenPos <= 0)
				sideGreenDir = +1;
		}

		if (topRedDir > 0)
		{
			if (topRedPos >= TOP_PIXELS)
				topRedDir = -2;
		}
		else
		{
			if (topRedPos <= 0)
				topRedDir = +2;
		}

		if (topGreenDir > 0)
		{
			if (topGreenPos >= TOP_PIXELS)
				topGreenDir = -1;
		}
		else
		{
			if (topGreenPos <= 0)
				topGreenDir = +1;
		}

		if (allWhiteDir > 0)
		{
			if (allWhitePos >= 600)
				allWhiteDir = -1;
		}
		else
		{
			if (allWhitePos <= 0)
				allWhiteDir = +1;
		}

	}
} // Cylon1 ****************************************************************************************************************************************************************************************************************************


void Sparkle()
{
	if (currentMillis - previousMillis > 60)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetHouse(40, 40, 40);
		}

		for (int i = 0; i < 50; i++)
		{
			StripTwinkle(&left, LEFT_PIXELS, 40, 40, 40, 80, 80, 80);
			StripTwinkle(&right, RIGHT_PIXELS, 40, 40, 40, 80, 80, 80);
			StripTwinkle(&top, TOP_PIXELS, 40, 40, 40, 80, 80, 80);

			/*
			int on = random(0, LEFT_PIXELS);
			int off = random(0, LEFT_PIXELS);

			left.setPixelColor(off, 40, 40, 40);
			left.setPixelColor(on, 200, 200, 200);

			on = random(0, TOP_PIXELS);
			off = random(0, TOP_PIXELS);

			top.setPixelColor(off, 40, 40, 40);
			top.setPixelColor(on, 200, 200, 200);

			on = random(0, RIGHT_PIXELS);
			off = random(0, RIGHT_PIXELS);

			right.setPixelColor(off, 40, 40, 40);
			right.setPixelColor(on, 200, 200, 200);
			*/
		}

		left.show();
		top.show();
		right.show();

	}
} // Sparkle ****************************************************************************************************************************************************************************************************************************


void Twinkle()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetHouse(0, 0, 0);
		}

		int l = 0;
		int h = 20;

		for (int i = 0; i < 20; i++)
		{
			StripTwinkle(&left, LEFT_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&right, RIGHT_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&top, TOP_PIXELS, l, l, l, h, h, h);
		}

		if (++stage > 4)
		{
			stage = 0;

			l = 40;
			h = 160;

			for (int i = 0; i < 10; i++)
			{
				StripTwinkle(&left, LEFT_PIXELS, l, l, l, h, h, h);
				StripTwinkle(&right, RIGHT_PIXELS, l, l, l, h, h, h);
				StripTwinkle(&top, TOP_PIXELS, l, l, l, h, h, h);
			}
		}

		left.show();
		top.show();
		right.show();

	}
} // Twinkle ****************************************************************************************************************************************************************************************************************************


void Snow()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetHouse(40, 40, 40);
		}

		for (int i = 0; i < 40; i++)
		{
			int on = random(0, LEFT_PIXELS);
			int off = random(0, LEFT_PIXELS);

			top.setPixelColor(off, 40, 40, 40);
			top.setPixelColor(on, 150, 150, 150);
		}

		for (int i = 0; i < LEFT_PIXELS; i += 3)
		{
			left.setPixelColor(i + counter + 0, 150, 150, 150);
			left.setPixelColor(i + counter + 1, 150, 150, 150);
			left.setPixelColor(i + counter + 2, 150, 150, 150);

			right.setPixelColor(i + counter + 0, 150, 150, 150);
			right.setPixelColor(i + counter + 1, 150, 150, 150);
			right.setPixelColor(i + counter + 2, 150, 150, 150);
		}

		if (++counter > 2)
			counter = 0;

		ShowHouse();
	}
} // Snow ****************************************************************************************************************************************************************************************************************************



int stars = 0;
//                 0 ,   1 , 2 , 3 ,     4 , 5 , 6 , 7 , 8 , 9 ,     10 , 11, 12 , 13
int starpos[] = { 20, 50, 80, 20, 20, 40, 60, 80, 100, 120, 40, 0, 0, 0, 0 };
int starmin[] = { 5, 45, 85, 115, 5, 40, 60, 80, 100, 12, 5, 45, 85, 115, 0 };
int starmax[] = { 40, 80, 110, 145, 40, 60, 80, 100, 120, 140, 40, 80, 110, 145, 0 };
int starseq[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int starlen[] = { 1, 20, 10, 15, 1, 47, 12, 54, 26, 48, 70, 1, 24, 36, 78 };

void StarBurst()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;
		int pos;
		int seq;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetHouse(0, 0, 0); // off
			ShowHouse();

			for (int i = 0; i < 14; i++)
			{
				starseq[i] = 0 - starlen[i];
				starpos[i] = random(starmin[i], starmax[i]);
			}

			//starseq[7] = starseq[4] + 140;
			//starseq[8] = starseq[5] + 140;
			//starseq[9] = starseq[6] + 140;
		}

		for (int i = 0; i < 4; i++)
		{
			pos = starpos[i];
			seq = starseq[i];
			seq++;
			if (seq > STARBURST_MAX)
			{
				seq = 0 - starlen[i];
				pos = random(starmin[i], starmax[i]);
				starpos[i] = pos;
			}
			starseq[i] = seq;

			StarBurst(&left, pos, seq);
		}

		for (int i = 4; i < 10; i++)
		{
			pos = starpos[i];
			seq = starseq[i];

			seq++;
			if (seq > STARBURST_MAX)
			{
				seq = 0 - starlen[i];
				pos = random(starmin[i], starmax[i]);
				starpos[i] = pos;
			}
			starseq[i] = seq;

			StarBurst(&top, pos, seq);
		}

		for (int i = 10; i < 14; i++)
		{
			pos = starpos[i];
			seq = starseq[i];

			seq++;
			if (seq > STARBURST_MAX)
			{
				seq = 0 - starlen[i];
				pos = random(starmin[i], starmax[i]);
				starpos[i] = pos;
			}
			starseq[i] = seq;

			StarBurst(&right, pos, seq);
		}

		left.show();
		top.show();
		right.show();

		//if (counter > 22)
		//counter = 0;
	}
} // StarBurst ****************************************************************************************************************************************************************************************************************************


void StarBurst(Adafruit_NeoPixel *strip, int c, int seq)
{

	if (seq == 1)
	{
		strip->setPixelColor(c, 200, 200, 200);
	}
	else if (seq == 2)
	{
		strip->setPixelColor(c - 1, 200, 200, 200);
		strip->setPixelColor(c, 40, 40, 40);
		strip->setPixelColor(c + 1, 200, 200, 200);
	}
	else if (seq == 3)
	{
		strip->setPixelColor(c - 2, 200, 200, 200);
		strip->setPixelColor(c - 1, 40, 40, 40);
		strip->setPixelColor(c, 40, 40, 40);
		strip->setPixelColor(c + 1, 40, 40, 40);
		strip->setPixelColor(c + 2, 200, 200, 200);
	}
	else if (seq == 4)
	{
		strip->setPixelColor(c - 3, 200, 200, 200);
		strip->setPixelColor(c - 2, 40, 40, 40);
		strip->setPixelColor(c - 1, 40, 40, 40);
		strip->setPixelColor(c, 40, 40, 40);
		strip->setPixelColor(c + 1, 40, 40, 40);
		strip->setPixelColor(c + 2, 40, 40, 40);
		strip->setPixelColor(c + 3, 200, 200, 200);
	}
	else if (seq == 5)
	{
		strip->setPixelColor(c - 4, 200, 200, 200);
		strip->setPixelColor(c - 3, 40, 40, 40);
		strip->setPixelColor(c - 2, 40, 40, 40);
		strip->setPixelColor(c - 1, 40, 40, 40);
		strip->setPixelColor(c, 40, 40, 40);
		strip->setPixelColor(c + 1, 40, 40, 40);
		strip->setPixelColor(c + 2, 40, 40, 40);
		strip->setPixelColor(c + 3, 40, 40, 40);
		strip->setPixelColor(c + 4, 200, 200, 200);
	}
	else if (seq == 6)
	{
		strip->setPixelColor(c - 5, 200, 200, 200);
		strip->setPixelColor(c - 4, 40, 40, 40);
		strip->setPixelColor(c - 3, 40, 40, 40);
		strip->setPixelColor(c - 2, 40, 40, 40);
		strip->setPixelColor(c - 1, 40, 40, 40);
		strip->setPixelColor(c, 40, 40, 40);
		strip->setPixelColor(c + 1, 40, 40, 40);
		strip->setPixelColor(c + 2, 40, 40, 40);
		strip->setPixelColor(c + 3, 40, 40, 40);
		strip->setPixelColor(c + 4, 40, 40, 40);
		strip->setPixelColor(c + 5, 200, 200, 200);
	}
	else if (seq == 7)
	{
		strip->setPixelColor(c - 6, 200, 200, 200); // 5
		strip->setPixelColor(c - 5, 40, 40, 40);    // 2
		strip->setPixelColor(c - 4, 40, 40, 40);    // 7
		strip->setPixelColor(c - 3, 40, 40, 40);    // 12
		strip->setPixelColor(c - 2, 40, 40, 40);    // 9
		strip->setPixelColor(c - 1, 40, 40, 40);    // 11
		strip->setPixelColor(c, 40, 40, 40);        // 13
		strip->setPixelColor(c + 1, 40, 40, 40);    // 3
		strip->setPixelColor(c + 2, 40, 40, 40);    // 6
		strip->setPixelColor(c + 3, 40, 40, 40);    // 1
		strip->setPixelColor(c + 4, 40, 40, 40);    // 4
		strip->setPixelColor(c + 5, 40, 40, 40);    // 8
		strip->setPixelColor(c + 6, 200, 200, 200); // 10
	}
	else if (seq == 8)
	{
		strip->setPixelColor(c - 6, 40, 40, 40);
		strip->setPixelColor(c + 6, 40, 40, 40);
	}
	else if (seq == 16)
	{
		//int list[] = { +3, -5, +1,  -6, +4, -1, +2, -3, +5, -2, +6, -4,  0 };

		strip->setPixelColor(c + 3, 200, 200, 200);
	}
	else if (seq == 17)
	{
		strip->setPixelColor(c + 3, 0, 0, 0);
		strip->setPixelColor(c - 5, 200, 200, 200);
	}
	else if (seq == 18)
	{
		strip->setPixelColor(c - 5, 0, 0, 0);
		strip->setPixelColor(c + 1, 200, 200, 200);
	}
	else if (seq == 19)
	{
		strip->setPixelColor(c + 1, 0, 0, 0);
		strip->setPixelColor(c - 6, 200, 200, 200);
	}
	else if (seq == 20)
	{
		strip->setPixelColor(c - 6, 0, 0, 0);
		strip->setPixelColor(c + 4, 200, 200, 200);
	}
	else if (seq == 21)
	{
		strip->setPixelColor(c + 4, 0, 0, 0);
		strip->setPixelColor(c - 1, 200, 200, 200);
	}
	else if (seq == 22)
	{
		//int list[] = { +3, -5, +1,  -6, +4, -1, +2, -3, +5, -2, +6, -4,  0 };
		strip->setPixelColor(c - 1, 0, 0, 0);
		strip->setPixelColor(c + 2, 200, 200, 200);
	}
	else if (seq == 23)
	{
		strip->setPixelColor(c + 2, 0, 0, 0);
		strip->setPixelColor(c - 3, 200, 200, 200);
	}
	else if (seq == 24)
	{
		strip->setPixelColor(c - 3, 0, 0, 0);
		strip->setPixelColor(c + 5, 200, 200, 200);
	}
	else if (seq == 25)
	{
		strip->setPixelColor(c + 5, 0, 0, 0);
		strip->setPixelColor(c - 2, 200, 200, 200);
	}
	else if (seq == 26)
	{
		strip->setPixelColor(c - 2, 0, 0, 0);
		strip->setPixelColor(c + 6, 200, 200, 200);
	}
	else if (seq == 27)
	{
		//int list[] = { +3, -5, +1,  -6, +4, -1, +2, -3, +5, -2, +6, -4,  0 };
		strip->setPixelColor(c + 6, 0, 0, 0);
		strip->setPixelColor(c - 4, 200, 200, 200);
	}
	else if (seq == 28)
	{
		strip->setPixelColor(c - 4, 0, 0, 0);
		strip->setPixelColor(c, 200, 200, 200);
	}
	else if (seq == 29)
	{
		strip->setPixelColor(c, 0, 0, 0);
	}

} // StarBurst ****************************************************************************************************************************************************************************************************************************


void Star1()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetStrips(0, 0, 0);
			ShowStrips();
			last = -1;
		}

		SetPixel(last, 0, 0, 0);
		SetPixel(counter, MAX, MAX, MAX);

		last = counter;

		counter++;
		if (counter > (TREE_PIXELS + GROUND_PIXELS + LEFT_PIXELS + TOP_PIXELS + RIGHT_PIXELS))
			counter = 0;

	}


} // Star1 ****************************************************************************************************************************************************************************************************************************


void Star2()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		NewScene();

		if (counter == 0)
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetStrips(0, 0, 0);
			ShowStrips();
		}

		SetPixel(counter, MAX, MAX, MAX);

		last = counter;

		counter++;
		if (counter > (TREE_PIXELS + GROUND_PIXELS + LEFT_PIXELS + TOP_PIXELS + RIGHT_PIXELS))
			counter = 0;
	}

} // Star2 ****************************************************************************************************************************************************************************************************************************


void Star3()
{
	//if (currentMillis - previousMillis > 2)
	//{
	previousMillis = currentMillis;

	NewScene();

	if (counter == 0)
	{
		Nets(0, 0, 0, 0, 0, 0);
		SetStrips(0, 0, 0);
		ShowStrips();
		last = -1;
		position = 0;
	}

	if (counter == 0 || counter == 1 || counter == 4 || counter == 7 || counter == 9)
	{
		SetPixel(position, MAX, MAX, MAX, false);
	}
	else if (counter == 2 || counter == 5 || counter == 8)
	{
		SetPixel(position, 40, 40, 40, false);
	}
	else if (counter == 3 || counter == 6 || counter == 10)
	{
		SetPixel(position, 0, 0, 0, false);
	}
	else if (position > (TREE_PIXELS + GROUND_PIXELS + LEFT_PIXELS + TOP_PIXELS + RIGHT_PIXELS))
	{
		stage++;
		counter = 1;
		position = 0;
		// enable porch starporch star needs to go in here
	}
	else
	{
		SetPixel(position - 12, 5, 5, 5, false);
		SetPixel(position - 11, 10, 10, 10, false);
		SetPixel(position - 10, 15, 15, 15, false);
		SetPixel(position - 9, 20, 20, 20, false);
		SetPixel(position - 8, 25, 25, 25, false);
		SetPixel(position - 7, 35, 35, 35, false);
		SetPixel(position - 6, 42, 42, 42, false);
		SetPixel(position - 5, 50, 50, 50, false);
		SetPixel(position - 4, 54, 54, 54, false);
		SetPixel(position - 3, 56, 56, 56, false);
		SetPixel(position - 2, 58, 58, 58, false);
		SetPixel(position - 1, 60, 60, 60, false);
		SetPixel(position, MAX, MAX, MAX, false);

		position++;
	}

	if (stage > 0)
	{
		PorchStarTurn();
	}

	ShowStrips();

	last = position;

	counter++;

	//}


} // Star3 ****************************************************************************************************************************************************************************************************************************


void DomeBar()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
		}

		SetHouse(LOW, LOW, LOW);

		//SetHouseBar(counter, MAX, 0, 0);
		//SetHouseBar(HOUSE_PIXELS - counter, MAX, 0, 0);

		uint32_t colour = left.Color(MAX, 0, 0);

		//colour = Wheel(&left, counter);
		SetHousePixel(counter - 2, colour, false);
		SetHousePixel(counter - 1, colour, false);
		SetHousePixel(counter + 0, colour, false);
		SetHousePixel(counter + 1, colour, false);
		SetHousePixel(counter + 2, colour, false);

		//colour = Wheel(&left, counter * 2);
		SetHousePixel(HOUSE_PIXELS - counter - 2, colour, false);
		SetHousePixel(HOUSE_PIXELS - counter - 1, colour, false);
		SetHousePixel(HOUSE_PIXELS - counter + 0, colour, false);
		SetHousePixel(HOUSE_PIXELS - counter + 1, colour, false);
		SetHousePixel(HOUSE_PIXELS - counter + 2, colour, false);

		ShowHouse();

		counter += sceneDir;

		if (counter <= 0)
		{
			sceneDir = +1;
			if (stage++ > 8)
				stage = 0;
		}
		else if (counter >= HOUSE_PIXELS)
		{
			if (stage++ > 8)
				stage = 0;

			sceneDir = -1;
		}

	}
} // DomeBar ****************************************************************************************************************************************************************************************************************************


void DomeTrail()
{
	if (currentMillis - previousMillis > 2)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetHouse(LOW, LOW, LOW);
		}

		uint32_t colour = 0;

		if (stage == 0)
		{
			colour = left.Color(0, MAX, 0);
			Nets(1, 0, 0, 0, 0, 0);
		}
		else if (stage == 1)
		{
			colour = left.Color(MAX, 0, 0);
			Nets(1, 0, 1, 0, 0, 0);
		}
		else if (stage == 2)
		{
			colour = left.Color(0, 0, MAX);
			Nets(1, 0, 1, 0, 1, 0);
		}
		else if (stage == 3)
		{
			colour = left.Color(MAX, MAX, 0);
			Nets(1, 1, 1, 0, 1, 0);
		}
		else if (stage == 4)
		{
			colour = left.Color(0, MAX, MAX);
			Nets(1, 1, 1, 1, 1, 0);
		}
		else if (stage == 5)
		{
			colour = left.Color(MAX, 0, MAX);
			Nets(1, 1, 1, 1, 1, 1);
		}
		else if (stage == 6)
		{
			colour = Wheel(&left, counter * 2);
			//colour = left.Color(MAX, 0, MAX);
			Nets(0, 0, 1, 1, 0, 0);
		}
		else if (stage == 7)
		{
			colour = left.Color(MAX, MAX, MAX);
			Nets(0, 0, 0, 0, 0, 0);
		}

		SetHousePixel(counter, colour, false);
		SetHousePixel(HOUSE_PIXELS - counter, colour, false);

		if (++counter > (HOUSE_PIXELS / 2))
		{
			counter = 0;
			stage++;
		}

		if (stage > 7)
			stage = 0;

		ShowHouse();
	}
} // DomeTrail ****************************************************************************************************************************************************************************************************************************


void Mash()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetHouse(LOW, LOW, LOW);
			Nets(0, 0, 0, 0, 0, 0);
			counter = 0;
			stage = 0;
		}

		uint32_t colour = 0;

		/*
		Divider(238, 201, 0); // yellow gold
		else if (stage == 5)
		Divider(0, 205, 205); // cyan
		else if (stage == 6)
		Divider(255, 20, 147); // pink
		else if (stage == 7)
		Divider(255, 128, 0); // orange
		*/

		if (stage == 0)
		{
			colour = left.Color(MAX, 0, 0); // red
			Nets(1, 0, 0, 0, 0, 0);
		}
		else if (stage == 1)
		{
			colour = left.Color(0, MAX, 0); // green
			Nets(1, 0, 1, 0, 0, 0);
		}
		else if (stage == 2)
		{
			colour = left.Color(0, 0, MAX); // blue
			Nets(1, 0, 1, 0, 1, 0);
		}
		else if (stage == 3)
		{
			colour = left.Color(MAX, MAX, 0);
			Nets(1, 1, 1, 0, 1, 0);
		}
		else if (stage == 4)
		{
			colour = left.Color(0, MAX, MAX);
			Nets(1, 1, 1, 1, 1, 0);
		}
		else if (stage == 5)
		{
			colour = left.Color(MAX, 0, MAX);
			Nets(1, 1, 1, 1, 1, 1);
		}
		else if (stage == 6)
		{
			colour = left.Color(255, 128, 0); // orange
			Nets(1, 0, 1, 1, 1, 1);
		}
		else if (stage == 7)
		{
			colour = left.Color(200, 20, 127); // pink 255, 20, 147
			Nets(1, 0, 1, 0, 1, 1);
		}
		else if (stage == 8)
		{
			colour = left.Color(0, 205, 205); // cyan
			Nets(1, 0, 1, 0, 1, 0);
		}
		else if (stage == 9)
		{
			colour = left.Color(238, 201, 0); // yellow gold
			Nets(0, 0, 1, 0, 1, 0);
		}
		else if (stage == 10)
		{
			colour = Wheel(&left, counter); // rainbow
			Nets(0, 0, 1, 0, 0, 0);
		}
		else if (stage == 11)
		{
			colour = left.Color(MAX, MAX, MAX); // white
			Nets(0, 0, 0, 0, 0, 0);
		}

		for (int i = 0; i < 5; i++)
		{
			SetHousePixel(counter, colour, false);
			counter += sceneDir;
		}

		ShowHouse();

		if (counter <= 0)
		{
			sceneDir = +1;
			stage++;
		}
		else if (counter >= HOUSE_PIXELS)
		{
			sceneDir = -1;
			stage++;
		}

		if (stage > 11)
			stage = 0;

	}

} // Mash ****************************************************************************************************************************************************************************************************************************


void Glimmer()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
		}

		SetStrips(LOW, LOW, LOW);

		if (++stage > 39)
			stage = 0;

		SetBar(&tree, stage + 00, white);
		SetBar(&tree, stage + 50, white);

		SetBar(&ground, stage + 00, white);
		SetBar(&ground, stage + 50, white);

		SetBar(&left, stage + 00, white);
		SetBar(&left, stage + 40, white);
		SetBar(&left, stage + 80, white);
		SetBar(&left, stage + 120, white);

		SetBar(&right, stage + 00, white);
		SetBar(&right, stage + 40, white);
		SetBar(&right, stage + 80, white);
		SetBar(&right, stage + 120, white);

		SetBar(&top, stage + 00, white);
		SetBar(&top, stage + 50, white);
		SetBar(&top, stage + 100, white);
		SetBar(&top, stage + 150, white);
		SetBar(&top, stage + 200, white);

		SetBar(&porch, stage + 00, white);
		SetBar(&porch, stage + 50, white);
		SetBar(&porch, stage + 100, white);
		SetBar(&porch, stage + 150, white);

		ShowStrips();
	}

} // Glimmer ***************************************************************************************************************************************************************************************************************************************


void Dull()
{
	if (currentMillis - previousMillis > 200)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			SetStrips(10, 10, 10);
			ShowStrips();
		}

		int l = 10;
		int h = 40;

		for (int i = 0; i < 50; i++)
		{
			StripTwinkle(&left, LEFT_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&right, RIGHT_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&top, TOP_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&tree, TREE_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&ground, GROUND_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&porch, PORCH_PIXELS, l, l, l, h, h, h);
		}

		ShowStrips();

		if (counter++ > 1000)
			counter = 0;

		if (counter > 900)
			Nets(1, 1, 1, 1, 1, 1);
		else if (counter > 800)
			Nets(1, 1, 0, 0, 1, 1);
		else if (counter > 700)
			Nets(1, 0, 1, 1, 1, 0);
		else if (counter > 600)
			Nets(0, 0, 1, 1, 0, 0);
		else if (counter > 500)
			Nets(0, 0, 0, 0, 1, 0);
		else if (counter > 400)
			Nets(0, 0, 1, 0, 0, 0);
		else if (counter > 300)
			Nets(1, 0, 0, 0, 0, 0);

	}
} // Dull ****************************************************************************************************************************************************************************************************************************


//unsigned long previousSpiral = 0;
//int spiral = 0;
int red = 10;

void TreeSpiral()
{
	if (currentMillis - previousTree > 200)
	{
		previousTree = currentMillis;

		tree.setPixelColor(0, 10, 10, 10);
		tree.setPixelColor(1, 20, 20, 20);
		tree.setPixelColor(2, 40, 40, 40);
		tree.setPixelColor(3, MAX, MAX, MAX);

		for (int i = treeCounter; i < TREE_PIXELS; i += 4)
		{
			tree.setPixelColor(i + 0, 10, 10, 10);
			tree.setPixelColor(i + 1, 20, 20, 20);
			tree.setPixelColor(i + 2, 40, 40, 40);
			tree.setPixelColor(i + 3, MAX, MAX, MAX);
		}

		tree.show();

		if (++treeCounter > 4)
			treeCounter = 1;

	}

} // TreeSpiral ***************************************************************************************************************************************************************************************************************************************


void TreeVert()
{
	if (currentMillis - previousTree > 600)
	{
		previousTree = currentMillis;

		SetColour(&tree, 0, 0, 0);

		for (int i = 0; i < 10; i++)
		{
			int pos = treeVerts[treeCounter][i];
			if (pos >= 0)
				tree.setPixelColor(pos, 200, 200, 200);
		}

		tree.show();

		if (++treeCounter > 10)
			treeCounter = 0;
	}
} // TreeVert ***************************************************************************************************************************************************************************************************************************************


void TreeHorz()
{
	if (currentMillis - previousTree > 600)
	{
		previousTree = currentMillis;

		SetColour(&tree, 0, 0, 0);

		for (int i = 0; i < 10; i++)
		{
			int pos = treeHorzs[treeCounter][i];
			if (pos >= 0)
				tree.setPixelColor(pos, 200, 200, 200);
		}

		tree.show();

		if (++treeCounter > 10)
			treeCounter = 0;
	}
} // TreeHorz ***************************************************************************************************************************************************************************************************************************************


void TreeSparkle()
{
	if (currentMillis - previousTree > 200)
	{
		previousTree = currentMillis;

		if (treeCounter++ == 0)
		{
			SetColour(&tree, 40, 40, 40);
		}

		for (int i = 0; i < (TREE_PIXELS / 2); i++)
		{
			int on = random(0, TREE_PIXELS);
			int off = random(0, TREE_PIXELS);

			tree.setPixelColor(off, 40, 40, 40);
			tree.setPixelColor(on, 200, 200, 200);
		}

		tree.show();
	}
} // TreeSparkle ***************************************************************************************************************************************************************************************************************************************


void TreeMash()
{
	if (currentMillis - previousMillis > 120)
	{
		previousMillis = currentMillis;

		uint32_t colour = 0;

		if (treeStage == 0)
		{
			colour = tree.Color(MAX, 0, 0); // red
		}
		else if (treeStage == 1)
		{
			colour = tree.Color(0, MAX, 0); // green
		}
		else if (treeStage == 2)
		{
			colour = tree.Color(0, 0, MAX); // blue
		}
		else if (treeStage == 3)
		{
			colour = tree.Color(MAX, MAX, 0);
		}
		else if (treeStage == 4)
		{
			colour = tree.Color(0, MAX, MAX);
		}
		else if (treeStage == 5)
		{
			colour = tree.Color(MAX, 0, MAX);
		}
		else if (treeStage == 6)
		{
			colour = tree.Color(255, 128, 0); // orange
		}
		else if (treeStage == 7)
		{
			colour = tree.Color(200, 20, 127); // pink 255, 20, 147
		}
		else if (treeStage == 8)
		{
			colour = tree.Color(0, 205, 205); // cyan
		}
		else if (treeStage == 9)
		{
			colour = tree.Color(238, 201, 0); // yellow gold
		}
		else if (treeStage == 10)
		{
			colour = Wheel(&tree, counter); // rainbow
		}
		else if (treeStage == 11)
		{
			colour = tree.Color(MAX, MAX, MAX); // white
		}

		for (int i = 0; i < 3; i++)
		{
			tree.setPixelColor(counter, colour);
			counter += treeDir;
		}

		tree.show();

		if (counter <= 0)
		{
			treeDir = +1;
			treeStage++;
		}
		else if (counter >= TREE_PIXELS)
		{
			treeDir = -1;
			treeStage++;
		}

		if (treeStage > 11)
			treeStage = 0;
	}

} // TreeMash ****************************************************************************************************************************************************************************************************************************


void TreeDomeTrail()
{
	if (currentMillis - previousMillis > 60)
	{
		previousMillis = currentMillis;

		uint32_t colour = 0;

		if (treeStage == 0)
		{
			colour = tree.Color(0, MAX, 0);
		}
		else if (treeStage == 1)
		{
			colour = tree.Color(MAX, 0, 0);
		}
		else if (treeStage == 2)
		{
			colour = tree.Color(0, 0, MAX);
		}
		else if (treeStage == 3)
		{
			colour = tree.Color(MAX, MAX, 0);
		}
		else if (treeStage == 4)
		{
			colour = tree.Color(0, MAX, MAX);
		}
		else if (treeStage == 5)
		{
			colour = tree.Color(MAX, 0, MAX);
		}
		else if (treeStage == 6)
		{
			colour = Wheel(&tree, counter * 2);
		}
		else if (treeStage == 7)
		{
			colour = tree.Color(MAX, MAX, MAX);
		}

		tree.setPixelColor(counter, colour);
		tree.setPixelColor(TREE_PIXELS - counter, colour);

		if (++counter > (TREE_PIXELS / 2))
		{
			counter = 0;
			treeStage++;
		}

		if (treeStage > 7)
			treeStage = 0;

		tree.show();
	}
} // TreeDomeTrail ****************************************************************************************************************************************************************************************************************************


void TreeTrain()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;

		//if (treeStage <= 0)
		//{
		//}

		SetColour(&tree, 40, 40, 40);
		SetBar(&tree, treeStage, white);

		treeStage++;

		if (treeStage > TREE_PIXELS)
			treeStage = 0;

		tree.show();
	}

} // TreeTrain ***************************************************************************************************************************************************************************************************************************************


void TreeBerries()
{
	if (currentMillis - previousTree > 200)
	{
		previousTree = currentMillis;

		if (treeCounter == 0)
		{
			SetColour(&tree, 50, 0, 0);
		}

		for (int i = 0; i < 40; i++)
		{
			int l = random(0, TREE_PIXELS);
			int h = random(0, TREE_PIXELS);

			tree.setPixelColor(l, 50, 0, 0);
			tree.setPixelColor(h, 200, 0, 0);
		}

		/*
		if (treeCounter++ > 10)
		treeCounter = 1;

		for (int i = 0; i < TREE_PIXELS; i += 10)
		{
		tree.setPixelColor(i + treeCounter, 10, 0, 0);
		tree.setPixelColor(i + treeCounter + 1, 15, 0, 0);
		tree.setPixelColor(i + treeCounter + 2, 20, 0, 0);
		tree.setPixelColor(i + treeCounter + 3, 25, 0, 0);
		tree.setPixelColor(i + treeCounter + 4, 30, 0, 0);
		tree.setPixelColor(i + treeCounter + 5, 35, 0, 0);
		tree.setPixelColor(i + treeCounter + 6, 40, 0, 0);
		tree.setPixelColor(i + treeCounter + 7, 45, 0, 0);
		tree.setPixelColor(i + treeCounter + 8, 50, 0, 0);
		tree.setPixelColor(i + treeCounter + 9, 55, 0, 0);
		}
		*/

		tree.show();
	}
} // TreeBerries ***************************************************************************************************************************************************************************************************************************************


void TreeBounce()
{
	if (currentMillis - previousTree > 50)
	{
		previousTree = currentMillis;

		for (int i = 0; i < TREE_PIXELS; i++)
		{
			if (i < treeCounter)
				tree.setPixelColor(i, MAX, 0, 0);
			else
				tree.setPixelColor(i, 0, 10, 10);
		}

		tree.show();

		treeCounter += treeDir;

		if (treeCounter >= TREE_PIXELS)
		{
			treeDir = -1;
		}
		else if (treeCounter <= 10)
		{
			treeDir = +1;
		}

	}
} // TreeBounce ***************************************************************************************************************************************************************************************************************************************


void TreeRocket()
{
	if (currentMillis - previousTree > 60)
	{
		previousTree = currentMillis;

		if (treeCounter <= 0)
		{
			treeStage = 0;
			treeCounter = 0;
			SetColour(&tree, 0, 0, 0);
			tree.show();
		}

		for (int i = 0; i < TREE_PIXELS; i++)
		{
			if (i < treeCounter)
			{
				tree.setPixelColor(i, 30, 30, 30);
			}
			else if (i == treeCounter)
			{
				int colour = 0;
				if (treeStage == 0)
					colour = 90;
				else if (treeStage == 1)
					colour = MAX;
				else if (treeStage == 2)
					colour = 200;

				if (++treeStage > 2)
					treeStage = 0;

				tree.setPixelColor(i, colour, colour, colour);
			}
			else
			{
				tree.setPixelColor(i, 0, 0, 0);
			}
		}

		tree.show();

		if (++treeCounter > TREE_PIXELS)
			treeCounter = 0;
	}

} // TreeRocket ***************************************************************************************************************************************************************************************************************************************


void TreeRainbowAll()
{
	if (currentMillis - previousMillis > 120)
	{
		previousTree = currentMillis;

		if (treeCounter++ > 256)
			treeCounter = 0;

		for (int i = 0; i < TREE_PIXELS; i++)
		{
			tree.setPixelColor(i, Wheel(&left, (i + treeCounter) & 255));
		}

		tree.show();
	}
} // TreeRainbow ****************************************************************************************************************************************************************************************************************************


void TreeRainbowPixel()
{
	if (currentMillis - previousMillis > 120)
	{
		previousTree = currentMillis;

		if (treeCounter++ > 256)
			treeCounter = 0;

		for (int i = 0; i < TREE_PIXELS; i++)
		{
			tree.setPixelColor(i, Wheel(&left, (i + treeCounter) & 255));
		}

		tree.show();
	}
} // TreeRainbow ****************************************************************************************************************************************************************************************************************************


void PorchSparkle()
{
	if (currentMillis - previousPorch > 200)
	{
		previousPorch = currentMillis;

		if (porchCounter++ == 0)
		{
			SetColour(&porch, 40, 40, 40);
		}

		for (int i = 0; i < 50; i++)
		{
			int on = random(0, PORCH_PIXELS);
			int off = random(0, PORCH_PIXELS);

			porch.setPixelColor(off, 40, 40, 40);
			porch.setPixelColor(on, 200, 200, 200);

		}

		porch.show();
	}
} // PorchSparkle ***************************************************************************************************************************************************************************************************************************************


void PorchFader()
{
	if (currentMillis - previousPorch > 70)
	{
		previousPorch = currentMillis;

		porchCounter += porchDir;

		if (porchCounter >= 100)
		{
			porchDir = -1;
		}
		else if (porchCounter <= 10)
		{
			porchDir = +1;
		}

		SetColour(&porch, porchCounter, porchCounter, porchCounter);

		porch.show();
	}
} // PorchFader ***************************************************************************************************************************************************************************************************************************************


void PorchRainbowPixel()
{
	if (currentMillis - previousMillis > 120)
	{
		previousPorch = currentMillis;

		if (porchCounter++ > 256)
			porchCounter = 0;

		for (int i = 0; i < PORCH_PIXELS; i++)
		{
			porch.setPixelColor(i, Wheel(&left, (i + porchCounter) & 255));
		}

		porch.show();
	}
} // PorchRainbowPixel ****************************************************************************************************************************************************************************************************************************


void PorchRainbowAll()
{
	if (currentMillis - previousMillis > 120)
	{
		previousPorch = currentMillis;

		if (porchCounter++ > 256)
			porchCounter = 0;

		for (int i = 0; i < PORCH_PIXELS; i++)
		{
			porch.setPixelColor(i, Wheel(&left, (i + porchCounter) & 255));
		}

		porch.show();
	}
} // PorchRainbowAll ****************************************************************************************************************************************************************************************************************************


void PorchStarTurn()
{
	if (currentMillis - previousPorch > 600)
	{
		previousPorch = currentMillis;

		ProcessPorchArray(porchStars);
		//SetColour(&porch, 0, 0, 0);

		//for (int i = 0; i < 10; i++)
		//{
		//	int pos = porchStars[porchCounter][i];
		//	if (pos >= 0)
		//		porch.setPixelColor(pos, 200, 200, 200);
		//}

		//porch.show();

		//if (++porchCounter > 10)
		//{
		//	porchCounter = 0;
		//}
	}
} // PorchStarTurn ***************************************************************************************************************************************************************************************************************************************


void ProcessPorchArray(int pixels[][10])
{
	SetColour(&porch, 0, 0, 0);

	for (int i = 0; i < 10; i++)
	{
		int pos = pixels[porchCounter][i];
		if (pos >= 0)
			porch.setPixelColor(pos, 200, 200, 200);
	}

	porch.show();

	if (++porchCounter > 10)
		porchCounter = 0;
} // ProcessPorchArray ***************************************************************************************************************************************************************************************************************************************


void ProcessTreeArray(int pixels[][10])
{
	SetColour(&tree, 0, 0, 0);

	for (int i = 0; i < 10; i++)
	{
		int pos = pixels[treeCounter][i];
		if (pos >= 0)
			tree.setPixelColor(pos, 200, 200, 200);
	}

	tree.show();

	if (++treeCounter > 10)
		treeCounter = 0;
} // ProcessTreeArray ***************************************************************************************************************************************************************************************************************************************


void TreeFader()
{
	if (currentMillis - previousTree > 70)
	{
		previousTree = currentMillis;

		treeCounter += treeDir;

		if (treeCounter >= 100)
		{
			treeDir = -1;
		}
		else if (treeCounter <= 10)
		{
			treeDir = +1;
		}

		SetColour(&tree, treeCounter, treeCounter, treeCounter);

		tree.show();
	}
} // TreeFader ***************************************************************************************************************************************************************************************************************************************


void StripTwinkle(Adafruit_NeoPixel *strip, int max, uint32_t lowred, uint32_t lowgreen, uint32_t lowblue, uint32_t highred, uint32_t highgreen, uint32_t highblue)
{
	int on = random(0, max);
	int off = random(0, max);

	strip->setPixelColor(off, lowred, lowgreen, lowblue);
	strip->setPixelColor(on, highred, highgreen, highblue);
} // StripTwinkle ***************************************************************************************************************************************************************************************************************************************


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


uint32_t Wheel(Adafruit_NeoPixel *strip, byte WheelPos)
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85)
	{
		return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	else if (WheelPos < 170)
	{
		WheelPos -= 85;
		return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	else
	{
		WheelPos -= 170;
		return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
} // Wheel ***************************************************************************************************************************************************************************************************************************************

