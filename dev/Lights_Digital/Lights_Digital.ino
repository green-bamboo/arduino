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
#define PIN_NEOFLAKE	25
#define PIN_NEOHOUSE	27
#define PIN_NEOPORCH	29
#define PIN_LED			13
#define PIN_NET1		22
#define PIN_NET2		24
#define PIN_NET3		26
#define PIN_NET4		28
#define PIN_NET5		30
#define PIN_NET6		32
#define PIN_HOLLY		34
#define PIN_LITTLESTAR	36
#define PIN_BELLS		38
#define PIN_WHITEGROUND	40
#define PIN_BLUEGROUND	42

#define MIN				0
#define LOW				20
#define MAX				60
#define STARBURST_MAX	30

#ifdef FULLHOUSE
#define TREE_PIXELS		300
#define FLAKE_PIXELS	1
#define LEFT_PIXELS		150
#define TOP_PIXELS		200
#define RIGHT_PIXELS	50
#define PORCH_PIXELS	450
#define HOUSE_PIXELS	450
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


Adafruit_NeoPixel tree = Adafruit_NeoPixel(TREE_PIXELS, PIN_NEOTREE, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel porch = Adafruit_NeoPixel(PORCH_PIXELS, PIN_NEOPORCH, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel flake = Adafruit_NeoPixel(FLAKE_PIXELS, PIN_NEOFLAKE, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel house = Adafruit_NeoPixel(HOUSE_PIXELS, PIN_NEOHOUSE, NEO_GRB + NEO_KHZ800);

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousRadio = 0;
unsigned long previousTree = 0;
unsigned long previousPorch = 0;
unsigned long previousFlake = 0;
unsigned long previousLED = 0;
unsigned long sceneMillis = 0;
unsigned long SceneLength = 12 * 1000; // 20 seconds for scene

int gap = 120;

int ledCounter = 0;
int currentAct = 101;
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
int porchStage = 0;
int porchX = 0;
int porchY = 0;
int porchXdir = 0;
int porchYdir = 0;

int flakeCounter = 0;


byte patternMerry[9][50] = // Merry
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};


byte patternChristmas[9][50] = // Christmas
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};



//byte patternShimmer[9][50] =
//{
//	{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 },
//	{ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 },
//	{ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52 },
//	{ 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53 },
//	{ 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54 },
//	{ 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55 },
//	{ 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56 },
//	{ 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57 },
//	{ 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58 },
//};

//byte patternDropping[9][50] =
//{
//	{ 7, 7, 7, 7, 7, 19, 19, 19, 19, 19, 1, 1, 1, 1, 1, 13, 13, 13, 13, 13, 28, 28, 28, 28, 28, 22, 22, 22, 22, 22, 10, 10, 10, 10, 10, 25, 25, 25, 25, 25, 4, 4, 4, 4, 4, 16, 16, 16, 16, 16 },
//	{ 8, 8, 8, 8, 8, 20, 20, 20, 20, 20, 2, 2, 2, 2, 2, 14, 14, 14, 14, 14, 29, 29, 29, 29, 29, 23, 23, 23, 23, 23, 11, 11, 11, 11, 11, 26, 26, 26, 26, 26, 5, 5, 5, 5, 5, 17, 17, 17, 17, 17 },
//	{ 9, 9, 9, 9, 9, 21, 21, 21, 21, 21, 3, 3, 3, 3, 3, 15, 15, 15, 15, 15, 30, 30, 30, 30, 30, 24, 24, 24, 24, 24, 12, 12, 12, 12, 12, 27, 27, 27, 27, 27, 6, 6, 6, 6, 6, 18, 18, 18, 18, 18 },
//	{ 10, 10, 10, 10, 10, 22, 22, 22, 22, 22, 4, 4, 4, 4, 4, 16, 16, 16, 16, 16, 31, 31, 31, 31, 31, 25, 25, 25, 25, 25, 13, 13, 13, 13, 13, 28, 28, 28, 28, 28, 7, 7, 7, 7, 7, 19, 19, 19, 19, 19 },
//	{ 11, 11, 11, 11, 11, 23, 23, 23, 23, 23, 5, 5, 5, 5, 5, 17, 17, 17, 17, 17, 32, 32, 32, 32, 32, 26, 26, 26, 26, 26, 14, 14, 14, 14, 14, 29, 29, 29, 29, 29, 8, 8, 8, 8, 8, 20, 20, 20, 20, 20 },
//	{ 12, 12, 12, 12, 12, 24, 24, 24, 24, 24, 6, 6, 6, 6, 6, 18, 18, 18, 18, 18, 33, 33, 33, 33, 33, 27, 27, 27, 27, 27, 15, 15, 15, 15, 15, 30, 30, 30, 30, 30, 9, 9, 9, 9, 9, 21, 21, 21, 21, 21 },
//	{ 13, 13, 13, 13, 13, 25, 25, 25, 25, 25, 7, 0, 7, 7, 7, 19, 19, 19, 19, 19, 34, 34, 34, 34, 34, 28, 28, 28, 28, 28, 16, 16, 16, 16, 16, 31, 31, 31, 31, 31, 10, 10, 10, 10, 10, 22, 22, 22, 22, 22 },
//	{ 14, 14, 14, 14, 14, 26, 26, 26, 26, 26, 8, 8, 8, 8, 8, 20, 20, 20, 20, 20, 35, 35, 35, 35, 35, 29, 29, 29, 29, 29, 17, 17, 17, 17, 17, 32, 32, 32, 32, 32, 11, 11, 11, 11, 11, 23, 23, 23, 23, 23 },
//	{ 15, 15, 15, 15, 15, 27, 27, 27, 27, 27, 9, 9, 9, 9, 9, 21, 21, 21, 21, 21, 36, 36, 36, 36, 36, 30, 30, 30, 30, 30, 18, 18, 18, 18, 18, 33, 33, 33, 33, 33, 12, 12, 12, 12, 12, 24, 24, 24, 24, 24 }
//
//};

//uint16_t patternSleigh[9][50] =
//{
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
//	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 }
//};


//byte patternBoxes[9][50] =
//{
//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
//	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
//	{ 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1 },
//	{ 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1 },
//	{ 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 4, 3, 2, 1 },
//	{ 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1 },
//	{ 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1 },
//	{ 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 },
//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
//};


//uint16_t patternSpiral[9][50] =
//{
//	{ 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58 },
//	{ 8, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 59 },
//	{ 7, 120, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 169, 60 },
//	{ 6, 119, 224, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 271, 170, 61 },
//	{ 5, 118, 223, 320, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 365, 272, 171, 62 },
//	{ 4, 117, 222, 319, 408, 407, 406, 405, 404, 403, 402, 401, 400, 399, 398, 397, 396, 395, 394, 393, 392, 391, 390, 389, 388, 387, 386, 385, 384, 383, 382, 381, 380, 379, 378, 377, 376, 375, 374, 373, 372, 371, 370, 369, 368, 367, 366, 273, 172, 63 },
//	{ 3, 116, 221, 318, 317, 316, 315, 314, 313, 312, 311, 310, 309, 308, 307, 306, 305, 304, 303, 302, 301, 300, 299, 298, 297, 296, 295, 294, 293, 292, 291, 290, 289, 288, 287, 286, 285, 284, 283, 282, 281, 280, 279, 278, 277, 276, 275, 274, 173, 64 },
//	{ 2, 115, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 65 },
//	{ 1, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66 }
//};


//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },




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


/*

Left		  1 - 150	150
bay left	151 - 176	 26
bay front	176 - 267	 91
bay right	267 - 289	 22
bathroom	293 - 362	 69
right		362 - 450	 88

*/

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

	pinMode(PIN_HOLLY, OUTPUT);
	pinMode(PIN_LITTLESTAR, OUTPUT);
	pinMode(PIN_BELLS, OUTPUT);
	pinMode(PIN_WHITEGROUND, OUTPUT);
	pinMode(PIN_BLUEGROUND, OUTPUT);

	mySwitch.enableReceive(0);  // Receiver on inerrupt 0 => that is pin #2

	tree.begin();
	flake.begin();
	porch.begin();
	house.begin();

	black = tree.Color(0, 0, 0);
	white = tree.Color(MAX, MAX, MAX);
	//red = tree.Color(0, 0, 0);
	//green = tree.Color(0, 0, 0);
	//blue = tree.Color(0, 0, 0);

	randomSeed(analogRead(2) + analogRead(4) + millis());

	Serial.println("Ready, v1.3");

	SetStrips(0, 0, 0);
	ShowStrips();

	previousLED = millis();

	NextScene();
	StartScene();
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
			ledCounter = 0;
			Serial.print("Tick ");
			Serial.print(currentScene);
			Serial.print(", (");
			Serial.print(previousScene);
			Serial.println(")");

		}

		if (ledCounter == 1 || ledCounter == 3)
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
		else
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


void PinOn(int pin)
{
	digitalWrite(pin, LOW);
} // ***************************************************************************************************************************************************************************************************************************************


void PinOff(int pin)
{
	digitalWrite(pin, HIGH);
} // ***************************************************************************************************************************************************************************************************************************************


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
	SetColour(&flake, red, green, blue);
	SetColour(&porch, red, green, blue);
	SetColour(&house, red, green, blue);
} // SetStrips ****************************************************************************************************************************************************************************************************************************


void SetStripsBrightness(int level)
{
	tree.setBrightness(level);
	flake.setBrightness(level);
	house.setBrightness(level);
	porch.setBrightness(level);
} // SetStripsBrightness ****************************************************************************************************************************************************************************************************************************


void SetHouse(int red, int green, int blue)
{
	SetColour(&house, red, green, blue);
} // SetStrips ****************************************************************************************************************************************************************************************************************************


void ShowStrips()
{
	tree.show();
	//flake.show();
	porch.show();
	house.show();
} // ShowStrips ****************************************************************************************************************************************************************************************************************************


