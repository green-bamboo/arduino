#include <SoftwareSerial.h>
#include <Time.h>

#define VERSION			"Test_Radio_ESP8266, Terminal, v2.0, 13/02/2015"

// Vendor:www.ai-thinker.com Version:0.9.2.4
// 0018000902-AI03

#define WIFI_SERIAL		wifiSerial
#define DEBUG_SERIAL	Serial

#define PIN_SOFTRX		2
#define PIN_SOFTTX		3
#define PIN_RADIO		5
#define PIN_LED			13

#define MAX_BUFFER		100
#define STARTUP_TIME	5
#define WAIT_TIME		30

#define IDX_1			12
#define IDX_2			11
#define IDX_3			36

// home
#define WAP_NAME			"VM934292-2G"
#define WAP_PWD				"qufunkkn"
#define IP_ADDRESS			"192.168.0.205" // elara
#define IP_PORT				"8080"

// work, work WiFi
//#define WAP_NAME			"Eastleigh WiFi"
//#define WAP_PWD				"11111222223333444455556666"
//#define IP_ADDRESS			"10.22.38.22" // my laptop
//#define IP_PORT				"6262"

//#define WAP_NAME		"BTHomeHub2-FTH5"
//#define WAP_PWD			"4b65ad7b28"
//#define IP_ADDRESS		"192.168.1.223" // my laptop
//#define IP_PORT			"80"
//#define URL				"/json.htm?type=command&param=udevice&idx=10&nvalue=0&svalue="

//#define WAP_NAME		"VM934292-5G"
//#define WAP_PWD			"qufunkkn"
//#define IP_ADDRESS		"192.168.0.205"
//#define IP_PORT			"2319"
//#define URL				"http://192.168.0.205:2319/json.htm?type=command&param=udevice&idx=10&nvalue=0&svalue="


