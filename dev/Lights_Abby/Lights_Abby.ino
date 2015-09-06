#include <Adafruit_NeoPixel.h>
#include <U8glib.h>

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

#define PIN_NEOHOUSE	7
#define PIN_NEOPORCH	9
#define PIN_LED			13

#define MIN				0
//#define LddOW				20
#define DAY_MAX			60
#define NIGHT_MAX		95

#ifdef FULLHOUSE
//#define LEFT_PIXELS		150
//#define TOP_PIXELS		200
//#define RIGHT_PIXELS	50
#define PORCH_PIXELS	150
//#define HOUSE_PIXELS	4
//#define ALL_PIXELS		1050
#endif

#ifdef TEST1
#define TREE_PIXELS		50
#define LEFT_PIXELS		150
#define TOP_PIXELS		150
#define RIGHT_PIXELS	150
#define PORCH_PIXELS	50
#define HOUSE_PIXELS	450
#define ALL_PIXELS		600
#endif


Adafruit_NeoPixel porch = Adafruit_NeoPixel(PORCH_PIXELS, PIN_NEOPORCH, NEO_RGB + NEO_KHZ800);
//Adafruit_NeoPixel house = Adafruit_NeoPixel(HOUSE_PIXELS, PIN_NEOHOUSE, NEO_GRB + NEO_KHZ800);

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send AC, I2C uses pins A4 and A5
// Nano
// GND  BLACK   GND
// VCC  RED     5V
// SCL  GREEN   A5
// SDA  YELLOW  A4

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousPorch = 0;
unsigned long previousLED = 0;
unsigned long sceneMillis = 0;
unsigned long SceneLength = 20 * 1000; // 20 seconds for scene

int gap = 120;

unsigned long running = 0;
int ledCounter = 0;
int mode = 2; // no abby 18 , mode==2 Abby 18 Today
int max = NIGHT_MAX;
int currentAct = 101;
int previousAct = -1;
int currentScene = 0;
int previousScene = -1;
int sceneDir = 0;
int stage = 0;
int position = 0;
int counter = 0;
int last = 0;

int porchCounter = 0;
int porchDir = 0;
int porchStage = 0;
int porchX = 0;
int porchY = 0;
int porchXdir = 0;
int porchYdir = 0;

String title;
String message;


/*

Left		  1 - 150	150
bay left	151 - 176	 26
bay front	176 - 267	 91
bay right	267 - 289	 22
bathroom	293 - 362	 69
right		362 - 450	 88

*/


void Display()
{
	u8g.firstPage();
	do {
		u8g_prepare();

		u8g.setFont(u8g_font_courB08);

		u8g.setPrintPos(0, 10);
		u8g.print(title);

		//u8g.setFont(u8g_font_courB24);
		//u8g.setFont(u8g_font_courB18);
		u8g.setFont(u8g_font_courB14);

		u8g.setPrintPos(5, 40);
		u8g.print(message);

		u8g.setPrintPos(5, 55);
		u8g.print(running);

		u8g.setPrintPos(75, 55);
		u8g.print(previousScene);

		u8g.setPrintPos(105, 55);
		u8g.print(currentScene);

	} while (u8g.nextPage());

}

void u8g_prepare(void) {
	//u8g.setFont(u8g_font_6x10);
	u8g.setFontRefHeightExtendedText();
	u8g.setDefaultForegroundColor();
	u8g.setFontPosTop();
}



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

		title = "Act101";
		message = "Start";
		Display();

	}
	else
	{
		// change scene every 20 seconds
		if (currentMillis - sceneMillis > SceneLength)
		{
			Serial.println("NextScene Auto");
			NextScene();
			title = "Act101";
			message = "NextScene";
			Display();
		}

		if (currentScene != previousScene)
		{
			title = "Scene";
			message = "Scene ";
			message += currentScene;
			message += ", (";
			message += previousScene;
			message += ")";

			//Serial.print("Scene ");
			//Serial.print(currentScene);
			//Serial.print(", (");
			//Serial.print(previousScene);
			//Serial.println(")");

			Serial.println(message);

			message = "z";
			//message += currentScene;
			//message += ", (";
			//message += previousScene;
			//message += ")";

			//Display();
		}

		if (currentScene == 1)
		{
			WhiteOnly(); //		Sparkle();
			PorchHappyBirthday1();
		}
		else if (currentScene == 2)
		{
			WhiteOnly(); // Twinkle();

			//if (mode == 1)
			PorchHappyBirthday4(); // Happy Birthday Abby
			//else
			//PorchHappyBirthday2(); // Abby 18 Today
		}
		else if (currentScene == 3)
		{
			WhiteOnly(); // HouseClock();
			PorchHappyBirthday3();
		}
		else if (currentScene == 4)
		{
			WhiteOnly(); // Divider();
			PorchHappyBirthday4();
		}
		else if (currentScene == 5)
		{
			WhiteOnly(); // DomeTrail();
			PorchHappyBirthday5();
		}
		//else if (currentScene == 6)
		//{
		//	WhiteOnly(); //RainbowHousePixel();
		//	PorchHappyBirthday1();
		//}
		//else if (currentScene == 7)
		//{
		//	WhiteOnly(); // Mash();
		//	PorchHappyBirthday1();
		//}
		//else if (currentScene == 8)
		//{
		//	WhiteOnly(); // RainbowHouseTwinkle();
		//	PorchHappyBirthday1();
		//}
		else
		{
			StartScene();
		}
	}
} // Act101 ***************************************************************************************************************************************************************************************************************************************




void setup()
{
	title = "Setup";
	message = "0";

	Serial.begin(9600);
	pinMode(PIN_LED, OUTPUT);

	porch.begin();
	//house.begin();

	randomSeed(analogRead(2) + analogRead(4) + millis());

	Serial.println("Ready, v2.0 Sheila");

	Serial.println("5");
	digitalWrite(PIN_LED, HIGH);
	delay(1000);
	Serial.println("4");
	digitalWrite(PIN_LED, LOW);
	delay(1000);
	Serial.println("3");
	digitalWrite(PIN_LED, HIGH);
	delay(1000);
	Serial.println("2");
	digitalWrite(PIN_LED, LOW);
	delay(1000);
	Serial.println("1");
	digitalWrite(PIN_LED, HIGH);
	delay(1000);
	Serial.println("0");
	digitalWrite(PIN_LED, LOW);

	message = "1";
	Display();

	SetStrips(0, 0, 0);
	ShowStrips();

	previousLED = millis();

	message = "2";
	Display();

	NextScene();
	StartScene();

	message = "3";
	Display();
} // setup ****************************************************************************************************************************************************************************************************************************



void loop()
{

	currentMillis = millis();

	// Heart beat LED

	if (currentMillis - previousLED > 100)
	{
		previousLED = currentMillis;

		if (++ledCounter > 10)
		{
			running++;
			ledCounter = 0;
			Serial.print("Tick ");
			Serial.print(currentScene);
			Serial.print(", (");
			Serial.print(previousScene);
			Serial.println(")");

			Display();
		}

		if (ledCounter == 1 || ledCounter == 2 || ledCounter == 4 || ledCounter == 5)
			digitalWrite(PIN_LED, HIGH);
		else
			digitalWrite(PIN_LED, LOW);

		//if (analogRead(A0) > 500)
		//	max = DAY_MAX;
		//else
		//	mode = NIGHT_MAX;

		//if (analogRead(A1) > 500)
		//	mode = 2;
		//else
		//	mode = 1;
	}

	// check for serial comms

	//if (Serial.available() > 0)
	//{
	//	int input = Serial.parseInt();

	//	if (input >= 0 && input < 1001)
	//	{
	//		currentAct = input;
	//	}
	//}


	//

	Act101();

	previousAct = currentAct;

	//Display();

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
	SetColour(&porch, red, green, blue);
	//SetColour(&house, red, green, blue);
} // SetStrips ****************************************************************************************************************************************************************************************************************************


void SetStripsBrightness(int level)
{
	//house.setBrightness(level);
	porch.setBrightness(level);
} // SetStripsBrightness ****************************************************************************************************************************************************************************************************************************


void SetHouse(int red, int green, int blue)
{
	//SetColour(&house, red, green, blue);
} // SetStrips ****************************************************************************************************************************************************************************************************************************


void ShowStrips()
{
	porch.show();
	//house.show();
} // ShowStrips ****************************************************************************************************************************************************************************************************************************


void ShowHouse()
{
	//house.show();
} // ShowHouse ****************************************************************************************************************************************************************************************************************************


void StartScene()
{
	previousScene = currentScene;
	currentScene = 1;
	stage = 0;
	counter = 0;
	porchCounter = 0;

	SetStripsBrightness(255);

} // StartScene ***************************************************************************************************************************************************************************************************************************************


void NextScene()
{
	previousScene = currentScene;
	currentScene++;
	sceneDir = 1;
	stage = 0;
	counter = 0;

	porchCounter = 0;
	porchDir = 1;
	porchStage = 0;
	porchX = 0;
	porchY = 0;

	sceneMillis = currentMillis;

} // NextScene ***************************************************************************************************************************************************************************************************************************************


bool NewScene()
{
	if (previousScene == currentScene)
		return false;

	previousScene = currentScene;

	return true;
} // NewScene ***************************************************************************************************************************************************************************************************************************************


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


//void SetHouseBar(int pixelIndex, int red, int green, int blue)
//{
//
//	SetHousePixel(pixelIndex - 2, red, green, blue);
//	SetHousePixel(pixelIndex - 1, red, green, blue);
//	SetHousePixel(pixelIndex + 0, red, green, blue);
//	SetHousePixel(pixelIndex + 1, red, green, blue);
//	SetHousePixel(pixelIndex + 2, red, green, blue);
//
//} // SetHouseBar ****************************************************************************************************************************************************************************************************************************


//void SetHousePixel(int pixelIndex, int red, int green, int blue)
//{
//	SetHousePixel(pixelIndex, red, green, blue, true);
//} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


