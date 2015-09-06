#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <Time.h>
#include <LowPower.h>

// =========================================================================================================================

#define VERSION			"v1.44" // Remote_WiFi_Sensor, v1.11, 27 Feb 2015"
// 1.43 No longer flash LED, use LowPower library and powerDown arduino for 10 minutes rather than loop/waiting

#if 1
__asm volatile ("nop");
#endif

#define						HOME
//#define						WORK
//#define						UTILITY
#define						SHED

#define WIFI_SERIAL			wifiSerial
#define DEBUG_SERIAL		Serial

#define	ROOM				1
#define FRIDGE				0
#define	EXCHANGER			2

#define	SHED_INDOOR			1
#define SHED_OUTDOOR		0

// =========================================================================================================================

#if defined(HOME) && defined(UTILITY)
#define FEEDBACK_IDX		192
#define SENSORS				3
int idx[] = { 186, 187, 195 };
float temperatures[] = { 0, 0, 0 };
//String names[] = { "12", "11", "13" };
// 186 TEMP12_BackFridge
// 187 TEMP11_UtilityRoom
// 195 TEMP13_HeatExchanger

#elif defined(HOME) && defined(SHED)

#define FEEDBACK_IDX		191
#define SENSORS				2
int idx[] = { 184, 185 };
float temperatures[] = { 0, 0, 0};
// 191 SHED_SENSOR
// 184 SHED_OUTDOOR
// 185 SHED_INDOOR

#elif defined(WORK) && defined(UTILITY)
#define FEEDBACK_IDX		29
#define SENSORS				3
int idx[] = { 12, 11, 36 };
float temperatures[] = { 0, 0, 0, 0, 0, 0 };
//String names[] = { "11UR", "12F", "13E" };

#elif defined(WORK) && defined(SHED)
#define FEEDBACK_IDX		34
#define SENSORS				2
int idx[] = { 10, 9 };
float temperatures[] = { 0, 0, 0, 0, 0, 0 };
//String names[] = { "TEMP21_ShedIndoors", "TEMP22_ShedOutdoors" };

#endif


#if defined(HOME)
#define WAP_NAME			"VM934292-2G"
#define WAP_PWD				"qufunkkn"
#define IP_ADDRESS			"192.168.0.205" // elara
#define IP_PORT				"8080"

// Utility
//#define SPARKFUN_STREAM		"g6M8ZX98J0sKnv34LxM3"
//#define SPARKFUN_KEY		"qz9mGb2mD0Cg6Mr8kRAr"

// Shed
#define SPARKFUN_STREAM		"XGv3QRmbxbTEArmdnzp3"
#define SPARKFUN_KEY		"1J0mp91xPxCl0x9apYqz"

#elif defined(WORK)

//#define WAP_NAME			"Eastleigh WiFi"
//#define WAP_PWD				"11111222223333444455556666"
//#define IP_ADDRESS			"10.22.38.15" // my laptop
//#define IP_PORT				"6262"

#define WAP_NAME			"BTHomeHub2-FTH5"
#define WAP_PWD				"4b65ad7b28"
#define IP_ADDRESS			"192.168.1.223" // my laptop
#define IP_PORT				"6262"

//#define SPARKFUN_STREAM		"ro1aggv676CyGod2JgDA"
//#define SPARKFUN_KEY		"jkeRYYJEXEtY6olkpebr"

#define SPARKFUN_STREAM		"XGv3QRmbxbTEArmdnzp3"
#define SPARKFUN_KEY		"1J0mp91xPxCl0x9apYqz"


#endif


/*
HOME
192  UtilityRoomSensor
187  TEMP11_UtilityRoom
186  TEMP12_Fridge
195  TEMP13_FridgeExchanger
TEMP14_ possible bedroom
191  ShedSensor
185  TEMP21_ShedIndoor
184  TEMP22_ShedOutdoor

LAPTOP
29   UtilityRoomSensor
12   TEMP11_UtilityRoom
11   TEMP12_Fridge
xx   TEMP13_FridgeExchanger
TEMP14_ possible bedroom
34   ShedSensor
10   TEMP21_ShedIndoor
09   TEMP22_ShedOutdoor
*/

#define PIN_SOFTRX			2
#define PIN_SOFTTX			3
#define PIN_RADIO			5
#define PIN_BUS				8
#define PIN_LED				13

#define MAX_BUFFER			40
//#define STARTUP_TIME		20
//#define WAIT_TIME			600
//#define MEASURE_TIME		10


//#define URL_GET				"json.htm?type=devices"
//#define URL_SET				"/json.htm?type=command&param=udevice&idx=XXXX&nvalue=0&svalue="

//#define WAP_NAME			"VM934292-5G"
//#define WAP_PWD			"qufunkkn"
//#define IP_ADDRESS		"192.168.0.205"
//#define IP_PORT			"2319"
//#define URL				"http : //192.168.0.205:2319/json.htm?type=command&param=udevice&idx=10&nvalue=0&svalue="


