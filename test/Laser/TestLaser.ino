#define LED 13
#define LASER1 2
#define LASER2 3


void setup()
{
	pinMode(LED, OUTPUT);
	pinMode(LASER1, OUTPUT);
	pinMode(LASER2, OUTPUT);

	digitalWrite(LED, LOW);
	digitalWrite(LASER1, LOW);
	digitalWrite(LASER2, LOW);
}

void loop()
{
	delay(500);

	digitalWrite(LED, HIGH);
	digitalWrite(LASER1, LOW);
	digitalWrite(LASER2, LOW);

	delay(500);

	digitalWrite(LED, LOW);
	digitalWrite(LASER1, HIGH);
	digitalWrite(LASER2, LOW);

	delay(500);

	digitalWrite(LED, LOW);
	digitalWrite(LASER1, LOW);
	digitalWrite(LASER2, HIGH);

	delay(500);

	digitalWrite(LED, HIGH);
	digitalWrite(LASER1, HIGH);
	digitalWrite(LASER2, HIGH);

}