//void SetHousePixel(int pixelIndex, int red, int green, int blue, bool show)
//{
//	uint32_t colour = house.Color(red, green, blue);
//	SetHousePixel(pixelIndex, colour, show);
//} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


//void SetHousePixel(int pixelIndex, uint32_t colour, bool show)
//{
//	house.setPixelColor(pixelIndex, colour);
//
//	if (show)
//		house.show();
//
//} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


//void SetLeftPixel(int pixelIndex, uint32_t colour)
//{
//	if (pixelIndex >= 0 && pixelIndex <= 150)
//		house.setPixelColor(pixelIndex, colour);
//} // SetLeftPixel ****************************************************************************************************************************************************************************************************************************


void SetBayPixel(int pixelIndex, uint32_t colour)
{
	//if (pixelIndex >= 176 && pixelIndex <= 267)
	//	house.setPixelColor(pixelIndex, colour);
} // SetBayPixel ****************************************************************************************************************************************************************************************************************************


void SetBathroomPixel(int pixelIndex, uint32_t colour)
{
	//if (pixelIndex >= 293 && pixelIndex <= 361)
	//	house.setPixelColor(pixelIndex, colour);
} // SetBathroomPixel ****************************************************************************************************************************************************************************************************************************


void SetRightPixel(int pixelIndex, uint32_t colour)
{
	//	if (pixelIndex >= 362 && pixelIndex <= 450)
	//	house.setPixelColor(pixelIndex, colour);
} // SetRightPixel ****************************************************************************************************************************************************************************************************************************


void WhiteOnly()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetHouse(40, 40, 40);
			ShowHouse();
		}
	}
}


//void Disco()
//{
//	if (currentMillis - previousMillis > 20)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			SetStrips(0, 0, 0); // off
//			ShowStrips();
//		}
//
//		for (uint16_t i = 0; i < house.numPixels(); i++)
//		{
//			uint32_t colour = house.Color(random(MIN, max), random(MIN, max), random(MIN, max));
//			house.setPixelColor(i, colour);
//		}
//
//		for (uint16_t i = 0; i < porch.numPixels(); i++)
//		{
//			uint32_t colour = porch.Color(random(MIN, max), random(MIN, max), random(MIN, max));
//			porch.setPixelColor(i, colour);
//		}
//
//		ShowStrips();
//	}
//} // Disco ****************************************************************************************************************************************************************************************************************************


//void Divider()
//{
//	if (currentMillis - previousMillis > 20)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			SetHouse(max, max, max); // white
//			ShowHouse();
//		}
//
//		if (stage == 0)
//			Divider(max, 0, 0); // red
//		else if (stage == 1)
//			Divider(125, 38, 205); // purple
//		else if (stage == 2)
//			Divider(0, max, 0); // green
//		else if (stage == 3)
//			Divider(0, 0, max); // blue
//		else if (stage == 4)
//			Divider(238, 201, 0); // yellw gold
//		else if (stage == 5)
//			Divider(0, 205, 205); // cyan
//		else if (stage == 6)
//			Divider(255, 20, 147); // pink
//		else if (stage == 7)
//			Divider(255, 128, 0); // orange
//		else if (stage == 8)
//			Divider(150, 150, 150); // white
//
//		counter++;
//		if (counter > 75)
//		{
//			counter = 0;
//
//			stage++;
//			if (stage > 8)
//				stage = 0;
//		}
//	}
//} // Divider ****************************************************************************************************************************************************************************************************************************



//void Divider(int r, int g, int b)
//{
//	int middle;
//	uint32_t colour = house.Color(r, g, b);
//
//	middle = 75;
//	SetLeftPixel(middle - counter, colour);
//	SetLeftPixel(middle + counter, colour);
//
//	middle = 221;
//	SetBayPixel(middle - counter, colour);
//	SetBayPixel(middle + counter, colour);
//
//	middle = 327;
//	SetBathroomPixel(middle - counter, colour);
//	SetBathroomPixel(middle + counter, colour);
//
//	middle = 406;
//	SetRightPixel(middle - counter, colour);
//	SetRightPixel(middle + counter, colour);
//
//	house.show();
//} // Divider


//void RainbowHousePixel()
//{
//	if (currentMillis - previousMillis > 20)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			counter = random(10, 250);
//		}
//
//		if (counter++ > 256)
//			counter = 0;
//
//		for (int i = 0; i < HOUSE_PIXELS; i++)
//		{
//			SetHousePixel(i, Wheel(&house, (i + counter) & 255), false);
//		}
//
//		ShowHouse();
//	}
//} // RainbowHousePixel ****************************************************************************************************************************************************************************************************************************


//int rainbow1 = 0;
//int rainbow2 = 0;
//int rainbow3 = 0;
//int rainbow4 = 0;
//int rainbow5 = 0;
//int rainbow6 = 0;
//
//void RainbowHouseSections()
//{
//	if (currentMillis - previousMillis > 20)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			counter = 0;
//			rainbow1 = random(10, 200);
//			rainbow2 = random(20, 150);
//			rainbow3 = random(30, 250);
//			rainbow4 = random(40, 250);
//			rainbow5 = random(50, 200);
//			rainbow6 = random(60, 100);
//		}
//
//		if (counter++ > 256)
//			counter = 0;
//
//		if (rainbow1++ > 256)
//			rainbow1 = 0;
//
//		if (rainbow2++ > 256)
//			rainbow2 = 0;
//
//		if (rainbow3++ > 256)
//			rainbow3 = 0;
//
//		if (rainbow4++ > 256)
//			rainbow4 = 0;
//
//		if (rainbow5++ > 256)
//			rainbow5 = 0;
//
//		if (rainbow6++ > 256)
//			rainbow6 = 0;
//
//		for (int i = 0; i < 150; i++)
//			house.setPixelColor(i, Wheel(&house, (rainbow1 + counter) & 255));
//
//		for (int i = 151; i < 175; i++)
//			house.setPixelColor(i, Wheel(&house, (rainbow2 + counter) & 255));
//
//		for (int i = 176; i < 267; i++)
//			house.setPixelColor(i, Wheel(&house, (rainbow3 + counter) & 255));
//
//		for (int i = 268; i < 292; i++)
//			house.setPixelColor(i, Wheel(&house, (rainbow4 + counter) & 255));
//
//		for (int i = 293; i < 361; i++)
//			house.setPixelColor(i, Wheel(&house, (rainbow5 + counter) & 255));
//
//		for (int i = 362; i <= 450; i++)
//			house.setPixelColor(i, Wheel(&house, (rainbow6 + counter) & 255));
//
//		house.show();
//	}
//} // RainbowHouseSections ****************************************************************************************************************************************************************************************************************************


//void RainbowHouseAll()
//{
//	if (currentMillis - previousMillis > 20)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			counter = random(25, 233);
//		}
//
//		if (counter++ > 256)
//			counter = 0;
//
//		uint32_t colour = Wheel(&house, (counter)& 255);
//
//		SetColour(&house, colour);
//
//		ShowHouse();
//	}
//} // RainbowHouseAll ****************************************************************************************************************************************************************************************************************************


//void RainbowHouseTwinkle()
//{
//	if (currentMillis - previousMillis > 20)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			counter = random(10, 250);
//		}
//
//		if (counter++ > 256)
//			counter = 0;
//
//		for (int i = 0; i < HOUSE_PIXELS; i++)
//		{
//			SetHousePixel(i, Wheel(&house, (i + counter) & 255), false);
//		}
//
//		int l = 70;
//		int h = 180;
//
//		for (int i = 0; i < 10; i++)
//		{
//			StripTwinkle(&house, HOUSE_PIXELS, l, l, l, h, h, h);
//		}
//
//		ShowHouse();
//	}
//} // RainbowHouseTwinkle ****************************************************************************************************************************************************************************************************************************


//void RainbowAll()
//{
//	if (currentMillis - previousMillis > 20)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			counter = random(25, 233);
//		}
//
//		if (counter++ > 256)
//			counter = 0;
//
//		int red, green, blue;
//
//		Wheel(counter, &red, &green, &blue);
//
//		SetColour(&house, red, green, blue);
//		SetColour(&porch, red, green, blue);
//
//		ShowStrips();
//
//	}
//} // RainbowAll ****************************************************************************************************************************************************************************************************************************


//void HouseColourFader()
//{
//
//	if (currentMillis - previousMillis > 80)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			SetHouse(0, 0, 0);
//			ShowHouse();
//
//			counter = 0;
//			sceneDir = 20;
//		}
//		else
//		{
//			counter += sceneDir;
//
//			uint32_t colour = StageColour(&house, stage);
//
//			SetColour(&house, colour);
//			house.setBrightness(counter);
//			house.show();
//
//			if (counter <= 0)
//			{
//				sceneDir = +20;
//				if (stage++ > 11)
//					stage = 0;
//			}
//			else if (counter >= 255)
//			{
//				sceneDir = -20;
//			}
//
//		}
//
//	}
//
//} // HouseColourFader ****************************************************************************************************************************************************************************************************************************


//void HouseClock()
//{
//
//	if (currentMillis - previousMillis > 80)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//		}
//
//		for (int i = counter; i < HOUSE_PIXELS; i += 4)
//		{
//			if (sceneDir == 1)
//			{
//				SetHousePixel(i + 0, 10, 10, 10, false);
//				SetHousePixel(i + 1, 20, 20, 20, false);
//				SetHousePixel(i + 2, 40, 40, 40, false);
//				SetHousePixel(i + 3, max, max, max, false);
//			}
//			else
//			{
//				SetHousePixel(HOUSE_PIXELS - (i + 0), 10, 10, 10, false);
//				SetHousePixel(HOUSE_PIXELS - (i + 1), 20, 20, 20, false);
//				SetHousePixel(HOUSE_PIXELS - (i + 2), 40, 40, 40, false);
//				SetHousePixel(HOUSE_PIXELS - (i + 3), max, max, max, false);
//			}
//		}
//
//		ShowHouse();
//
//		counter++;
//		if (counter > 3)
//			counter = 0;
//
//		if (stage++ > 20)
//		{
//			stage = 0;
//			sceneDir = !sceneDir;
//		}
//	}
//} // HouseClock ****************************************************************************************************************************************************************************************************************************


