#include <Modules.h> 
//#include "Arduino.h"
void setup() {
    Serial.begin(115200);
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        commandParser.parse(command);
    }
}
