#ifndef PIN_H
#define PIN_H
#include "moduleParse.h"
class Pin {
public:
    // Инициализация пина (вход или выход).
    static void init(const String &params) {
        int dot = params.indexOf(',');
        String pinStr = params.substring(0, dot);
        String mode = params.substring(dot + 1);
        int pin = mdp.check(pinStr).toInt();
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
        String pinStr = params.substring(dot + 1);
        int pin = mdp.check(pinStr).toInt();
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
        String pinStr = params.substring(firstDot + 1, secondDot);
        String valueStr = params.substring(secondDot + 1);

        int pin = mdp.check(pinStr).toInt();
        int value = mdp.check(valueStr).toInt();

        if (type == "an") {
            analogWrite(pin, value);
        } else if (type == "dg") {
            digitalWrite(pin, value == 1 ? HIGH : LOW);
        }
    }
};
Pin pin;

#endif