//void HouseClockWise()
//{
//
//	if (currentMillis - previousMillis > 80)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//		}
//
//		for (int i = counter; i < HOUSE_PIXELS; i += 4)
//		{
//			SetHousePixel(i + 0, 10, 10, 10, false);
//			SetHousePixel(i + 1, 20, 20, 20, false);
//			SetHousePixel(i + 2, 40, 40, 40, false);
//			SetHousePixel(i + 3, max, max, max, false);
//		}
//
//		ShowHouse();
//
//		counter++;
//		if (counter > 3)
//			counter = 0;
//	}
//} // HouseClockWise ****************************************************************************************************************************************************************************************************************************


//void HouseAntiClockWise()
//{
//
//	if (currentMillis - previousMillis > 80)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//		}
//
//		for (int i = counter; i < HOUSE_PIXELS; i += 4)
//		{
//			SetHousePixel(HOUSE_PIXELS - (i + 0), 10, 10, 10, false);
//			SetHousePixel(HOUSE_PIXELS - (i + 1), 20, 20, 20, false);
//			SetHousePixel(HOUSE_PIXELS - (i + 2), 40, 40, 40, false);
//			SetHousePixel(HOUSE_PIXELS - (i + 3), max, max, max, false);
//		}
//
//		ShowHouse();
//
//		counter++;
//		if (counter > 3)
//			counter = 0;
//	}
//
//} // HouseAntiClockWise ****************************************************************************************************************************************************************************************************************************

//int sideRedPos = 0;
//int sideRedDir = 2;
//int sideGreenPos = 0;
//int sideGreenDir = 1;
//
//int topRedPos = 0;
//int topRedDir = 2;
//int topGreenPos = 0;
//int topGreenDir = 1;
//
//int allWhitePos = 0;
//int allWhiteDir = 1;


//void Sparkle()
//{
//	if (currentMillis - previousMillis > 60)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			SetHouse(40, 40, 40);
//		}
//
//		for (int i = 0; i < 50; i++)
//		{
//			StripTwinkle(&house, HOUSE_PIXELS, 40, 40, 40, 80, 80, 80);
//		}
//
//		house.show();
//
//	}
//} // Sparkle ****************************************************************************************************************************************************************************************************************************


//void Twinkle()
//{
//	if (currentMillis - previousMillis > 100)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			SetHouse(0, 0, 0);
//		}
//
//		int l = 0;
//		int h = 20;
//
//		for (int i = 0; i < 20; i++)
//		{
//			StripTwinkle(&house, HOUSE_PIXELS, l, l, l, h, h, h);
//		}
//
//		if (++stage > 4)
//		{
//			stage = 0;
//
//			l = 40;
//			h = 160;
//
//			for (int i = 0; i < 10; i++)
//			{
//				StripTwinkle(&house, HOUSE_PIXELS, l, l, l, h, h, h);
//			}
//		}
//
//		house.show();
//
//	}
//} // Twinkle ****************************************************************************************************************************************************************************************************************************


//void HouseSnow()
//{
//	if (currentMillis - previousMillis > 100)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			SetHouse(40, 40, 40);
//		}
//
//		/*
//		Left		  1 - 150	150
//		bay left	151 - 176	 26
//		bay front	176 - 267	 91
//		bay right	267 - 289	 22
//		bathroom	293 - 362	 69
//		right		362 - 450	 88
//		*/
//
//		// sparkle for the top
//		for (int i = 0; i < 40; i++)
//		{
//			int on = random(151, 362);
//			int off = random(151, 362);
//
//			house.setPixelColor(off, 40, 40, 40);
//			house.setPixelColor(on, 150, 150, 150);
//		}
//
//		int w;
//
//		// falling left
//		for (int i = counter; i < 150; i += 4)
//		{
//			SetHousePixel(i + 0, 0, 0, 0, false);
//			w = random(10, 30);
//			SetHousePixel(i + 1, w, w, w, false);
//
//			w = random(40, 60);
//			SetHousePixel(i + 2, w, w, w, false);
//
//			w = random(70, 90);
//			SetHousePixel(i + 3, w, w, w, false);
//		}
//
//		// falling right
//
//		for (int i = counter; i < 88; i += 4)
//		{
//			SetHousePixel(362 + (i + 0), 0, 0, 0, false);
//
//			w = random(10, 30);
//			SetHousePixel(362 + (i + 1), w, w, w, false);
//
//			w = random(40, 60);
//			SetHousePixel(362 + (i + 2), w, w, w, false);
//
//			w = random(70, 90);
//			SetHousePixel(362 + (i + 3), w, w, w, false);
//		}
//
//		//
//
//		if (++counter > 5)
//			counter = 0;
//
//		ShowHouse();
//	}
//} // HouseSnow ****************************************************************************************************************************************************************************************************************************


//void DomeBar()
//{
//	if (currentMillis - previousMillis > 15)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//		}
//
//		SetHouse(LddOW, LdddOW, LddOW);
//
//		//SetHouseBar(counter, MAX, 0, 0);
//		//SetHouseBar(HOUSE_PIXELS - counter, MAX, 0, 0);
//
//		uint32_t colour = house.Color(max, 0, 0);
//
//		//colour = Wheel(&house, counter);
//		SetHousePixel(counter - 2, colour, false);
//		SetHousePixel(counter - 1, colour, false);
//		SetHousePixel(counter + 0, colour, false);
//		SetHousePixel(counter + 1, colour, false);
//		SetHousePixel(counter + 2, colour, false);
//
//		//colour = Wheel(&house, counter * 2);
//		SetHousePixel(HOUSE_PIXELS - counter - 2, colour, false);
//		SetHousePixel(HOUSE_PIXELS - counter - 1, colour, false);
//		SetHousePixel(HOUSE_PIXELS - counter + 0, colour, false);
//		SetHousePixel(HOUSE_PIXELS - counter + 1, colour, false);
//		SetHousePixel(HOUSE_PIXELS - counter + 2, colour, false);
//
//		ShowHouse();
//
//		counter += sceneDir;
//
//		if (counter <= 0)
//		{
//			sceneDir = +1;
//			if (stage++ > 8)
//				stage = 0;
//		}
//		else if (counter >= HOUSE_PIXELS)
//		{
//			if (stage++ > 8)
//				stage = 0;
//
//			sceneDir = -1;
//		}
//
//	}
//} // DomeBar ****************************************************************************************************************************************************************************************************************************


//void DomeTrail()
//{
//	if (currentMillis - previousMillis > 2)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			SetHouse(LddOW, LddOW, LddOW);
//		}
//
//		uint32_t colour = 0;
//
//		if (stage == 0)
//		{
//			colour = house.Color(0, max, 0);
//		}
//		else if (stage == 1)
//		{
//			colour = house.Color(max, 0, 0);
//		}
//		else if (stage == 2)
//		{
//			colour = house.Color(0, 0, max);
//		}
//		else if (stage == 3)
//		{
//			colour = house.Color(max, max, 0);
//		}
//		else if (stage == 4)
//		{
//			colour = house.Color(0, max, max);
//		}
//		else if (stage == 5)
//		{
//			colour = house.Color(max, 0, max);
//		}
//		else if (stage == 6)
//		{
//			colour = Wheel(&house, counter * 2);
//			//colour = house.Color(MAX, 0, MAX);
//		}
//		else if (stage == 7)
//		{
//			colour = house.Color(max, max, max);
//		}
//
//		SetHousePixel(counter, colour, false);
//		SetHousePixel(HOUSE_PIXELS - counter, colour, false);
//
//		if (++counter > (HOUSE_PIXELS / 2))
//		{
//			counter = 0;
//			stage++;
//		}
//
//		if (stage > 7)
//			stage = 0;
//
//		ShowHouse();
//	}
//} // DomeTrail ****************************************************************************************************************************************************************************************************************************


//void Mash()
//{
//	if (currentMillis - previousMillis > 5)
//	{
//		previousMillis = currentMillis;
//
//		if (NewScene())
//		{
//			SetHouse(LddOW, LddOW, LddOW);
//
//			counter = 0;
//			stage = 0;
//		}
//
//		uint32_t colour = 0;
//
//		/*
//		Divider(238, 201, 0); // yellw gold
//		else if (stage == 5)
//		Divider(0, 205, 205); // cyan
//		else if (stage == 6)
//		Divider(255, 20, 147); // pink
//		else if (stage == 7)
//		Divider(255, 128, 0); // orange
//		*/
//
//		if (stage == 0)
//		{
//			colour = house.Color(max, 0, 0); // red
//		}
//		else if (stage == 1)
//		{
//			colour = house.Color(0, max, 0); // green
//		}
//		else if (stage == 2)
//		{
//			colour = house.Color(0, 0, max); // blue
//		}
//		else if (stage == 3)
//		{
//			colour = house.Color(max, max, 0);
//		}
//		else if (stage == 4)
//		{
//			colour = house.Color(0, max, max);
//		}
//		else if (stage == 5)
//		{
//			colour = house.Color(max, 0, max);
//		}
//		else if (stage == 6)
//		{
//			colour = house.Color(255, 128, 0); // orange
//		}
//		else if (stage == 7)
//		{
//			colour = house.Color(200, 20, 127); // pink 255, 20, 147
//		}
//		else if (stage == 8)
//		{
//			colour = house.Color(0, 205, 205); // cyan
//		}
//		else if (stage == 9)
//		{
//			colour = house.Color(238, 201, 0); // yellw gold
//		}
//		else if (stage == 10)
//		{
//			colour = Wheel(&house, counter); // rainbow
//		}
//		else if (stage == 11)
//		{
//			colour = house.Color(max, max, max); // white
//		}
//
//		for (int i = 0; i < 5; i++)
//		{
//			SetHousePixel(counter, colour, false);
//			counter += sceneDir;
//		}
//
//		ShowHouse();
//
//		if (counter <= 0)
//		{
//			sceneDir = +1;
//			stage++;
//		}
//		else if (counter >= HOUSE_PIXELS)
//		{
//			sceneDir = -1;
//			stage++;
//		}
//
//		if (stage > 11)
//			stage = 0;
//
//	}
//
//} // Mash ****************************************************************************************************************************************************************************************************************************


