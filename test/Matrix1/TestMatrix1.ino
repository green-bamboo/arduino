//We always have to include the library
#include "LedControl.h"

/*
Now we need a LedControl to work with.
***** These pin numbers will probably not work with your hardware *****
pin D11 MOSI - is connected to the DataIn
pin D13  SCK - is connected to the CLK
pin  8 - 10 is connected to LOAD
We have only a single MAX72XX.

We use pins 12,11 and 10 on the Arduino for the SPI interface
* Pin 12 is connected to the DATA IN-pin of the first MAX7221
* Pin 11 is connected to the CLK-pin of the first MAX7221
* Pin 10 is connected to the LOAD(/CS)-pin of the first MAX7221
* There will only be a single MAX7221 attached to the arduino

*/
//LedControl lc = LedControl(11, 13, 8, 1);
LedControl lc = LedControl(12, 11, 10, 1);

/* we always wait a bit between updates of the display */
unsigned long delaytime1 = 1000;
unsigned long delaytime2 = 200;


void setup() {
	Serial.begin(9600);	// Debugging only
	Serial.println("Test_Matrix1, v1.0");

	/*
	The MAX72XX is in power-saving mode on startup,
	we have to do a wakeup call
	*/
	lc.shutdown(0, false);
	/* Set the brightness to a medium values */
	lc.setIntensity(0, 3);
	/* and clear the display */
	lc.clearDisplay(0);
}

/*
This method will display the characters for the
word "Arduino" one after the other on the matrix.
(you need at least 5x7 leds to see the whole chars)
*/
void writeArduinoOnMatrix() {
	Serial.println("writeArduino");

	/* here is the data for the characters */
	byte a[5] = { B01111110, B10001000, B10001000, B10001000, B01111110 };
	byte r[5] = { B00111110, B00010000, B00100000, B00100000, B00010000 };
	byte d[5] = { B00011100, B00100010, B00100010, B00010010, B11111110 };
	byte u[5] = { B00111100, B00000010, B00000010, B00000100, B00111110 };
	byte i[5] = { B00000000, B00100010, B10111110, B00000010, B00000000 };
	byte n[5] = { B00111110, B00010000, B00100000, B00100000, B00011110 };
	byte o[5] = { B00011100, B00100010, B00100010, B00100010, B00011100 };

	/* now display them one by one with a small delay */
	lc.setRow(0, 0, a[0]);
	lc.setRow(0, 1, a[1]);
	lc.setRow(0, 2, a[2]);
	lc.setRow(0, 3, a[3]);
	lc.setRow(0, 4, a[4]);
	delay(delaytime1);
	lc.setRow(0, 0, r[0]);
	lc.setRow(0, 1, r[1]);
	lc.setRow(0, 2, r[2]);
	lc.setRow(0, 3, r[3]);
	lc.setRow(0, 4, r[4]);
	delay(delaytime1);
	lc.setRow(0, 0, d[0]);
	lc.setRow(0, 1, d[1]);
	lc.setRow(0, 2, d[2]);
	lc.setRow(0, 3, d[3]);
	lc.setRow(0, 4, d[4]);
	delay(delaytime1);
	lc.setRow(0, 0, u[0]);
	lc.setRow(0, 1, u[1]);
	lc.setRow(0, 2, u[2]);
	lc.setRow(0, 3, u[3]);
	lc.setRow(0, 4, u[4]);
	delay(delaytime1);
	lc.setRow(0, 0, i[0]);
	lc.setRow(0, 1, i[1]);
	lc.setRow(0, 2, i[2]);
	lc.setRow(0, 3, i[3]);
	lc.setRow(0, 4, i[4]);
	delay(delaytime1);
	lc.setRow(0, 0, n[0]);
	lc.setRow(0, 1, n[1]);
	lc.setRow(0, 2, n[2]);
	lc.setRow(0, 3, n[3]);
	lc.setRow(0, 4, n[4]);
	delay(delaytime1);
	lc.setRow(0, 0, o[0]);
	lc.setRow(0, 1, o[1]);
	lc.setRow(0, 2, o[2]);
	lc.setRow(0, 3, o[3]);
	lc.setRow(0, 4, o[4]);
	delay(delaytime1);
	lc.setRow(0, 0, 0);
	lc.setRow(0, 1, 0);
	lc.setRow(0, 2, 0);
	lc.setRow(0, 3, 0);
	lc.setRow(0, 4, 0);
	delay(delaytime1);
}

/*
This function lights up a some Leds in a row.
The pattern will be repeated on every row.
The pattern will blink along with the row-number.
row number 4 (index==3) will blink 4 times etc.
*/
void rows() {
	Serial.println("Rows");

	for (int row = 0; row < 8; row++) {
		delay(delaytime2);
		lc.setRow(0, row, B10100000);
		delay(delaytime2);
		lc.setRow(0, row, (byte)0);
		for (int i = 0; i < row; i++) {
			delay(delaytime2);
			lc.setRow(0, row, B10100000);
			delay(delaytime2);
			lc.setRow(0, row, (byte)0);
		}
	}
}

/*
This function lights up a some Leds in a column.
The pattern will be repeated on every column.
The pattern will blink along with the column-number.
column number 4 (index==3) will blink 4 times etc.
*/
void columns() {
	Serial.println("Columns");

	for (int col = 0; col < 8; col++) {
		delay(delaytime2);
		lc.setColumn(0, col, B10100000);
		delay(delaytime2);
		lc.setColumn(0, col, (byte)0);
		for (int i = 0; i < col; i++) {
			delay(delaytime2);
			lc.setColumn(0, col, B10100000);
			delay(delaytime2);
			lc.setColumn(0, col, (byte)0);
		}
	}
}

/*
This function will light up every Led on the matrix.
The led will blink along with the row-number.
row number 4 (index==3) will blink 4 times etc.
*/
void single() {
	Serial.println("Single");

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			delay(delaytime2);
			lc.setLed(0, row, col, true);
			delay(delaytime2);
			for (int i = 0; i < col; i++) {
				lc.setLed(0, row, col, false);
				delay(delaytime2);
				lc.setLed(0, row, col, true);
				delay(delaytime2);
			}
		}
	}
}


void Test1()
{
	Serial.println("Test1");

	for (int row = 0; row < 8; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			lc.setLed(0, row, col, true);
			delay(50);
		}
	}

	delay(delaytime1);

	lc.clearDisplay(0);
	for (int row = 0; row < 8; row++)
	{
		lc.setRow(0, row, 255);
		delay(90);
	}

	delay(delaytime1);

	lc.clearDisplay(0);
	for (int col = 0; col< 8; col++)
	{
		lc.setColumn(0, col, 255);
		delay(90);
	}
}



void loop() {
	writeArduinoOnMatrix();

	lc.clearDisplay(0);
	Test1();

	lc.clearDisplay(0);
	rows();

	lc.clearDisplay(0);
	columns();

	lc.clearDisplay(0);
	single();


}