/*


Simple AT commands:
AT#								- simply returns OK
AT+RST
AT+CWLAP						- list access points
AT+CWJAP="name","password"#		- join access point
AT+CWJAP=?						- doesnt work, just returns no this fun
AT+CWQAP						- quit access point
AT+CIFSR						- get the ip address
AT+CIPSTATUS					- status

AT+CWJAP="BTHomeHub2-FTH5","4b65ad7b28"

Issue Reset Status 4
Joined AP	Status 2
Open TCP	Status 3
Send data	Status 3
Close		Status 5

#############################################################################################################################################

http://<username:password@>domoticz-ip<:port>/json.htm?api-call

http : //localhost:2319/json.htm?type=command_param=switchlight_idx={0}_switchcmd={1}

Set a dim-able light to a certain level
/json.htm?type=command&param=switchlight&idx=&switchcmd=Set%20Level&level=6

Humidity
/json.htm?type=command&param=udevice&idx=10&nvalue=45&svalue=2

Temperature
/json.htm?type=command&param=udevice&idx=10&nvalue=0&svalue=12.3

Temerature/Humidity/Barometer
/json.htm?type=command&param=udevice&idx=10&nvalue=0&svalue=12.3;45;2;1013;1

Wind
/json.htm?type=command&param=udevice&idx=10&nvalue=0&svalue=305;NW;4.2;5.5;22;24


*/

// =========================================================================================================================

SoftwareSerial wifiSerial(PIN_SOFTRX, PIN_SOFTTX); // RX, TX
OneWire oneWire(PIN_BUS);
DallasTemperature sensors(&oneWire);

unsigned long currentMillis = 0;
unsigned long previousLED = 0;
int ledCounter = 0;
int timeCounter = 0;
//int seconds = 0;
//int minutes = 0;
//int hours = 0;
//int days = 0;
int measureSeconds;
int sendSeconds;
unsigned long counter = 0;
unsigned long error;
unsigned long errors = 0;
//int waitTime = STARTUP_TIME;
float currentTemp = 0;
float previousTemp = 0;
int numSensors = SENSORS;
time_t startedTime;
time_t nowTime;
String startedDate = "NONE";
//String message = "";
String runningText = "";
String debug = "";
int iTemp = 0;
char letter;
//String line1 = "";
//String line2 = "";
//String line3 = "";
//String line4 = "";

String wifiCommand = "";
String wifiParams = "";
String wifiData = "";
String wifiReceived = "";
//String wifiMyIp = "NONE";
int wifiLength = 0;
uint8_t buffer[MAX_BUFFER];


int mode = 2;
// -1 = no ids
// -2 = no sensors
//  1 = get device ids from device names
//  2 = normal operation

// #############################################################################################################################################

void setup()
{
	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, LOW);

	pinMode(PIN_RADIO, OUTPUT);
	digitalWrite(PIN_RADIO, LOW);

	setTime(0, 0, 0, 1, 1, 2001);
	startedTime = now();

	// wait seconds, in case we need to use serial port to upload new sketch

	ledCounter = 0;
	for (int i = 0; i < 5; i++)
	{
		if (ledCounter == 0)
		{
			ledCounter = 1;
			digitalWrite(PIN_LED, HIGH);
		}
		else
		{
			ledCounter = 0;
			digitalWrite(PIN_LED, LOW);
		}

		delay(1000);
	}

	digitalWrite(PIN_LED, LOW);

#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.begin(9600);
#endif

	debug = VERSION;
	Debug();

	WIFI_SERIAL.begin(9600); // 9600, 19200, 34800,57600

	//pixel.begin();

	//previousLED = millis();

	measureSeconds = 0;
	sendSeconds = 0;

	delay(5000);
	GetMeasurements();
	delay(5000);
}


void loop()
{
	//currentMillis = millis();

	//unsigned long gap = currentMillis - previousLED;

	//if (gap > 100)
	//{
	//	previousLED = currentMillis;

	//	if (++ledCounter > 10)
	//	{
	//		ledCounter = 0;
	//		measureSeconds++;
	//		sendSeconds++;
	//		//Debug("Tick");
	//		//Display();
	//	}

	//	if (error)
	//	{
	//		if (ledCounter == 1 || ledCounter == 3 || ledCounter == 5) // previous error
	//			digitalWrite(PIN_LED, HIGH);
	//		else
	//			digitalWrite(PIN_LED, LOW);
	//	}
	//	else
	//	{
	//		if (ledCounter == 1 || ledCounter == 2 || ledCounter == 4 || ledCounter == 5) // normal operation, double flash green
	//			digitalWrite(PIN_LED, HIGH);
	//		else
	//			digitalWrite(PIN_LED, LOW);
	//	}

	/*
	// status led pixel
	pixel.setPixelColor(0, 0);

	if (mode == 1) // looking for device ids from names, single long flash green
	{
	if (ledCounter == 1 || ledCounter == 2 || ledCounter == 3 || ledCounter == 4)
	pixel.setPixelColor(0, 0, 50, 0);
	}
	else if (mode == 2) // normal operation, double flash green
	{
	if (ledCounter == 1 || ledCounter == 2 || ledCounter == 4 || ledCounter == 5)
	pixel.setPixelColor(0, 0, 50, 0);
	}

	pixel.show();
	*/

	//}

	//

	//if (measureSeconds > MEASURE_TIME)
	//{
	//RunningTime();
	//debug = runningText;
	//Debug();

	previousTemp = currentTemp;

	GetMeasurements();
	measureSeconds = 0;

	// if the temp has changed by more than 2 degree, update central straight way
	//float diff = previousTemp - currentTemp;
	//if (diff < -2 || diff > 2)
	//	sendSeconds = waitTime + 1;
	//}

	//

	//if (sendSeconds > waitTime)
	//{
	if (startedDate.length() <= 0)
	{
		// have not got startup server time yet

		//GetServerTime();

		sendSeconds = 0;
		//waitTime = 60;
	}
	else
	{
		SendMeasurements();

		sendSeconds = 0;
		//waitTime = WAIT_TIME;
	}
	//}

	//
	
	debug = "Waiting";
	Debug();
	DEBUG_SERIAL.flush();
	delay(100);

	// 4 seconds * 15 = 2 minutes
	// 8 seconds * 75 = 600 seconds = 10 minutes

	for (int waiting = 0; waiting < 20; waiting++)
	{
		LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
		adjustTime(8L);
	}

	//long adj = 5L * 8L * 1000L;
	//adjustTime(adj);

	debug = "Awake";
	Debug();
} // Loop


