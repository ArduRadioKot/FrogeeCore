#include <Modules.h> 
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

const int PIN_CHIP_S_SD = 7; // Пин для работы с SD картой
String fString, fStringFileName, rwBuff;
String line;
boolean FileOpen, FileClose, FileReady;
String file = "/programm.txt"; // Имя файла для чтения с SD карты

void setup() {
    Serial.begin(115200);
    Serial.println("read file");
    delay(1000);
    FileRead();
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        commandParser.Parse(command);
    }
}

void FileRead() {
    if (FileOpen != 1) {
        if (!SD.begin(PIN_CHIP_S_SD)) {
            Serial.println("SD Card initialization failed!");
            while (1);
        }
        FileOpen = 1;
        File dataFile = SD.open(file);
        if (dataFile) {
            Serial.println("Sd ready");
            FileReady = 1;
            while (dataFile.available()) {
                line = dataFile.readStringUntil('\n');
                commandParser.Parse(line);
            }
            dataFile.close();
        } else {
            Serial.println("Error opening file");
        }
        FileClose = 1;
        FileOpen = 0;
    }
}