void SetPorchRow(int row, int red, int green, int blue)
{
	uint32_t colour = porch.Color(red, green, blue);

	SetPorchRow(row, colour);
} // SetPorchRow ****************************************************************************************************************************************************************************************************************************


void SetPorchRow(int row, uint32_t colour)
{
	if (row >= 0 & row <= 9)
	{
		for (int x = 0; x < 50; x++)
		{
			int p = (row * 50) + x;
			porch.setPixelColor(p, colour);
		}
	}
} // SetPorchRow ****************************************************************************************************************************************************************************************************************************


void SetPorchColumn(int column, int red, int green, int blue)
{
	uint32_t colour = porch.Color(red, green, blue);
	SetPorchColumn(column, colour);
} // SetPorchColumn ****************************************************************************************************************************************************************************************************************************


void SetPorchColumn(int column, uint32_t colour)
{
	if (column >= 0 & column <= 49)
	{
		for (int y = 0; y < 9; y++)
		{
			//int p = (y * 50) + column;
			//SetPorchPixel(p, colour);
			SetPorchPixel(column, y, colour);
		}
	}
} // SetPorchColumn ****************************************************************************************************************************************************************************************************************************


void SetPorchPixel(int x, int y, int red, int green, int blue)
{
	uint32_t colour = porch.Color(red, green, blue);
	SetPorchPixel(x, y, colour);
} // SetPorchPixel ****************************************************************************************************************************************************************************************************************************


void SetPorchPixel(int x, int y, uint32_t colour)
{
	if (x >= 0 && x <= 49 && y >= 0 && y <= 9)
	{
		int p = y * 50;

		if (y == 1 || y == 3 || y == 5 || y == 7)
			p += x;
		else
			p += (49 - x);

		porch.setPixelColor(p, colour);
	}
} // SetPorchPixel ****************************************************************************************************************************************************************************************************************************


void SetPorchPixel(int pixelIndex, int red, int green, int blue)
{
	uint32_t colour = porch.Color(red, green, blue);
	SetPorchPixel(pixelIndex, colour);
} // SetPorchPixel ****************************************************************************************************************************************************************************************************************************


void SetPorchPixel(int pixelIndex, uint32_t colour)
{
	// Virtual
	// 8				400	401	402	403	404	405	406	407	408	409	410	411	412	413	414	415	416	417	418	419	420	421	422	423	424	425	426	427	428	429	430	431	432	433	434	435	436	437	438	439	440	441	442	443	444	445	446	447	448	449
	// 7				350	351	352	353	354	355	356	357	358	359	360	361	362	363	364	365	366	367	368	369	370	371	372	373	374	375	376	377	378	379	380	381	382	383	384	385	386	387	388	389	390	391	392	393	394	395	396	397	398	399
	// 6				300	301	302	303	304	305	306	307	308	309	310	311	312	313	314	315	316	317	318	319	320	321	322	323	324	325	326	327	328	329	330	331	332	333	334	335	336	337	338	339	340	341	342	343	344	345	346	347	348	349
	// 5				250	251	252	253	254	255	256	257	258	259	260	261	262	263	264	265	266	267	268	269	270	271	272	273	274	275	276	277	278	279	280	281	282	283	284	285	286	287	288	289	290	291	292	293	294	295	296	297	298	299
	// 4				200	201	202	203	204	205	206	207	208	209	210	211	212	213	214	215	216	217	218	219	220	221	222	223	224	225	226	227	228	229	230	231	232	233	234	235	236	237	238	239	240	241	242	243	244	245	246	247	248	249
	// 3				150	151	152	153	154	155	156	157	158	159	160	161	162	163	164	165	166	167	168	169	170	171	172	173	174	175	176	177	178	179	180	181	182	183	184	185	186	187	188	189	190	191	192	193	194	195	196	197	198	199
	// 2				100	101	102	103	104	105	106	107	108	109	110	111	112	113	114	115	116	117	118	119	120	121	122	123	124	125	126	127	128	129	130	131	132	133	134	135	136	137	138	139	140	141	142	143	144	145	146	147	148	149
	// 1				050	051	052	053	054	055	056	057	058	059	060	061	062	063	064	065	066	067	068	069	070	071	072	073	074	075	076	077	078	079	080	081	082	083	084	085	086	087	088	089	090	091	092	093	094	095	096	097	098	099
	// 0				000	001	002	003	004	005	006	007	008	009	010	011	012	013	014	015	016	017	018	019	020	021	022	023	024	025	026	027	028	029	030	031	032	033	034	035	036	037	038	039	040	041	042	043	044	045	046	047	048	049

	// Physical
	// 8  449 400  <	449	448	447	446	445	444	443	442	441	440	439	438	437	436	435	434	433	432	431	430	429	428	427	426	425	424	423	422	421	420	419	418	417	416	415	414	413	412	411	410	409	408	407	406	405	404	403	402	401	400
	// 7  350 399  >	350	351	352	353	354	355	356	357	358	359	360	361	362	363	364	365	366	367	368	369	370	371	372	373	374	375	376	377	378	379	380	381	382	383	384	385	386	387	388	389	390	391	392	393	394	395	396	397	398	399
	// 6  349 300  <	349	348	347	346	345	344	343	342	341	340	339	338	337	336	335	334	333	332	331	330	329	328	327	326	325	324	323	322	321	320	319	318	317	316	315	314	313	312	311	310	309	308	307	306	305	304	303	302	301	300
	// 5  250 299  >	250	251	252	253	254	255	256	257	258	259	260	261	262	263	264	265	266	267	268	269	270	271	272	273	274	275	276	277	278	279	280	281	282	283	284	285	286	287	288	289	290	291	292	293	294	295	296	297	298	299
	// 4  249 200  <	249	248	247	246	245	244	243	242	241	240	239	238	237	236	235	234	233	232	231	230	229	228	227	226	225	224	223	222	221	220	219	218	217	216	215	214	213	212	211	210	209	208	207	206	205	204	203	202	201	200
	// 3  150 199  >	150	151	152	153	154	155	156	157	158	159	160	161	162	163	164	165	166	167	168	169	170	171	172	173	174	175	176	177	178	179	180	181	182	183	184	185	186	187	188	189	190	191	192	193	194	195	196	197	198	199
	// 2  149 100  <	149	148	147	146	145	144	143	142	141	140	139	138	137	136	135	134	133	132	131	130	129	128	127	126	125	124	123	122	121	120	119	118	117	116	115	114	113	112	111	110	109	108	107	106	105	104	103	102	101	100
	// 1  050 099  >	050	051	052	053	054	055	056	057	058	059	060	061	062	063	064	065	066	067	068	069	070	071	072	073	074	075	076	077	078	079	080	081	082	083	084	085	086	087	088	089	090	091	092	093	094	095	096	097	098	099
	// 0  049 000  <	049	048	047	046	045	044	043	042	041	040	039	038	037	036	035	034	033	032	031	030	029	028	027	026	025	024	023	022	021	020	019	018	017	016	015	014	013	012	011	010	009	008	007	006	005	004	003	002	001	000

	//

	if (pixelIndex >= 0 && pixelIndex <= 49)
	{
		pixelIndex = 49 - pixelIndex;
		porch.setPixelColor(pixelIndex, colour);
	}
	else if (pixelIndex >= 50 && pixelIndex <= 99)
	{
		porch.setPixelColor(pixelIndex, colour);
	}
	else if (pixelIndex >= 100 && pixelIndex <= 149)
	{
		pixelIndex = 149 - pixelIndex;
		porch.setPixelColor(pixelIndex, colour);
	}
	else if (pixelIndex >= 150 && pixelIndex <= 199)
	{
		porch.setPixelColor(pixelIndex, colour);
	}
	else if (pixelIndex >= 200 && pixelIndex <= 249)
	{
		pixelIndex = 249 - pixelIndex;
		porch.setPixelColor(pixelIndex, colour);
	}
	else if (pixelIndex >= 250 && pixelIndex <= 299)
	{
		porch.setPixelColor(pixelIndex, colour);
	}
	else if (pixelIndex >= 300 && pixelIndex <= 349)
	{
		pixelIndex = 349 - pixelIndex;
		porch.setPixelColor(pixelIndex, colour);
	}
	else if (pixelIndex >= 350 && pixelIndex <= 399)
	{
		porch.setPixelColor(pixelIndex, colour);
	}
	else if (pixelIndex >= 400 && pixelIndex <= 449)
	{
		pixelIndex = 449 - pixelIndex;
		porch.setPixelColor(pixelIndex, colour);
	}

} // SetPorchPixel ****************************************************************************************************************************************************************************************************************************

/*
void PorchCalibrate()
{
if (currentMillis - previousPorch > 100)
{
previousPorch = currentMillis;

if (porchCounter++ == 0)
{
SetColour(&porch, 0, 0, 0);

uint32_t w = porch.Color(200, 200, 200);
uint32_t r = porch.Color(200, 0, 0);
uint32_t g = porch.Color(0, 200, 0);

SetPorchColumn(24, r);
SetPorchColumn(25, g);

SetPorchColumn(0, w);
SetPorchColumn(9, w);
SetPorchColumn(19, w);
SetPorchColumn(29, w);
SetPorchColumn(39, w);
SetPorchColumn(49, w);

SetPorchRow(8, w);
//SetPorchRow(7, w);
//SetPorchRow(6, w);
//SetPorchRow(5, w);
SetPorchRow(4, w);
//SetPorchRow(3, w);
//SetPorchRow(2, w);
//SetPorchRow(1, w);
SetPorchRow(0, w);

porch.show();
}
}
} // PorchCalibrate ***************************************************************************************************************************************************************************************************************************************
*/