void ShowHouse()
{
	house.show();
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
			Serial.println("NextScene Auto");
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
			Sparkle();
			TreeSparkle();
			PorchTree();

			//Glimmer(); // All over
			//Sparkle();
			//TreeSparkle();
			//PorchSparkle();
			//B2();
			//PorchRaindeers();
		}
		else if (currentScene == 2)
		{
			Sparkle();
			TreeSparkle();
			PorchTree();
			//B2();
			//FlakeSparkle();
		}
		else if (currentScene == 3)
		{
			Twinkle();
			TreeSparkle();
			//B2();
			PorchMerryChristmas();
			//FadeOutAll();
		}
		else if (currentScene == 4)
		{
			Twinkle();
			TreeSparkle();
			PorchSleigh();
			//PorchMerryChristmas();
			//B2();
			//Star3();
		}
		else if (currentScene == 5)
		{
			Twinkle();
			TreeRocket();
			PorchRaindeers();
			//FlakeTwinkle();
		}
		else if (currentScene == 6)
		{
			FillUpWhite();
			TreeBeat();
			//TreeBerries();
			//PorchTree();
		}
		else if (currentScene == 7)
		{
			Twinkle(); // House
			//TreeVert();
			TreeBounce();
			PorchMerryChristmas();
		}
		else if (currentScene == 8)
		{
			Sparkle();
			TreeDomeTrail();
			PorchSleigh();
		}
		else if (currentScene == 9)
		{
			Dull(); // All over
		}
		else if (currentScene == 10)
		{
			Sparkle();
			TreeSpiral();
			PorchTree();
		}
		else if (currentScene == 11)
		{
			Twinkle();
			// StarBurst();
			TreeSparkle();
			PorchMerryChristmas();
		}
		else if (currentScene == 12)
		{
			HouseClock(); // HouseAntiClockWise
			TreeBerries();
			PorchRaindeers();
		}
		else if (currentScene == 13)
		{
			HouseClock(); // HouseClockWise
			TreeBerries();
			PorchTree();
		}
		else if (currentScene == 14)
		{
			Twinkle();
			TreeBeat();
			//FadeOutAll();
			PorchMerryChristmas();
		}
		else if (currentScene == 15)
		{
			Sparkle();
			//Star1();
			TreeTrain();
			PorchTree();
		}
		else if (currentScene == 16)
		{
			Sparkle();
			TreeBerries();
			//TreeVert();
			//PorchFader();
			PorchMerryChristmas();
		}
		else if (currentScene == 17)
		{
			Twinkle();
			TreeWaveBlue();
			//Star2();
			PorchSleigh();
		}
		else if (currentScene == 18)
		{
			HouseSnow();
			//PorchSnow();
			TreeBerries();
			PorchTree();
		}
		else if (currentScene == 19)
		{
			Divider();
			TreeBerries();
			//TreeHorz();
			PorchDivider();
		}
		else if (currentScene == 20)
		{
			DomeBar();
			TreeRainbowAll();
			PorchTree();
		}
		else if (currentScene == 21)
		{
			DomeTrail();
			//FillUpRed();
			TreeSpiral();
			PorchMerryChristmas();
		}
		else if (currentScene == 22)
		{
			Disco(); // All over
		}
		else if (currentScene == 23)
		{
			DomeTrail();
			TreeSparkle();
			PorchMerryChristmas();
		}
		else if (currentScene == 24)
		{
			RainbowHousePixel(); // House
			TreeSpiral();
			PorchTree();
		}
		else if (currentScene == 25)
		{
			Mash();
			TreeMash();
			//TreeFader();
			PorchSleigh();
		}
		else if (currentScene == 26)
		{
			RainbowHouseSections();
			TreeMash();
			PorchSparkle();
		}
		else if (currentScene == 27)
		{
			RainbowHouseAll(); // House
			//Sparkle();
			TreeSparkle();
			//TreeRainbowAll();
			PorchMerryChristmas();
		}
		else if (currentScene == 28)
		{
			RainbowHouseTwinkle();
			TreeSparkle();
			PorchTree();
		}
		else if (currentScene == 29)
		{
			HouseColourFader();
			TreeBerries();
			PorchMerryChristmas();
		}
		//else if (currentScene == 30)
		//{
		//RainbowAll(); // House, Tree and Porch
		//}
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



void B2()
{
	//PorchTree();
	//PorchSparkle();
	//PorchSpiral();
	//PorchDivider();
	//	PorchWipe();
	//PorchSleigh();
	//PorchHorz();
	PorchMerryChristmas();
}



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

	SetStripsBrightness(255);

	//SetColour(&ground, 0, 0, 0);
	//ground.show();
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
	treeStage = 0;

	porchCounter = 0;
	porchDir = 1;
	porchStage = 0;
	porchX = 0;
	porchY = 0;

	sceneMillis = currentMillis;
	previousTree = currentMillis;

	//SetColour(&ground, 0, 0, 0);
	//ground.show();
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
	if (currentMillis - previousMillis > gap)
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
	if (currentMillis - previousMillis > gap)
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
	if (currentMillis - previousMillis > gap)
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
	if (currentMillis - previousMillis > gap)
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
	if (currentMillis - previousMillis > gap)
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
	if (currentMillis - previousMillis > gap)
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
	if (currentMillis - previousMillis > gap)
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
	if (currentMillis - previousMillis > gap)
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
	if (currentMillis - previousMillis > 40)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			counter = 255;
		}

		counter -= 5;

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

	if (pixelIndex >= 0 & pixelIndex < 300) // 1 tree
	{
		int pixel = 300 - pixelIndex;
		tree.setPixelColor(pixel, red, green, blue);

		if (show)
			tree.show();
	}
	else if (pixelIndex >= 300 && pixelIndex < 750) // 2 house
	{
		int pixel = pixelIndex - 300;
		house.setPixelColor(pixel, red, green, blue);

		if (show)
			house.show();
	}
	//else if (pixelIndex >= 750 && pixelIndex <= 1200) // 3 porch
	//{
	//	int pixel = pixelIndex - 750;
	//	porch.setPixelColor(pixel, red, green, blue);

	//	if (show)
	//		porch.show();
	//}



	/* Original, with ground string

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
	else if (pixelIndex >= 150 && pixelIndex < 600) // 3 house
	{
	int pixel = pixelIndex - 150;
	house.setPixelColor(pixel, red, green, blue);

	if (show)
	house.show();
	}
	else if (pixelIndex >= 600 && pixelIndex <= 800) // 6 porch
	{
	int pixel = pixelIndex - 600;
	porch.setPixelColor(pixel, red, green, blue);

	if (show)
	porch.show();
	}
	*/

} // SetPixel ****************************************************************************************************************************************************************************************************************************


void zzSetPorchRow(int row, int red, int green, int blue)
{
	uint32_t colour = porch.Color(red, green, blue);

	zzSetPorchRow(row, colour);
} // SetPorchRow ****************************************************************************************************************************************************************************************************************************


void zzSetPorchRow(int row, uint32_t colour)
{
	for (int x = 0; x < 50; x++)
	{
		int p = (row * 50) + x;
		porch.setPixelColor(p, colour);
	}
} // SetPorchRow ****************************************************************************************************************************************************************************************************************************


void zzSetPorchColumn(int column, int red, int green, int blue)
{
	uint32_t colour = porch.Color(red, green, blue);
	SetPorchColumn(column, colour);
} // SetPorchColumn ****************************************************************************************************************************************************************************************************************************


void zzSetPorchColumn(int column, uint32_t colour)
{
	for (int y = 0; y < 9; y++)
	{
		int p = (y * 50) + column;
		zzSetPorchPixel(p, colour);
	}
} // SetPorchColumn ****************************************************************************************************************************************************************************************************************************


void zzSetPorchPixel(int pixelIndex, int red, int green, int blue)
{
	uint32_t colour = porch.Color(red, green, blue);
	zzSetPorchPixel(pixelIndex, colour);
}


void zzSetPorchPixel(int pixelIndex, uint32_t colour)
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


void SetHousePixel(int pixelIndex, int red, int green, int blue)
{
	SetHousePixel(pixelIndex, red, green, blue, true);
} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


void SetHousePixel(int pixelIndex, int red, int green, int blue, bool show)
{
	uint32_t colour = house.Color(red, green, blue);
	SetHousePixel(pixelIndex, colour, show);
} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


void SetHousePixel(int pixelIndex, uint32_t colour, bool show)
{
	house.setPixelColor(pixelIndex, colour);

	if (show)
		house.show();

} // SetHousePixel ****************************************************************************************************************************************************************************************************************************


void SetLeftPixel(int pixelIndex, uint32_t colour)
{
	if (pixelIndex >= 0 && pixelIndex <= 150)
		house.setPixelColor(pixelIndex, colour);
} // SetLeftPixel ****************************************************************************************************************************************************************************************************************************


void SetBayPixel(int pixelIndex, uint32_t colour)
{
	if (pixelIndex >= 176 && pixelIndex <= 267)
		house.setPixelColor(pixelIndex, colour);
} // SetBayPixel ****************************************************************************************************************************************************************************************************************************


void SetBathroomPixel(int pixelIndex, uint32_t colour)
{
	if (pixelIndex >= 293 && pixelIndex <= 361)
		house.setPixelColor(pixelIndex, colour);
} // SetBathroomPixel ****************************************************************************************************************************************************************************************************************************


void SetRightPixel(int pixelIndex, uint32_t colour)
{
	if (pixelIndex >= 362 && pixelIndex <= 450)
		house.setPixelColor(pixelIndex, colour);
} // SetRightPixel ****************************************************************************************************************************************************************************************************************************