/*

I used this artical : http://www.instructables.com/id/Using-the-ESP8266-module/

AT commands PDF here : http://woodsgood.ca/projects/wp-content/uploads/ESP8266ATCommandsSet.pdf

This is a simple "relay" sketch that sends/received serial from the PC/USB to the Wifi module
All the commands end in a hash "#", this is to tell the sketch to send a CR LF.

You cannot connect the PC via USB and connect the module to the Arduino serial pins at the same time.
They use the sames pins, 0 and 1, RX and TX.

The sketch uses the USB connection to set data to the PC.

It uses the SoftwareSerial library to create another serial connection to the WiFi module, using D2 and D3.

Wiring, most of the internet docs show the pin numbers on the top of the board, the side with the chips.
NOT the side with the pins.

I left the wires on the module.

Red and Orange both need to go to 5v on the Arduino.
White goes to GND on the Arduino.
Green is module TX, and goes to Arduino D2 (software serial RX)
Yellow is module RX, and to to Arduino D3 (software serial TX)

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

#############################################################################################################################################

http://<username:password@>domoticz-ip<:port>/json.htm?api-call

http://localhost:2319/json.htm?type=command_param=switchlight_idx={0}_switchcmd={1}

/json.htm?type=command&param=switchlight&idx=2&switchcmd=On

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

SoftwareSerial wifiSerial(PIN_SOFTRX, PIN_SOFTTX); // RX, TX

int mode = 99;
char blueToothVal;           //value sent over via bluetooth
char lastValue;              //stores last state of device (on/off)
unsigned long currentMillis = 0;
unsigned long previousLED = 0;
int ledCounter = 0;
int seconds;
int counter = 1;
int error;
int waitTime = STARTUP_TIME;
bool joined = false;
bool opened = false;
time_t poweron;
time_t started;
time_t finished;
String received = "";
String command = "";
String data = "";

// #############################################################################################################################################

// Simple Terminal
void setup()
{
	pinMode(PIN_LED, OUTPUT);
	digitalWrite(PIN_LED, LOW);

	pinMode(PIN_RADIO, OUTPUT);
	//	if (powerSaving)
	digitalWrite(PIN_RADIO, LOW);
	//else
	//digitalWrite(PIN_RADIO, HIGH);

#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.begin(9600);
	DEBUG_SERIAL.println(VERSION);
	DEBUG_SERIAL.println(WAP_NAME);
	DEBUG_SERIAL.println(IP_ADDRESS);
#endif

	WIFI_SERIAL.begin(9600); // 9600, 19200, 34800,57600

	setTime(0, 0, 0, 1, 1, 1970);

	poweron = now();
}


void loop()
{
	if (mode == 1) // radio on
	{
		started = now();
		RadioOn();
		delay(2000);
		mode = 2;
	}
	else if (mode == 2) // simple AT
	{
		command = "AT";
		Send();
		GetMySerial(1);

		if (received.indexOf("OK") >= 0)
		{
			DebugOK();
			delay(1000);
			mode = 3;
		}
		else
		{
			Debug(" !! FAILED !! ");
			DebugError(received);
			mode = 99;
		}

	}
	else if (mode == 3) // Check joined to AP
	{
		command = "AT+CWJAP?";
		Send();

		GetMySerial(1);

		if (received.indexOf("ERROR") >= 0)
		{
			Debug("Error, not joined to AP");
			Debug(" !! FAILED !! ");
			DebugError(received);
			mode = 99;
			return;
		}

		DebugReceived(received);
		delay(400);
		mode = 4;

	}
	else if (mode == 4) // Check we have IP address
	{
		command = "AT+CIFSR";
		Send();

		GetMySerial(1);

		if (received == "" || received.indexOf("0.0.0.0") >= 0)
		{
			Debug("Error, no IP address");
			DebugError(received);
			mode = 99;
			return;
		}

		DebugReceived(received);
		delay(400);
		mode = 5;
	}
	else if (mode == 5) // 1 open connection
	{
		Open2();
		delay(100);
		mode = 6;
	}
	else if (mode == 6) // 1
	{
		SendData2();
		//SendData(IDX_1);
		delay(100);
		mode = 7;
	}
	else if (mode == 7) // 1
	{
		Reset();
		delay(6000);
		mode = 14; // 8 or 14
	}
	else if (mode == 8) // 2 open connection
	{
		Open();
		delay(100);
		mode = 9;
	}
	else if (mode == 9) // 2
	{
		SendData(IDX_2);
		delay(100);
		mode = 10;
	}
	else if (mode == 10) // 2
	{
		Reset();
		delay(6000);
		mode = 11;
	}
	else if (mode == 11) // 3 open connection
	{
		Open();
		delay(100);
		mode = 12;
	}
	else if (mode == 12) // 3
	{
		SendData(IDX_3);
		delay(100);
		mode = 13;
	}
	else if (mode == 13) // 3
	{
		Reset();
		delay(6000);
		mode = 14;
	}
	else if (mode == 14)
	{
		RadioOff();
		mode = 99;

		finished = now();

		String temp;

		time_t running = finished - started;
		//temp = "Running=" + running;
		//Debug( temp);

		//Debug("hour=" + hour(running));
		//Debug("minute=" + minute(running));
		//Debug("second=" + second(running));

	}

	//

	String typed = GetSerial();
	if (typed.length() > 0)
	{
		String upper = typed;
		upper.trim();
		upper.toUpperCase();

		if (upper == "RESET")
		{
			Reset();
		}
		else if (upper == "RADIOON")
		{
			Debug("RadioOn");
			RadioOn();
		}
		else if (upper == "RADIOOFF")
		{
			Debug("RadioOff");
			RadioOff();
		}
		else if (upper == "GO")
		{
			mode = 1;
		}
		else if (upper == "INFO")
		{
			Debug(WAP_NAME);
			Debug(WAP_PWD);
			Debug(IP_ADDRESS);
			Debug(IP_PORT);
		}
		else if (upper == "STATUS")
		{
			command = "AT+CIPSTATUS";
			Send();
			GetMySerial(1);
			DebugReceived(received);
		}
		else if (upper == "COLD")
		{
			counter = 9;
			String temp = String("Counter=");
			temp.concat(counter);
			Debug(temp);
		}
		else if (upper == "HOT")
		{
			counter = 19;
			String temp = String("Counter=");
			temp.concat(counter);
			Debug(temp);
		}
		else if (upper == "COUNT")
		{
			counter = 1;
			String temp = String("Counter=");
			temp.concat(counter);
			Debug(temp);
		}
		else if (upper == "DEC")
		{
			counter--;
			String temp = String("Counter=");
			temp.concat(counter);
			Debug(temp);
		}
		else if (upper == "INC")
		{
			counter++;
			String temp = String("Counter=");
			temp.concat(counter);
			Debug(temp);
		}
		else if (upper == "VERSION")
		{
			command = "AT+GMR";
			Send();
		}
		else if (upper == "IP")
		{
			command = "AT+CIFSR";
			Send();
		}
		else if (upper == "LIST")
		{
			command = "AT+CWLAP";
			Send();
		}
		else if (upper == "ALARM")
		{
			Alarm();
		}
		else if (upper == "JOIN")
		{
			Join();
		}
		else if (upper == "JOINCOOP")
		{
			Join2();
		}
		else if (upper == "JOINED")
		{
			command = "AT+CWJAP?";
			Send();
		}
		else if (upper == "LEAVE")
		{
			Leave();
		}
		else if (upper == "OPEN")
		{
			Open2();
		}
		else if (upper == "CLOSE")
		{
			Close();
		}
		else if (upper == "SEND")
		{
			//SendData(IDX_1);
			SendData2();
		}
		else
		{
			WIFI_SERIAL.println(typed);
		}
	}


	GetMySerial(0);
	if (received.length() > 0)
	{
		DebugReceived(received);
	}

}



void Send()
{
#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.print("Sending : ");
#endif

	for (int pos = 0; pos < command.length(); pos++)
	{
		char letter = command.charAt(pos);
		int ascii = letter;

#ifdef DEBUG_SERIAL
		if (ascii == 10)
			DEBUG_SERIAL.print("~");
		else if (ascii == 13)
			DEBUG_SERIAL.print("#");
		else
			DEBUG_SERIAL.print(letter);
#endif

		WIFI_SERIAL.print(letter);
		delay(10);
	}

#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.println();
#endif

	delay(90);

	WIFI_SERIAL.println();

}


void DebugOK()
{
#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.println("Received [OK]");
#endif
} // DebugOK ===============================================================================================================================================================================


void DebugOK(String s)
{
#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.print("Received OK [");

	for (int pos = 0; pos < s.length(); pos++)
	{
		char letter = s.charAt(pos);
		int ascii = letter;
		if (ascii == 10)
			DEBUG_SERIAL.print("~");
		else if (ascii == 13)
			DEBUG_SERIAL.print("#");
		else
			DEBUG_SERIAL.print(letter);

		//delay(2);
	}

	DEBUG_SERIAL.println("]");

#endif
} // DebugOK ===============================================================================================================================================================================


void DebugError(String s)
{
#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.print("Received Error [");

	for (int pos = 0; pos < s.length(); pos++)
	{
		char letter = s.charAt(pos);
		int ascii = letter;
		if (ascii == 10)
			DEBUG_SERIAL.print("~");
		else if (ascii == 13)
			DEBUG_SERIAL.print("#");
		else
			DEBUG_SERIAL.print(letter);

		//delay(2);
	}

	DEBUG_SERIAL.println("]");

#endif
} // DebugError ===============================================================================================================================================================================


void DebugReceived(String s)
{
#ifdef DEBUG_SERIAL
	DEBUG_SERIAL.print("Received [[");

	for (int pos = 0; pos < s.length(); pos++)
	{
		char letter = s.charAt(pos);
		int ascii = letter;
		if (ascii == 10)
			DEBUG_SERIAL.print("~");
		else if (ascii == 13)
			DEBUG_SERIAL.print("#");
		else
			DEBUG_SERIAL.print(letter);

		//delay(2);
	}

	DEBUG_SERIAL.println("]]");

#endif
} // DebugReceived ===============================================================================================================================================================================


void Debug(String s)
{
#ifdef DEBUG_SERIAL_xx
	DEBUG_SERIAL.println(s);
#endif

#ifdef DEBUG_SERIAL
	for (int pos = 0; pos < s.length(); pos++)
	{
		char letter = s.charAt(pos);
		int ascii = letter;
		if (ascii == 10)
			DEBUG_SERIAL.print("~");
		else if (ascii == 13)
			DEBUG_SERIAL.print("#");
		else
			DEBUG_SERIAL.print(letter);

		//delay(2);
	}

	DEBUG_SERIAL.println();

#endif
}


void SendData2()
{
	// http :// www.earthtools.org/timezone-1.1/0/0

	//

	data = "GET /timezone/0/0 HTTP/1.1\r\nHost: 1.1.1.1\r\nUser-Agent: Arduino\r\n\r\n";

	int length = data.length();

	Debug("Data");
	Debug(data);

	// Send data

	delay(200);

	command = "AT+CIPSEND=";
	command.concat(length);
	Send();

	//delay(100);

	GetMySerial(1);

	DebugReceived(received);

	delay(100);

	//Send(data);
	//delay(50);
	for (int letter = 0; letter < length; letter++)
	{
		GetMySerial(0);
		//Debug("[[" + received + "]]");

		delay(10);
		//Serial.print(data.charAt(letter));
		WIFI_SERIAL.print(data.charAt(letter));
	}

	GetMySerial(10);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugReceived(received);
		//return -21;
	}

	//


} // SendData2 **********************************************************************************************************************************


void DisConnect()
{
	// disconnect from any access point

	command = "AT+CWQAP";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -2;
	}

	// Check IP address has been releases

	command = "AT+CIFSR";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -2;
	}

	// Check the status

	command = "AT+CIPSTATUS";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -2;
	}

}


void RadioOn()
{
	Debug("RadioOn");

	//	if (powerSaving)
	digitalWrite(PIN_RADIO, HIGH);
} // RadioOn **********************************************************************************************************************************


void RadioOff()
{
	Debug("RadioOff");

	//if (powerSaving)
	digitalWrite(PIN_RADIO, LOW);
} // RadioOff **********************************************************************************************************************************


void Alarm()
{
	counter++;

	Debug("Alarm");

	//

	RadioOn();

	// wait for radio to "warm up"
	delay(3000);

	// Clear buffer

	//Debug("Clear buffer");
	//WIFI_SERIAL.flush();

	GetMySerial(0);
	GetMySerial(0);
	GetMySerial(0);
	GetMySerial(0);

	// AT hello

	delay(100);

	command = "AT";
	Send();

	GetMySerial(1);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		return;
	}

	// wait for connection and IP address

	//delay(2000);

	// Check joined to AP

	command = "AT+CWJAP?";
	Send();

	GetMySerial(1);

	if (received.indexOf("ERROR") >= 0)
	{
		Debug("Error, not joined to AP");
		return;
	}
	else if (received == "")
	{
		Debug("Error, no data");
		return;
	}
	else
	{
		DebugReceived(received);
	}

	// Check IP

	command = "AT+CIFSR";
	Send();

	GetMySerial(1);

	if (received.indexOf("0.0.0.0") >= 0)
	{
		Debug("Error, no IP address");
		return;
	}
	else if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -21;
	}

	// Open Connection

	command = "AT+CIPSTART=\"TCP\",\"";
	command += IP_ADDRESS;
	command += "\",";
	command += IP_PORT;

	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -21;
	}

	//

	String temp = String(counter);

	data = "GET ";
	data += "/json.htm?type=command&param=udevice&idx=10&svalue=";
	data += temp;
	data += " HTTP/1.1\r\n";
	data += "\r\n";

	int length = data.length();

	// Send data

	command = "AT+CIPSEND=";
	command += length;
	Send();

	//delay(60);

	GetMySerial(2);

	if (received == NULL)
		Debug("NULL 1");
	else
		DebugReceived(received);

	//delay(60);

	//Debug("Sending: [[" + data + "]]");

	delay(60);

	for (int letter = 0; letter < length; letter++)
	{
		while (WIFI_SERIAL.available())
			int temp = WIFI_SERIAL.read();

		//received = GetMySerial(0, "");
		//Debug("[[" + received + "]]");

		delay(20);
		//Serial.print(data.charAt(letter));
		WIFI_SERIAL.print(data.charAt(letter));
	}

	GetMySerial(4);

	//Debug("Checking");
	//Debug("length=" + received.length());

	if (received == NULL)
	{
		Debug("NULL 2");
	}
	else
	{
		DebugReceived(received);
		//return -21;
	}

	//

	command = "AT+RST";
	Send();

	delay(3000);

	GetMySerial(5);

	//

	Debug("RadioOff");

	RadioOff();

} // Alarm **********************************************************************************************************************************


void Reset()
{
	joined = false;
	opened = false;

	// reset the module

	command = "AT+RST";
	Send();

	GetMySerial(5);

	//Debug("Checking");

	if (received == NULL || !received || received == "")
	{
		Debug("received == NULL");
	}
	else
	{
		//Debug("length=" + received.length());

		if (received.indexOf("OK") >= 0)
		{
			DebugOK(received);
		}
		else
		{
			// DebugError(received);
		}
	}

} // Reset **********************************************************************************************************************************


void Leave()
{
	seconds = 0;
	joined = false;
	opened = false;

	// disconnect from any access point

	command = "AT+CWQAP";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
	}

	// join fake AP

	command = "AT+CWJAP=";
	command += "\"";
	command += ",";
	command += "\"";

	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
	}

	// disconnect from any access point

	command = "AT+CWQAP";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
	}

} // Leave **********************************************************************************************************************************


int Join()
{
	int errors = 0;
	seconds = 0;
	joined = false;

	// check for basic comms to module

	command = "AT";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -1;
	}

	// disconnect from any access point

	command = "AT+CWQAP";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -2;
	}

	// put into stand alone mode

	command = "AT+CWMODE=1";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else if (received.indexOf("no change") >= 0)
	{
		Debug("RECEIVED: No Change");
	}
	else
	{
		DebugError(received);
		//return -2;
	}

	// put into multi mode

	//Send("AT+CIPMUX=1");

	//delay(1000);

	//received = GetMySerial();

	//if (received.indexOf("OK") >= 0)
	//{
	//	Debug("RECEIVED: OK [[" + received + "]]");
	//}
	//else if (received.indexOf("no change") >= 0)
	//{
	//	Debug("RECEIVED: No Change");
	//}
	//else
	//{
	//	Debug("RECEIVED: Error [[" + received + "]]");
	//	return -2;
	//}

	// Connect to Access Point

	command = "AT+CWJAP=";
	command += "\"";
	command += WAP_NAME;
	command += "\"";
	command += ",";
	command += "\"";
	command += WAP_PWD;
	command += "\"";

	Send(); // "AT+CWJAP=\"VM934292-5G\", \"qufunkkn\"";

	GetMySerial(10);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -3;
	}

	for (int i = 0; i < 100; i++)
	{
		GetMySerial(0);

		if (received.length() > 0)
			DebugReceived(received);
	}

	// Check the IP address

	command = "AT+CIFSR";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -4;
	}

	// Check the status

	command = "AT+CIPSTATUS";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -2;
	}

	//

	joined = true;

	Debug("Joined");

	return 0;
} // Join ==========================================================================================================================================================


int Join2()
{
	int errors = 0;
	seconds = 0;
	joined = false;

	// check for basic comms to module

	command = "AT";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -1;
	}

	// disconnect from any access point

	command = "AT+CWQAP";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -2;
	}

	// put into stand alone mode

	command = "AT+CWMODE=1";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else if (received.indexOf("no change") >= 0)
	{
		Debug("RECEIVED: No Change");
	}
	else
	{
		DebugError(received);
		//return -2;
	}

	// put into multi mode

	//Send("AT+CIPMUX=1");

	//delay(1000);

	//received = GetMySerial();

	//if (received.indexOf("OK") >= 0)
	//{
	//	Debug("RECEIVED: OK [[" + received + "]]");
	//}
	//else if (received.indexOf("no change") >= 0)
	//{
	//	Debug("RECEIVED: No Change");
	//}
	//else
	//{
	//	Debug("RECEIVED: Error [[" + received + "]]");
	//	return -2;
	//}

	// Connect to Access Point
	//#define WAP_NAME			"Eastleigh WiFi"
	//#define WAP_PWD				"11111222223333444455556666"


	command = "AT+CWJAP=";
	command += "\"";
	command += "Eastleigh WiFi";
	command += "\"";
	command += ",";
	command += "\"";
	command += "11111222223333444455556666";
	command += "\"";

	Send(); // "AT+CWJAP=\"VM934292-5G\", \"qufunkkn\"";

	GetMySerial(10);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -3;
	}

	for (int i = 0; i < 100; i++)
	{
		GetMySerial(0);

		if (received.length() > 0)
			DebugReceived(received);
	}


	// Check the IP address

	command = "AT+CIFSR";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -4;
	}

	// Check the status

	command = "AT+CIPSTATUS";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -2;
	}

	//

	joined = true;

	Debug("Joined");

	return 0;
} // Join2 ==========================================================================================================================================================


int Open()
{

	// Open Connection
	command = "AT+CIPSTART=\"TCP\",\"";
	command += IP_ADDRESS;
	command += "\",";
	command += IP_PORT;

	Send();

	GetMySerial(1);

	if (received.indexOf("OK") >= 0)
	{
		opened = true;
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -21;
	}

} // Open


int Open2()
{

	command = "AT+CIPSTART=\"TCP\",\"www.earthtools.org\",80";

	// www.earthtools.org

	Send();

	GetMySerial(1);

	if (received.indexOf("OK") >= 0)
	{
		opened = true;
		DebugOK(received);
	}
	else
	{
		DebugError(received);
		//return -21;
	}

} // Open2


int Close()
{
	String received = "";

	// Close Connection

	command = "AT+CIPCLOSE";
	Send();

	GetMySerial(2);

	if (received.indexOf("OK") >= 0)
	{
		DebugOK(received);
		opened = false;
	}
	else
	{
		DebugError(received);
		return -21;
	}


} // Close


int SendData(int idx)
{
	counter++;

	//if (!joined)
	//{
	//	Debug("Not joined to Access Point");
	//}

	//if (!opened)
	//{
	//	Debug("TCP connection Not Open");
	//}

	//Debug("Sending " + counter);

	GetMySerial(0);

	String idxString = String(idx);
	String counterString = String(counter);

	data = "GET ";
	data += "/json.htm?type=command&param=udevice&idx=";
	data += idxString;
	data += "&svalue=";
	data += counterString;
	//data += "/json.htm?type=devices&filter=temp&used=true&order=Name"; //  type = devices";
	data += " HTTP/1.1\r\n";
	//data += "Host: 1.1.1.1\r\n";
	//data += "User-Agent: Arduino\r\n";
	data += "\r\n";

	int length = data.length();

	// Send data

	command = "AT+CIPSEND=";
	command += length;
	Send();

	//delay(100);

	GetMySerial(2);

	DebugReceived(received);

	delay(50);

	Debug("Sending: [[" + data + "]]");

	delay(50);

	for (int letter = 0; letter < length; letter++)
	{
		GetMySerial(0);
		//Debug("[[" + received + "]]");

		delay(10);
		//Serial.print(data.charAt(letter));
		WIFI_SERIAL.print(data.charAt(letter));
	}

	//mySerial.flush();

	//Debug("Waiting");
	GetMySerial(1);

	DebugReceived(received);

	for (int i = 0; i < 100; i++)
	{
		GetMySerial(0);

		if (received.length()>0)
			DebugReceived(received);
	}

	//Debug("Checking");
	//Debug("length=" + received.length());

	//if (received.indexOf("OK") >= 0)
	//{
	//	DebugOK(received);
	//}
	//else
	//{
	//	DebugReceived(received);
	//	//return -21;
	//}


	//delay(3000);


	//delay(6000);

	//received = GetMySerial();
	//Debug("RECEIVED: [[" + received + "]]");

	return 0;
}


String GetSerial()
{
	bool didReceive = false;
	String r = "";

	while (Serial.available())
	{
		didReceive = true;

		int value = Serial.read();
		char letter = value;
		r.concat(letter);

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

	return r;
}


void GetMySerial(int timeoutSeconds)
{
	received = "";
	int count = 0;
	uint8_t buffer[MAX_BUFFER];

	for (int i = 0; i < MAX_BUFFER; i++)
	{
		buffer[i] = 0;
		//	received. .concat("|");
	}

	//received.reserve(MAX_BUFFER);

	if (timeoutSeconds <= 0)
	{
		while (WIFI_SERIAL.available() && count < MAX_BUFFER)
		{
			int temp = WIFI_SERIAL.read();
			//char letter = value;
			//Debug("Value=" + value);
			//Debug("Char=" + letter);
			//received.concat(letter);
			//count++;

			//received.setCharAt(count++, letter);
			uint8_t value = temp;
			buffer[count++] = value;
			//received.concat(letter);
		} // while
	}
	else
	{
		unsigned long timeout = timeoutSeconds;
		timeout *= 1000;
		unsigned long endMillis = millis() + timeout;

		while (millis() < endMillis && count < MAX_BUFFER)
		{
			while (WIFI_SERIAL.available() && count < MAX_BUFFER)
			{
				int temp = WIFI_SERIAL.read();
				uint8_t value = temp;

				//if (count < MAX_BUFFER)
				buffer[count++] = value;
			} // while


			//if (finished.length() > 0 && received.length() > 0)
			//{
			//	if (received.indexOf(finished) >= 0)
			//		break;
			//}
		} // while millis
	}

	if (count == 0)
	{
		if (timeoutSeconds <= 0)
		{
			received = "";
		}
		else
		{
			received = " ** NONE ** ";
		}
	}
	else if (count > 0)
	{
		received = "*";

		for (int i = 0; i < count && i < MAX_BUFFER; i++)
		{
			uint8_t value = buffer[i]; // received.charAt(i);
			// char letter = received.charAt(i); //value; // received.charAt(i);
			//char letter = value;
			//int value = letter;

			if (value == 10)
			{
				received.concat("~");
				//Serial.print("~");
			}
			else if (value == 13)
			{
				received.concat("#");
				//Serial.print("#");
			}
			else if (value < 32)
			{
				//Serial.print("|");
				received.concat("{");
				received.concat(value);
				received.concat("}");
			}
			else
			{
				//Serial.print(letter);
				char letter = value;
				received.concat(letter);
			}
		}
	}


	/*
	while (mySerial.available())
	{
	buffer[count++] = mySerial.read();
	}

	if (count <= 0)
	return "";

	if (count > 0)
	{
	received.concat("[");
	received.concat(count);
	received.concat("]   ");

	for (int pos = 0; pos < count; pos++)
	{
	int value = buffer[pos];
	if (value == 10)
	{
	received.concat("~");
	}
	else if (value == 13)
	{
	received.concat("#");
	}
	else if (value < 32)
	{
	received.concat("{");
	received.concat(value);
	received.concat("}");
	}
	else
	{
	char letter = value;
	received.concat(letter);
	}
	} // for count
	} // if count
	*/

}



