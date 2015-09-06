
#define PIN1	8
#define PIN2	8

String received = "";

void setup()
{
	Serial.begin(57600);

	delay(100);

	Serial.println();
	Serial.println();
	Serial.println("Serial_Control_Relay, v1.0");

	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(13, OUTPUT);

	digitalWrite(8, HIGH);
	digitalWrite(9, HIGH);
	digitalWrite(10, HIGH);
	digitalWrite(11, HIGH);
	digitalWrite(13, LOW);
}

void loop()
{
	if (Serial.available())
	{
		received = "";
		while (Serial.available())
		{
			int value = Serial.read();
			char letter = value;
			received += letter;
		}
	}

	//digitalWrite(13, HIGH);

	//Serial.print("  Received=");
	//Serial.print(s);
	//Serial.println();

	if (received.length() > 2)
	{

		int relay = s[0] - 48;
		int action = s[1] - 48;

		if (relay >= 1 && relay <= 4 && action >= 0 && action <= 1)
		{
			int onoff = HIGH;
			if (action == 1)
				onoff = LOW;

			digitalWrite(7 + relay, onoff);

			//Serial.print("Relay=");
			//Serial.print(relay);
			//Serial.print("  Action=");
			//S/erial.print(action);
			//Serial.println();
		}


		/*while (Serial.available())
		{
		Serial.read();

		}*/
	}

	//digitalWrite(13, LOW);
	//}

}
