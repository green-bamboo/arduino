/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega 2560 or Mega ADK, Platform=avr, Package=arduino
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define __AVR_ATmega2560__
#define ARDUINO 106
#define ARDUINO_MAIN
#define __AVR__
#define __avr__
#define F_CPU 16000000L
#define __cplusplus
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__

#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

//
//
void SetColour(Adafruit_NeoPixel *strip, int red, int green, int blue);
void SetColour(Adafruit_NeoPixel *strip, uint32_t colour);
void SetStrips(int red, int green, int blue);
void SetStripsBrightness(int level);
void SetHouse(int red, int green, int blue);
void ShowStrips();
void ShowHouse();
void SimpleAct();
void Act101();
void Act102();
void Act103();
void Act104();
void Act105();
void Act109();
void ManualControl();
void StartScene();
void NextScene();
bool NewScene();
void Action4();
void Action5();
void Action6();
void Action7();
void Action8();
void Action9();
void FadeUpWhite();
void FadeDownWhite();
void FadeOutAll();
void Pause2Seconds();
void Nets(int net1, int net2, int net3, int net4, int net5, int net6);
void SetBar(Adafruit_NeoPixel *strip, int pixelIndex, uint32_t colour);
void SetHouseBar(int pixelIndex, int red, int green, int blue);
void SetPixel(int pixelIndex, int red, int green, int blue);
void SetPixel(int pixelIndex, int red, int green, int blue, bool show);
void SetHousePixel(int pixelIndex, int red, int green, int blue);
void SetHousePixel(int pixelIndex, int red, int green, int blue, bool show);
void SetHousePixel(int pixelIndex, uint32_t colour, bool show);
void FillUpWhite();
void FillUpRed();
void Disco();
void Divider();
void Divider(int r, int g, int b);
void RainbowHousePixel();
void RainbowHouseAll();
void RainbowHouseTwinkle();
void RainbowAll();
void HouseClockWise();
void HouseAntiClockWise();
void Cylon1();
void Sparkle();
void Twinkle();
void Snow();
void StarBurst();
void StarBurst(Adafruit_NeoPixel *strip, int c, int seq);
void Star1();
void Star2();
void Star3();
void DomeBar();
void DomeTrail();
void Mash();
void Glimmer();
void Dull();
void TreeSpiral();
void TreeVert();
void TreeHorz();
void TreeSparkle();
void TreeMash();
void TreeDomeTrail();
void TreeTrain();
void TreeBerries();
void TreeBounce();
void TreeRocket();
void TreeRainbowAll();
void TreeRainbowPixel();
void PorchSparkle();
void PorchFader();
void PorchRainbowPixel();
void PorchRainbowAll();
void PorchStarTurn();
void ProcessPorchArray(int pixels[][10]);
void ProcessTreeArray(int pixels[][10]);
void TreeFader();
void StripTwinkle(Adafruit_NeoPixel *strip, int max, uint32_t lowred, uint32_t lowgreen, uint32_t lowblue, uint32_t highred, uint32_t highgreen, uint32_t highblue);
void Wheel(byte WheelPos, int *red, int *green, int *blue);
uint32_t Wheel(Adafruit_NeoPixel *strip, byte WheelPos);

#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\arduino\sketches\Lights_Digital\Lights_Digital.ino"
#endif
