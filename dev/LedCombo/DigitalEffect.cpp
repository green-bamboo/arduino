// 
// 
// 

#include "DigitalEffect.h"

#define DIGITAL_MIN	  0
#define DIGITAL_MAX	 10


DigitalEffect::DigitalEffect() : Adafruit_NeoPixel(1, 1, 1)
{

} // ****************************************************************************************************************************************************************************************************************************


DigitalEffect::DigitalEffect(uint16_t n, uint8_t p, uint8_t t) : Adafruit_NeoPixel(n, p, t)
{
	program = 1;
	previous = 0;
	step = 0;
	pixel = 0;
	pixelStep = 1;

} // ****************************************************************************************************************************************************************************************************************************

DigitalEffect::~DigitalEffect()
{

} // ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::Init()
{
	begin();
	show();
} // ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::Process()
{

	currentMillis = millis();

	if (program == 1)
	{
		Serial.println("DigitalProgram1");
		DigitalProgram1();
	}
	else if (program == 2)
	{
		DigitalProgram2();
	}
	else if (program == 3)
	{
		DigitalProgram3();
	}
	else if (program == 4)
	{
		DigitalProgram4();
	}
	else if (program == 5)
	{
		DigitalProgram5();
	}
	else if (program == 6)
	{
		DigitalProgram6();
	}
	else if (program == 7)
	{
		DigitalProgram7();
	}
	//else if (program == 8)
	//{
	//	DigitalProgram8();
	//}
	//else if (program == 9)
	//{
	//	DigitalProgram9();
	//}
	else
	{
		program = 1;
		step = 0;
	}
} // Process ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::NextProgram()
{
	step = 0;
	pixel = 0;
	program++;
} // NextProgram ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::SetBar(int pixelIndex, uint32_t colour)
{
	int index;

	index = pixelIndex - 2;
	if (index >= 0 && index < numPixels())
		setPixelColor(index, colour);

	index = pixelIndex - 1;
	if (index >= 0 && index < numPixels())
		setPixelColor(index, colour);

	index = pixelIndex;
	if (index >= 0 && index < numPixels())
		setPixelColor(index, colour);

	index = pixelIndex + 1;
	if (index >= 0 && index < numPixels())
		setPixelColor(index, colour);

	index = pixelIndex + 2;
	if (index >= 0 && index < numPixels())
		setPixelColor(index, colour);

} // SetBar ****************************************************************************************************************************************************************************************************************************



