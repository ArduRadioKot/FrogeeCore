#include <U8g2lib.h> // Библиотека для работы с OLED-дисплеем U8g2.
#include <iostream>  // Стандартная библиотека ввода-вывода.
#include <string>    // Библиотека для работы со строками.

#define Var_cnt 10  // Максимальное количество переменных.
int varCount = 0;   // Счетчик текущих переменных.

struct Mem {
    String name; // Имя переменной.
    String type; // Тип переменной (например, "int", "str", "float").
    String var;  // Значение переменной.
};

Mem variable[Var_cnt]; // Массив для хранения переменных.

// Функция проверки, является ли строка числом.
bool isNumber(const String &str) {
    return str.length() > 0 && (str.toInt() != 0 || str == "0");
}

// Класс для работы с переменными.
class Assign {
public:
    // Создает новую переменную.
    static void assign_var(const String &n_name, const String &n_type, const String &n_var) {
        if (varCount <= Var_cnt) {
            variable[varCount].name = n_name;
            variable[varCount].type = n_type;
            variable[varCount].var = n_var;
            varCount++;
        }
    }

    // Перезаписывает значение существующей переменной.
    static void reWrite_var(const int &id, const String &n_var) {
        variable[id].var = n_var;
    }

    // Находит индекс переменной по имени.
    static int find_var(const String &name_var) {
        for (int i = 0; i <= Var_cnt; i++) {
            if (variable[i].name == name_var) {
                return i;
            }
        }
        return -1;
    }

    // Обрабатывает строку с назначением переменной.
    static void tramit_var(const String &params) {
        int equal = params.indexOf('=');
        String name = params.substring(0, equal);
        String n_var = params.substring(equal + 1);
        int firstQuo = params.indexOf('"');
        int secondQuo = params.indexOf(firstQuo, '"');

        if (find_var(n_var) != -1) { // Если n_var - другая переменная.
            n_var = variable[find_var(n_var)].var;
        } else if (find_var(name) != -1) { // Если переменная с таким именем уже существует.
            reWrite_var(find_var(name), n_var);
        } else {
            // Создание новой переменной с определением типа.
            if ((firstQuo != -1) && (secondQuo != -1)) {
                assign_var(name, "str", n_var);
            } else if (isNumber(n_var)) {
                if (n_var.indexOf('.') == -1) {
                    assign_var(name, "int", n_var);
                } else {
                    assign_var(name, "float", n_var);
                }
            }
        }
    }
};

// Класс для работы с дисплеем.
class Display {
public:
    static U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2; // Объект дисплея.

    // Инициализация дисплея.
    static void begin() {
        u8g2.begin();
        u8g2.clearDisplay();
    }

    // Отображение текста на дисплее.
    static void displayText(const String &params) {
        int startQuo = params.indexOf('"');
        int endQuo = params.indexOf('"', startQuo + 1);
        int firstDot = params.indexOf(',', endQuo + 1);
        int secondDot = params.indexOf(',', firstDot + 1);
        int x = 0, y = 0;

        // Извлечение текста и координат.
        if (startQuo == -1 || endQuo == -1 || startQuo >= endQuo) return;
        if (Assign::find_var(params.substring(firstDot + 2, secondDot)) != -1) {
            x = variable[Assign::find_var(params.substring(firstDot + 2, secondDot))].var.toInt();
        } else {
            x = params.substring(firstDot + 2, secondDot).toInt();
        }
        if (Assign::find_var(params.substring(secondDot + 2)) != -1) {
            y = variable[Assign::find_var(params.substring(secondDot + 2))].var.toInt();
        } else {
            y = params.substring(secondDot + 2).toInt();
        }

        String text = params.substring(startQuo + 1, endQuo);
        Serial.println(x, y);
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(x, y, text.c_str());
        u8g2.sendBuffer();
    }

    // Отображение прямоугольника на дисплее.
    static void displayRect(const String &params) {
        int comma1 = params.indexOf(',');
        int comma2 = params.indexOf(',', comma1 + 1);
        int comma3 = params.indexOf(',', comma2 + 1);
        int x, y;

        if (comma1 == -1 || comma2 == -1 || comma3 == -1) return;
        if (Assign::find_var(params.substring(0, comma1)) != -1) {
            x = variable[Assign::find_var(params.substring(0, comma1))].var.toInt();
        } else {
            x = params.substring(0, comma1).toInt();
        }
        if (Assign::find_var(params.substring(comma1 + 2, comma2)) != -1) {
            y = variable[Assign::find_var(params.substring(comma1 + 2, comma2))].var.toInt();
        } else {
            y = params.substring(comma1 + 2, comma2).toInt();
        }

        Serial.println(x, y);
        int width = params.substring(comma2 + 2, comma3).toInt();
        int height = params.substring(comma3 + 2).toInt();

        u8g2.drawBox(x, y, width, height);
        u8g2.sendBuffer();
    }

    // Отображение пикселя на дисплее.
    static void displayPixel(const String &params) {
        int dot = params.indexOf(',');
        int x = params.substring(0, dot).toInt();
        int y = params.substring(dot + 2).toInt();

        u8g2.drawPixel(x, y);
        u8g2.sendBuffer();
    }

    // Очистка дисплея.
    static void displayClear() {
        u8g2.clearDisplay();
        u8g2.sendBuffer();
    }
};

// Класс для работы с последовательной печатью.
class MyPrint {
public:
    // Печать текста без переноса строки.
    static void printSerial(const String &params) {
        if (params.startsWith("\"") && params.endsWith("\"")) {
            Serial.print(params.substring(1, params.length() - 1));
        } else {
            Serial.print(params);
        }
    }

    // Печать текста с переносом строки.
    static void printlnSerial(const String &params) {
        if (Assign::find_var(params) != -1) {
            Serial.println(variable[Assign::find_var(params)].var);
        } else if (params.startsWith("\"") && params.endsWith("\"")) {
            Serial.println(params.substring(1, params.length() - 1));
        } else {
            Serial.println(params);
        }
    }
};

// Инициализация дисплея.
U8G2_SSD1306_128X64_NONAME_F_SW_I2C Display::u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);

// Класс для работы с пинами Arduino.
class Pin {
public:
    // Инициализация пина (вход или выход).
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

    // Чтение значения с пина.
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

    // Запись значения на пин.
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

// Функция вычисления простых математических операций.
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

// Парсер команд.
class CommandParser {
public:
    void parse(String args) {
        args.trim();
        int startParams = args.indexOf('(');
        int endParams = args.lastIndexOf(')');
        String commands = args.substring(0, startParams);
        String command_1, command_2;

        // Разделение команды на основные и подкоманды.
        if (commands.indexOf(".") != 1) {
            int dot = commands.indexOf('.');
            command_1 = commands.substring(0, dot);
            command_2 = commands.substring(dot + 1);
        } else {
            command_1 = commands;
        }

        String params = args.substring(startParams + 1, endParams);

        // Обработка команд.
        if (command_1 == "print") {
            if (command_2 == "Serial") {
                MyPrint::printSerial(params);
            } else if (command_2 == "lnSerial") {
                MyPrint::printlnSerial(params);
            } else if (command_2 == "print") {
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
        } else if (command_1 == "assign") {
            Assign::tramit_var(params);
        } else {
            Serial.println("Error: command not found");
        }
    }
};

// Глобальные объекты.
CommandParser commandParser;
