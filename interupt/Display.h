#ifndef DISPLAY_H
#define DISPLAY_H

#include <GyverOLED.h>
#include "AuxFunc.h"
#include "moduleParse.h"

class Display {
public:
    static GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
    
    void DisplayBegin();
    void DisplayTest();
    void DisplayClear();
    void DisplayPrint(const String &params);
    void DisplayPixel(const String &params);
    void DisplayLine(const String &params);
    void DisplayCircle(const String &params);
    void DisplayPrintLN(String params);
    void DisplaySetScale(const String &params);
};

extern Display display;

#endif