void DigitalEffect::SetAll(int red, int green, int blue)
{
	uint32_t colour = Color(red, green, blue);

	for (uint16_t i = 0; i < numPixels(); i++)
		setPixelColor(i, colour);
} // SetAll ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::ShowAll(int red, int green, int blue)
{
	SetAll(red, green, blue);
	show();
} // ShowAll ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::Clash(int index1, int index2, uint32_t colour)
{
	for (int a = index1 - 2; a < index1 + 2; a++)
	{
		for (int b = index2 - 2; b < index2 + 2; b++)
		{
			if (a == b && a >= 0 && a < numPixels())
				setPixelColor(a, colour);
		}

	}

} // Clash ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::DigitalProgram1() // Single moving Red
{
	if (currentMillis - previous >= 10)
	{
		previous = currentMillis;

		step++;

		if (step < 0)
			step = 0;

		if (step > numPixels())
		{
			NextProgram();
		}
		else
		{
			uint32_t black = Color(0, 0, 0);

			for (uint16_t i = 0; i < numPixels(); i++)
				setPixelColor(i, black);

			uint32_t red = Color(10, 0, 0);
			setPixelColor(step - 1, red);

			show();
		}
	}

} // DigitalProgram1 ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::DigitalProgram2() // Increasing size blue
{
	if (currentMillis - previous >= 10)
	{
		previous = currentMillis;

		step++;

		if (step < 0)
			step = 0;

		if (step > numPixels())
		{
			NextProgram();
		}
		else
		{
			uint32_t black = Color(0, 0, 0);
			uint32_t blue = Color(0, 0, DIGITAL_MAX);

			for (uint16_t i = 0; i < numPixels(); i++)
			{
				if (i <= step - 1)
					setPixelColor(i, blue);
				else
					setPixelColor(i, black);
			}

			show();
		}
	}
} // DigitalProgram2 ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::DigitalProgram3() // Random colour on each pixel
{
	if (currentMillis - previous >= 300)
	{
		previous = currentMillis;

		step++;

		if (step > 10)
		{
			NextProgram();
		}
		else
		{

			for (uint16_t i = 0; i < numPixels(); i++)
			{
				uint32_t colour = Color(random(DIGITAL_MIN, DIGITAL_MAX), random(DIGITAL_MIN, DIGITAL_MAX), random(DIGITAL_MIN, DIGITAL_MAX));
				setPixelColor(i, colour);
			}

			show();
		}
	}
} // DigitalProgram3 ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::DigitalProgram4() // RGB shifting
{
	if (currentMillis - previous >= 200)
	{
		previous = currentMillis;

		step++;

		if (step > 40)
		{
			NextProgram();
		}
		else
		{
			uint32_t red = Color(DIGITAL_MAX, 0, 0);
			uint32_t green = Color(0, DIGITAL_MAX, 0);
			uint32_t blue = Color(0, 0, DIGITAL_MAX);

			int colour;

			pixel++;

			if (pixel <= 1)
			{
				colour = 0;
			}
			else if (pixel == 2)
			{
				colour = 1;
			}
			else if (pixel <= 3)
			{
				colour = 2;
				pixel = 0;
			}

			for (uint16_t i = 0; i < numPixels(); i++)
			{
				colour++;

				if (colour == 1)
				{
					setPixelColor(i, red);
				}
				else if (colour == 2)
				{
					setPixelColor(i, green);
				}
				else if (colour == 3)
				{
					setPixelColor(i, blue);
					colour = 0;
				}
			}

			show();
		}
	}
} // DigitalProgram4 ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::DigitalProgram5() // Off for 5 seconds
{
	if (step == 0) // first set all pixels off
	{
		previous = currentMillis;
		step++;

		uint32_t black = Color(0, 0, 0);

		for (uint16_t i = 0; i < numPixels(); i++)
		{
			setPixelColor(i, black);
		}

		show();
	}

	if (currentMillis - previous >= 1000) // wait for 5 seconds before moving to next program
	{
		NextProgram();
	}

} // DigitalProgram5 ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::DigitalProgram6() // red cyclone
{
	if (step == 0) // first set all pixels off
	{
		previous = currentMillis;
		step++;
		pixel = 0;

		uint32_t black = Color(0, 0, 0);

		for (uint16_t i = 0; i < numPixels(); i++)
		{
			setPixelColor(i, black);
		}

		show();
	}
	else if (currentMillis - previous >= 10)
	{
		previous = currentMillis;

		//digitalStep++;

		if (step > 5)
		{
			NextProgram();
		}
		else
		{
			uint32_t black = Color(0, 0, 0);
			uint32_t red1 = Color(10, 0, 0);
			uint32_t red2 = Color(40, 0, 0);
			uint32_t red3 = Color(90, 0, 0);
			uint32_t red4 = Color(130, 0, 0);

			for (uint16_t i = 0; i < numPixels(); i++)
			{
				setPixelColor(i, black);
			}

			int index;

			// 
			// 43211234

			index = pixel - 3;
			if (index >= 0 & index < numPixels())
				setPixelColor(index, red1);

			index = pixel - 2;
			if (index >= 0 & index < numPixels())
				setPixelColor(index, red2);

			index = pixel - 1;
			if (index >= 0 & index < numPixels())
				setPixelColor(index, red3);

			index = pixel;
			if (index >= 0 & index < numPixels())
				setPixelColor(index, red4);

			index = pixel + 1;
			if (index >= 0 & index < numPixels())
				setPixelColor(index, red4);

			index = pixel + 2;
			if (index >= 0 & index < numPixels())
				setPixelColor(index, red3);

			index = pixel + 3;
			if (index >= 0 & index < numPixels())
				setPixelColor(index, red2);

			index = pixel + 4;
			if (index >= 0 & index < numPixels())
				setPixelColor(index, red1);

			show();

			pixel += pixelStep;

			if (pixelStep == 1)
			{
				if (pixel >= numPixels())
				{
					step++;
					pixelStep = -1;
				}
			}
			else
			{
				if (pixel <= 0)
				{
					step++;
					pixelStep = 1;
				}
			}

		}
	}

} // DigitalProgram6 ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::DigitalProgram7() // Criss Cross
{
	if (step == 0) // first set all pixels off
	{
		previous = currentMillis;
		step++;
		pixel1 = 0;
		pixel2 = numPixels();

		uint32_t black = Color(0, 0, 0);

		for (uint16_t i = 0; i < numPixels(); i++)
		{
			setPixelColor(i, black);
		}

		show();
	}
	else if (currentMillis - previous >= 20)
	{
		previous = currentMillis;

		//digitalStep++;

		if (step > 5)
		{
			NextProgram();
		}
		else
		{
			uint32_t black = Color(0, 0, 0);
			uint32_t blue = Color(0, 0, DIGITAL_MAX);
			uint32_t green = Color(0, DIGITAL_MAX, 0);
			uint32_t white = Color(DIGITAL_MAX, DIGITAL_MAX, DIGITAL_MAX);

			for (uint16_t i = 0; i < numPixels(); i++)
			{
				setPixelColor(i, black);
			}

			pixel1 = pixel;
			pixel2 = numPixels() - pixel;

			SetBar(pixel1, white);
			SetBar(pixel2, blue);
			Clash(pixel1, pixel2, green);

			show();

			pixel += pixelStep;

			if (pixelStep == 1)
			{
				if (pixel >= numPixels())
				{
					step++;
					pixelStep = -1;
				}
			}
			else
			{
				if (pixel <= 0)
				{
					step++;
					pixelStep = 1;
				}
			}

		}
	}
} // DigitalProgram7 ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::DigitalProgram8() // Fade up white
{
	if (currentMillis - previous >= 50)
	{
		previous = currentMillis;

		step++;

		if (step < 0)
			step = 0;

		if (step >= DIGITAL_MAX)
		{
			NextProgram();
		}
		else
		{
			uint32_t colour = Color(step, step, step);

			for (uint16_t i = 0; i < numPixels(); i++)
			{
				setPixelColor(i, colour);
			}

			show();
		}
	}
} // DigitalProgram8 ****************************************************************************************************************************************************************************************************************************


void DigitalEffect::DigitalProgram9() // Fade down white
{
	if (step == 0)
	{
		step = DIGITAL_MAX;
	}
	else if (currentMillis - previous >= 50)
	{
		previous = currentMillis;

		step--;

		if (step <= 1)
		{
			NextProgram();
		}
		else
		{
			uint32_t colour = Color(step, step, step);

			for (uint16_t i = 0; i < numPixels(); i++)
			{
				setPixelColor(i, colour);
			}

			show();
		}
	}
} // DigitalProgram9 ****************************************************************************************************************************************************************************************************************************



// ****************************************************************************************************************************************************************************************************************************
// ****************************************************************************************************************************************************************************************************************************
