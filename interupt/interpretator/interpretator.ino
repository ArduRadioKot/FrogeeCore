#include <Modules.h> 

void setup() {
    Serial.begin(115200);
    Display::begin();
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        commandParser.parse(command);
    }
}