void LedOn()
{
	//digitalWrite(PIN_LED, HIGH);
}


void LedOff()
{
	//digitalWrite(PIN_LED, LOW);
}


void GetMeasurements()
{
	debug = "GM_R";
	Debug();
	sensors.requestTemperatures();

	currentTemp = 0;

	for (int i = 0; i < numSensors; i++)
	{
		debug = "S";
		debug += i;
		debug += " ";
		//debug += names[i];

		float tempC = sensors.getTempCByIndex(i);
		currentTemp += tempC;
		temperatures[i] = tempC;

		debug += ", C=";
		debug += tempC;

		//float tempFvalue = DallasTemperature::toFahrenheit(tempC);
		//char buffer[10];
		//String tempF = dtostrf(tempFvalue, 4, 1, buffer);

		//output += ", F=";
		//output += tempF;

		Debug();
	}

	currentTemp = currentTemp / numSensors;
	//Debug("Current Average =" + currentTemp);

	//

}


void SendMeasurements()
{
	//

	debug = "SM";
	Debug();

	//

	RadioOn();

	Reset();

	//

	counter++;

	//

	error = TryConnect();

	if (!error)
	{
		LedOn();

		SendToCloud();

		Close();

		Reset();

		LedOff();

		delay(1000);
	}

	if (!error && FEEDBACK_IDX > 0)
	{
		error = TryConnect();

		if (!error)
		{
			LedOn();

			SendHeartbeat();

			Close();

			Reset();

			LedOff();

			delay(1000);
		}
	}

	//

	if (!error)
	{

		LedOn();

		//BuildStartCommand();
		//SendLine();
		//GetMySerial(3);
		//CheckWiFiReceived();

		for (int i = 0; i < numSensors; i++)
		{
			error = TryConnect();

			if (!error)
			{

				//error = TryConnect();

				//if (error == 0)
				error = SendData(i);

				Close();

				//DisConnect();

				Reset();

				//delay(2000);
			}
		}

		//Close();

		LedOff();
	}

	if (error)
	{
		errors++;
	}

	//

	Reset();

	DisConnect();

	//

	RadioOff();

	//

	mode = 99;
} // SendMeasurements


/*
void GetServerTime()
{
title = "GetServerTime";

//

debug = "GST";
Debug();

String text;

//

RadioOn();

//

digitalWrite(PIN_LED, HIGH);

error = TryConnect();

//

if (error == 0)
{
// Open Connection
//BuildStartCommand();

wifiCommand = "AT+CIPSTART=\"TCP\",\"www.currentmillis.com\",80";

SendLine();

GetMySerial(5);

CheckWiFiReceived();

delay(50);

wifiParams = "/api/seconds-since-unix-epoch.php";
BuildWifiData();

BuildSendCommand();
SendLine();

//data = "GET ";
//data += "/json.htm?type=command&param=getSunRiseSet";
//data += " HTTP/1.1\r\n";
//data += "Host: 1.1.1.1\r\n";
//data += "User-Agent: Arduino\r\n";
//data += "\r\n";

//Debug(data);

//int length = data.length();

//// Send data

//String command = "AT+CIPSEND=";
//command += length;
//SendLine(command);

GetMySerial(2);

CheckWiFiReceived();

//DebugReceived(received);

delay(500);

SendChars();

while (WIFI_SERIAL.available())
{
if (WIFI_SERIAL.find("html"))
{
WIFI_SERIAL.flush();
GetMySerial(10);

DebugReceived();

}
}

GetMySerial(10);

DebugReceived();

//temp = wifiReceived;

//if (wifiReceived != NULL && wifiReceived.length() > 0)
//temp += wifiReceived;

for (int i = 0; i < 100; i++)
{
GetMySerial(0);
if (wifiReceived != NULL && wifiReceived.length() > 0)
DebugReceived();
//temp += wifiReceived;
}

//wifiReceived = temp;

//

int idx = wifiReceived.indexOf("ServerTime");

if (idx > 0)
{
wifiReceived = wifiReceived.substring(idx + 10);

if (wifiReceived.length() > 26)
{
startedDate = wifiReceived.substring(5, 25);
//Debug(data);
}
}

//DEBUG_SERIAL.println("SetTime");
//setTime(1433256538);

//DEBUG_SERIAL.println("now");
//startedTime = now();

//DEBUG_SERIAL.println("times");
//DEBUG_SERIAL.println(startedTime);
//DEBUG_SERIAL.println(year());
//DEBUG_SERIAL.println(month());
//DEBUG_SERIAL.println(day());
//DEBUG_SERIAL.println(hour());
//DEBUG_SERIAL.println(minute());
//DEBUG_SERIAL.println(second());
//DEBUG_SERIAL.println(weekday());

//DEBUG_SERIAL.println("waiting");

//delay(5000);



//

Reset();

delay(1000);
}

//

DisConnect();

digitalWrite(PIN_LED, LOW);

//

RadioOff();

//

title = "Running";

mode = 99;
} // GetServerTime

*/