/*
void PorchHearts()
{
if (currentMillis - previousPorch > 70)
{
previousPorch = currentMillis;

SetColour(&porch, 0, 0, 0);

uint32_t pink = porch.Color(100, 20, 67);
// colour = strip->Color(100, 20, 67); // pink 255, 20, 147

PorchHeart(porchCounter - 40, 0, pink);
PorchHeart(porchCounter - 30, 0, pink);
PorchHeart(porchCounter - 20, 0, pink);
PorchHeart(porchCounter - 10, 0, pink);
PorchHeart(porchCounter, 0, pink);
PorchHeart(porchCounter + 10, 0, pink);
PorchHeart(porchCounter + 20, 0, pink);
PorchHeart(porchCounter + 30, 0, pink);
PorchHeart(porchCounter + 40, 0, pink);
PorchHeart(porchCounter + 50, 0, pink);

porch.show();

porchCounter += porchDir;

if (porchCounter > 40)
{
porchDir = -1;
}
else if (porchCounter < 1)
{
porchDir = +1;
}

}

} // PorchHearts ***************************************************************************************************************************************************************************************************************************************


void PorchHeart(int xOffset, int yOffset, uint32_t colour)
{
if (xOffset>50)
return;

SetPorchPixel(xOffset + 4, yOffset + 0, colour);

SetPorchPixel(xOffset + 3, yOffset + 1, colour);
SetPorchPixel(xOffset + 4, yOffset + 1, colour);
SetPorchPixel(xOffset + 5, yOffset + 1, colour);

SetPorchPixel(xOffset + 2, yOffset + 2, colour);
SetPorchPixel(xOffset + 3, yOffset + 2, colour);
SetPorchPixel(xOffset + 4, yOffset + 2, colour);
SetPorchPixel(xOffset + 5, yOffset + 2, colour);
SetPorchPixel(xOffset + 6, yOffset + 2, colour);

SetPorchPixel(xOffset + 2, yOffset + 3, colour);
SetPorchPixel(xOffset + 3, yOffset + 3, colour);
SetPorchPixel(xOffset + 4, yOffset + 3, colour);
SetPorchPixel(xOffset + 5, yOffset + 3, colour);
SetPorchPixel(xOffset + 6, yOffset + 3, colour);

SetPorchPixel(xOffset + 1, yOffset + 4, colour);
SetPorchPixel(xOffset + 2, yOffset + 4, colour);
SetPorchPixel(xOffset + 3, yOffset + 4, colour);
SetPorchPixel(xOffset + 4, yOffset + 4, colour);
SetPorchPixel(xOffset + 5, yOffset + 4, colour);
SetPorchPixel(xOffset + 6, yOffset + 4, colour);
SetPorchPixel(xOffset + 7, yOffset + 4, colour);

SetPorchPixel(xOffset + 1, yOffset + 5, colour);
SetPorchPixel(xOffset + 2, yOffset + 5, colour);
SetPorchPixel(xOffset + 3, yOffset + 5, colour);
SetPorchPixel(xOffset + 4, yOffset + 5, colour);
SetPorchPixel(xOffset + 5, yOffset + 5, colour);
SetPorchPixel(xOffset + 6, yOffset + 5, colour);
SetPorchPixel(xOffset + 7, yOffset + 5, colour);

SetPorchPixel(xOffset + 1, yOffset + 6, colour);
SetPorchPixel(xOffset + 2, yOffset + 6, colour);
SetPorchPixel(xOffset + 3, yOffset + 6, colour);
SetPorchPixel(xOffset + 5, yOffset + 6, colour);
SetPorchPixel(xOffset + 6, yOffset + 6, colour);
SetPorchPixel(xOffset + 7, yOffset + 6, colour);

SetPorchPixel(xOffset + 2, yOffset + 7, colour);
SetPorchPixel(xOffset + 3, yOffset + 7, colour);
SetPorchPixel(xOffset + 5, yOffset + 7, colour);
SetPorchPixel(xOffset + 6, yOffset + 7, colour);

} // PorchHeart ***************************************************************************************************************************************************************************************************************************************
*/

void PorchHappyBirthday1() // Happy Birthday Abby
{
	if (currentMillis - previousPorch > 60)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 0);

		uint32_t red = porch.Color(220, 0, 0);
		uint32_t green = porch.Color(0, 210, 0);
		uint32_t blue = porch.Color(0, 0, 210);

		if (porchStage == 0)
		{
			PorchHappy(porchCounter, 0, red);
		}
		else if (porchStage == 1)
		{
			PorchBirthday(porchCounter, 0, green);
		}
		else if (porchStage == 2)
		{
			PorchSheila(porchCounter, 0, blue);
		}

		porchCounter += porchDir;

		if (porchCounter > 35)
		{
			porchDir = -1;
		}
		else if (porchCounter < 0)
		{
			porchDir = +1;

			if (porchStage == 0)
				porchStage = 1;
			else if (porchStage == 1)
				porchStage = 2;
			else if (porchStage == 2)
				porchStage = 0;
		}


		porch.show();
	}

} // PorchHappyBirthday1 ***************************************************************************************************************************************************************************************************************************************


void PorchHappyBirthday2() // Abby 18 Today
{
	if (currentMillis - previousPorch > 60)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 0);

		uint32_t red = porch.Color(200, 0, 0);
		uint32_t green = porch.Color(0, 180, 0);
		uint32_t blue = porch.Color(0, 0, 210);

		if (porchStage == 0)
		{
			PorchAbby(porchCounter, 0, red);
		}
		else if (porchStage == 1)
		{
			Porch18(porchCounter, 0, green);
		}
		else if (porchStage == 2)
		{
			PorchToday(porchCounter, 0, blue);
		}

		porchCounter += porchDir;

		if (porchCounter > 35)
		{
			porchDir = -1;
		}
		else if (porchCounter < 0)
		{
			porchDir = +1;

			if (porchStage == 0)
				porchStage = 1;
			else if (porchStage == 1)
				porchStage = 2;
			else if (porchStage == 2)
				porchStage = 0;
		}


		porch.show();
	}

} // PorchHappyBirthday2 ***************************************************************************************************************************************************************************************************************************************


void PorchHappyBirthday3()
{
	if (currentMillis - previousPorch > 60)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 0);

		uint32_t red = porch.Color(max, 0, 0);
		uint32_t green = porch.Color(0, max, 0);
		uint32_t blue = porch.Color(0, 0, max);

		PorchHappy(porchCounter + 2, 0, red);
		PorchBirthday(porchCounter + 30, 0, green);
		PorchSheila(porchCounter + 65, 0, blue);

		porchCounter--;

		if (porchCounter < -65)
		{
			porchCounter = 50;
		}

		porch.show();
	}

} // PorchHappyBirthday3 ***************************************************************************************************************************************************************************************************************************************


void PorchHappyBirthday4() // Happy Birthday Abby, twinkle
{
	if (currentMillis - previousPorch > 600)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 0);

		uint32_t w = porch.Color(90, 90, 90);

		if (porchStage == 0)
		{
			PorchHappy(14, 0, w);
			PorchRandomTwinkle(25);
		}
		else if (porchStage == 1)
		{
			PorchBirthday(10, 0, w);
			PorchRandomTwinkle(45);
		}
		else if (porchStage == 2)
		{
			PorchSheila(11, 0, w);
			PorchRandomTwinkle(15);
		}

		porchCounter++;

		if (porchCounter > 6)
		{
			porchCounter = 0;

			if (porchStage == 0)
				porchStage = 1;
			else if (porchStage == 1)
				porchStage = 2;
			else if (porchStage == 2)
				porchStage = 0;
		}

		porch.show();
	}

} // PorchHappyBirthday4 ***************************************************************************************************************************************************************************************************************************************


void PorchHappyBirthday5() // Happy Birthday Abby, random colours
{
	if (currentMillis - previousPorch > 800)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 0);

		uint32_t w = porch.Color(90, 90, 90);

		if (porchStage == 0)
		{
			PorchHappy(14, 0, w);
			PorchRandomColour(25);
		}
		else if (porchStage == 1)
		{
			PorchBirthday(10, 0, w);
			PorchRandomColour(45);
		}
		else if (porchStage == 2)
		{
			PorchSheila(11, 0, w);
			PorchRandomColour(15);
		}

		porchCounter++;

		if (porchCounter > 5)
		{
			porchCounter = 0;

			if (porchStage == 0)
				porchStage = 1;
			else if (porchStage == 1)
				porchStage = 2;
			else if (porchStage == 2)
				porchStage = 0;
		}

		porch.show();
	}

} // PorchHappyBirthday5 ***************************************************************************************************************************************************************************************************************************************


void PorchRandomTwinkle(int count)
{
	int a;

	for (int i = 0; i < count; i++)
	{
		for (int pixel = 0; pixel < PORCH_PIXELS; pixel++)
		{
			uint32_t colour = porch.getPixelColor(pixel);

			if (colour > 0)
			{
				a = random(0, 5);

				if (a == 1)
					porch.setPixelColor(pixel, 60, 60, 60);
				else if (a == 2)
					porch.setPixelColor(pixel, 90, 90, 90);
				else if (a == 3)
					porch.setPixelColor(pixel, 120, 120, 120);
				else if (a == 4)
					porch.setPixelColor(pixel, 200, 200, 200);
			}
		}
	}

} // PorchRandomTwinkle ****************************************************************************************************************************************************************************************************************************


void PorchRandomColour(int count)
{
	int a;
	uint32_t colour;

	for (int i = 0; i < count; i++)
	{
		for (int pixel = 0; pixel < PORCH_PIXELS; pixel++)
		{
			colour = porch.getPixelColor(pixel);

			if (colour > 0)
			{
				a = random(0, 12);

				if (a >= 0 && a <= 9)
				{
					colour = StageColour(&porch, a);
					porch.setPixelColor(pixel, colour);
				}
				else if (a == 10)
				{
					a = random(10, 250);
					colour = Wheel(&porch, a);
					porch.setPixelColor(pixel, colour);
				}
			}
		}
	}

} // PorchRandomColour ****************************************************************************************************************************************************************************************************************************