void FillUpWhite()
{
	if (currentMillis - previousMillis > 60)
	{
		previousMillis = currentMillis;

		NewScene();

		if (counter == 0)
		{
			Nets(0, 0, 0, 0, 0, 0);
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);

			SetHouse(0, 0, 0);
			SetColour(&porch, 0, 0, 0);
			ShowHouse();
			porch.show();
		}

		if (counter <= 150)
		{
			int h = 60;

			for (int i = 1; i < counter; i++)
			{
				house.setPixelColor(i, 60, 60, 60);

				if (i >= 62 && i <= 154)
					house.setPixelColor(512 - i, 60, 60, 60);
			}

			int silver = 0;
			if (counter == 0)
				SetColour(&house, 0, 0, 0);
			else if (counter == 40)
				Nets(1, 0, 0, 0, 0, 0);
			else if (counter == 60)
				Nets(1, 1, 0, 0, 0, 0);
			else if (counter == 61)
				SetPorchRow(0, MAX, MAX, MAX);
			else if (counter == 63)
				SetPorchRow(1, MAX, MAX, MAX);
			else if (counter == 65)
				SetPorchRow(2, MAX, MAX, MAX);
			else if (counter == 69)
				SetPorchRow(3, MAX, MAX, MAX);
			else if (counter == 72)
				SetPorchRow(4, MAX, MAX, MAX);
			else if (counter == 75)
				SetPorchRow(5, MAX, MAX, MAX);
			else if (counter == 79)
				SetPorchRow(6, MAX, MAX, MAX);
			else if (counter == 80)
				Nets(1, 1, 1, 0, 0, 0);
			else if (counter == 82)
				SetPorchRow(7, MAX, MAX, MAX);
			else if (counter == 85)
				SetPorchRow(8, MAX, MAX, MAX);
			else if (counter == 100)
				Nets(1, 1, 1, 1, 0, 0);
			else if (counter == 120)
				Nets(1, 1, 1, 1, 1, 0);
			else if (counter == 140)
				Nets(1, 1, 1, 1, 1, 1);
			//else if (counter == 136 || counter == 142)
			//	SetColour(&house, h, h, h);
			//else if (counter == 138 || counter == 144)
			//	SetColour(&house, h, h, h);
			//else if (counter == 140 || counter == 146)
			//	SetColour(&house, h, h, h);
			else if (counter == 148 || counter == 150)
				SetColour(&house, 60, 60, 60);
			//else if (counter > 135)
			//	SetColour(&house, 0, 0, 0);

			house.show();
			porch.show();
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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOff(PIN_LITTLESTAR);
			PinOff(PIN_BELLS);

			SetColour(&porch, 0, 0, 0);
			SetHouse(0, 10, 0);

			ShowHouse();
			porch.show();
		}

		if (counter <= 150)
		{
			int h = 60;

			for (int i = 1; i < counter; i++)
			{
				house.setPixelColor(i, 80, 0, 0);

				if (i >= 62)
					house.setPixelColor(450 - i, 80, 0, 0);
			}

			int silver = 0;
			if (counter == 0)
				SetColour(&house, 0, 0, 0);
			else if (counter == 40)
				Nets(1, 0, 0, 0, 0, 0);
			else if (counter == 60)
				Nets(1, 1, 0, 0, 0, 0);
			else if (counter == 61)
				SetPorchRow(0, MAX, 0, 0);
			else if (counter == 63)
				SetPorchRow(1, MAX, 0, 0);
			else if (counter == 65)
				SetPorchRow(2, MAX, 0, 0);
			else if (counter == 69)
				SetPorchRow(3, MAX, 0, 0);
			else if (counter == 72)
				SetPorchRow(4, MAX, 0, 0);
			else if (counter == 75)
				SetPorchRow(5, MAX, 0, 0);
			else if (counter == 79)
				SetPorchRow(6, MAX, 0, 0);
			else if (counter == 80)
				Nets(1, 1, 1, 0, 0, 0);
			else if (counter == 82)
				SetPorchRow(7, MAX, 0, 0);
			else if (counter == 85)
				SetPorchRow(8, MAX, 0, 0);
			else if (counter == 100)
				Nets(1, 1, 1, 1, 0, 0);
			else if (counter == 120)
				Nets(1, 1, 1, 1, 1, 0);
			else if (counter == 140)
				Nets(1, 1, 1, 1, 1, 1);
			//else if (counter == 136 || counter == 142)
			//	SetColour(&house, h, h, h);
			//else if (counter == 138 || counter == 144)
			//	SetColour(&house, h, h, h);
			//else if (counter == 140 || counter == 146)
			//	SetColour(&house, h, h, h);
			else if (counter == 148 || counter == 150)
				SetColour(&house, h, 0, 0);
			//else if (counter > 135)
			//SetColour(&house, 0, 0, 0);

			house.show();
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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);
			SetStrips(0, 0, 0); // off
			ShowStrips();
		}

		for (uint16_t i = 0; i < tree.numPixels(); i++)
		{
			uint32_t colour = tree.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			tree.setPixelColor(i, colour);
		}

		//for (uint16_t i = 0; i < flake.numPixels(); i++)
		//{
		//	uint32_t colour = flake.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
		//	flake.setPixelColor(i, colour);
		//}

		for (uint16_t i = 0; i < house.numPixels(); i++)
		{
			uint32_t colour = house.Color(random(MIN, MAX), random(MIN, MAX), random(MIN, MAX));
			house.setPixelColor(i, colour);
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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);

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
		if (counter > 75)
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
	int middle;
	uint32_t colour = house.Color(r, g, b);

	middle = 75;
	SetLeftPixel(middle - counter, colour);
	SetLeftPixel(middle + counter, colour);

	middle = 221;
	SetBayPixel(middle - counter, colour);
	SetBayPixel(middle + counter, colour);

	middle = 327;
	SetBathroomPixel(middle - counter, colour);
	SetBathroomPixel(middle + counter, colour);

	middle = 406;
	SetRightPixel(middle - counter, colour);
	SetRightPixel(middle + counter, colour);

	house.show();
} // Divider


void RainbowHousePixel()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);
			counter = random(10, 250);
		}

		if (counter++ > 256)
			counter = 0;

		for (int i = 0; i < HOUSE_PIXELS; i++)
		{
			SetHousePixel(i, Wheel(&house, (i + counter) & 255), false);
		}

		ShowHouse();
	}
} // RainbowHousePixel ****************************************************************************************************************************************************************************************************************************


int rainbow1 = 0;
int rainbow2 = 0;
int rainbow3 = 0;
int rainbow4 = 0;
int rainbow5 = 0;
int rainbow6 = 0;

void RainbowHouseSections()
{
	if (currentMillis - previousMillis > 20)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);

			counter = 0;
			rainbow1 = random(10, 200);
			rainbow2 = random(20, 150);
			rainbow3 = random(30, 250);
			rainbow4 = random(40, 250);
			rainbow5 = random(50, 200);
			rainbow6 = random(60, 100);
		}

		if (counter++ > 256)
			counter = 0;

		if (rainbow1++ > 256)
			rainbow1 = 0;

		if (rainbow2++ > 256)
			rainbow2 = 0;

		if (rainbow3++ > 256)
			rainbow3 = 0;

		if (rainbow4++ > 256)
			rainbow4 = 0;

		if (rainbow5++ > 256)
			rainbow5 = 0;

		if (rainbow6++ > 256)
			rainbow6 = 0;

		for (int i = 0; i < 150; i++)
			house.setPixelColor(i, Wheel(&house, (rainbow1 + counter) & 255));

		for (int i = 151; i < 175; i++)
			house.setPixelColor(i, Wheel(&house, (rainbow2 + counter) & 255));

		for (int i = 176; i < 267; i++)
			house.setPixelColor(i, Wheel(&house, (rainbow3 + counter) & 255));

		for (int i = 268; i < 292; i++)
			house.setPixelColor(i, Wheel(&house, (rainbow4 + counter) & 255));

		for (int i = 293; i < 361; i++)
			house.setPixelColor(i, Wheel(&house, (rainbow5 + counter) & 255));

		for (int i = 362; i <= 450; i++)
			house.setPixelColor(i, Wheel(&house, (rainbow6 + counter) & 255));

		house.show();
	}
} // RainbowHouseSections ****************************************************************************************************************************************************************************************************************************


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

		uint32_t colour = Wheel(&house, (counter)& 255);

		SetColour(&house, colour);

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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);

			counter = random(10, 250);
		}

		if (counter++ > 256)
			counter = 0;

		for (int i = 0; i < HOUSE_PIXELS; i++)
		{
			SetHousePixel(i, Wheel(&house, (i + counter) & 255), false);
		}

		int l = 70;
		int h = 180;

		for (int i = 0; i < 10; i++)
		{
			StripTwinkle(&house, HOUSE_PIXELS, l, l, l, h, h, h);
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

		SetColour(&house, red, green, blue);
		SetColour(&tree, red, green, blue);
		SetColour(&flake, red, green, blue);
		SetColour(&porch, red, green, blue);

		ShowStrips();

	}
} // RainbowAll ****************************************************************************************************************************************************************************************************************************


void HouseColourFader()
{

	if (currentMillis - previousMillis > 80)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(1, 1, 1, 1, 1, 1);
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);
			SetHouse(0, 0, 0);
			ShowHouse();

			counter = 0;
			sceneDir = 20;
		}
		else
		{
			counter += sceneDir;

			uint32_t colour = StageColour(&house, stage);

			SetColour(&house, colour);
			house.setBrightness(counter);
			house.show();

			if (counter <= 0)
			{
				sceneDir = +20;
				if (stage++ > 11)
					stage = 0;
			}
			else if (counter >= 255)
			{
				sceneDir = -20;
			}

		}

	}

} // HouseColourFader ****************************************************************************************************************************************************************************************************************************


void HouseClock()
{

	if (currentMillis - previousMillis > 80)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);
		}

		for (int i = counter; i < HOUSE_PIXELS; i += 4)
		{
			if (sceneDir == 1)
			{
				SetHousePixel(i + 0, 10, 10, 10, false);
				SetHousePixel(i + 1, 20, 20, 20, false);
				SetHousePixel(i + 2, 40, 40, 40, false);
				SetHousePixel(i + 3, MAX, MAX, MAX, false);
			}
			else
			{
				SetHousePixel(HOUSE_PIXELS - (i + 0), 10, 10, 10, false);
				SetHousePixel(HOUSE_PIXELS - (i + 1), 20, 20, 20, false);
				SetHousePixel(HOUSE_PIXELS - (i + 2), 40, 40, 40, false);
				SetHousePixel(HOUSE_PIXELS - (i + 3), MAX, MAX, MAX, false);
			}
		}

		ShowHouse();

		counter++;
		if (counter > 3)
			counter = 0;

		if (stage++ > 20)
		{
			stage = 0;
			sceneDir = !sceneDir;
		}
	}
} // HouseClock ****************************************************************************************************************************************************************************************************************************


void HouseClockWise()
{

	if (currentMillis - previousMillis > 80)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);
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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);
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

		uint32_t l = house.Color(40, 40, 40);
		uint32_t h = house.Color(200, 200, 200);
		uint32_t red = house.Color(100, 0, 0);
		uint32_t green = house.Color(0, 100, 0);

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

		SetBar(&house, sideRedPos, red);
		SetBar(&house, sideGreenPos, green);

		ShowHouse();

		//SetBar(&house, allWhitePos, white);

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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOff(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);
			SetHouse(40, 40, 40);
		}

		for (int i = 0; i < 50; i++)
		{
			StripTwinkle(&house, HOUSE_PIXELS, 40, 40, 40, 80, 80, 80);

			/*
			int on = random(0, LEFT_PIXELS);
			int off = random(0, LEFT_PIXELS);

			house.setPixelColor(off, 40, 40, 40);
			house.setPixelColor(on, 200, 200, 200);

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

		house.show();

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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);

			SetHouse(0, 0, 0);
		}

		int l = 0;
		int h = 20;

		for (int i = 0; i < 20; i++)
		{
			StripTwinkle(&house, HOUSE_PIXELS, l, l, l, h, h, h);
		}

		if (++stage > 4)
		{
			stage = 0;

			l = 40;
			h = 160;

			for (int i = 0; i < 10; i++)
			{
				StripTwinkle(&house, HOUSE_PIXELS, l, l, l, h, h, h);
			}
		}

		house.show();

	}
} // Twinkle ****************************************************************************************************************************************************************************************************************************


void HouseSnow()
{
	if (currentMillis - previousMillis > 100)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(1, 1, 1, 1, 1, 1);
			SetHouse(40, 40, 40);
		}

		/*
		Left		  1 - 150	150
		bay left	151 - 176	 26
		bay front	176 - 267	 91
		bay right	267 - 289	 22
		bathroom	293 - 362	 69
		right		362 - 450	 88
		*/

		// sparkle for the top
		for (int i = 0; i < 40; i++)
		{
			int on = random(151, 362);
			int off = random(151, 362);

			house.setPixelColor(off, 40, 40, 40);
			house.setPixelColor(on, 150, 150, 150);
		}

		int w;

		// falling left
		for (int i = counter; i < 150; i += 4)
		{
			SetHousePixel(i + 0, 0, 0, 0, false);
			w = random(10, 30);
			SetHousePixel(i + 1, w, w, w, false);

			w = random(40, 60);
			SetHousePixel(i + 2, w, w, w, false);

			w = random(70, 90);
			SetHousePixel(i + 3, w, w, w, false);
		}

		// falling right

		for (int i = counter; i < 88; i += 4)
		{
			SetHousePixel(362 + (i + 0), 0, 0, 0, false);

			w = random(10, 30);
			SetHousePixel(362 + (i + 1), w, w, w, false);

			w = random(40, 60);
			SetHousePixel(362 + (i + 2), w, w, w, false);

			w = random(70, 90);
			SetHousePixel(362 + (i + 3), w, w, w, false);
		}

		//

		if (++counter > 5)
			counter = 0;

		ShowHouse();
	}
} // HouseSnow ****************************************************************************************************************************************************************************************************************************



