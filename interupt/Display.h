#ifndef DISPLAY_H
#define DISPLAY_H
#include "Arduino.h"
#include <GyverOLED.h>
#include <Wire.h>
#include "AuxFunc.h"
#include "moduleParse.h"
class Display {

  public:
      static GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
      void DisplayBegin(){
        oled.init();
        oled.clear();
        oled.update();
      }
      void DisplayTest(){
        oled.clear();
        oled.setCursorXY(0, 0);
        oled.print("TesT");	// write something to the internal memory
        oled.update();
      }
      void DisplayClear(){
        oled.clear();
        oled.update();
      }
      void DisplayPrint(const String &params){
        oled.autoPrintln(false);
        String param_1 = Aux::trim_space(params.substring(0, params.indexOf('"')));
        String param_2 = params.substring(params.indexOf('"'));
        param_1 = param_1.substring(0, param_1.length() - 1);
        int dot1 = param_1.indexOf(',');
        int dot2 = param_1.indexOf(',', dot1);
        int x = (mdp.check(param_1.substring(0, dot1))).toInt();
        int y = (mdp.check(param_1.substring(dot1 + 1))).toInt();
        oled.setCursorXY(x, y);
        oled.print(param_2.substring(param_2.indexOf('"') + 1, param_2.indexOf(param_2.indexOf('"'), param_2.indexOf('"')) - 1));
        oled.update();
      }
      void DisplayPixel(const String &params){
        String param = Aux::trim_space(params);
        int dot1 = param.indexOf(',');
        int x = param.substring(0, dot1).toInt();
        int y = param.substring(dot1 + 1).toInt();
        oled.dot(x, y);
        oled.sendBuffer();
        oled.update();
      }
      void DisplayLine(const String &params){
        String param = Aux::trim_space(params);
        int dot1 = param.indexOf(',');
        int dot2 = param.indexOf(',', dot1 + 1);
        int dot3 = param.indexOf(',', dot2 + 1);
        int x1 = param.substring(0, dot1).toInt();
        int y1 = param.substring(dot1 + 1, dot2).toInt();
        int x2 = param.substring(dot2 + 1, dot3).toInt();
        int y2 = param.substring(dot3 + 1).toInt();
        oled.line(x1, y1, x2, y2);
        oled.update();
      }
      void DisplayCircle(const String &params){
        String param = Aux::trim_space(params);
        int dot1 = param.indexOf(',');
        int dot2 = param.indexOf(',', dot1 + 1);
        int x = param.substring(0, dot1).toInt();
        int y = param.substring(dot1 + 1, dot2).toInt();
        int r = param.substring(dot2 + 1).toInt();
        oled.circle(x, y, r, OLED_STROKE);        // окружность с центром в (x,y, с радиусом)
        oled.update();
      }
      void DisplayPrintLN(String params){
        oled.autoPrintln(true);   // автоматически переносить текст
        if((params.indexOf('"') == 0) && (params.lastIndexOf('"') != -1)){
          params = params.substring(1, params.indexOf('"', params.indexOf('"') + 1));
          oled.print((params));
          oled.update();
        }
        else{
          
        }
      }
      void DisplaySetScale(const String &params){
        if(Aux::isNumber(params)){
          oled.setScale(params.toInt());
          oled.update();
        }
      }
};

Display display; 

#endif