#include <Adafruit_NeoPixel.h>



#define PIN_NEOTREE		8
#define PIN_LED			13

#define MIN				0
#define LOW				20
#define MAX				70
#define STARBURST_MAX	30

#define TREE_PIXELS		300
#define FLAKE_PIXELS	50

Adafruit_NeoPixel tree = Adafruit_NeoPixel(TREE_PIXELS, PIN_NEOTREE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel flake = Adafruit_NeoPixel(FLAKE_PIXELS, 9, NEO_RGB + NEO_KHZ800);


unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousRadio = 0;
unsigned long previousTree = 0;
unsigned long previousPorch = 0;
unsigned long sceneMillis = 0;
unsigned long SceneLength = 5 * 1000; // 20 seconds for scene

int ledCounter = 0;
unsigned long previousLED = 0;
int currentAct = 109;
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
int porchCounter = 0;
int porchDir = 0;

unsigned long previousFlake = 0;
int flakeCounter = 0;

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
//uint32_t red;
//uint32_t green;
//uint32_t blue;


void setup()
{
	Serial.begin(9600);
	pinMode(PIN_LED, OUTPUT);


	tree.begin();

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

	// 

	Act109();

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
} // SetStrips ****************************************************************************************************************************************************************************************************************************


void SetStripsBrightness(int level)
{
	tree.setBrightness(counter);
} // SetStripsBrightness ****************************************************************************************************************************************************************************************************************************



void ShowStrips()
{

	tree.show();
} // ShowStrips ****************************************************************************************************************************************************************************************************************************


void Act109()
{
	if (previousAct != currentAct)
	{
		Serial.println("Start 109, Tree Only");
		previousAct = currentAct;
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
			TreeFader();
		}
		else if (currentScene == 2)
		{
			Glimmer(); // All over
		}
		else if (currentScene == 3)
		{
			TreeSparkle();
		}
		else if (currentScene == 4)
		{
			TreeBerries();
		}
		else if (currentScene == 5)
		{
			TreeRocket();
		}
		else if (currentScene == 6)
		{
			TreeRainbowPixel();
		}
		else if (currentScene == 7)
		{
			TreeSpiral();
		}
		else if (currentScene == 8)
		{
			Disco(); // All over
		}
		else if (currentScene == 9)
		{
			TreeMash();
		}
		else if (currentScene == 10)
		{
			TreeDomeTrail();
		}
		else
		{
			StartScene();
		}
	}
} // Act109 ***************************************************************************************************************************************************************************************************************************************


void StartScene()
{
	previousScene = currentScene;
	currentScene = 1;
	stage = 0;
	counter = 0;
	treeCounter = 0;
	porchCounter = 0;

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

} // NextScene ***************************************************************************************************************************************************************************************************************************************


bool NewScene()
{
	if (previousScene == currentScene)
		return false;

	previousScene = currentScene;

	return true;
} // NewScene ***************************************************************************************************************************************************************************************************************************************


void FadeUpWhite()
{
	if (currentMillis - previousMillis > 60)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetStrips(0, 0, 0);
			ShowStrips();
		}

		if (++counter > MAX)
		{
			NextScene();
		}
		else
		{
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
		}

		if (++counter > MAX)
		{
			NextScene();
		}
		else
		{
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

} // SetPixel ****************************************************************************************************************************************************************************************************************************


void SetHousePixel(int pixelIndex, int red, int green, int blue)
{
	SetHousePixel(pixelIndex, red, green, blue, true);
} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


void SetHousePixel(int pixelIndex, int red, int green, int blue, bool show)
{
	//uint32_t colour = left.Color(red, green, blue);
	//SetHousePixel(pixelIndex, colour, show);
} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


void SetHousePixel(int pixelIndex, uint32_t colour, bool show)
{


} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


void Disco()
{
	if (currentMillis - previousMillis > 120)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetStrips(0, 0, 0); // off
			ShowStrips();
		}

		for (uint16_t i = 0; i < tree.numPixels(); i++)
		{
			uint32_t colour = tree.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			tree.setPixelColor(i, colour);
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
			//SetHouse(MAX, MAX, MAX); // white
			//ShowHouse();
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
	uint32_t colour = tree.Color(r, g, b);

	int middle = 50;
	tree.setPixelColor(middle - counter, colour);
	tree.setPixelColor(middle + counter, colour);

	tree.show();
} // Divider