void PorchHappy(int xOffset, int yOffset, uint32_t colour)
{

	SetPorchPixel(xOffset + 11, yOffset + 0, colour);
	SetPorchPixel(xOffset + 16, yOffset + 0, colour);
	SetPorchPixel(xOffset + 21, yOffset + 0, colour);

	SetPorchPixel(xOffset + 1, yOffset + 1, colour);
	SetPorchPixel(xOffset + 4, yOffset + 1, colour);
	SetPorchPixel(xOffset + 9, yOffset + 1, colour);
	SetPorchPixel(xOffset + 11, yOffset + 1, colour);
	SetPorchPixel(xOffset + 16, yOffset + 1, colour);
	SetPorchPixel(xOffset + 22, yOffset + 1, colour);

	SetPorchPixel(xOffset + 1, yOffset + 2, colour);
	SetPorchPixel(xOffset + 4, yOffset + 2, colour);
	SetPorchPixel(xOffset + 7, yOffset + 2, colour);
	SetPorchPixel(xOffset + 8, yOffset + 2, colour);
	SetPorchPixel(xOffset + 9, yOffset + 2, colour);
	SetPorchPixel(xOffset + 11, yOffset + 2, colour);
	SetPorchPixel(xOffset + 12, yOffset + 2, colour);
	SetPorchPixel(xOffset + 13, yOffset + 2, colour);
	SetPorchPixel(xOffset + 16, yOffset + 2, colour);
	SetPorchPixel(xOffset + 17, yOffset + 2, colour);
	SetPorchPixel(xOffset + 18, yOffset + 2, colour);
	SetPorchPixel(xOffset + 23, yOffset + 2, colour);

	SetPorchPixel(xOffset + 1, yOffset + 3, colour);
	SetPorchPixel(xOffset + 4, yOffset + 3, colour);
	SetPorchPixel(xOffset + 6, yOffset + 3, colour);
	SetPorchPixel(xOffset + 9, yOffset + 3, colour);
	SetPorchPixel(xOffset + 11, yOffset + 3, colour);
	SetPorchPixel(xOffset + 14, yOffset + 3, colour);
	SetPorchPixel(xOffset + 16, yOffset + 3, colour);
	SetPorchPixel(xOffset + 19, yOffset + 3, colour);
	SetPorchPixel(xOffset + 23, yOffset + 3, colour);

	SetPorchPixel(xOffset + 1, yOffset + 4, colour);
	SetPorchPixel(xOffset + 2, yOffset + 4, colour);
	SetPorchPixel(xOffset + 3, yOffset + 4, colour);
	SetPorchPixel(xOffset + 4, yOffset + 4, colour);
	SetPorchPixel(xOffset + 6, yOffset + 4, colour);
	SetPorchPixel(xOffset + 9, yOffset + 4, colour);
	SetPorchPixel(xOffset + 11, yOffset + 4, colour);
	SetPorchPixel(xOffset + 14, yOffset + 4, colour);
	SetPorchPixel(xOffset + 16, yOffset + 4, colour);
	SetPorchPixel(xOffset + 19, yOffset + 4, colour);
	SetPorchPixel(xOffset + 22, yOffset + 4, colour);
	SetPorchPixel(xOffset + 24, yOffset + 4, colour);

	SetPorchPixel(xOffset + 1, yOffset + 5, colour);
	SetPorchPixel(xOffset + 4, yOffset + 5, colour);
	SetPorchPixel(xOffset + 7, yOffset + 5, colour);
	SetPorchPixel(xOffset + 8, yOffset + 5, colour);
	SetPorchPixel(xOffset + 11, yOffset + 5, colour);
	SetPorchPixel(xOffset + 12, yOffset + 5, colour);
	SetPorchPixel(xOffset + 13, yOffset + 5, colour);
	SetPorchPixel(xOffset + 16, yOffset + 5, colour);
	SetPorchPixel(xOffset + 17, yOffset + 5, colour);
	SetPorchPixel(xOffset + 18, yOffset + 5, colour);
	SetPorchPixel(xOffset + 21, yOffset + 5, colour);
	SetPorchPixel(xOffset + 25, yOffset + 5, colour);

	SetPorchPixel(xOffset + 1, yOffset + 6, colour);
	SetPorchPixel(xOffset + 4, yOffset + 6, colour);
	SetPorchPixel(xOffset + 21, yOffset + 6, colour);
	SetPorchPixel(xOffset + 25, yOffset + 6, colour);

	SetPorchPixel(xOffset + 1, yOffset + 7, colour);
	SetPorchPixel(xOffset + 4, yOffset + 7, colour);

} // PorchHappy ***************************************************************************************************************************************************************************************************************************************


void PorchBirthday(int xOffset, int yOffset, uint32_t colour)
{
	SetPorchPixel(xOffset + 28, yOffset + 0, colour);

	SetPorchPixel(xOffset + 1, yOffset + 1, colour);
	SetPorchPixel(xOffset + 2, yOffset + 1, colour);
	SetPorchPixel(xOffset + 5, yOffset + 1, colour);
	SetPorchPixel(xOffset + 7, yOffset + 1, colour);
	SetPorchPixel(xOffset + 13, yOffset + 1, colour);
	SetPorchPixel(xOffset + 15, yOffset + 1, colour);
	SetPorchPixel(xOffset + 17, yOffset + 1, colour);
	SetPorchPixel(xOffset + 20, yOffset + 1, colour);
	SetPorchPixel(xOffset + 21, yOffset + 1, colour);
	SetPorchPixel(xOffset + 26, yOffset + 1, colour);
	SetPorchPixel(xOffset + 29, yOffset + 1, colour);

	SetPorchPixel(xOffset + 1, yOffset + 2, colour);
	SetPorchPixel(xOffset + 3, yOffset + 2, colour);
	SetPorchPixel(xOffset + 5, yOffset + 2, colour);
	SetPorchPixel(xOffset + 7, yOffset + 2, colour);
	SetPorchPixel(xOffset + 12, yOffset + 2, colour);
	SetPorchPixel(xOffset + 15, yOffset + 2, colour);
	SetPorchPixel(xOffset + 17, yOffset + 2, colour);
	SetPorchPixel(xOffset + 19, yOffset + 2, colour);
	SetPorchPixel(xOffset + 21, yOffset + 2, colour);
	SetPorchPixel(xOffset + 24, yOffset + 2, colour);
	SetPorchPixel(xOffset + 25, yOffset + 2, colour);
	SetPorchPixel(xOffset + 26, yOffset + 2, colour);
	SetPorchPixel(xOffset + 30, yOffset + 2, colour);

	SetPorchPixel(xOffset + 1, yOffset + 3, colour);
	SetPorchPixel(xOffset + 3, yOffset + 3, colour);
	SetPorchPixel(xOffset + 5, yOffset + 3, colour);
	SetPorchPixel(xOffset + 7, yOffset + 3, colour);
	SetPorchPixel(xOffset + 12, yOffset + 3, colour);
	SetPorchPixel(xOffset + 15, yOffset + 3, colour);
	SetPorchPixel(xOffset + 17, yOffset + 3, colour);
	SetPorchPixel(xOffset + 19, yOffset + 3, colour);
	SetPorchPixel(xOffset + 21, yOffset + 3, colour);
	SetPorchPixel(xOffset + 23, yOffset + 3, colour);
	SetPorchPixel(xOffset + 26, yOffset + 3, colour);
	SetPorchPixel(xOffset + 30, yOffset + 3, colour);

	SetPorchPixel(xOffset + 1, yOffset + 4, colour);
	SetPorchPixel(xOffset + 2, yOffset + 4, colour);
	SetPorchPixel(xOffset + 5, yOffset + 4, colour);
	SetPorchPixel(xOffset + 7, yOffset + 4, colour);
	SetPorchPixel(xOffset + 12, yOffset + 4, colour);
	SetPorchPixel(xOffset + 15, yOffset + 4, colour);
	SetPorchPixel(xOffset + 16, yOffset + 4, colour);
	SetPorchPixel(xOffset + 20, yOffset + 4, colour);
	SetPorchPixel(xOffset + 21, yOffset + 4, colour);
	SetPorchPixel(xOffset + 23, yOffset + 4, colour);
	SetPorchPixel(xOffset + 26, yOffset + 4, colour);
	SetPorchPixel(xOffset + 29, yOffset + 4, colour);
	SetPorchPixel(xOffset + 31, yOffset + 4, colour);

	SetPorchPixel(xOffset + 1, yOffset + 5, colour);
	SetPorchPixel(xOffset + 3, yOffset + 5, colour);
	SetPorchPixel(xOffset + 8, yOffset + 5, colour);
	SetPorchPixel(xOffset + 11, yOffset + 5, colour);
	SetPorchPixel(xOffset + 12, yOffset + 5, colour);
	SetPorchPixel(xOffset + 13, yOffset + 5, colour);
	SetPorchPixel(xOffset + 15, yOffset + 5, colour);
	SetPorchPixel(xOffset + 21, yOffset + 5, colour);
	SetPorchPixel(xOffset + 24, yOffset + 5, colour);
	SetPorchPixel(xOffset + 25, yOffset + 5, colour);
	SetPorchPixel(xOffset + 28, yOffset + 5, colour);
	SetPorchPixel(xOffset + 32, yOffset + 5, colour);

	SetPorchPixel(xOffset + 1, yOffset + 6, colour);
	SetPorchPixel(xOffset + 3, yOffset + 6, colour);
	SetPorchPixel(xOffset + 5, yOffset + 6, colour);
	SetPorchPixel(xOffset + 9, yOffset + 6, colour);
	SetPorchPixel(xOffset + 12, yOffset + 6, colour);
	SetPorchPixel(xOffset + 15, yOffset + 6, colour);
	SetPorchPixel(xOffset + 21, yOffset + 6, colour);
	SetPorchPixel(xOffset + 28, yOffset + 6, colour);
	SetPorchPixel(xOffset + 32, yOffset + 6, colour);

	SetPorchPixel(xOffset + 1, yOffset + 7, colour);
	SetPorchPixel(xOffset + 2, yOffset + 7, colour);
	SetPorchPixel(xOffset + 15, yOffset + 7, colour);
	SetPorchPixel(xOffset + 21, yOffset + 7, colour);

} // PorchBirthday ***************************************************************************************************************************************************************************************************************************************