int stars = 0;
//                 0 ,   1 , 2 , 3 ,     4 , 5 , 6 , 7 , 8 , 9 ,     10 , 11, 12 , 13
int starpos[] = { 20, 50, 80, 110, 160, 190, 250, 290, 370, 420, 0, 0, 0, 0, 0 };
int starmin[] = { 5, 45, 85, 115, 145, 180, 240, 290, 320, 350, 0, 0, 0, 0, 0 };
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

			for (int i = 0; i < 10; i++)
			{
				starseq[i] = 0 - starlen[i];
				starpos[i] = random(starmin[i], starmax[i]);
			}

			//starseq[7] = starseq[4] + 140;
			//starseq[8] = starseq[5] + 140;
			//starseq[9] = starseq[6] + 140;
		}

		for (int i = 0; i < 10; i++)
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

			StarBurst(&house, pos, seq);
		}

		house.show();

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
			Nets(1, 1, 1, 1, 1, 1);
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOff(PIN_BELLS);

			SetStrips(0, 0, 0);
			ShowStrips();
			last = -1;
		}

		SetPixel(last, 0, 0, 0);
		SetPixel(counter, MAX, MAX, MAX);

		last = counter;

		counter++;
		if (counter > (TREE_PIXELS + HOUSE_PIXELS + 20))
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
			Nets(1, 1, 1, 1, 1, 1);
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOff(PIN_BELLS);

			SetStrips(0, 0, 0);
			ShowStrips();
		}

		SetPixel(counter, MAX, MAX, MAX);

		last = counter;

		counter++;
		if (counter > (TREE_PIXELS + HOUSE_PIXELS + 20))
			counter = 0;
	}

} // Star2 ****************************************************************************************************************************************************************************************************************************


void Star3()
{
	//if (currentMillis - previousMillis > 2)
	//{
	previousMillis = currentMillis;

	if (NewScene())
	{
		Nets(0, 0, 0, 0, 0, 0);
		PinOff(PIN_HOLLY);
		PinOff(PIN_WHITEGROUND);
		PinOff(PIN_BLUEGROUND);
		PinOff(PIN_LITTLESTAR);
		PinOff(PIN_BELLS);

		SetStripsBrightness(255);
		SetStrips(0, 0, 0);
		ShowStrips();
	}

	if (counter == 0)
	{
		SetStrips(0, 0, 0);
		ShowStrips();
		last = -1;
		position = 0;
	}

	if (counter >= 0 && counter <= 20)
	{
		int i = counter * 10;
		SetPixel(0, i, i, i, false);
		SetPixel(1, i, i, i, false);
		SetPixel(2, i, i, i, false);
		SetPixel(3, i, i, i, false);
		SetPixel(4, i, i, i, false);
		SetHouse(i, i, i);
	}
	else if (counter >= 21 && counter <= 40)
	{
		int i = 200 - ((21 - counter) * 10);
		SetPixel(0, i, i, i, false);
		SetPixel(1, i, i, i, false);
		SetPixel(2, i, i, i, false);
		SetPixel(3, i, i, i, false);
		SetPixel(4, i, i, i, false);
		SetHouse(i, i, i);
	}
	else if (counter >= 41 && counter <= 60)
	{
		SetPixel(0, 200, 200, 200, false);
		SetPixel(1, 200, 200, 200, false);
		SetPixel(2, 200, 200, 200, false);
		SetPixel(3, 200, 200, 200, false);
		SetPixel(4, 200, 200, 200, false);
	}
	else if (counter >= 61 && counter <= 80)
	{
		SetPixel(0, 0, 0, 0, false);
		SetPixel(1, 0, 0, 0, false);
		SetPixel(2, 0, 0, 0, false);
		SetPixel(3, 0, 0, 0, false);
		SetPixel(4, 0, 0, 0, false);
	}
	else if (counter >= 81 && counter <= 100)
	{
		SetPixel(0, 200, 200, 200, false);
		SetPixel(1, 200, 200, 200, false);
		SetPixel(2, 200, 200, 200, false);
		SetPixel(3, 200, 200, 200, false);
		SetPixel(4, 200, 200, 200, false);
	}
	else if (counter >= 101 && counter <= 120)
	{
		SetPixel(0, 0, 0, 0, false);
		SetPixel(1, 0, 0, 0, false);
		SetPixel(2, 0, 0, 0, false);
		SetPixel(3, 0, 0, 0, false);
		SetPixel(4, 0, 0, 0, false);
	}
	else if (counter >= 121 && counter <= 140)
	{
		SetPixel(0, 200, 200, 200, false);
		SetPixel(1, 200, 200, 200, false);
		SetPixel(2, 200, 200, 200, false);
		SetPixel(3, 200, 200, 200, false);
		SetPixel(4, 200, 200, 200, false);
	}
	else if (counter >= 141 && counter <= 160)
	{
		SetPixel(0, 0, 0, 0, false);
		SetPixel(1, 0, 0, 0, false);
		SetPixel(2, 0, 0, 0, false);
		SetPixel(3, 0, 0, 0, false);
		SetPixel(4, 0, 0, 0, false);
	}
	else if (position > (TREE_PIXELS + HOUSE_PIXELS + 20))
	{
		stage++;
		counter = 1;
		position = 0;
		PinOn(PIN_LITTLESTAR);

		//SetColour(&flake, MAX, MAX, MAX);
		//flake.show();
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
		//PorchStarTurn();
	}

	ShowStrips();

	last = position;

	counter++;

	//}


} // Star3 ****************************************************************************************************************************************************************************************************************************


void DomeBar()
{
	if (currentMillis - previousMillis > 15)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			Nets(0, 0, 0, 0, 0, 0);
			PinOff(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOff(PIN_BELLS);
		}

		SetHouse(LOW, LOW, LOW);

		//SetHouseBar(counter, MAX, 0, 0);
		//SetHouseBar(HOUSE_PIXELS - counter, MAX, 0, 0);

		uint32_t colour = house.Color(MAX, 0, 0);

		//colour = Wheel(&house, counter);
		SetHousePixel(counter - 2, colour, false);
		SetHousePixel(counter - 1, colour, false);
		SetHousePixel(counter + 0, colour, false);
		SetHousePixel(counter + 1, colour, false);
		SetHousePixel(counter + 2, colour, false);

		//colour = Wheel(&house, counter * 2);
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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);

			SetHouse(LOW, LOW, LOW);
		}

		uint32_t colour = 0;

		if (stage == 0)
		{
			colour = house.Color(0, MAX, 0);
			Nets(1, 0, 0, 0, 0, 0);
		}
		else if (stage == 1)
		{
			colour = house.Color(MAX, 0, 0);
			Nets(1, 0, 1, 0, 0, 0);
		}
		else if (stage == 2)
		{
			colour = house.Color(0, 0, MAX);
			Nets(1, 0, 1, 0, 1, 0);
		}
		else if (stage == 3)
		{
			colour = house.Color(MAX, MAX, 0);
			Nets(1, 1, 1, 0, 1, 0);
		}
		else if (stage == 4)
		{
			colour = house.Color(0, MAX, MAX);
			Nets(1, 1, 1, 1, 1, 0);
		}
		else if (stage == 5)
		{
			colour = house.Color(MAX, 0, MAX);
			Nets(1, 1, 1, 1, 1, 1);
		}
		else if (stage == 6)
		{
			colour = Wheel(&house, counter * 2);
			//colour = house.Color(MAX, 0, MAX);
			Nets(0, 0, 1, 1, 0, 0);
		}
		else if (stage == 7)
		{
			colour = house.Color(MAX, MAX, MAX);
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
	if (currentMillis - previousMillis > 5)
	{
		previousMillis = currentMillis;

		if (NewScene())
		{
			SetHouse(LOW, LOW, LOW);
			Nets(0, 0, 0, 0, 0, 0);
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOff(PIN_LITTLESTAR);
			PinOff(PIN_BELLS);

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
			colour = house.Color(MAX, 0, 0); // red
			Nets(1, 0, 0, 0, 0, 0);
		}
		else if (stage == 1)
		{
			colour = house.Color(0, MAX, 0); // green
			Nets(1, 0, 1, 0, 0, 0);
		}
		else if (stage == 2)
		{
			colour = house.Color(0, 0, MAX); // blue
			Nets(1, 0, 1, 0, 1, 0);
		}
		else if (stage == 3)
		{
			colour = house.Color(MAX, MAX, 0);
			Nets(1, 1, 1, 0, 1, 0);
		}
		else if (stage == 4)
		{
			colour = house.Color(0, MAX, MAX);
			Nets(1, 1, 1, 1, 1, 0);
		}
		else if (stage == 5)
		{
			colour = house.Color(MAX, 0, MAX);
			Nets(1, 1, 1, 1, 1, 1);
		}
		else if (stage == 6)
		{
			colour = house.Color(255, 128, 0); // orange
			Nets(1, 0, 1, 1, 1, 1);
		}
		else if (stage == 7)
		{
			colour = house.Color(200, 20, 127); // pink 255, 20, 147
			Nets(1, 0, 1, 0, 1, 1);
		}
		else if (stage == 8)
		{
			colour = house.Color(0, 205, 205); // cyan
			Nets(1, 0, 1, 0, 1, 0);
		}
		else if (stage == 9)
		{
			colour = house.Color(238, 201, 0); // yellow gold
			Nets(0, 0, 1, 0, 1, 0);
		}
		else if (stage == 10)
		{
			colour = Wheel(&house, counter); // rainbow
			Nets(0, 0, 1, 0, 0, 0);
		}
		else if (stage == 11)
		{
			colour = house.Color(MAX, MAX, MAX); // white
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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOff(PIN_BELLS);
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

		//SetBar(&ground, stage + 00, white);
		//SetBar(&ground, stage + 50, white);

		for (int i = 0; i < 12; i++)
		{
			int bar = (i * 40) + stage;
			SetBar(&house, bar, white);
		}

		for (int i = 0; i < 9; i++)
		{
			int line = stage + (i * 50);
			SetBar(&porch, line + 5, white);
			SetBar(&porch, line + 25, white);
		}

		SetBar(&flake, stage + 15, white);
		SetBar(&flake, stage + 35, white);

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
			PinOn(PIN_HOLLY);
			PinOn(PIN_WHITEGROUND);
			PinOn(PIN_BLUEGROUND);
			PinOn(PIN_LITTLESTAR);
			PinOn(PIN_BELLS);

			SetStrips(10, 10, 10);
			ShowStrips();
		}

		int l = 10;
		int h = 40;

		for (int i = 0; i < 50; i++)
		{
			StripTwinkle(&house, HOUSE_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&tree, TREE_PIXELS, l, l, l, h, h, h);
			StripTwinkle(&flake, FLAKE_PIXELS, l, l, l, h, h, h);
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
	if (currentMillis - previousTree > 100)
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

		for (int i = 0; i < 50; i++)
		{
			int on = random(0, TREE_PIXELS);
			int off = random(0, TREE_PIXELS);

			tree.setPixelColor(off, 30, 30, 30);
			tree.setPixelColor(on, 140, 140, 140);
		}

		tree.show();
	}
} // TreeSparkle ***************************************************************************************************************************************************************************************************************************************


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
			StripTwinkle(&tree, TREE_PIXELS, l, l, l, h, h, h);
		}

		if (++stage > 4)
		{
			stage = 0;

			l = 40;
			h = 160;

			for (int i = 0; i < 10; i++)
			{
				StripTwinkle(&tree, TREE_PIXELS, l, l, l, h, h, h);
			}
		}

		tree.show();

	}
} // TreeTwinkle ****************************************************************************************************************************************************************************************************************************


