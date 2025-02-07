#ifndef DISPLAY_H
#define DISPLAY_H
#include <U8g2lib.h>

class Display {

public:
    void DisplayBegin(){
        U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
        u8g2.begin();
        u8g2.clearBuffer();
        u8g2.drawStr(0,10,"Hello World!");	// write something to the internal memory
        u8g2.sendBuffer();			
    }

};
Display display;

#endif