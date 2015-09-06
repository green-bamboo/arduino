#include <time.h>

int counter = 0;
time_t started;

void setup()
{
	delay(1000);

	Serial.begin(9600);
	Serial.println("Test Time, v1.0");

	delay(1000);

	setTime(0, 0, 0, 1, 1, 2001);

	delay(1000);

	started = now();

	Out1(started);



}

void loop()
{
	counter++;

	delay(2500);
	Serial.println();
	Serial.println();
	Serial.println(counter);

	time_t n1 = now();
	Out1(n1);

	time_t n2 = n1 - started;
	Out1(n2);

}


void Out1(time_t t)
{
	delay(100);
	Serial.print("Total=");
	Serial.println(t);
	delay(100);
	Serial.print("Year=");
	Serial.println(year(t));
	delay(100);
	Serial.print("Month=");
	Serial.println(month(t));
	delay(100);
	Serial.print("Day=");
	Serial.println(day(t));
	delay(100);
	Serial.print("Hour=");
	Serial.println(hour(t));
	delay(100);
	Serial.print("Min=");
	Serial.println(minute(t));
	delay(100);
	Serial.print("Sec=");
	Serial.println(second(t));
	delay(100);
}