/*void GetDevices()
{
error = TryConnect();

if (error == 0)
{
String data = SendRequestDevices();

if (data.length() > 10)
ProcessReceivedDevices(data);
}

DisConnect();

}
*/

void RadioOn()
{
	debug = "RadioOn";
	Debug();

	digitalWrite(PIN_RADIO, HIGH);

	delay(5000); // wait for radio to power up
} // RadioOn ================================================================================================================================================================================================================================================================================


void RadioOff()
{
	debug = "RadioOff";
	Debug();

	digitalWrite(PIN_RADIO, LOW);

	delay(100);
} // RadioOff ================================================================================================================================================================================================================================================================================


void DebugOK()
{
#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.print("OK [");

	if (wifiReceived == NULL)
	{
		DEBUG_SERIAL.print("*1*");
	}
	else if (wifiReceived.length() == 0)
	{
		DEBUG_SERIAL.print("*2*");
	}
	else if (wifiReceived == "")
	{
		DEBUG_SERIAL.print("*3*");
	}
	else
	{
		for (int i = 0; i < wifiReceived.length(); i++)
		{
			letter = wifiReceived.charAt(i);
			int value = letter;

			if (value == 10)
			{
				DEBUG_SERIAL.print("~");
			}
			else if (value == 13)
			{
				DEBUG_SERIAL.print("#");
			}
			else if (value < 32)
			{
				DEBUG_SERIAL.print("{");
				DEBUG_SERIAL.print(value);
				DEBUG_SERIAL.print("}");
			}
			else
			{
				DEBUG_SERIAL.print(letter);
			}
		}
	}
	DEBUG_SERIAL.println("]");
#endif
} // DebugOK


void DebugError()
{
#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.print(": Error [");

	if (wifiReceived == NULL)
	{
		DEBUG_SERIAL.print("*1*");
	}
	else if (wifiReceived.length() == 0)
	{
		DEBUG_SERIAL.print("*2*");
	}
	else if (wifiReceived == "")
	{
		DEBUG_SERIAL.print("*3*");
	}
	else
	{
		for (int i = 0; i < wifiReceived.length(); i++)
		{
			letter = wifiReceived.charAt(i);
			int value = letter;

			if (value == 10)
			{
				DEBUG_SERIAL.print("~");
			}
			else if (value == 13)
			{
				DEBUG_SERIAL.print("#");
			}
			else if (value < 32)
			{
				DEBUG_SERIAL.print("{");
				DEBUG_SERIAL.print(value);
				DEBUG_SERIAL.print("}");
			}
			else
			{
				DEBUG_SERIAL.print(letter);
			}
		}
	}
	DEBUG_SERIAL.println("]");
#endif
} // DebugError


void DebugReceived()
{
#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.print("[");

	if (wifiReceived == NULL)
	{
		DEBUG_SERIAL.print("*1*");
	}
	else if (wifiReceived.length() == 0)
	{
		DEBUG_SERIAL.print("*2*");
	}
	else if (wifiReceived == "")
	{
		DEBUG_SERIAL.print("*3*");
	}
	else
	{
		for (int i = 0; i < wifiReceived.length(); i++)
		{
			letter = wifiReceived.charAt(i);
			int value = letter;

			if (value == 10)
			{
				DEBUG_SERIAL.print("~");
			}
			else if (value == 13)
			{
				DEBUG_SERIAL.print("#");
			}
			else if (value < 32)
			{
				DEBUG_SERIAL.print("{");
				DEBUG_SERIAL.print(value);
				DEBUG_SERIAL.print("}");
			}
			else
			{
				DEBUG_SERIAL.print(letter);
			}
		}
	}
	DEBUG_SERIAL.println("]");
#endif
} // DebugReceived


void Debug()
{
#ifdef DEBUG_SERIAL
	//DEBUG_SERIAL.println(s);

	RunningTime();
	DEBUG_SERIAL.print(runningText);
	DEBUG_SERIAL.print(" - ");

	if (debug == NULL)
	{
		DEBUG_SERIAL.print("*1N*");
	}
	else if (debug.length() == 0)
	{
		DEBUG_SERIAL.print("*20*");
	}
	else if (debug == "")
	{
		DEBUG_SERIAL.print("*3z*");
	}
	else
	{
		for (int i = 0; i < debug.length(); i++)
		{
			letter = debug.charAt(i);
			int value = letter;

			if (value == 10)
			{
				DEBUG_SERIAL.print("~");
			}
			else if (value == 13)
			{
				DEBUG_SERIAL.print("#");
			}
			else if (value < 32)
			{
				DEBUG_SERIAL.print("{");
				DEBUG_SERIAL.print(value);
				DEBUG_SERIAL.print("}");
			}
			else
			{
				DEBUG_SERIAL.print(letter);
			}
		}
	}

	DEBUG_SERIAL.println();

#endif
	//message = debug;
}


