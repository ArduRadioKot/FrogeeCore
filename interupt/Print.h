#ifndef PRINT_H
#define PRINT_H

//#include <Arduino.h>  // For Serial
#include "Assign.h"   // Ensure Assign class is declared
#include "Arduino.h"

// Class for serial printing
class MyPrint {
public:
    // Print without newline
    static void printSerial(const String &params) {
        if (Assign::find_var(params) != -1) {
            Serial.print(variable[Assign::find_var(params)].var);
        } else if (params.startsWith("\"") && params.endsWith("\"")) {
            Serial.print(params.substring(params.indexOf('"') + 1, params.length() - 1));
        } else {
            Serial.print(params);
        }
    }

    // Print with newline
    static void printlnSerial(const String &params) {
        if (Assign::find_var(params) != -1) {
            Serial.println(variable[Assign::find_var(params)].var);
        } else if (params.startsWith("\"") && params.endsWith("\"")) {
            Serial.println(params.substring(params.indexOf('"') + 1, params.length() - 1));
        } else {
            Serial.println(params);
        }
    }
};

MyPrint myPrint;

#endif
