#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <GyverOLED.h>
#include "Modules.h"

GyverOLED<SSD1306_128x64, OLED_BUFFER> Display::oled;
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
// Конфигурация
const uint8_t SD_CS = 7;
const uint8_t BTN_LEFT = 10;
const uint8_t BTN_CENTER = 1;
const uint8_t BTN_RIGHT = 0;
const uint8_t VISIBLE_FILES = 3;
const uint8_t LINE_HEIGHT = 12;
const uint8_t MARGIN = 2;

// Меню
String fileList[20];
uint8_t fileCount = 0;
uint8_t selectedIndex = 0;

void setup() {
    Serial.begin(115200);
    initDisplay();
    initButtons();
    
    if (!initSDCard()) showFatalError("SD Card Error");
    if (!scanFiles()) showFatalError("No Files Found");
    
    while (true) {
        drawMenu();
        handleButtons();
    }
}

void loop() {}

void initDisplay() {
    Wire.begin(8, 9);
    oled.init();
    oled.clear();
    oled.setScale(1);
}

void initButtons() {
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_CENTER, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
}

bool initSDCard() {
    return SD.begin(SD_CS, SPI, 4000000);
}

bool scanFiles() {
    File root = SD.open("/");
    if (!root) return false;
    
    while (File file = root.openNextFile()) {
        if (!file.isDirectory() && file.name()[0] != '.' && fileCount < 20) {
            fileList[fileCount++] = file.name();
        }
    }
    return fileCount > 0;
}

void drawMenu() {
    oled.clear();
    
    // Заголовок
    oled.setCursorXY(MARGIN, 0);
    oled.print("FrogeeCore BootLoader");
    
    // Отображение 3 файлов
    for (uint8_t i = 0; i < VISIBLE_FILES; i++) {
        uint8_t fileNum = selectedIndex - 1 + i;
        
        // Коррекция для крайних положений
        if (selectedIndex == 0) fileNum = i;
        else if (selectedIndex == fileCount - 1) fileNum = fileCount - VISIBLE_FILES + i;
        
        if (fileNum >= fileCount) continue;
        
        uint8_t yPos = 12 + i * LINE_HEIGHT;
        
        // Курсор-стрелка для выбранного файла
        oled.setCursorXY(MARGIN, yPos);
        if (fileNum == selectedIndex) {
            oled.print(">");
        } else {
            oled.print(" ");
        }
        
        // Имя файла
        oled.setCursorXY(MARGIN + 8, yPos);
        String name = fileList[fileNum];
        if (name.length() > 18) name = name.substring(0, 15) + "...";
        oled.print(name);
    }
    
    // Статус бар (без разделительной линии)
    oled.setCursorXY(MARGIN, 55);
    oled.print(selectedIndex + 1);
    oled.print("/");
    oled.print(fileCount);
    oled.print("  <Select>  [Run]");
    
    oled.update();
}

void handleButtons() {
    static uint32_t lastPress = 0;
    if (millis() - lastPress < 200) return;
    
    if (!digitalRead(BTN_LEFT)) {
        if (selectedIndex > 0) selectedIndex--;
        lastPress = millis();
    }
    
    if (!digitalRead(BTN_RIGHT)) {
        if (selectedIndex < fileCount - 1) selectedIndex++;
        lastPress = millis();
    }
    
    if (!digitalRead(BTN_CENTER)) {
        executeSelectedFile();
    }
}

void executeSelectedFile() {
    bool trig = false;
    String filename = "/" + fileList[selectedIndex];
    File file = SD.open(filename);
    
    if (!file) {
        showError("File Error");
        return;
    }
    
    oled.clear();
    oled.setCursorXY(10, 20);
    oled.print("Executing:");
    oled.setCursorXY(10, 35);
    oled.print(fileList[selectedIndex]);
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

void showError(String msg) {
    oled.clear();
    oled.setCursorXY(10, 20);
    oled.print("ERROR:");
    oled.setCursorXY(10, 35);
    oled.print(msg);
    oled.update();
    delay(2000);
}

void showFatalError(String msg) {
    oled.clear();
    oled.setCursorXY(10, 20);
    oled.print("FATAL ERROR:");
    oled.setCursorXY(10, 35);
    oled.print(msg);
    oled.update();
    while(true);
}