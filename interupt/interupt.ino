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
    FileRead();
    
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        commandParser.parse(command);
    }
}

void FileRead() {
  File dataFile;
  if (FileOpen != 1) {
    if (!SD.begin(PIN_CHIP_S_SD)) {
      while (1);
    }
    FileOpen = 1;
    dataFile = SD.open(file);
    if (dataFile) {
      Serial.println("Sd ready");
      FileReady = 1;
    }
  }

  while (dataFile.available()) {
    line = dataFile.readStringUntil('\n');
    commandParser.parse(line);
  }
  FileClose = 1;
  
  if (FileClose == 1) {
    dataFile.close();
    FileClose = 0;
    FileOpen = 0;
  }
}