void TreeMash()
{
	if (currentMillis - previousTree > 10)
	{
		previousTree = currentMillis;

		uint32_t colour = StageColour(&tree, treeStage);

		for (int i = 0; i < 3; i++)
		{
			tree.setPixelColor(treeCounter, colour);
			treeCounter += treeDir;
		}

		tree.show();

		if (treeCounter <= 0)
		{
			treeDir = +1;
			treeStage++;
		}
		else if (treeCounter >= TREE_PIXELS)
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
	if (currentMillis - previousTree > 6)
	{
		previousTree = currentMillis;

		//uint32_t colour = 0;

		uint32_t colour = StageColour(&tree, treeStage);

		/*if (treeStage == 0)
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
		*/

		tree.setPixelColor(treeCounter, colour);
		tree.setPixelColor(TREE_PIXELS - treeCounter, colour);

		if (++treeCounter > 150)
		{
			treeCounter = 0;
			treeStage++;
		}

		if (treeStage > 11)
			treeStage = 0;

		tree.show();
	}
} // TreeDomeTrail ****************************************************************************************************************************************************************************************************************************


void TreeTrain()
{
	if (currentMillis - previousTree > 100)
	{
		previousTree = currentMillis;

		//if (treeStage <= 0)
		//{
		//}

		uint32_t w = tree.Color(150, 150, 150);

		SetColour(&tree, 20, 20, 20);
		SetBar(&tree, treeStage, w);

		treeStage++;

		if (treeStage > TREE_PIXELS)
			treeStage = 0;

		tree.show();
	}

} // TreeTrain ***************************************************************************************************************************************************************************************************************************************


void TreeBerries()
{
	if (currentMillis - previousTree > 300)
	{
		previousTree = currentMillis;

		if (treeCounter == 0)
		{
			SetColour(&tree, 40, 0, 0);
		}

		for (int i = 0; i < 60; i++)
		{
			int l = random(0, TREE_PIXELS);
			int h = random(0, TREE_PIXELS);

			tree.setPixelColor(l, 40, 0, 0);
			tree.setPixelColor(h, 110, 0, 0);
		}

		tree.show();
	}
} // TreeBerries ***************************************************************************************************************************************************************************************************************************************


void TreeBeat()
{
	if (currentMillis - previousTree > 40)
	{
		previousTree = currentMillis;

		if (treeCounter == 1 || treeCounter == 5)
		{
			SetColour(&tree, 60, 0, 0);
		}
		else if (treeCounter == 2 || treeCounter == 6)
		{
			SetColour(&tree, 130, 10, 10);
		}
		else if (treeCounter == 3 || treeCounter == 7)
		{
			SetColour(&tree, 60, 0, 0);
		}
		else
		{
			//SetColour(&tree, 10, 0, 0);

			for (int i = counter; i < TREE_PIXELS; i += 4)
			{
				SetHousePixel(i + 0, 10, 10, 10, false);
				SetHousePixel(i + 1, 20, 20, 20, false);
				SetHousePixel(i + 2, 30, 40, 40, false);
				SetHousePixel(i + 3, 40, 40, 40, false);
			}
		}

		tree.show();

		treeCounter++;

		if (treeCounter > 21)
			treeCounter = 0;
	}
} // TreeBeat ***************************************************************************************************************************************************************************************************************************************


void TreeBounce()
{
	if (currentMillis - previousTree > 20)
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
	if (currentMillis - previousTree > 6)
	{
		previousTree = currentMillis;

		if (treeCounter <= 0)
		{
			treeStage = 0;
			treeCounter = 0;
			SetColour(&tree, 10, 10, 10);
			tree.show();
		}

		for (int i = 0; i < TREE_PIXELS; i++)
		{
			if (i < treeCounter)
			{
				tree.setPixelColor(i, 40, 40, 40);
			}
			else if (i == treeCounter)
			{
				int colour = 0;
				if (treeStage == 0)
					colour = 80;
				else if (treeStage == 1)
					colour = 120;
				else if (treeStage == 2)
					colour = 180;

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
	if (currentMillis - previousTree > 120)
	{
		previousTree = currentMillis;

		if (treeCounter++ > 256)
			treeCounter = 0;

		SetColour(&tree, Wheel(&tree, treeCounter));
		//SetColour(&tree, Wheel(&tree, (treeCounter)& 255));

		tree.show();
	}
} // TreeRainbowAll ****************************************************************************************************************************************************************************************************************************


void TreeRainbowPixel()
{
	if (currentMillis - previousTree > 120)
	{
		previousTree = currentMillis;

		for (int i = 0; i < TREE_PIXELS; i++)
		{
			if (treeCounter++ > 256)
				treeCounter = 0;

			tree.setPixelColor(i, Wheel(&house, treeCounter));
			//tree.setPixelColor(i, Wheel(&house, (treeCounter)& 255));
		}

		tree.show();
	}
} // TreeRainbow ****************************************************************************************************************************************************************************************************************************


void TreeWaveBlue()
{
	if (currentMillis - previousTree > 100)
	{
		previousTree = currentMillis;

		TreeWave(0, 0, MAX, 0, 0, 5);
	}

} // TreeWaveBlue ***************************************************************************************************************************************************************************************************************************************


void TreeWave(int sr, int sg, int sb, int ir, int ig, int ib)
{
	uint32_t colour;

	SetColour(&tree, sr, sg, sb);

	colour = tree.Color(sr, sg, sb);
	SetBar(&tree, treeCounter + 10, colour);

	treeStage++;

	if (treeStage > 20)
		treeStage = 0;

	tree.show();

} // TreeWave ***************************************************************************************************************************************************************************************************************************************


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


void PorchSparkle()
{
	if (currentMillis - previousPorch > 100)
	{
		previousPorch = currentMillis;

		if (porchCounter++ == 0)
		{
			SetColour(&porch, 40, 40, 40);
		}

		for (int i = 0; i < 80; i++)
		{
			int on = random(0, PORCH_PIXELS);
			int med = random(0, PORCH_PIXELS);
			int off = random(0, PORCH_PIXELS);

			porch.setPixelColor(off, 40, 40, 40);
			porch.setPixelColor(med, 110, 110, 110);
			//porch.setPixelColor(on, 200, 200, 200);

		}

		porch.show();
	}
} // PorchSparkle ***************************************************************************************************************************************************************************************************************************************


void PorchSnow()
{
	if (currentMillis - previousPorch > 100)
	{
		previousPorch = currentMillis;

		porch.show();
	}
} // PorchSnow ***************************************************************************************************************************************************************************************************************************************


void PorchShimmer()
{
	if (currentMillis - previousPorch > 100)
	{
		previousPorch = currentMillis;

		porch.show();
	}
} // PorchShimmer ***************************************************************************************************************************************************************************************************************************************


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


void PorchSliders()
{
	if (currentMillis - previousPorch > 40)
	{
		previousPorch = currentMillis;

		//if (porchCounter++ == 0)
		//{
		//	SetColour(&porch, 0, 0, 0);
		//}

		SetColour(&porch, 0, 0, 0);

		uint32_t white = porch.Color(MAX, MAX, MAX);

		int p = porchCounter;
		for (int i = 0; i < 10; i++)
		{
			for (int s = 0; s < 5; s++)
			{
				int p = (i * 7) - 10 + porchCounter + s;
				SetPorchPixel(p + 0, 0, white);
				SetPorchPixel(p + 7, 1, white);
				SetPorchPixel(p + 0, 2, white);
				SetPorchPixel(p + 7, 3, white);
				SetPorchPixel(p + 0, 4, white);
				SetPorchPixel(p + 7, 5, white);
				SetPorchPixel(p + 0, 6, white);
				SetPorchPixel(p + 7, 7, white);
				SetPorchPixel(p + 0, 8, white);
			}

		}

		porch.show();

		if (++porchCounter > 7)
			porchCounter = 0;

	}
} // PorchSliders ***************************************************************************************************************************************************************************************************************************************


void PorchBerries()
{
	if (currentMillis - previousPorch > 200)
	{
		previousPorch = currentMillis;

		if (porchCounter == 0)
		{
			SetColour(&porch, 40, 0, 0);
		}

		for (int i = 0; i < 60; i++)
		{
			int l = random(0, PORCH_PIXELS);
			int h = random(0, PORCH_PIXELS);

			porch.setPixelColor(l, 40, 0, 0);
			porch.setPixelColor(h, 110, 0, 0);
		}

		porch.show();
	}
} // PorchBerries ***************************************************************************************************************************************************************************************************************************************


void PorchWhiteOut()
{
	//if (currentMillis - previousPorch > 3)
	//{//
	previousPorch = currentMillis;

	if (porchCounter++ == 0)
	{
		SetColour(&porch, 0, 0, 0);
	}

	for (int i = 0; i < 40; i++)
	{
		int colour = 0;
		int on = 0;
		int max = 0;

		while (colour == 0 && max++ < 50)
		{
			on = random(0, PORCH_PIXELS);
			colour = porch.getPixelColor(on);
		}

		porch.setPixelColor(on, MAX, MAX, MAX);

	}

	porch.show();
	//}
} // PorchWhiteOut ***************************************************************************************************************************************************************************************************************************************


void PorchBlackOut()
{
	//if (currentMillis - previousPorch > 5)
	//	/{
	previousPorch = currentMillis;

	if (porchCounter++ == 0)
	{
		SetColour(&porch, MAX, MAX, MAX);
	}

	for (int i = 0; i < 40; i++)
	{
		int colour = 255;
		int on = 0;
		int max = 0;

		while (colour != 0 && max++ < 50)
		{
			on = random(0, PORCH_PIXELS);
			colour = porch.getPixelColor(on);
		}

		porch.setPixelColor(on, 0, 0, 0);

	}

	porch.show();
	//}
} // PorchBlackOut ***************************************************************************************************************************************************************************************************************************************


// like to hands of fingers pulling apart
void PorchFingers()
{
	// 0    0 - 49
	// 1   50 - 99
	// 2  100 - 149
	// 3  150 - 199
	// 4  200 - 249
	// 5  250 - 299  
	// 6  300 - 349
	// 7  350 - 399  
	// 8  400 - 449


	if (currentMillis - previousPorch > 120)
	{
		previousPorch = currentMillis;

		if (porchStage == 0)
		{
			SetColour(&porch, MAX, MAX, MAX);
			porchStage = 1;
		}
		else
		{
			uint32_t colour = porch.Color(0, 0, 0);

			for (int y = 0; y < 9; y++)
			{
				int p = (y * 50) + porchCounter;
				porch.setPixelColor(p, colour);
			}

		}


		/*
		for (int y = 0; y < 9; y++)
		{
		//if (porchStage++ > 11)
		//porchStage = 1;

		//uint32_t colour = StageColour(&porch, porchStage);

		uint32_t colour = porch.Color(0, 0, 0);

		for (int x = 0; x < 50; x++)
		{
		int p = (x * 50) + y + porchCounter;

		for (int i = 0; i < 9; i++)
		{
		porch.setPixelColor(p, colour);
		}

		}
		}

		porchCounter += porchDir;

		if (porchCounter > 50)
		{
		porchDir = -1;
		}
		else if (porchCounter < 0)
		{
		porchDir = +1;
		}

		*/

		if (++porchCounter > 50)
			porchCounter = 0;


		porch.show();

	}
} // PorchFingers ***************************************************************************************************************************************************************************************************************************************


void PorchWipe()
{
	if (currentMillis - previousPorch > 50)
	{
		previousPorch = currentMillis;

		if (porchX++ == 0)
		{
			porchY = 1;// random(1, 4);
		}

		uint32_t colour = StageColour(&porch, porchStage);

		if (porchY == 1) // Upwards
		{
			SetPorchRow(porchCounter, colour);

			if (++porchCounter > 9)
			{
				porchStage++;
				porchCounter = 0;
				porchY = 2;
			}
		}
		else if (porchY == 2) // Left to Right
		{
			SetPorchColumn(porchCounter, colour);

			if (++porchCounter > 49)
			{
				porchStage++;
				porchCounter = 9;
				porchY = 3;
			}

		}
		else if (porchY == 3) // Downwards
		{
			SetPorchRow(porchCounter, colour);

			if (--porchCounter < 0)
			{
				porchStage++;
				porchCounter = 49;
				porchY = 4;
			}

		}
		else if (porchY == 4) // Right to Left
		{
			SetPorchColumn(porchCounter, colour);

			if (--porchCounter < 0)
			{
				porchStage++;
				porchCounter = 0;
				porchY = 1;
			}

		}

		if (porchStage > 5)
			porchStage = 0;

		porch.show();
	}

} // PorchWipe ***************************************************************************************************************************************************************************************************************************************


// vertical bars
void PorchVert()
{
	if (currentMillis - previousPorch > 20)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 20, 20, 20);

		//for (int i = 0; i < 9; i++)
		//{
		//	int p = (i * 50) + porchCounter;
		//	porch.setPixelColor(p, 200, 200, 200);
		//}

		SetPorchColumn(porchCounter, 0, MAX, 0);

		porchCounter += porchDir;

		if (porchCounter > 49)
		{
			porchDir = -1;
		}
		else if (porchCounter < 0)
		{
			porchDir = +1;
		}

		porch.show();
	}
} // PorchVert ***************************************************************************************************************************************************************************************************************************************


// horizontal bars
void PorchHorz()
{
	// 0    0 - 49
	// 1   50 - 99
	// 2  100 - 149
	// 3  150 - 199
	// 4  200 - 249
	// 5  250 - 299  
	// 6  300 - 349
	// 7  350 - 399  
	// 8  400 - 449

	if (currentMillis - previousPorch > 80)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 20, 20, 20);

		//for (int i = 0; i < 9; i++)
		//{
		//			porch.setPixelColor((porchCounter * 50) + i, 200, 200, 200);
		//	}

		SetPorchRow(porchCounter, MAX, MAX, MAX);

		porchCounter += porchDir;

		if (porchCounter > 8)
		{
			porchDir = -1;
		}
		else if (porchCounter < 0)
		{
			porchDir = +1;
		}

		porch.show();
	}
} // PorchHorz ***************************************************************************************************************************************************************************************************************************************


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


