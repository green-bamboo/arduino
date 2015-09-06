#include "U8glib.h"

#define VERSION				"Test_OLED_1, v1.1"

#define PIN_LED				13
#define SENSORS				2
int idx[] = { 11, 12 };
float temperatures[] = { 21.3, 21.5 };
String names[] = { "TEMP11_UtilityRoom", "TEMP12_BackFridge" };

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);	// Display which does not send AC, I2C uses pins A4 and A5
// Nano
// GND  BLACK   GND
// VCC  RED     5V
// SCL  GREEN   A5
// SDA  YELLOW  A4

unsigned long currentMillis = 0;
unsigned long previousLED = 0;
int ledCounter = 0;
int seconds = 99;
int index = 0;
String line = "Ready";
String line1 = "";
String line2 = "";
String line3 = "";
String line4 = "";
String line5 = VERSION;

void setup(void)
{
	Serial.begin(9600);
	Serial.println(VERSION);

	pinMode(PIN_LED, OUTPUT);

	previousLED = 0;

	u8g.firstPage();
	do {
		u8g_prepare();

		u8g.setFont(u8g_font_courB08);
		u8g.setPrintPos(5, 12);
		u8g.print("Ready");

		u8g.setFont(u8g_font_5x8);

		u8g.setPrintPos(5, 62);
		u8g.print(VERSION);
	} while (u8g.nextPage());

}


void loop(void)
{
	currentMillis = millis();

	if (currentMillis - previousLED > 100)
	{
		previousLED = currentMillis;

		if (++ledCounter > 10)
		{
			ledCounter = 0;
			seconds++;
			//Serial.println("Tick");
		}

		if (ledCounter == 1 || ledCounter == 2 || ledCounter == 4 || ledCounter == 5) // // normal operation, double flash green
			digitalWrite(PIN_LED, HIGH);
		else
			digitalWrite(PIN_LED, LOW);

	}

	//

	bool pickup = false;
	if (Serial.available())
	{
		//line1 = line;
		//line = Serial.readString();

		pickup = true;
		int value = Serial.read();
		if (value < 32)
		{
		line += "{";
		line += value;
		line += "}";
		}
		else
		{
			char letter = value;
			line += letter;
		}

		/*
		char letter = Serial.read();

		//if (value < 32)
		//{
		//line += "{";
		//line += value;
		//line += "}";
		//}
		if (letter == '\n')
		{
			line1 = line;
			line = "";
		}
		else if (letter == '\r')
		{
			line1 = line;
			line = "";
		}
		else
		{
			//pickup = true;

			//char letter = value;
			line += letter;

			//if (line.length() > 16)
			//{
			//	line1 = line;
			//	line = "";
			//}

		}*/
	}

	//if (pickup)
	//{
	//	line1 = line;
	//	line = "";
	//}

	//

	if (pickup)
	{
		u8g.firstPage();
		do {
			u8g_prepare();

			u8g.setFont(u8g_font_courB08);

			//u8g.setFont(u8g_font_courB07);
			//u8g.setf

			//u8g.drawStr(5, 5, "TEMP12_BackFridge"); // names[index].c_str
			u8g.setPrintPos(5, 12);
			u8g.print(line);

			//String t1 = String(temperatures[index]);
			//u8g.print(temperatures[index]);


			//u8g.setPrintPos(5, 15);
			//u8g.print("1234568901234567890");

			u8g.setFont(u8g_font_5x8);

			u8g.setPrintPos(5, 22);
			u8g.print(line1);

			u8g.setPrintPos(5, 32);
			u8g.print(line2);

			u8g.setPrintPos(5, 42);
			u8g.print(line3);

			u8g.setPrintPos(5, 52);
			u8g.print(line4);

			u8g.setPrintPos(5, 62);
			u8g.print(line5);

			if (line1 != line2)
			{
				line5 = line4;
				line4 = line3;
				line3 = line2;
				line2 = line1;
			}

		} while (u8g.nextPage());
	}

	//

	/*
	// picture loop
	u8g.firstPage();
	do {
	draw();
	} while (u8g.nextPage());

	// increase the state
	draw_state++;
	if (draw_state >= 9 * 8)
	draw_state = 0;

	// rebuild the picture after some delay
	//delay(150);
	*/

}






void u8g_prepare(void) {
	//u8g.setFont(u8g_font_6x10);
	u8g.setFontRefHeightExtendedText();
	u8g.setDefaultForegroundColor();
	u8g.setFontPosTop();
}

void u8g_box_frame(uint8_t a) {
	u8g.drawStr(0, 0, "drawBox");
	u8g.drawBox(5, 10, 20, 10);
	u8g.drawBox(10 + a, 15, 30, 7);
	u8g.drawStr(0, 30, "drawFrame");
	u8g.drawFrame(5, 10 + 30, 20, 10);
	u8g.drawFrame(10 + a, 15 + 30, 30, 7);
}

void u8g_disc_circle(uint8_t a) {
	u8g.drawStr(0, 0, "drawDisc");
	u8g.drawDisc(10, 18, 9);
	u8g.drawDisc(24 + a, 16, 7);
	u8g.drawStr(0, 30, "drawCircle");
	u8g.drawCircle(10, 18 + 30, 9);
	u8g.drawCircle(24 + a, 16 + 30, 7);
}

void u8g_r_frame(uint8_t a) {
	u8g.drawStr(0, 0, "drawRFrame/Box");
	u8g.drawRFrame(5, 10, 40, 30, a + 1);
	u8g.drawRBox(50, 10, 25, 40, a + 1);
}