void SendLine()
{
	wifiReceived = "";

	//Debug("Sending Line: (" + s + ")");
	debug = "SL";
	Debug();

	//debug = wifiCommand;
	//Debug();
	DEBUG_SERIAL.println(wifiCommand);

	//WIFI_SERIAL.println(s);

	for (int pos = 0; pos < wifiCommand.length(); pos++)
	{
		// 	ignore echo characters
		// 	GetMySerial(0);
		while (WIFI_SERIAL.available())
		{
			iTemp = WIFI_SERIAL.read();
		}

		delay(20);

		letter = wifiCommand.charAt(pos);
		WIFI_SERIAL.print(letter);
	}

	delay(20);

	WIFI_SERIAL.println();
}


void SendChars() // int length, String s
{
	wifiReceived = "";

	//String len = String(length);
	//Debug("SC");
	//Debug(len);
	//Debug(" (" + s + ")");

	//WIFI_SERIAL.println(s);

	for (int pos = 0; pos < wifiLength; pos++)
	{
		// 	ignore echo characters
		// 	GetMySerial(0);
		while (WIFI_SERIAL.available())
		{
			iTemp = WIFI_SERIAL.read();
		}

		delay(20);

		letter = wifiData.charAt(pos);
		WIFI_SERIAL.print(letter);
		delay(10);
	}

} // SendChars


int TryConnect()
{
	debug = "TC";
	Debug();

	// check for basic comms to module

	//WIFI_SERIAL.flush();
	//delay(1000);
	//received = GetMySerial(0, "");
	//received = GetMySerial(0, "");
	//received = GetMySerial(0, "");
	GetMySerial(0);

	wifiCommand = "AT";
	SendLine();

	GetMySerial(1);
	DebugReceived();

	if (wifiReceived.indexOf("** NONE **") >= 0)
	{
		debug = "NONE, Error";
		Debug();
		return -1;
	}
	else if (wifiReceived.indexOf("OK") >= 0)
	{
		DebugOK();
	}
	else
	{
		DebugReceived();
		return -2;
	}

	// put into station/client mode

	wifiCommand = "AT+CWMODE=1";
	SendLine();

	// no change
	GetMySerial(3);

	if (wifiReceived.indexOf("OK") >= 0 || wifiReceived.indexOf("no change") >= 0)
	{
		DebugOK();
	}
	else
	{
		DebugReceived();
		return -4;
	}

	delay(1000);

	debug = "Join";
	Debug();

	// Connect to Access Point

	wifiCommand = ""; // new String();
	wifiCommand.concat("AT+CWJAP=");
	wifiCommand.concat("\"");
	wifiCommand.concat(WAP_NAME);
	wifiCommand.concat("\"");
	wifiCommand.concat(",");
	wifiCommand.concat("\"");
	wifiCommand.concat(WAP_PWD);
	wifiCommand.concat("\"");

	//DEBUG_SERIAL.println(wifiCommand);

	//if (wifiCommand == NULL)
	//	debug = "Whats up 1";
	//else
	//	debug = "Not null";
	//Debug();

	//debug = "Length=" + wifiCommand.length();

	//Debug();

	//debug = wifiCommand;
	//Debug();

	SendLine();

	GetMySerial(10);
	// OK

	CheckWiFiReceived();

	delay(1000);

	// Check the status

	wifiCommand = "AT+CIPSTATUS";
	SendLine();

	GetMySerial(5);
	// *S##~STATUS:5#~#~OK#~
	// busy p...

	if (wifiReceived.indexOf("OK") >= 0 && wifiReceived.indexOf("STATUS") >= 0)
	{
		DebugOK();
	}
	else
	{
		DebugReceived();
		return -5;
	}

	delay(1000);

	//

	// Check the IP address

	wifiCommand = "AT+CIFSR";
	SendLine();

	GetMySerial(5);
	// ##~192.168.1.70#~#~OK#~

	if (wifiReceived.indexOf("0.0.0.0") >= 0)
	{
		debug = ": Error, NO IP [[";
		debug += wifiReceived;
		debug += "]]";
		Debug();
		return -6;
	}
	else if (wifiReceived.indexOf("OK") >= 0)
	{
		//wifiMyIp = wifiReceived;
		DebugOK();
	}
	else
	{
		DebugReceived();
		return -7;
	}

	//

	return 0;
} // TryConnect


void DisConnect()
{
	debug = "D";
	Debug();

	// disconnect from any access point

	delay(500);

	wifiCommand = "AT+CWQAP";
	SendLine();

	GetMySerial(2);

	CheckWiFiReceived();

	// join fake AP

	delay(500);

	wifiCommand = "AT+CWJAP=\",\"";
	SendLine();

	GetMySerial(2);

	// disconnect again

	delay(500);

	wifiCommand = "AT+CWQAP";
	SendLine();

	GetMySerial(2);

	CheckWiFiReceived();

	//wifiMyIp = "DIS";
}


void Reset()
{
	debug = "R";
	Debug();

	// restart the module

	delay(100);

	wifiCommand = "AT+RST";
	SendLine();

	delay(4000);

	GetMySerial(3);

	CheckWiFiReceived();

} // Reset ======================================================================================================================================================================================================


void Close()
{
	debug = "C";
	Debug();

	// restart the module

	delay(100);

	wifiCommand = "AT+CIPCLOSE";
	SendLine();

	GetMySerial(3);

	CheckWiFiReceived();

	delay(1000);

} // Close ======================================================================================================================================================================================================