// dividing bars, from centre out to edge
void PorchDivider()
{
	if (currentMillis - previousPorch > 10)
	{
		previousPorch = currentMillis;

		if (porchCounter == 0)
		{
			SetColour(&porch, 0, 0, 0);
		}

		uint32_t colour = StageColour(&porch, porchStage);

		for (int line = 0; line < 9; line++)
		{
			int middle = (line * 50) + 25;
			porch.setPixelColor(middle - porchCounter, colour);
			porch.setPixelColor(middle, colour);
			porch.setPixelColor(middle + porchCounter, colour);
		}

		porch.show();

		porchCounter++;
		if (porchCounter > 25)
		{
			porchCounter = 1;

			porchStage++;
			if (porchStage > 5)
				porchStage = 0;
		}
	}
} // PorchDivider ****************************************************************************************************************************************************************************************************************************


// rainbow, each pixel is different
void PorchRainbowPixel()
{
	if (currentMillis - previousPorch > 280)
	{
		previousPorch = currentMillis;

		for (int i = 0; i < PORCH_PIXELS; i++)
		{
			if (porchCounter++ > 256)
				porchCounter = 0;

			porch.setPixelColor(i, Wheel(&porch, porchCounter));
		}

		porch.show();
	}
} // PorchRainbowPixel ****************************************************************************************************************************************************************************************************************************


// rainbox, all pixels using the same colour
void PorchRainbowAll()
{
	if (currentMillis - previousPorch > 50)
	{
		previousPorch = currentMillis;

		if (porchCounter++ > 256)
			porchCounter = 0;

		SetColour(&porch, Wheel(&porch, porchCounter));

		porch.show();
	}
} // PorchRainbowAll ****************************************************************************************************************************************************************************************************************************


// rainbow, another type
void PorchRainbowVert()
{
	if (currentMillis - previousPorch > 120)
	{
		previousPorch = currentMillis;

		for (int x = 0; x < 50; x++)
		{
			if (porchCounter++ > 256)
				porchCounter = 0;

			uint32_t colour = Wheel(&porch, porchCounter);

			for (int y = 0; y < 9; y++)
			{
				//int p = (x * 50) + y;

				SetPorchPixel(x, y, colour);
				//porch.setPixelColor(p, colour);
			}
		}

		/*
		porchCounter += porchDir;

		if (porchCounter > 50)
		{
		porchDir = -1;
		}
		else if (porchCounter < 0)
		{
		porchDir = +1;
		}
		*/

		porch.show();
	}

} // PorchRainbowVert ***************************************************************************************************************************************************************************************************************************************


void PorchBoxes()
{
	if (currentMillis - previousPorch > 320)
	{
		previousPorch = currentMillis;

		if (porchCounter == 0)
		{
			SetColour(&porch, MIN, MIN, MIN);
		}
		else
		{
			uint32_t colour = StageColour(&porch, porchStage);

			SetPorchColumn(0 + porchCounter, colour);
			SetPorchColumn(49 - porchCounter, colour);

			SetPorchRow(0 + porchCounter, colour);
			SetPorchRow(9 - porchCounter, colour);

			if (porchStage++ > 8)
				porchStage = 0;
		}

		if (++porchCounter > 5)
			porchCounter = 0;

		/*
		porchCounter += porchDir;

		if (porchCounter > 50)
		{
		porchDir = -1;
		}
		else if (porchCounter < 0)
		{
		porchDir = +1;
		}
		*/

		porch.show();
	}

} // PorchBoxes ***************************************************************************************************************************************************************************************************************************************


// moving block around the grid
void PorchBlockOut()
{
	if (currentMillis - previousPorch > 80)
	{
		previousPorch = currentMillis;

		if (porchStage == 0)
		{
			porchStage = 1;

			porchXdir = 1;
			porchYdir = 1;

			porchX = random(15, 35);
			porchY = random(2, 6);

		}
		else
		{
			SetColour(&porch, 20, 20, 20);

			porchX += porchXdir;

			if (porchX > 49)
			{
				porchXdir = -1;
			}
			else if (porchX < 0)
			{
				porchXdir = +1;
			}

			porchY += porchYdir;

			if (porchY > 9)
			{
				porchYdir = -1;
			}
			else if (porchY < 0)
			{
				porchYdir = +1;
			}

			//

			//SetColour(&porch, 0, 20, 0);

			//int p = (porchY * 50) + porchX;
			//SetPorchPixel(p + 0, 120, 0, 0);
			//  2222
			//  2012
			//  2112
			//  2222

			uint32_t colour = porch.Color(120, 0, 0);
			SetPorchPixel(porchX, porchY, colour);
			SetPorchPixel(porchX + 1, porchY, colour);
			SetPorchPixel(porchX, porchY + 1, colour);
			SetPorchPixel(porchX + 1, porchY + 1, colour);

			colour = porch.Color(60, 0, 0);
			SetPorchPixel(porchX - 1, porchY + 1, colour);
			SetPorchPixel(porchX - 0, porchY + 1, colour);
			SetPorchPixel(porchX + 1, porchY + 1, colour);
			SetPorchPixel(porchX + 2, porchY + 1, colour);
			SetPorchPixel(porchX - 1, porchY, colour);
			SetPorchPixel(porchX + 2, porchY, colour);
			SetPorchPixel(porchX - 1, porchY - 1, colour);
			SetPorchPixel(porchX + 2, porchY - 1, colour);
			SetPorchPixel(porchX - 1, porchY - 2, colour);
			SetPorchPixel(porchX - 0, porchY - 2, colour);
			SetPorchPixel(porchX + 1, porchY - 2, colour);
			SetPorchPixel(porchX + 2, porchY - 2, colour);
		}

		porch.show();
	}
} // PorchBlockOut ***************************************************************************************************************************************************************************************************************************************


void PorchWave()
{
	if (currentMillis - previousPorch > 30)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 60);

		for (int y = 0; y < 9; y++)
		{
			SetPorchPixel(porchCounter - 5, y, 0, 0, 70);
			SetPorchPixel(porchCounter - 4, y, 0, 0, 80);
			SetPorchPixel(porchCounter - 3, y, 0, 0, 90);
			SetPorchPixel(porchCounter - 2, y, 0, 0, 100);
			SetPorchPixel(porchCounter - 1, y, 0, 0, 130);
			SetPorchPixel(porchCounter + 0, y, 0, 0, 160);
			SetPorchPixel(porchCounter + 1, y, 0, 0, 110);
			SetPorchPixel(porchCounter + 2, y, 0, 0, 130);
			SetPorchPixel(porchCounter + 3, y, 0, 0, 90);
			SetPorchPixel(porchCounter + 4, y, 0, 0, 80);
			SetPorchPixel(porchCounter + 5, y, 0, 0, 70);
		}

		porchCounter += porchDir;

		if (porchCounter > 49)
		{
			porchDir = -1;
		}
		else if (porchCounter < 0)
		{
			porchDir = +1;
		}

		porch.show();
	}
} // PorchWave ***************************************************************************************************************************************************************************************************************************************


void PorchStarTurn()
{
	if (currentMillis - previousPorch > 600)
	{
		previousPorch = currentMillis;

		//ProcessPorchArray(porchStars);
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


void PorchHappyNewYear2()
{
	if (currentMillis - previousPorch > 60)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 0);

		uint32_t red = porch.Color(200, 0, 0);
		uint32_t green = porch.Color(0, 180, 0);
		uint32_t blue = porch.Color(0, 0, 210);
		uint32_t w = porch.Color(200, 190, 200);

		PorchHappy(porchCounter, 0, red);
		PorchNew(porchCounter + 30, 0, green);
		PorchYear(porchCounter + 56, 0, blue);

		//else if (porchStage == 3)
		//{
		//	PorchHappy(porchCounter, 0, w);
		//	PorchNew(porchCounter, 0, w);
		//	PorchYear(porchCounter, 0, blue);
		//}

		porchCounter += porchDir;

		if (porchCounter > 49)
		{
			porchDir = -1;
		}
		else if (porchCounter < -42)
		{
			porchDir = +1;

		}

		porch.show();
	}

} // PorchHappyNewYear2 ***************************************************************************************************************************************************************************************************************************************