void u8g_string(uint8_t a) {
	u8g.drawStr(30 + a, 31, " 0");
	u8g.drawStr90(30, 31 + a, " 90");
	u8g.drawStr180(30 - a, 31, " 180");
	u8g.drawStr270(30, 31 - a, " 270");
}

void u8g_line(uint8_t a) {
	u8g.drawStr(0, 0, "drawLine");
	u8g.drawLine(7 + a, 10, 40, 55);
	u8g.drawLine(7 + a * 2, 10, 60, 55);
	u8g.drawLine(7 + a * 3, 10, 80, 55);
	u8g.drawLine(7 + a * 4, 10, 100, 55);
}

void u8g_triangle(uint8_t a) {
	uint16_t offset = a;
	u8g.drawStr(0, 0, "drawTriangle");
	u8g.drawTriangle(14, 7, 45, 30, 10, 40);
	u8g.drawTriangle(14 + offset, 7 - offset, 45 + offset, 30 - offset, 57 + offset, 10 - offset);
	u8g.drawTriangle(57 + offset * 2, 10, 45 + offset * 2, 30, 86 + offset * 2, 53);
	u8g.drawTriangle(10 + offset, 40 + offset, 45 + offset, 30 + offset, 86 + offset, 53 + offset);
}

void u8g_ascii_1() {
	char s[2] = " ";
	uint8_t x, y;
	u8g.drawStr(0, 0, "ASCII page 1");
	for (y = 0; y < 6; y++) {
		for (x = 0; x < 16; x++) {
			s[0] = y * 16 + x + 32;
			u8g.drawStr(x * 7, y * 10 + 10, s);
		}
	}
}

void u8g_ascii_2() {
	char s[2] = " ";
	uint8_t x, y;
	u8g.drawStr(0, 0, "ASCII page 2");
	for (y = 0; y < 6; y++) {
		for (x = 0; x < 16; x++) {
			s[0] = y * 16 + x + 160;
			u8g.drawStr(x * 7, y * 10 + 10, s);
		}
	}
}

void u8g_extra_page(uint8_t a)
{
	if (u8g.getMode() == U8G_MODE_HICOLOR || u8g.getMode() == U8G_MODE_R3G3B2) {
		/* draw background (area is 128x128) */
		u8g_uint_t r, g, b;
		b = a << 5;
		for (g = 0; g < 64; g++)
		{
			for (r = 0; r < 64; r++)
			{
				u8g.setRGB(r << 2, g << 2, b);
				u8g.drawPixel(g, r);
			}
		}
		u8g.setRGB(255, 255, 255);
		u8g.drawStr(66, 0, "Color Page");
	}
	else if (u8g.getMode() == U8G_MODE_GRAY2BIT)
	{
		u8g.drawStr(66, 0, "Gray Level");
		u8g.setColorIndex(1);
		u8g.drawBox(0, 4, 64, 32);
		u8g.drawBox(70, 20, 4, 12);
		u8g.setColorIndex(2);
		u8g.drawBox(0 + 1 * a, 4 + 1 * a, 64 - 2 * a, 32 - 2 * a);
		u8g.drawBox(74, 20, 4, 12);
		u8g.setColorIndex(3);
		u8g.drawBox(0 + 2 * a, 4 + 2 * a, 64 - 4 * a, 32 - 4 * a);
		u8g.drawBox(78, 20, 4, 12);
	}
	else
	{
		u8g.drawStr(0, 12, "setScale2x2");
		u8g.setScale2x2();
		u8g.drawStr(0, 6 + a, "setScale2x2");
		u8g.undoScale();
	}
}


uint8_t draw_state = 0;

void draw(void) {
	u8g_prepare();
	switch (draw_state >> 3) {
	case 0: u8g_box_frame(draw_state & 7); break;
	case 1: u8g_disc_circle(draw_state & 7); break;
	case 2: u8g_r_frame(draw_state & 7); break;
	case 3: u8g_string(draw_state & 7); break;
	case 4: u8g_line(draw_state & 7); break;
	case 5: u8g_triangle(draw_state & 7); break;
	case 6: u8g_ascii_1(); break;
	case 7: u8g_ascii_2(); break;
	case 8: u8g_extra_page(draw_state & 7); break;
	}
}






/*
uint8_t offset = 0;

void draw(void) {
// graphic commands to redraw the complete screen should be placed here
u8g.setFont(u8g_font_unifont);
u8g.drawStr(0 + 0, 20 + 0, "Hello!");
u8g.drawStr(0 + 2, 20 + 16, "Hello!");

u8g.drawBox(0, 0, 3, 3);
u8g.drawBox(u8g.getWidth() - 6, 0, 6, 6);
u8g.drawBox(u8g.getWidth() - 9, u8g.getHeight() - 9, 9, 9);
u8g.drawBox(0, u8g.getHeight() - 12, 12, 12);
}



void rotate(void) {
static  uint8_t dir = 0;
static  unsigned long next_rotation = 0;

if (next_rotation < millis())
{
switch (dir) {
case 0: u8g.undoRotation(); break;
case 1: u8g.setRot90(); break;
case 2: u8g.setRot180(); break;
case 3: u8g.setRot270(); offset = (offset + 1) & 0x0f; break;
}

dir++;
dir &= 3;
next_rotation = millis();
next_rotation += 1000;
}
}

void loop(void) {
// screen rotation
rotate();

// picture loop
u8g.firstPage();
do {
draw();
} while (u8g.nextPage());

// rebuild the picture after some delay
delay(100);
}
*/