void TreeTwinkle()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetStrips(0, 0, 0);
		}

		int l = 0;
		int h = 20;

		for (int i = 0; i < 20; i++)
		{
			StripTwinkle(l, l, l, h, h, h);
		}

		if (++stage > 4)
		{
			stage = 0;

			l = 40;
			h = 160;

			for (int i = 0; i < 10; i++)
			{
				StripTwinkle(l, l, l, h, h, h);
			}
		}

		tree.show();

	}
} // TreeTwinkle ****************************************************************************************************************************************************************************************************************************


void Glimmer()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
		}

		SetStrips(LOW, LOW, LOW);

		if (++stage > 39)
			stage = 0;

		SetBar(&tree, stage + 00, white);
		SetBar(&tree, stage + 50, white);
		SetBar(&tree, stage + 100, white);
		SetBar(&tree, stage + 150, white);
		SetBar(&tree, stage + 200, white);
		SetBar(&tree, stage + 250, white);

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
			SetStrips(10, 10, 10);
			ShowStrips();
		}

		int l = 10;
		int h = 40;

		for (int i = 0; i < 50; i++)
		{
			StripTwinkle(l, l, l, h, h, h);
		}

		ShowStrips();

		if (counter++ > 1000)
			counter = 0;
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

		NewScene();

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

		NewScene();

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
	if (currentMillis - previousMillis > 5)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetStrips(LOW, LOW, LOW);
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
			colour = tree.Color(MAX, 0, 0); // red
		}
		else if (stage == 1)
		{
			colour = tree.Color(0, MAX, 0); // green
		}
		else if (stage == 2)
		{
			colour = tree.Color(0, 0, MAX); // blue
		}
		else if (stage == 3)
		{
			colour = tree.Color(MAX, MAX, 0);
		}
		else if (stage == 4)
		{
			colour = tree.Color(0, MAX, MAX);
		}
		else if (stage == 5)
		{
			colour = tree.Color(MAX, 0, MAX);
		}
		else if (stage == 6)
		{
			colour = tree.Color(255, 128, 0); // orange
		}
		else if (stage == 7)
		{
			colour = tree.Color(200, 20, 127); // pink 255, 20, 147
		}
		else if (stage == 8)
		{
			colour = tree.Color(0, 205, 205); // cyan
		}
		else if (stage == 9)
		{
			colour = tree.Color(238, 201, 0); // yellow gold
		}
		else if (stage == 10)
		{
			colour = Wheel(&tree, counter); // rainbow
		}
		else if (stage == 11)
		{
			colour = tree.Color(MAX, MAX, MAX); // white
		}

		for (int i = 0; i < 5; i++)
		{
			tree.setPixelColor(counter, colour);
			counter += sceneDir;
		}

		tree.show();

		if (counter <= 0)
		{
			sceneDir = +1;
			stage++;
		}
		else if (counter >= TREE_PIXELS)
		{
			sceneDir = -1;
			stage++;
		}

		if (stage > 11)
			stage = 0;

	}

} // TreeMash ****************************************************************************************************************************************************************************************************************************


void TreeDomeTrail()
{
	if (currentMillis - previousMillis > 10)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetStrips(LOW, LOW, LOW);
		}

		uint32_t colour = 0;

		if (stage == 0)
		{
			colour = tree.Color(0, MAX, 0);
		}
		else if (stage == 1)
		{
			colour = tree.Color(MAX, 0, 0);
		}
		else if (stage == 2)
		{
			colour = tree.Color(0, 0, MAX);
		}
		else if (stage == 3)
		{
			colour = tree.Color(MAX, MAX, 0);
		}
		else if (stage == 4)
		{
			colour = tree.Color(0, MAX, MAX);
		}
		else if (stage == 5)
		{
			colour = tree.Color(MAX, 0, MAX);
		}
		else if (stage == 6)
		{
			colour = Wheel(&tree, counter * 2);
			//colour = left.Color(MAX, 0, MAX);
		}
		else if (stage == 7)
		{
			colour = tree.Color(MAX, MAX, MAX);
		}

		tree.setPixelColor(counter, colour);
		tree.setPixelColor(TREE_PIXELS - counter, colour);

		if (++counter > (TREE_PIXELS / 2))
		{
			counter = 0;
			stage++;
		}

		if (stage > 7)
			stage = 0;

		tree.show();
	}
} // TreeDomeTrail ****************************************************************************************************************************************************************************************************************************