void Porch18(int xOffset, int yOffset, uint32_t colour)
{
	SetPorchPixel(xOffset + 2, yOffset + 1, colour);
	SetPorchPixel(xOffset + 5, yOffset + 1, colour);
	SetPorchPixel(xOffset + 6, yOffset + 1, colour);

	SetPorchPixel(xOffset + 2, yOffset + 2, colour);
	SetPorchPixel(xOffset + 4, yOffset + 2, colour);
	SetPorchPixel(xOffset + 7, yOffset + 2, colour);

	SetPorchPixel(xOffset + 2, yOffset + 3, colour);
	SetPorchPixel(xOffset + 4, yOffset + 3, colour);
	SetPorchPixel(xOffset + 7, yOffset + 3, colour);

	SetPorchPixel(xOffset + 2, yOffset + 4, colour);
	SetPorchPixel(xOffset + 5, yOffset + 4, colour);
	SetPorchPixel(xOffset + 6, yOffset + 4, colour);

	SetPorchPixel(xOffset + 2, yOffset + 5, colour);
	SetPorchPixel(xOffset + 4, yOffset + 5, colour);
	SetPorchPixel(xOffset + 7, yOffset + 5, colour);

	SetPorchPixel(xOffset + 2, yOffset + 6, colour);
	SetPorchPixel(xOffset + 4, yOffset + 6, colour);
	SetPorchPixel(xOffset + 7, yOffset + 6, colour);

	SetPorchPixel(xOffset + 2, yOffset + 7, colour);
	SetPorchPixel(xOffset + 5, yOffset + 7, colour);
	SetPorchPixel(xOffset + 6, yOffset + 7, colour);

} // Porch18 ***************************************************************************************************************************************************************************************************************************************


void Porch18th(int xOffset, int yOffset, uint32_t colour)
{
	SetPorchPixel(xOffset + 2, yOffset + 1, colour);
	SetPorchPixel(xOffset + 5, yOffset + 1, colour);
	SetPorchPixel(xOffset + 6, yOffset + 1, colour);
	SetPorchPixel(xOffset + 11, yOffset + 1, colour);
	SetPorchPixel(xOffset + 13, yOffset + 1, colour);
	SetPorchPixel(xOffset + 15, yOffset + 1, colour);

	SetPorchPixel(xOffset + 2, yOffset + 2, colour);
	SetPorchPixel(xOffset + 4, yOffset + 2, colour);
	SetPorchPixel(xOffset + 7, yOffset + 2, colour);
	SetPorchPixel(xOffset + 10, yOffset + 2, colour);
	SetPorchPixel(xOffset + 13, yOffset + 2, colour);
	SetPorchPixel(xOffset + 15, yOffset + 2, colour);

	SetPorchPixel(xOffset + 2, yOffset + 3, colour);
	SetPorchPixel(xOffset + 4, yOffset + 3, colour);
	SetPorchPixel(xOffset + 7, yOffset + 3, colour);
	SetPorchPixel(xOffset + 10, yOffset + 3, colour);
	SetPorchPixel(xOffset + 13, yOffset + 3, colour);
	SetPorchPixel(xOffset + 15, yOffset + 3, colour);

	SetPorchPixel(xOffset + 2, yOffset + 4, colour);
	SetPorchPixel(xOffset + 5, yOffset + 4, colour);
	SetPorchPixel(xOffset + 6, yOffset + 4, colour);
	SetPorchPixel(xOffset + 10, yOffset + 4, colour);
	SetPorchPixel(xOffset + 13, yOffset + 4, colour);
	SetPorchPixel(xOffset + 14, yOffset + 4, colour);

	SetPorchPixel(xOffset + 2, yOffset + 5, colour);
	SetPorchPixel(xOffset + 4, yOffset + 5, colour);
	SetPorchPixel(xOffset + 7, yOffset + 5, colour);
	SetPorchPixel(xOffset + 9, yOffset + 5, colour);
	SetPorchPixel(xOffset + 10, yOffset + 5, colour);
	SetPorchPixel(xOffset + 11, yOffset + 5, colour);
	SetPorchPixel(xOffset + 13, yOffset + 5, colour);

	SetPorchPixel(xOffset + 2, yOffset + 6, colour);
	SetPorchPixel(xOffset + 4, yOffset + 6, colour);
	SetPorchPixel(xOffset + 7, yOffset + 6, colour);
	SetPorchPixel(xOffset + 10, yOffset + 6, colour);
	SetPorchPixel(xOffset + 13, yOffset + 6, colour);

	SetPorchPixel(xOffset + 2, yOffset + 7, colour);
	SetPorchPixel(xOffset + 5, yOffset + 7, colour);
	SetPorchPixel(xOffset + 6, yOffset + 7, colour);
	SetPorchPixel(xOffset + 13, yOffset + 7, colour);

} // Porch18th ***************************************************************************************************************************************************************************************************************************************


void PorchToday(int xOffset, int yOffset, uint32_t colour)
{
	SetPorchPixel(xOffset + 20, yOffset + 0, colour);

	SetPorchPixel(xOffset + 3, yOffset + 1, colour);
	SetPorchPixel(xOffset + 7, yOffset + 1, colour);
	SetPorchPixel(xOffset + 8, yOffset + 1, colour);
	SetPorchPixel(xOffset + 12, yOffset + 1, colour);
	SetPorchPixel(xOffset + 13, yOffset + 1, colour);
	SetPorchPixel(xOffset + 18, yOffset + 1, colour);
	SetPorchPixel(xOffset + 21, yOffset + 1, colour);

	SetPorchPixel(xOffset + 3, yOffset + 2, colour);
	SetPorchPixel(xOffset + 6, yOffset + 2, colour);
	SetPorchPixel(xOffset + 9, yOffset + 2, colour);
	SetPorchPixel(xOffset + 11, yOffset + 2, colour);
	SetPorchPixel(xOffset + 13, yOffset + 2, colour);
	SetPorchPixel(xOffset + 16, yOffset + 2, colour);
	SetPorchPixel(xOffset + 17, yOffset + 2, colour);
	SetPorchPixel(xOffset + 18, yOffset + 2, colour);
	SetPorchPixel(xOffset + 22, yOffset + 2, colour);

	SetPorchPixel(xOffset + 3, yOffset + 3, colour);
	SetPorchPixel(xOffset + 6, yOffset + 3, colour);
	SetPorchPixel(xOffset + 9, yOffset + 3, colour);
	SetPorchPixel(xOffset + 11, yOffset + 3, colour);
	SetPorchPixel(xOffset + 13, yOffset + 3, colour);
	SetPorchPixel(xOffset + 15, yOffset + 3, colour);
	SetPorchPixel(xOffset + 18, yOffset + 3, colour);
	SetPorchPixel(xOffset + 22, yOffset + 3, colour);

	SetPorchPixel(xOffset + 3, yOffset + 4, colour);
	SetPorchPixel(xOffset + 6, yOffset + 4, colour);
	SetPorchPixel(xOffset + 9, yOffset + 4, colour);
	SetPorchPixel(xOffset + 11, yOffset + 4, colour);
	SetPorchPixel(xOffset + 13, yOffset + 4, colour);
	SetPorchPixel(xOffset + 15, yOffset + 4, colour);
	SetPorchPixel(xOffset + 18, yOffset + 4, colour);
	SetPorchPixel(xOffset + 21, yOffset + 4, colour);
	SetPorchPixel(xOffset + 23, yOffset + 4, colour);

	SetPorchPixel(xOffset + 3, yOffset + 5, colour);
	SetPorchPixel(xOffset + 7, yOffset + 5, colour);
	SetPorchPixel(xOffset + 8, yOffset + 5, colour);
	SetPorchPixel(xOffset + 12, yOffset + 5, colour);
	SetPorchPixel(xOffset + 13, yOffset + 5, colour);
	SetPorchPixel(xOffset + 16, yOffset + 5, colour);
	SetPorchPixel(xOffset + 17, yOffset + 5, colour);
	SetPorchPixel(xOffset + 20, yOffset + 5, colour);
	SetPorchPixel(xOffset + 24, yOffset + 5, colour);

	SetPorchPixel(xOffset + 3, yOffset + 6, colour);
	SetPorchPixel(xOffset + 13, yOffset + 6, colour);
	SetPorchPixel(xOffset + 20, yOffset + 6, colour);
	SetPorchPixel(xOffset + 24, yOffset + 6, colour);

	SetPorchPixel(xOffset + 1, yOffset + 7, colour);
	SetPorchPixel(xOffset + 2, yOffset + 7, colour);
	SetPorchPixel(xOffset + 3, yOffset + 7, colour);
	SetPorchPixel(xOffset + 4, yOffset + 7, colour);
	SetPorchPixel(xOffset + 5, yOffset + 7, colour);
	SetPorchPixel(xOffset + 13, yOffset + 7, colour);

} // PorchToday ***************************************************************************************************************************************************************************************************************************************