void PorchTree()
{
	PorchHappyNewYear2();
}


void PorchTree2()
{
	if (currentMillis - previousPorch > 70)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 0);

		uint32_t red = porch.Color(200, 0, 0);
		uint32_t green = porch.Color(0, 200, 0);
		uint32_t blue = porch.Color(0, 0, 200);

		PorchTree(porchCounter - 25, 0, green);
		PorchTree(porchCounter - 10, 0, green);
		PorchTree(porchCounter + 5, 0, green);
		PorchTree(porchCounter + 20, 0, green);
		PorchTree(porchCounter + 35, 0, green);

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

} // PorchTree ***************************************************************************************************************************************************************************************************************************************


void PorchTree(int xOffset, int yOffset, uint32_t colour)
{
	// 0	{ 0, 0, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	SetPorchPixel(xOffset + 4, yOffset + 0, colour);
	SetPorchPixel(xOffset + 5, yOffset + 0, colour);
	SetPorchPixel(xOffset + 6, yOffset + 0, colour);

	// 1	{ 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	SetPorchPixel(xOffset + 1, yOffset + 1, colour);
	SetPorchPixel(xOffset + 2, yOffset + 1, colour);
	SetPorchPixel(xOffset + 3, yOffset + 1, colour);
	SetPorchPixel(xOffset + 4, yOffset + 1, colour);
	SetPorchPixel(xOffset + 5, yOffset + 1, colour);
	SetPorchPixel(xOffset + 6, yOffset + 1, colour);
	SetPorchPixel(xOffset + 7, yOffset + 1, colour);
	SetPorchPixel(xOffset + 8, yOffset + 1, colour);
	SetPorchPixel(xOffset + 9, yOffset + 1, colour);

	// 2	{ 0, 0, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	SetPorchPixel(xOffset + 2, yOffset + 2, colour);
	SetPorchPixel(xOffset + 3, yOffset + 2, colour);
	SetPorchPixel(xOffset + 4, yOffset + 2, colour);
	SetPorchPixel(xOffset + 5, yOffset + 2, colour);
	SetPorchPixel(xOffset + 6, yOffset + 2, colour);
	SetPorchPixel(xOffset + 7, yOffset + 2, colour);
	SetPorchPixel(xOffset + 8, yOffset + 2, colour);

	// 3	{ 0, 0, 0, 6, 6, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	SetPorchPixel(xOffset + 3, yOffset + 3, colour);
	SetPorchPixel(xOffset + 4, yOffset + 3, colour);
	SetPorchPixel(xOffset + 5, yOffset + 3, colour);
	SetPorchPixel(xOffset + 6, yOffset + 3, colour);
	SetPorchPixel(xOffset + 7, yOffset + 3, colour);

	// 4	{ 0, 0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	SetPorchPixel(xOffset + 4, yOffset + 4, colour);
	SetPorchPixel(xOffset + 5, yOffset + 4, colour);
	SetPorchPixel(xOffset + 6, yOffset + 4, colour);

	// 5	{ 0, 0, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	SetPorchPixel(xOffset + 2, yOffset + 5, colour);
	SetPorchPixel(xOffset + 3, yOffset + 5, colour);
	SetPorchPixel(xOffset + 4, yOffset + 5, colour);
	SetPorchPixel(xOffset + 5, yOffset + 5, colour);
	SetPorchPixel(xOffset + 6, yOffset + 5, colour);

	// 6	{ 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	SetPorchPixel(xOffset + 3, yOffset + 6, colour);
	SetPorchPixel(xOffset + 4, yOffset + 6, colour);
	SetPorchPixel(xOffset + 5, yOffset + 6, colour);
	SetPorchPixel(xOffset + 6, yOffset + 6, colour);
	SetPorchPixel(xOffset + 7, yOffset + 6, colour);

	// 7	{ 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	SetPorchPixel(xOffset + 4, yOffset + 7, colour);
	SetPorchPixel(xOffset + 5, yOffset + 7, colour);
	SetPorchPixel(xOffset + 6, yOffset + 7, colour);

	// 8	{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	SetPorchPixel(xOffset + 5, yOffset + 8, colour);

} // PorchTree ***************************************************************************************************************************************************************************************************************************************


//
//void PorchSpiral()
//{
//	if (currentMillis - previousPorch > 2)
//	{
//		previousPorch = currentMillis;
//
//		if (porchCounter++ == 0)
//		{
//			SetColour(&porch, 0, 0, 0);
//		}
//		else if (porchCounter > 451)
//		{
//			porchCounter = 1;
//
//			if (++porchStage > 5)
//				porchStage = 1;
//		}
//		else
//		{
//			uint32_t colour = StageColour(&porch, porchStage);
//
//			ProcessPorchArray(0, 0, patternSpiral, porchCounter, colour);
//		}
//
//		porch.show();
//
//	}
//
//} // PorchSpiral ***************************************************************************************************************************************************************************************************************************************
//


void PorchSleigh()
{
	PorchMerryChristmas();
}


void PorchSleigh2()
{
	if (currentMillis - previousPorch > 70)
	{
		previousPorch = currentMillis;

		if (porchCounter++ == 0)
		{
		}
		else if (porchCounter > 80)
		{
			porchCounter = 0;
		}
		else
		{
			SetColour(&porch, 0, 0, 0);
			//ProcessPorchArray(50 - porchCounter, 0, patternSleigh, 1, 180, 180, 180);
			//ProcessPorchArray(50 - porchCounter, 0, patternSleigh, 2, 200, 0, 0);

			uint32_t red = porch.Color(200, 0, 0);
			uint32_t white = porch.Color(180, 180, 180);

			PorchRaindeer(50 - porchCounter, 0, white);
			PorchRaindeer(60 - porchCounter, 0, white);
			PorchSleigh(70 - porchCounter, 0, red);
		}

		porch.show();
	}

} // PorchSleigh ***************************************************************************************************************************************************************************************************************************************


void PorchRaindeers()
{
	PorchHappyNewYear2();
}


void PorchRaindeers2()
{
	if (currentMillis - previousPorch > 300)
	{
		previousPorch = currentMillis;

		porchCounter++;

		if (porchCounter > 50)
		{
			porchCounter = 0;
		}
		else
		{
			SetColour(&porch, 0, 0, 0);

			uint32_t white = porch.Color(180, 180, 180);

			PorchRaindeer(10 - porchCounter, 0, white);
			PorchRaindeer(22 - porchCounter, 0, white);
			PorchRaindeer(34 - porchCounter, 0, white);
			PorchRaindeer(46 - porchCounter, 0, white);
			PorchRaindeer(58 - porchCounter, 0, white);

		}

		porch.show();
	}

} // PorchRaindeers ***************************************************************************************************************************************************************************************************************************************


void PorchRaindeer(int xOffset, int yOffset, uint32_t colour)
{
	//                                                     1  2  3  4  5  6  7  8  9
	//{ 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },
	//{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },
	//{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 }

	SetPorchPixel(xOffset + 1, yOffset + 0, colour);
	SetPorchPixel(xOffset + 7, yOffset + 0, colour);

	SetPorchPixel(xOffset + 2, yOffset + 1, colour);
	SetPorchPixel(xOffset + 6, yOffset + 1, colour);

	SetPorchPixel(xOffset + 3, yOffset + 2, colour);
	SetPorchPixel(xOffset + 4, yOffset + 2, colour);

	SetPorchPixel(xOffset + 3, yOffset + 3, colour);
	SetPorchPixel(xOffset + 4, yOffset + 3, colour);
	SetPorchPixel(xOffset + 5, yOffset + 3, colour);

	SetPorchPixel(xOffset + 3, yOffset + 4, colour);
	SetPorchPixel(xOffset + 4, yOffset + 4, colour);
	SetPorchPixel(xOffset + 5, yOffset + 4, colour);

	SetPorchPixel(xOffset + 2, yOffset + 5, colour);
	SetPorchPixel(xOffset + 6, yOffset + 5, colour);

	SetPorchPixel(xOffset + 1, yOffset + 6, colour);
	SetPorchPixel(xOffset + 2, yOffset + 6, colour);

	SetPorchPixel(xOffset + 2, yOffset + 7, colour);

	SetPorchPixel(xOffset + 3, yOffset + 8, colour);

} // PorchRaindeer ***************************************************************************************************************************************************************************************************************************************


void PorchSleigh(int xOffset, int yOffset, uint32_t colour)
{
	//                                                                                                           1  2  3  4  5  6  7  8  9  10 11 12 13 14
	//{ 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },
	//{ 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0 },
	//{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
	//{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0 }

	for (int i = 1; i <= 14; i++)
		SetPorchPixel(xOffset + i, yOffset + 0, colour);

	SetPorchPixel(xOffset + 3, yOffset + 1, colour);
	SetPorchPixel(xOffset + 12, yOffset + 1, colour);

	SetPorchPixel(xOffset + 2, yOffset + 2, colour);
	SetPorchPixel(xOffset + 12, yOffset + 2, colour);

	for (int i = 1; i <= 12; i++)
		SetPorchPixel(xOffset + i, yOffset + 3, colour);

	SetPorchPixel(xOffset + 1, yOffset + 4, colour);
	SetPorchPixel(xOffset + 12, yOffset + 4, colour);

	SetPorchPixel(xOffset + 1, yOffset + 5, colour);
	SetPorchPixel(xOffset + 12, yOffset + 5, colour);

	for (int i = 2; i <= 12; i++)
		SetPorchPixel(xOffset + i, yOffset + 6, colour);

	SetPorchPixel(xOffset + 12, yOffset + 7, colour);

	SetPorchPixel(xOffset + 12, yOffset + 8, colour);

} // PorchSleigh ***************************************************************************************************************************************************************************************************************************************


void PorchHappyNewYear()
{
	if (currentMillis - previousPorch > 60)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 0);

		uint32_t red = porch.Color(200, 0, 0);
		uint32_t green = porch.Color(0, 180, 0);
		uint32_t blue = porch.Color(0, 0, 210);
		uint32_t w = porch.Color(200, 190, 200);

		if (porchStage == 0)
		{
			PorchHappy(porchCounter, 0, red);
		}
		else if (porchStage == 1)
		{
			PorchNew(porchCounter, 0, green);
		}
		else if (porchStage == 2)
		{
			PorchYear(porchCounter, 0, blue);
		}
		//else if (porchStage == 3)
		//{
		//	PorchHappy(porchCounter, 0, w);
		//	PorchNew(porchCounter, 0, w);
		//	PorchYear(porchCounter, 0, blue);
		//}

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

} // PorchHappyNewYear ***************************************************************************************************************************************************************************************************************************************


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


