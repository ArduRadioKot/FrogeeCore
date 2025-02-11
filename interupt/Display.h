#ifndef DISPLAY_H
#define DISPLAY_H
#include "Arduino.h"
#include <GyverOLED.h>
#include <Wire.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
class Display {

  public:
      void DisplayBegin(){
        oled.init();
        oled.clear();
      }
      void DisplayTest(){
        oled.clear();
        oled.setCursorXY(0, 0);
        oled.print("TesT");	// write something to the internal memory
      }
      void DisplayClear(){
        oled.clear();
      }
      void DisplayPrint(const String &params){
        
      }

};

Display display;

#endif