void PorchAbby(int xOffset, int yOffset, uint32_t colour)
{
	/*
	SetPorchPixel(xOffset + 14, yOffset + 0, colour);

	SetPorchPixel(xOffset + 1, yOffset + 1, colour);
	SetPorchPixel(xOffset + 4, yOffset + 1, colour);
	SetPorchPixel(xOffset + 6, yOffset + 1, colour);
	SetPorchPixel(xOffset + 7, yOffset + 1, colour);
	SetPorchPixel(xOffset + 10, yOffset + 1, colour);
	SetPorchPixel(xOffset + 11, yOffset + 1, colour);
	SetPorchPixel(xOffset + 15, yOffset + 1, colour);

	SetPorchPixel(xOffset + 1, yOffset + 2, colour);
	SetPorchPixel(xOffset + 4, yOffset + 2, colour);
	SetPorchPixel(xOffset + 6, yOffset + 2, colour);
	SetPorchPixel(xOffset + 8, yOffset + 2, colour);
	SetPorchPixel(xOffset + 10, yOffset + 2, colour);
	SetPorchPixel(xOffset + 12, yOffset + 2, colour);
	SetPorchPixel(xOffset + 16, yOffset + 2, colour);

	SetPorchPixel(xOffset + 1, yOffset + 3, colour);
	SetPorchPixel(xOffset + 4, yOffset + 3, colour);
	SetPorchPixel(xOffset + 6, yOffset + 3, colour);
	SetPorchPixel(xOffset + 8, yOffset + 3, colour);
	SetPorchPixel(xOffset + 10, yOffset + 3, colour);
	SetPorchPixel(xOffset + 12, yOffset + 3, colour);
	SetPorchPixel(xOffset + 16, yOffset + 3, colour);

	SetPorchPixel(xOffset + 1, yOffset + 4, colour);
	SetPorchPixel(xOffset + 2, yOffset + 4, colour);
	SetPorchPixel(xOffset + 3, yOffset + 4, colour);
	SetPorchPixel(xOffset + 4, yOffset + 4, colour);
	SetPorchPixel(xOffset + 6, yOffset + 4, colour);
	SetPorchPixel(xOffset + 7, yOffset + 4, colour);
	SetPorchPixel(xOffset + 10, yOffset + 4, colour);
	SetPorchPixel(xOffset + 11, yOffset + 4, colour);
	SetPorchPixel(xOffset + 15, yOffset + 4, colour);
	SetPorchPixel(xOffset + 17, yOffset + 4, colour);

	SetPorchPixel(xOffset + 1, yOffset + 5, colour);
	SetPorchPixel(xOffset + 4, yOffset + 5, colour);
	SetPorchPixel(xOffset + 6, yOffset + 5, colour);
	SetPorchPixel(xOffset + 10, yOffset + 5, colour);
	SetPorchPixel(xOffset + 14, yOffset + 5, colour);
	SetPorchPixel(xOffset + 18, yOffset + 5, colour);

	SetPorchPixel(xOffset + 1, yOffset + 6, colour);
	SetPorchPixel(xOffset + 4, yOffset + 6, colour);
	SetPorchPixel(xOffset + 6, yOffset + 6, colour);
	SetPorchPixel(xOffset + 10, yOffset + 6, colour);
	SetPorchPixel(xOffset + 14, yOffset + 6, colour);
	SetPorchPixel(xOffset + 18, yOffset + 6, colour);

	SetPorchPixel(xOffset + 2, yOffset + 7, colour);
	SetPorchPixel(xOffset + 3, yOffset + 7, colour);
	*/

} // PorchAbby ***************************************************************************************************************************************************************************************************************************************


void PorchSheila(int xOffset, int yOffset, uint32_t colour)
{

	SetPorchPixel(xOffset + 14, yOffset + 0, colour);

	SetPorchPixel(xOffset + 3, yOffset + 1, colour);
	SetPorchPixel(xOffset + 4, yOffset + 1, colour);
	SetPorchPixel(xOffset + 7, yOffset + 1, colour);
	SetPorchPixel(xOffset + 10, yOffset + 1, colour);
	SetPorchPixel(xOffset + 13, yOffset + 1, colour);
	SetPorchPixel(xOffset + 14, yOffset + 1, colour);
	SetPorchPixel(xOffset + 15, yOffset + 1, colour);
	SetPorchPixel(xOffset + 18, yOffset + 1, colour);
	SetPorchPixel(xOffset + 19, yOffset + 1, colour);
	SetPorchPixel(xOffset + 20, yOffset + 1, colour);
	SetPorchPixel(xOffset + 22, yOffset + 1, colour);
	SetPorchPixel(xOffset + 23, yOffset + 1, colour);
	SetPorchPixel(xOffset + 24, yOffset + 1, colour);
	SetPorchPixel(xOffset + 27, yOffset + 1, colour);
	SetPorchPixel(xOffset + 29, yOffset + 1, colour);

	SetPorchPixel(xOffset + 2, yOffset + 2, colour);
	SetPorchPixel(xOffset + 5, yOffset + 2, colour);
	SetPorchPixel(xOffset + 7, yOffset + 2, colour);
	SetPorchPixel(xOffset + 10, yOffset + 2, colour);
	SetPorchPixel(xOffset + 12, yOffset + 2, colour);
	SetPorchPixel(xOffset + 19, yOffset + 2, colour);
	SetPorchPixel(xOffset + 23, yOffset + 2, colour);
	SetPorchPixel(xOffset + 26, yOffset + 2, colour);
	SetPorchPixel(xOffset + 28, yOffset + 2, colour);
	SetPorchPixel(xOffset + 29, yOffset + 2, colour);

	SetPorchPixel(xOffset + 5, yOffset + 3, colour);
	SetPorchPixel(xOffset + 7, yOffset + 3, colour);
	SetPorchPixel(xOffset + 10, yOffset + 3, colour);
	SetPorchPixel(xOffset + 12, yOffset + 3, colour);
	SetPorchPixel(xOffset + 13, yOffset + 3, colour);
	SetPorchPixel(xOffset + 14, yOffset + 3, colour);
	SetPorchPixel(xOffset + 15, yOffset + 3, colour);
	SetPorchPixel(xOffset + 16, yOffset + 3, colour);
	SetPorchPixel(xOffset + 19, yOffset + 3, colour);
	SetPorchPixel(xOffset + 23, yOffset + 3, colour);
	SetPorchPixel(xOffset + 26, yOffset + 3, colour);
	SetPorchPixel(xOffset + 29, yOffset + 3, colour);

	SetPorchPixel(xOffset + 4, yOffset + 4, colour);
	SetPorchPixel(xOffset + 7, yOffset + 4, colour);
	SetPorchPixel(xOffset + 8, yOffset + 4, colour);
	SetPorchPixel(xOffset + 10, yOffset + 4, colour);
	SetPorchPixel(xOffset + 12, yOffset + 4, colour);
	SetPorchPixel(xOffset + 16, yOffset + 4, colour);
	SetPorchPixel(xOffset + 19, yOffset + 4, colour);
	SetPorchPixel(xOffset + 23, yOffset + 4, colour);
	SetPorchPixel(xOffset + 26, yOffset + 4, colour);
	SetPorchPixel(xOffset + 29, yOffset + 4, colour);

	SetPorchPixel(xOffset + 3, yOffset + 5, colour);
	SetPorchPixel(xOffset + 7, yOffset + 5, colour);
	SetPorchPixel(xOffset + 9, yOffset + 5, colour);
	SetPorchPixel(xOffset + 13, yOffset + 5, colour);
	SetPorchPixel(xOffset + 14, yOffset + 5, colour);
	SetPorchPixel(xOffset + 15, yOffset + 5, colour);
	SetPorchPixel(xOffset + 23, yOffset + 5, colour);
	SetPorchPixel(xOffset + 27, yOffset + 5, colour);
	SetPorchPixel(xOffset + 28, yOffset + 5, colour);

	SetPorchPixel(xOffset + 2, yOffset + 6, colour);
	SetPorchPixel(xOffset + 7, yOffset + 6, colour);
	SetPorchPixel(xOffset + 19, yOffset + 6, colour);
	SetPorchPixel(xOffset + 23, yOffset + 6, colour);

	SetPorchPixel(xOffset + 2, yOffset + 7, colour);
	SetPorchPixel(xOffset + 5, yOffset + 7, colour);
	SetPorchPixel(xOffset + 7, yOffset + 7, colour);
	SetPorchPixel(xOffset + 23, yOffset + 7, colour);

	SetPorchPixel(xOffset + 3, yOffset + 8, colour);
	SetPorchPixel(xOffset + 4, yOffset + 8, colour);

} // PorchSheila ***************************************************************************************************************************************************************************************************************************************


void StripTwinkle(Adafruit_NeoPixel *strip, int max, uint32_t lowred, uint32_t lowgreen, uint32_t lowblue, uint32_t highred, uint32_t highgreen, uint32_t highblue)
{
	int on = random(0, max);
	int off = random(0, max);

	strip->setPixelColor(off, lowred, lowgreen, lowblue);
	strip->setPixelColor(on, highred, highgreen, highblue);
} // StripTwinkle ***************************************************************************************************************************************************************************************************************************************


uint32_t StageColour(Adafruit_NeoPixel *strip, int stageNumber)
{
	uint32_t colour = 0;

	if (stageNumber == 0)
	{
		colour = strip->Color(max, 0, 0); // red
	}
	else if (stageNumber == 1)
	{
		colour = strip->Color(0, max, 0); // green
	}
	else if (stageNumber == 2)
	{
		colour = strip->Color(0, 0, max); // blue
	}
	else if (stageNumber == 3)
	{
		colour = strip->Color(max, max, 0); // yellw
	}
	else if (stageNumber == 4)
	{
		colour = strip->Color(0, max, max); // cyan
	}
	else if (stageNumber == 5)
	{
		colour = strip->Color(max, 0, max); // magenta
	}
	else if (stageNumber == 6)
	{
		colour = strip->Color(128, 64, 0); // orange
	}
	else if (stageNumber == 7)
	{
		colour = strip->Color(100, 20, 67); // pink 255, 20, 147
	}
	else if (stageNumber == 8)
	{
		colour = strip->Color(0, 50, 100);
	}
	else if (stageNumber == 9)
	{
		colour = strip->Color(139, 117, 0); // gold 4
	}
	else if (stageNumber == 10)
	{
		colour = Wheel(strip, counter); // rainbow
	}
	else // if (stageNumber == 11)
	{
		colour = strip->Color(max, max, max); // white
	}

} // StageColour ***************************************************************************************************************************************************************************************************************************************


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

