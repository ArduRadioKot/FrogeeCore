#include <U8g2lib.h>
#define Val_Int 10

// Класс DisplayManager

class Display {
public:
    static U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2;
    static void begin() {
        u8g2.begin();
        u8g2.clearDisplay();
    }
    static void displayText(const String &params) {
        int startQuo = params.indexOf('"');
        int endQuo = params.indexOf('"', startQuo + 1);
        int firstDot = params.indexOf(',', endQuo + 1);
        int secondDot = params.indexOf(',', firstDot + 1);

        if (startQuo == -1 || endQuo == -1 || startQuo >= endQuo) return;

        int x = params.substring(firstDot + 2, secondDot).toInt();
        int y = params.substring(secondDot + 2).toInt();
        String text = params.substring(startQuo + 1, endQuo);

        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(x, y, text.c_str());
        u8g2.sendBuffer();
    }

    static void displayRect(const String &params) {
        int comma1 = params.indexOf(',');
        int comma2 = params.indexOf(',', comma1 + 1);
        int comma3 = params.indexOf(',', comma2 + 1);

        if (comma1 == -1 || comma2 == -1 || comma3 == -1) return;

        int x = params.substring(0, comma1).toInt();
        int y = params.substring(comma1 + 2, comma2).toInt();
        int width = params.substring(comma2 + 2, comma3).toInt();
        int height = params.substring(comma3 + 2).toInt();

        u8g2.drawBox(x, y, width, height);
        u8g2.sendBuffer();
    }

    static void displayPixel(const String &params) {
        int dot = params.indexOf(',');
        int x = params.substring(0, dot).toInt();
        int y = params.substring(dot + 2).toInt();

        u8g2.drawPixel(x, y);
        u8g2.sendBuffer();
    }

    static void displayClear() {
        u8g2.clearDisplay();
        u8g2.sendBuffer();
    }

};

class MyPrint {
public:


    static void printSerial(const String &params) {
        if(params.startsWith("\"") && params.endsWith("\"")) {
            Serial.print(params.substring(1, params.length() - 1));
        } else {
            Serial.print(params);
        }
    }

    static void printlnSerial(const String &params) {
        if (params.startsWith("\"") && params.endsWith("\"")) {
            Serial.println(params.substring(1, params.length() - 1));
        } else {
            Serial.println(params);
        }
    }

    
};

// Инициализация дисплея
U8G2_SSD1306_128X64_NONAME_F_SW_I2C Display::u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

// Класс Pin
class Pin {
public:
    static void init(const String &params) {
        int dot = params.indexOf(',');
        String pinStr = params.substring(0, dot);
        String mode = params.substring(dot + 2);
        int pin = pinStr.toInt();
        if (mode == "input") {
            pinMode(pin, INPUT);
        } else if (mode == "output") {
            pinMode(pin, OUTPUT);
        }
    }

    static void read(const String &params) {
        int dot = params.indexOf(',');
        String type = params.substring(0, dot);
        String pinStr = params.substring(dot + 2);
        int pin = pinStr.toInt();

        if (type == "an") {
            int value = analogRead(pin);
            Serial.println(value);
        } else if (type == "dg") {
            int value = digitalRead(pin);
            Serial.println(value);
        }
    }

    static void write(const String &params) {
        int firstDot = params.indexOf(',');
        int secondDot = params.indexOf(',', firstDot + 1);

        String type = params.substring(0, firstDot);
        String pinStr = params.substring(firstDot + 2, secondDot);
        String valueStr = params.substring(secondDot + 2);

        int pin = pinStr.toInt();
        int value = valueStr.toInt();

        if (type == "an") {
            analogWrite(pin, value);
        } else if (type == "dg") {
            digitalWrite(pin, value == 1 ? HIGH : LOW);
        }
    }
};

// Вспомогательные функции
bool isNumber(const String &str) {
    return str.length() > 0 && (str.toInt() != 0 || str == "0");
}

float calculate(const String &params) {
    int firstSpace = params.indexOf(' ');
    int secondSpace = params.indexOf(' ', firstSpace + 1);

    String firstParamStr = params.substring(0, firstSpace);
    String operand = params.substring(firstSpace + 1, secondSpace);
    String secondParamStr = params.substring(secondSpace + 1);

    if (isNumber(firstParamStr) && isNumber(secondParamStr)) {
        float firstParam = firstParamStr.toFloat();
        float secondParam = secondParamStr.toFloat();

        if (operand == "+") return firstParam + secondParam;
        if (operand == "-") return firstParam - secondParam;
        if (operand == "*") return firstParam * secondParam;
        if (operand == "/") return secondParam != 0 ? firstParam / secondParam : 0;
    }
    return 0;
}

// Парсер
class CommandParser {
public:
    void parse(const String &args) {
        if (args.indexOf('.') != -1) {
            int dot = args.indexOf('.');
            int startParams = args.indexOf('(');
            int endParams = args.lastIndexOf(')');

            String command_1 = args.substring(0, dot);
            String command_2 = args.substring(dot + 1, startParams);
            String params = args.substring(startParams + 1, endParams);

            if (command_1 == "print") {
                if (command_2 == "Serial") {
                    MyPrint::printSerial(params);
                } else if (command_2 == "lnSerial") {
                    MyPrint::printlnSerial(params);
                }else{
                    Serial.println("Error: command not found");
                }
            } else if(command_1 == "display"){
                if (command_2 == "print") {
                    Display::displayText(params);
                } else if (command_2 == "rect") {
                    Display::displayRect(params);
                } else if (command_2 == "pixel") {
                    Display::displayPixel(params);
                } else if (command_2 == "clear") {
                    Display::displayClear();
                } else {
                    Serial.println("Error: command not found");
                }
            } else if (command_1 == "pin") {
                if (command_2 == "read") {
                    Pin::read(params);
                } else if (command_2 == "write") {
                    Pin::write(params);
                } else if (command_2 == "init") {
                    Pin::init(params);
                } else {
                    Serial.println("Error: command not found");
                }
            } else {
                Serial.println("Error: command not found");
            }
        } else {
            Serial.println("Error: command not found");
        }
    }
};

// Глобальные объекты
CommandParser commandParser;