void BuildStartCommand()
{
	debug = "BSC";
	Debug();

	wifiCommand = "";
	wifiCommand += "AT+CIPSTART=\"TCP\",\"";
	wifiCommand += IP_ADDRESS;
	wifiCommand += "\",";
	wifiCommand += IP_PORT;
} // BuildStartCommand ======================================================================================================================================================================================================


void BuildWifiData()
{
	debug = "BWD";
	Debug();

	//debug = wifiParams;
	//Debug();

	DEBUG_SERIAL.println(wifiParams);

	wifiData = "";
	wifiData.concat("GET ");
	wifiData.concat(wifiParams);
	wifiData.concat(" HTTP/1.1\r\n");
	wifiData.concat("Host: 1.1.1.1\r\n");
	wifiData.concat("User-Agent: Arduino\r\n");
	wifiData.concat("\r\n");

	wifiLength = wifiData.length();

	//debug = wifiData;
	//Debug();
	DEBUG_SERIAL.println(wifiData);


} // BuildWifiData ======================================================================================================================================================================================================


void BuildSendCommand()
{
	wifiCommand = "";
	wifiCommand.concat("AT+CIPSEND=");
	wifiCommand.concat(wifiLength);
} // BuildSendCommand ======================================================================================================================================================================================================


int SendData(int sensor)
{
	debug = "SD-TD" + sensor;
	Debug();

	// Open Connection

	BuildStartCommand();

	SendLine();

	GetMySerial(20);

	CheckWiFiReceived();

	delay(50);

	/*
	String data = "GET ";
	data += "/json.htm?type=command&param=udevice";
	data += "&idx=";
	data += idx[sensor];
	data += "&nvalue=0";
	//data += temperatures[sensor];
	data += "&svalue=";
	data += temperatures[sensor];
	data += " HTTP/1.1\r\n";
	data += "Host: 1.1.1.1\r\n";
	data += "User-Agent: Arduino\r\n";
	data += "\r\n";

	int length = data.length();

	// Send data

	command = "AT+CIPSEND=";
	command += length;
	SendLine(command);

	received = GetMySerial(5);

	DebugReceived(received);

	delay(500);

	SendChars(length, data);
	*/


	wifiParams = "/json.htm?type=command&param=udevice";
	wifiParams += "&idx=";
	wifiParams += idx[sensor];
	wifiParams += "&nvalue=0";
	//data += temperatures[sensor];
	wifiParams += "&svalue=";
	wifiParams += temperatures[sensor];
	//wifiParams += " ";

	BuildWifiData();
	BuildSendCommand();

	SendLine();
	// [*2##~#~ERROR#~Unlink#~]

	GetMySerial(2);

	DebugReceived();

	delay(200);

	SendChars();

	//for (int letter = 0; letter < length; letter++)
	//{
	//	String ignore = GetMySerial(0, "");
	//	delay(50);
	//	WIFI_SERIAL.print(data.charAt(letter));
	//}

	GetMySerial(5);
	// [*7##~link is not#~]

	if (wifiReceived.indexOf("busy p") >= 0)
	{
		DebugError();
		return -23;
	}
	else
	{
		DebugReceived();
	}

	DebugReceived();

	for (int i = 0; i < 10; i++)
	{
		GetMySerial(0);
		if (wifiReceived != NULL && wifiReceived.length() > 0)
			DebugReceived();
	}

	return 0;
} // SendData()


int SendHeartbeat()
{
	debug = "SHB";
	Debug();

	RunningTime();

	//text.replace(" ", "%20");

	BuildStartCommand();

	SendLine();

	GetMySerial(3);

	CheckWiFiReceived();

	delay(50);

	//runningText.replace(" ", "%20");

	wifiParams = "/json.htm?type=command&param=udevice";
	wifiParams += "&idx=";
	wifiParams += FEEDBACK_IDX;
	wifiParams += "&nvalue=0";
	wifiParams += "&svalue=";
	wifiParams += runningText;
	//wifiParams += " ";

	// Send data

	BuildWifiData();
	BuildSendCommand();

	SendLine();

	GetMySerial(2);

	DebugReceived();

	delay(500);

	SendChars();

	GetMySerial(3);

	DebugReceived();

	for (int i = 0; i < 10; i++)
	{
		GetMySerial(0);
		if (wifiReceived != NULL && wifiReceived.length() > 0)
			DebugReceived();
	}

	return 0;
} // SendHeartbeat


