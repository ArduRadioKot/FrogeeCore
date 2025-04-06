#include "Display.h"
#include "moduleParse.h"

// Инициализация статического члена класса
GyverOLED<SSD1306_128x64, OLED_BUFFER> Display::oled;

void Display::DisplayBegin() {
    Wire.begin(8, 9);  // Инициализация I2C с указанием пинов SDA, SCL
    oled.init();
    oled.clear();
    oled.setScale(1);
    oled.update();
}

void Display::DisplayTest() {
    oled.clear();
    oled.setCursorXY(0, 0);
    oled.print("Test");
    oled.update();
}

void Display::DisplayClear() {
    oled.clear();
    oled.update();
}

void Display::DisplayPrint(const String &params) {
    oled.autoPrintln(false);
    int dot2 = 0;
    int dt = 0;
    
    // Поиск второй запятой в параметрах
    for(int i = 0; i <= params.length(); i++) {
        if(params[i] == ',') {
            dt++;
            if(dt == 2) {
                dot2 = i;
                break;
            }
        }
    }
    
    String param_1 = Aux::trim_space(params.substring(0, dot2));
    String param_2 = params.substring(dot2 + 1);
    param_1 = param_1.substring(0, param_1.length());
    
    int dot1 = param_1.indexOf(',');
    int x = (mdp.check(param_1.substring(0, dot1))).toInt();
    int y = (mdp.check(param_1.substring(dot1 + 1))).toInt();
    
    oled.setCursorXY(x, y);
    
    if ((param_2.indexOf('"') != -1) && 
        (param_2.lastIndexOf('"') != -1) && 
        (param_2.indexOf('"') != param_2.lastIndexOf('"'))) {
        oled.print(param_2.substring(
            param_2.indexOf('"') + 1,
            param_2.lastIndexOf('"')
        ));
    } else {
        oled.print(mdp.check(Aux::trim_space(param_2)));
    }
    
    oled.update();
}

void Display::DisplayPixel(const String &params) {
    String param = Aux::trim_space(params);
    int dot1 = param.indexOf(',');
    int x = mdp.check(param.substring(0, dot1)).toInt();
    int y = mdp.check(param.substring(dot1 + 1)).toInt();
    
    oled.dot(x, y);
    oled.update();
}

void Display::DisplayLine(const String &params) {
    String param = Aux::trim_space(params);
    int dot1 = param.indexOf(',');
    int dot2 = param.indexOf(',', dot1 + 1);
    int dot3 = param.indexOf(',', dot2 + 1);
    
    int x1 = mdp.check(param.substring(0, dot1)).toInt();
    int y1 = mdp.check(param.substring(dot1 + 1, dot2)).toInt();
    int x2 = mdp.check(param.substring(dot2 + 1, dot3)).toInt();
    int y2 = mdp.check(param.substring(dot3 + 1)).toInt();
    
    oled.line(x1, y1, x2, y2);
    oled.update();
}

void Display::DisplayCircle(const String &params) {
    String param = Aux::trim_space(params);
    int dot1 = param.indexOf(',');
    int dot2 = param.indexOf(',', dot1 + 1);
    
    int x = mdp.check(param.substring(0, dot1)).toInt();
    int y = mdp.check(param.substring(dot1 + 1, dot2)).toInt();
    int r = mdp.check(param.substring(dot2 + 1)).toInt();
    
    oled.circle(x, y, r, OLED_STROKE);
    oled.update();
}

void Display::DisplayPrintLN(String params) {
    oled.autoPrintln(true);
    
    if ((params.indexOf('"') != -1) && 
        (params.lastIndexOf('"') != -1) && 
        (params.indexOf('"') != params.lastIndexOf('"'))) {
        oled.print(params.substring(
            params.indexOf('"') + 1,
            params.lastIndexOf('"')
        ));
    } else {
        oled.print(mdp.check(Aux::trim_space(params)));
    }
    
    oled.update();
}

void Display::DisplaySetScale(const String &params) {
    oled.setScale(mdp.check(params).toInt());
    oled.update();
}