void TreeBerries()
{
	if (currentMillis - previousTree > 400)
	{
		previousTree = currentMillis;

		NewScene();

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


void TreeRocket()
{
	if (currentMillis - previousTree > 1)
	{
		previousTree = currentMillis;

		NewScene();

		//if (treeCounter == 0)
		//{
		//SetColour(&tree, 0, 100, 100);
		//tree.show();
		//}

		//if (++treeCounter > TREE_PIXELS)
		//treeCounter = 0;

		for (int i = 0; i < TREE_PIXELS; i++)
		{
			if (i < treeCounter)
				tree.setPixelColor(i, MAX, 0, 0);
			else
				tree.setPixelColor(i, 0, 10, 10);
			//SetColour(&tree, 0, 10, 10);
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
} // TreeRocket ***************************************************************************************************************************************************************************************************************************************


void TreeRainbowAll()
{
	if (currentMillis - previousMillis > 140)
	{
		previousTree = currentMillis;

		NewScene();

		if (treeCounter++ > 256)
			treeCounter = 0;

		for (int i = 0; i < TREE_PIXELS; i++)
		{
			tree.setPixelColor(i, Wheel(&tree, (i + treeCounter) & 255));
		}

		tree.show();
	}
} // TreeRainbow ****************************************************************************************************************************************************************************************************************************


void TreeRainbowPixel()
{
	if (currentMillis - previousMillis > 140)
	{
		previousTree = currentMillis;

		NewScene();

		if (treeCounter++ > 256)
			treeCounter = 0;

		for (int i = 0; i < TREE_PIXELS; i++)
		{
			tree.setPixelColor(i, Wheel(&tree, (i + treeCounter) & 255));
		}

		tree.show();
	}
} // TreeRainbow ****************************************************************************************************************************************************************************************************************************


void TreeFader()
{
	if (currentMillis - previousTree > 70)
	{
		previousTree = currentMillis;

		NewScene();

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


void StripTwinkle(uint32_t lowred, uint32_t lowgreen, uint32_t lowblue, uint32_t highred, uint32_t highgreen, uint32_t highblue)
{
	int on = random(0, 100);
	int off = random(0, 100);

	tree.setPixelColor(off, lowred, lowgreen, lowblue);
	tree.setPixelColor(on, highred, highgreen, highblue);
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



void FlakeSparkle()
{
	if (currentMillis - previousFlake > 200)
	{
		previousFlake = currentMillis;

		//NewScene();

		if (flakeCounter++ == 0)
		{
			SetColour(&flake, 40, 40, 40);
		}

		for (int i = 0; i < 20; i++)
		{
			int on = random(0, FLAKE_PIXELS);
			int off = random(0, FLAKE_PIXELS);

			flake.setPixelColor(off, 40, 40, 40);
			flake.setPixelColor(on, 200, 200, 200);
		}

		flake.show();
	}
} // FlakeSparkle ***************************************************************************************************************************************************************************************************************************************


void FlakeRainbow()
{
	if (currentMillis - previousFlake > 140)
	{
		previousFlake = currentMillis;

		//NewScene();

		if (flakeCounter++ > 256)
			flakeCounter = 0;

		for (int i = 0; i < FLAKE_PIXELS; i++)
		{
			flake.setPixelColor(i, Wheel(&flake, flakeCounter));
		}

		flake.show();
	}
} // FlakeRainbow ****************************************************************************************************************************************************************************************************************************


void FlakeDisco()
{
	if (currentMillis - previousFlake > 120)
	{
		previousFlake = currentMillis;

		for (uint16_t i = 0; i < flake.numPixels(); i++)
		{
			uint32_t colour = flake.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			flake.setPixelColor(i, colour);
		}

		flake.show();
	}
} // FlakeDisco ****************************************************************************************************************************************************************************************************************************


void FlakeStepper()
{
	if (currentMillis - previousFlake > 120)
	{
		previousFlake = currentMillis;

		if (flakeCounter++ <= 0)
		{
			flakeCounter = 0;
			SetColour(&flake, 0, 0, 0);
		}
		else
		{
			flake.setPixelColor(flakeCounter, MAX, MAX, MAX);
		}

		flake.show();
	}
} // FlakeStepper ****************************************************************************************************************************************************************************************************************************