void PorchNew(int xOffset, int yOffset, uint32_t colour)
{

	SetPorchPixel(xOffset + 1, yOffset + 1, colour);
	SetPorchPixel(xOffset + 7, yOffset + 1, colour);
	SetPorchPixel(xOffset + 11, yOffset + 1, colour);
	SetPorchPixel(xOffset + 12, yOffset + 1, colour);
	SetPorchPixel(xOffset + 17, yOffset + 1, colour);
	SetPorchPixel(xOffset + 19, yOffset + 1, colour);

	SetPorchPixel(xOffset + 1, yOffset + 2, colour);
	SetPorchPixel(xOffset + 6, yOffset + 2, colour);
	SetPorchPixel(xOffset + 7, yOffset + 2, colour);
	SetPorchPixel(xOffset + 10, yOffset + 2, colour);
	SetPorchPixel(xOffset + 16, yOffset + 2, colour);
	SetPorchPixel(xOffset + 18, yOffset + 2, colour);
	SetPorchPixel(xOffset + 20, yOffset + 2, colour);

	SetPorchPixel(xOffset + 1, yOffset + 3, colour);
	SetPorchPixel(xOffset + 5, yOffset + 3, colour);
	SetPorchPixel(xOffset + 7, yOffset + 3, colour);
	SetPorchPixel(xOffset + 9, yOffset + 3, colour);
	SetPorchPixel(xOffset + 15, yOffset + 3, colour);
	SetPorchPixel(xOffset + 21, yOffset + 3, colour);

	SetPorchPixel(xOffset + 1, yOffset + 4, colour);
	SetPorchPixel(xOffset + 4, yOffset + 4, colour);
	SetPorchPixel(xOffset + 7, yOffset + 4, colour);
	SetPorchPixel(xOffset + 9, yOffset + 4, colour);
	SetPorchPixel(xOffset + 10, yOffset + 4, colour);
	SetPorchPixel(xOffset + 11, yOffset + 4, colour);
	SetPorchPixel(xOffset + 12, yOffset + 4, colour);
	SetPorchPixel(xOffset + 13, yOffset + 4, colour);
	SetPorchPixel(xOffset + 15, yOffset + 4, colour);
	SetPorchPixel(xOffset + 21, yOffset + 4, colour);

	SetPorchPixel(xOffset + 1, yOffset + 5, colour);
	SetPorchPixel(xOffset + 3, yOffset + 5, colour);
	SetPorchPixel(xOffset + 7, yOffset + 5, colour);
	SetPorchPixel(xOffset + 10, yOffset + 5, colour);
	SetPorchPixel(xOffset + 12, yOffset + 5, colour);
	SetPorchPixel(xOffset + 15, yOffset + 5, colour);
	SetPorchPixel(xOffset + 21, yOffset + 5, colour);

	SetPorchPixel(xOffset + 1, yOffset + 6, colour);
	SetPorchPixel(xOffset + 2, yOffset + 6, colour);
	SetPorchPixel(xOffset + 7, yOffset + 6, colour);
	SetPorchPixel(xOffset + 11, yOffset + 6, colour);

	SetPorchPixel(xOffset + 1, yOffset + 7, colour);
	SetPorchPixel(xOffset + 7, yOffset + 7, colour);


} // PorchNew ***************************************************************************************************************************************************************************************************************************************


void PorchYear(int xOffset, int yOffset, uint32_t colour)
{

	SetPorchPixel(xOffset + 3, yOffset + 0, colour);

	SetPorchPixel(xOffset + 3, yOffset + 1, colour);
	SetPorchPixel(xOffset + 9, yOffset + 1, colour);
	SetPorchPixel(xOffset + 10, yOffset + 1, colour);
	SetPorchPixel(xOffset + 16, yOffset + 1, colour);
	SetPorchPixel(xOffset + 18, yOffset + 1, colour);

	SetPorchPixel(xOffset + 3, yOffset + 2, colour);
	SetPorchPixel(xOffset + 8, yOffset + 2, colour);
	SetPorchPixel(xOffset + 14, yOffset + 2, colour);
	SetPorchPixel(xOffset + 15, yOffset + 2, colour);
	SetPorchPixel(xOffset + 16, yOffset + 2, colour);
	SetPorchPixel(xOffset + 18, yOffset + 2, colour);

	SetPorchPixel(xOffset + 3, yOffset + 3, colour);
	SetPorchPixel(xOffset + 7, yOffset + 3, colour);
	SetPorchPixel(xOffset + 13, yOffset + 3, colour);
	SetPorchPixel(xOffset + 16, yOffset + 3, colour);
	SetPorchPixel(xOffset + 18, yOffset + 3, colour);

	SetPorchPixel(xOffset + 2, yOffset + 4, colour);
	SetPorchPixel(xOffset + 4, yOffset + 4, colour);
	SetPorchPixel(xOffset + 7, yOffset + 4, colour);
	SetPorchPixel(xOffset + 8, yOffset + 4, colour);
	SetPorchPixel(xOffset + 9, yOffset + 4, colour);
	SetPorchPixel(xOffset + 10, yOffset + 4, colour);
	SetPorchPixel(xOffset + 11, yOffset + 4, colour);
	SetPorchPixel(xOffset + 13, yOffset + 4, colour);
	SetPorchPixel(xOffset + 16, yOffset + 4, colour);
	SetPorchPixel(xOffset + 18, yOffset + 4, colour);

	SetPorchPixel(xOffset + 1, yOffset + 5, colour);
	SetPorchPixel(xOffset + 5, yOffset + 5, colour);
	SetPorchPixel(xOffset + 8, yOffset + 5, colour);
	SetPorchPixel(xOffset + 10, yOffset + 5, colour);
	SetPorchPixel(xOffset + 14, yOffset + 5, colour);
	SetPorchPixel(xOffset + 15, yOffset + 5, colour);
	SetPorchPixel(xOffset + 19, yOffset + 5, colour);

	SetPorchPixel(xOffset + 1, yOffset + 6, colour);
	SetPorchPixel(xOffset + 5, yOffset + 6, colour);
	SetPorchPixel(xOffset + 9, yOffset + 6, colour);
	SetPorchPixel(xOffset + 20, yOffset + 6, colour);
	SetPorchPixel(xOffset + 21, yOffset + 6, colour);

	SetPorchPixel(xOffset + 1, yOffset + 7, colour);
	SetPorchPixel(xOffset + 5, yOffset + 7, colour);

} // PorchYear ***************************************************************************************************************************************************************************************************************************************


void PorchMerryChristmas()
{
	PorchHappyNewYear();
}


void PorchMerryChristmas2()
{
	if (currentMillis - previousPorch > 60)
	{
		previousPorch = currentMillis;

		SetColour(&porch, 0, 0, 0);

		if (porchStage == 0)
		{
			ProcessPorchArray(porchCounter, 0, patternMerry, 1, 140, 0, 0);
		}
		else if (porchStage == 1)
		{
			ProcessPorchArray(porchCounter, 0, patternChristmas, 1, 140, 0, 0);
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
				porchStage = 0;
		}


		porch.show();
	}

} // PorchMerryChristmas ***************************************************************************************************************************************************************************************************************************************



/*



void PorchShimmer()
{
if (currentMillis - previousPorch > 80)
{
previousPorch = currentMillis;

if (porchCounter++ == 0)
{
SetColour(&porch, 20, 0, 0);
}
else if (porchCounter > 91)
{
porchCounter = 0;
}
else
{
ProcessPorchArray(patternShimmer, porchCounter, 0, 120, 0);
ProcessPorchArray(patternShimmer, porchCounter - 10, 0, 90, 0);
ProcessPorchArray(patternShimmer, porchCounter - 20, 0, 60, 0);
ProcessPorchArray(patternShimmer, porchCounter - 30, 0, 30, 0);
ProcessPorchArray(patternShimmer, porchCounter - 40, 0, 10, 0);
}

porch.show();
}

} // PorchShimmer ***************************************************************************************************************************************************************************************************************************************



void PorchDropping()
{
if (currentMillis - previousPorch > 100)
{
previousPorch = currentMillis;

if (porchCounter++ == 0)
{
SetColour(&porch, 40, 0, 0);
}
else if (porchCounter > 60)
{
porchCounter = 0;
}
else
{
ProcessPorchArray(patternDropping, porchCounter, 0, 40, 0);
}

porch.show();
}

} // PorchDropping ***************************************************************************************************************************************************************************************************************************************



void PorchTree()
{
if (currentMillis - previousPorch > 300)
{
previousPorch = currentMillis;

if (porchCounter++ == 0)
{
SetColour(&porch, 0, 0, 0);
}
else if (porchCounter > 70)
{
porchCounter = 0;
}
else
{
ProcessPorchArray(patternDropping, 999, 0, 40, 0, porchCounter, 0);
}

porch.show();
}

} // PorchTree ***************************************************************************************************************************************************************************************************************************************



*/


void ProcessPorchArray(byte pixels[][50], int index, int r, int g, int b)
{
	ProcessPorchArray(0, 0, pixels, index, r, g, b);
} // ProcessPorchArray ***************************************************************************************************************************************************************************************************************************************


void ProcessPorchArray(int offsetX, int offsetY, byte pixels[][50], int index, int r, int g, int b)
{
	uint32_t colour = porch.Color(r, g, b);
	ProcessPorchArray(offsetX, offsetY, pixels, index, colour);
} // ProcessPorchArray ***************************************************************************************************************************************************************************************************************************************


void ProcessPorchArray(int offsetX, int offsetY, byte pixels[][50], int index, uint32_t colour)
{

	if (index < 0 || index > 1000)
		return;

	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 50; x++)
		{
			int data = pixels[y][x]; // pixels

			if ((data == index) || (index == 999 && data != 0))
			{
				int px = x + offsetX;
				int py = y + offsetY;

				if (px >= 0 && px <= 49 && py >= 0 && py <= 9)
				{
					int p = ((8 - py) * 50);
					if (py == 1 || py == 3 || py == 5 || py == 7)
						p += px;
					else
						p += (49 - px);

					porch.setPixelColor(p, colour);
				}
			}

		} // for x
	} // for y

} // ProcessPorchArray ***************************************************************************************************************************************************************************************************************************************



void FlakeSparkle()
{
} // FlakeSparkle ***************************************************************************************************************************************************************************************************************************************


void FlakeRainbow()
{
} // FlakeRainbow ****************************************************************************************************************************************************************************************************************************


void FlakeDisco()
{
} // FlakeDisco ****************************************************************************************************************************************************************************************************************************


void FlakeTwinkle()
{
} // FlakeTwinkle ****************************************************************************************************************************************************************************************************************************


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
		colour = strip->Color(MAX, 0, 0); // red
	}
	else if (stageNumber == 1)
	{
		colour = strip->Color(0, MAX, 0); // green
	}
	else if (stageNumber == 2)
	{
		colour = strip->Color(0, 0, MAX); // blue
	}
	else if (stageNumber == 3)
	{
		colour = strip->Color(MAX, MAX, 0); // yellow
	}
	else if (stageNumber == 4)
	{
		colour = strip->Color(0, MAX, MAX); // cyan
	}
	else if (stageNumber == 5)
	{
		colour = strip->Color(MAX, 0, MAX); // magenta
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
		colour = strip->Color(MAX, MAX, MAX); // white
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

