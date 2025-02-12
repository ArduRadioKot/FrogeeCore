#ifndef DISPLAY_H
#define DISPLAY_H
#include "Arduino.h"
#include <GyverOLED.h>
#include <Wire.h>
#include "AuxFunc.h"
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
        String param_1 = Aux::trim_space(params.substring(0, params.indexOf('"')));
        String param_2 = params.substring(params.indexOf('"'));
        int dot1 = param_1.indexOf(',');
        int dot2 = param_1.indexOf(dot1, ',');
        int x = param_1.substring(0, dot1).toInt();
        int y = param_1.substring(dot1 + 1).toInt();
        oled.setCursorXY(x, y);
        oled.print(param_2.substring(param_2.indexOf('"') + 1, param_2.indexOf(param_2.indexOf('"'), param_2.indexOf('"')) - 1));
      }

};

Display display; 

#endif