int SendToCloud()
{
	/*
	http:// data.sparkfun.com/input/g6M8ZX98J0sKnv34LxM3?private_key=qz9mGb2mD0Cg6Mr8kRAr&exhangertemp=28.89&fridgetemp=0.59&roomtemp=22.27
	http:// data.sparkfun.com/input/XGv3QRmbxbTEArmdnzp3?private_key=1J0mp91xPxCl0x9apYqz&attempt=28.05&errors=14.07&outdoors=4.27&run=8.36&shed=12.34
	data.sparkfun.com
	/input/g6M8ZX98J0sKnv34LxM3?private_key=qz9mGb2mD0Cg6Mr8kRAr&exhangertemp=28.89&fridgetemp=0.59&roomtemp=22.27
	*/

	debug = "STC";
	Debug();

	wifiCommand = "";
	wifiCommand += "AT+CIPSTART=\"TCP\",\"";
	wifiCommand += "data.sparkfun.com";
	wifiCommand += "\",";
	wifiCommand += "80";

	SendLine();

	GetMySerial(12);
	// when connected ok.  *0##~#~OK#~Linked#~
	// when connecting to IP that doesnt exist, [*2##~]
	// [*0##~DNS Fail#~

	CheckWiFiReceived();

	delay(50);

	//RunningTime();

	//runningText.replace(" ", "%20");

	wifiParams = "/input/";
	wifiParams += SPARKFUN_STREAM;
	wifiParams += "?private_key=";
	wifiParams += SPARKFUN_KEY;
	wifiParams += "&attempt=";
	//wifiParams += "1";
	wifiParams += counter;
	wifiParams += "&errors=";
	wifiParams += errors;
	//wifiParams += runningText;
	//wifiParams += "&exchanger=";
	//wifiParams += "27.";
	//wifiParams += ledCounter;
	//wifiParams += temperatures[EXCHANGER];
	wifiParams += "&outdoors=";
	//wifiParams += "&fridge=";
	//wifiParams += measureSeconds;
	//wifiParams += "12.00";
	wifiParams += temperatures[SHED_OUTDOOR];
	//wifiParams += "&room=";
	//wifiParams += sendSeconds;
	//wifiParams += ".47";
	//wifiParams += temperatures[ROOM];
	wifiParams += "&run=";

	if (++timeCounter > 4)
		timeCounter = 0;

	if (timeCounter == 0) // total seconds
	{
		nowTime = now();
		unsigned long seconds = nowTime;
		wifiParams += seconds;
		wifiParams += "a";
	}
	else if (timeCounter == 1) // formatted date, usually fails
	{
		if (runningText.length() < 1)
			wifiParams += "ERR";
		else
			wifiParams += runningText;

		wifiParams += "b";
	}
	else if (timeCounter == 2) // formatted date 2, usually OK
	{
		RunningTime2();
		if (runningText.length() < 1)
			wifiParams += "ERR";
		else
			wifiParams += runningText;

		wifiParams += "c";
	}
	else if (timeCounter == 3) // formatted date 3, usually OK
	{
		RunningTime3();
		if (runningText.length() < 1)
			wifiParams += "ERR";
		else
			wifiParams += runningText;

		wifiParams += "d";
	}
	else if (timeCounter == 4) // elasped seconds
	{
		nowTime = now();
		time_t running = nowTime - startedTime;
		unsigned long seconds = running;
		wifiParams += seconds;
		wifiParams += "e";
	}
	else // 
	{
		wifiParams += "q";
	}

	wifiParams += "&shed=";
	//wifiParams += "15.00";
	wifiParams += temperatures[SHED_INDOOR];

	// Send data

	BuildWifiData();
	BuildSendCommand();

	SendLine();

	GetMySerial(2);

	DebugReceived();

	delay(100);

	SendChars();

	GetMySerial(10);
	// *#~SEND OK#~#~+IPD,433:HTTP/1.1 200 OK#~Access-Cont

	DebugReceived();

	for (int i = 0; i < 10; i++)
	{
		GetMySerial(0);
		if (wifiReceived != NULL && wifiReceived.length() > 0)
			DebugReceived();
	}

	return 0;
} // SendToCloud


/*String SendRequestDevices()
{
counter++;

String received = "";
String data = "GET ";
//data += URL_GET;

int length = data.length() + 2;

// Prepare Connection
//mySerial.print("AT+CIPMUX=0");

// Open Connection
String command;

BuildStartCommand();

SendLine(startCommand);

delay(2000);

received = GetMySerial(1);

if (received.indexOf("OK") >= 0)
{
DebugOK(received);
}
else
{
DebugError(received);
return "";
}

// Send data

command = "AT+CIPSEND=";
command += length;
SendLine(command);

delay(1000);

received = GetMySerial(1);

DebugReceived(received);

SendLine(data);
//mySerial.print(data);

delay(9000);

received = GetMySerial(1);

if (received.indexOf("OK") >= 0)
{
DebugOK();
}
else
{
DebugReceived(received);
}

delay(30);

// Close Connection
//Send("AT+CIPCLOSE");

//delay(6000);

//received = GetMySerial();
//Debug("RECEIVED: [[" + received + "]]");

return received;
} // SendRequestDevices
*/


/*void ProcessReceivedDevices(String data)
{
for (int i = 0; i < numSensors; i++)
{
int j = i + 1;

String locate = "TEMP";
locate += id;
locate += j;
names[i] = locate;
Serial.println("Locate Device " + locate);

int start = data.indexOf(locate);

idx[i] = 1;
}
}
*/


/*
void GetSerial55555()
{
bool didReceive = false;
serialReceived= "";

while (Serial.available())
{
didReceive = true;

int value = Serial.read();
letter = value;
serialReceived.concat(letter);

//Serial.print("Received ");
//Serial.println(value);
//Serial.print(", [");
//Serial.print(blueToothVal);
//Serial.println("]");

delay(10);
}

if (!didReceive)
{
//received = NULL;
}

}
*/


