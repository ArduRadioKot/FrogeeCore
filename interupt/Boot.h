#ifndef BOOT_H
#define BOOT_H
#include <SPI.h>
#include <SD.h>
#include <GyverOLED.h>
#include <Wire.h>
#include "Modules.h"
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
class Boot{
public:
    void executeSelectedFile(const String& filename) {  // Теперь принимает аргумент
        bool trig = false;
        String fullPath = "/" + filename;
        File file = SD.open(fullPath);
        
        if (!file) {
            showError("File Error");
            return;
        }
        
        oled.clear();
        oled.setCursorXY(10, 20);
        oled.print("Executing:");
        oled.setCursorXY(10, 35);
        oled.print(filename);
        oled.update();
        
        while (file.available()) {
            if(!trig){
              oled.clear();
              oled.update();
              trig = true;
            }
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() > 0) commandParser.Parse(line);
        }
        
        file.close();
        while(true);
    }
private:
    void showError(String msg) {
    oled.clear();
    oled.setCursorXY(10, 20);
    oled.print("ERROR:");
    oled.setCursorXY(10, 35);
    oled.print(msg);
    oled.update();
    delay(2000);
    }
};
Boot boot;
#endif