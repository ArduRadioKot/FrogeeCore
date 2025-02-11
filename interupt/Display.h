#ifndef DISPLAY_H
#define DISPLAY_H
#include "Arduino.h"
#include <GyverOLED.h>
#include <Wire.h>

class Display {

  public:
      void DisplayBegin(){
        GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
        oled.init();
        oled.clear();
      }
      void DisplayTest(){
        oled.clear();
        oled.print(0,10,"TesT,.?-_@#$123456789");	// write something to the internal memory
      }
      void DisplayClear(){
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
        u8g2.clear();
        u8g2.sendBuffer();
      }

};

Display display;

#endif