/* void GetMySerial(int timeoutSeconds)
{
wifiReceived = "";
int count = 0;

//

if (timeoutSeconds <= 0)
{
while (WIFI_SERIAL.available())
{
count++;
int temp = WIFI_SERIAL.read();
uint8_t value = temp;

if (value == 10)
{
wifiReceived.concat("~");
}
else if (value == 13)
{
wifiReceived.concat("#");
}
else if (value < 32)
{
wifiReceived.concat("{");
wifiReceived.concat(value);
wifiReceived.concat("}");
}
else
{
char letter = value;
wifiReceived.concat(letter);
}
} // while
}
else
{
unsigned long timeout = timeoutSeconds;
timeout *= 1000L;
unsigned long endMillis = millis() + timeout;

while (millis() < endMillis)
{

if (WIFI_SERIAL.available())
{
count++;

int r1 = WIFI_SERIAL.read();
uint8_t value = r1;

if (value == 10)
{
wifiReceived.concat("~");
}
else if (value == 13)
{
wifiReceived.concat("#");
}
else if (value < 32)
{
//wifiReceived.concat("{");
//wifiReceived.concat(value);
//wifiReceived.concat("}");
}
else
{
char letter = value;
wifiReceived.concat(letter);
//DEBUG_SERIAL.print(letter);
}

} // while

} // while millis
}

if (count == 0)
{
if (timeoutSeconds <= 0)
{
wifiReceived = "";
}
else
{
wifiReceived = "** NONE **";
}
}

}
*/



void GetMySerial(int timeoutSeconds)
{
	wifiReceived = "";
	int count = 0;

	for (int i = 0; i < MAX_BUFFER; i++)
	{
		buffer[i] = 0;
	}

	//

	if (timeoutSeconds <= 0)
	{
		while (WIFI_SERIAL.available() && count < MAX_BUFFER)
		{
			int temp = WIFI_SERIAL.read();
			uint8_t value = temp;
			buffer[count++] = value;
		} // while
	}
	else
	{
		unsigned long timeout = timeoutSeconds;
		timeout *= 1000L;
		unsigned long endMillis = millis() + timeout;

		while (millis() < endMillis && count < MAX_BUFFER)
		{
			if (WIFI_SERIAL.available())
			{
				int temp = WIFI_SERIAL.read();
				uint8_t value = temp;
				buffer[count++] = value;
			} // while
			else
			{
				delay(5);
			}
		} // while millis
	}

	if (count == 0)
	{
		if (timeoutSeconds <= 0)
		{
			wifiReceived = "";
		}
		else
		{
			wifiReceived = "** NONE **";
		}
	}
	//else if (count >= MAX_BUFFER)
	//{
	//		data = "** TOO BIG ** "; // +count;
	//}
	else if (count > 0)
	{
		wifiReceived = "*";

		for (int i = 0; i < count && i < MAX_BUFFER; i++)
		{
			uint8_t value = buffer[i];

			if (value == 10)
			{
				wifiReceived.concat("~");
			}
			else if (value == 13)
			{
				wifiReceived.concat("#");
			}
			else if (value < 32)
			{
				wifiReceived.concat("{");
				wifiReceived.concat(value);
				wifiReceived.concat("}");
			}
			else
			{
				char letter = value;
				wifiReceived.concat(letter);
			}
		}
	}


}



int CheckWiFiReceived()
{
	if (wifiReceived.indexOf("OK") >= 0)
	{
		DebugOK();
		return 0;
	}
	else
	{
		DebugReceived();
		return -21;
	}
} // CheckWiFiReceived --------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void RunningTime()
{
	runningText = "ERR";

	// build string

	runningText = "";

	int months = month() - 1;
	if (months < 10)
		runningText += "0";
	runningText += months;

	runningText += ",";

	int days = day() - 1;
	if (days < 10)
		runningText += "0";
	runningText += days;

	runningText += ",";

	int hours = hour();
	if (hours < 10)
		runningText += "0";
	runningText += hours;

	runningText += ":";

	int minutes = minute();
	if (minutes < 10)
		runningText += "0";
	runningText += minutes;

	runningText += ":";

	int seconds = second();
	if (seconds < 10)
		runningText += "0";
	runningText += seconds;

} // RunningTime --------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void RunningTime2()
{
	runningText = "ERR";

	// build string

	runningText = "";

	int months = month() - 1;
	runningText += months;

	runningText += ",";

	int days = day() - 1;
	runningText += days;

	runningText += ",";

	int hours = hour();
	runningText += hours;

	runningText += ":";

	int minutes = minute();
	runningText += minutes;

	runningText += ":";

	int seconds = second();
	runningText += seconds;

} // RunningTime2 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------


void RunningTime3()
{
	runningText = "ERR";

	nowTime = now();
	time_t running = nowTime - startedTime;
	unsigned long seconds = running;
	unsigned long div;

	// days
	div = SECS_PER_DAY;
	unsigned long days = seconds / div;

	if (days > 0UL)
	{
		seconds -= (days*div);
	}

	// hours
	div = SECS_PER_HOUR;
	unsigned long hours = seconds / div;

	if (hours > 0UL)
	{
		seconds -= (hours*div);
	}

	// minutes
	div = SECS_PER_MIN;
	unsigned long minutes = seconds / div;

	if (minutes > 0UL)
	{
		seconds -= (minutes*div);
	}

	// build string

	runningText = "";

	//if (days < 10UL)
	//runningText += "0";
	runningText += days;

	runningText += ",";

	//if (hours < 10UL)
	//		runningText += "0";
	runningText += hours;

	runningText += ":";

	// if (minutes < 10UL)
	//	runningText += "0";
	runningText += minutes;

	runningText += ":";

	//if (seconds < 10UL)
	//runningText += "0";
	runningText += seconds;

} // RunningTime3 --------------------------------------------------------------------------------------------------------------------------------------------------------------------------


