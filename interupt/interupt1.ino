#include <Modules.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <GyverOLED.h>



// Pins for ESP32-C3
const int BTN_LEFT = 10;
const int BTN_CENTER = 1;
const int BTN_RIGHT = 0;
const int SD_CS = 7;

// Menu states
const int MAX_FILES = 10;
String files[MAX_FILES];
int fileCount = 0;
int selectedFile = 0;
int scrollOffset = 0;
bool isRunning = false;
bool needRefresh = true;

void setup() {
    Serial.begin(115200);
    
    // Initialize buttons
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_CENTER, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    
    // Initialize display
    Wire.begin(8, 9); // SDA, SCL for ESP32-C3
    oled.init();
    oled.clear();
    
    // Initialize SD card
    if (!SD.begin(SD_CS, SPI, 4000000)) {
        showError("SD Card Error!");
    }
    
    // Find files
    File root = SD.open("/");
    listFiles(root);
    root.close();
    
    if(fileCount == 0) {
        showError("No files found!");
    }
    
    drawMenu();
}

void loop() {
    // Handle Serial input
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        commandParser.parse(command);
    }

    static uint32_t debounceTime = 0;
    if(millis() - debounceTime > 50) {
        checkButtons();
        debounceTime = millis();
    }
    
    if(needRefresh) {
        drawMenu();
        needRefresh = false;
    }
}

void listFiles(File dir) {
    while(File entry = dir.openNextFile()) {
        if(!entry.isDirectory() && entry.name()[0] != '.' && fileCount < MAX_FILES) {
            files[fileCount++] = "/" + String(entry.name());
        }
        entry.close();
    }
}

void checkButtons() {
    // Left button - previous file
    if(!digitalRead(BTN_LEFT)) {
        if(!isRunning) {
            selectedFile = (selectedFile - 1 + fileCount) % fileCount;
            updateScroll();
            needRefresh = true;
        }
    }
    
    // Right button - next file
    if(!digitalRead(BTN_RIGHT)) {
        if(!isRunning) {
            selectedFile = (selectedFile + 1) % fileCount;
            updateScroll();
            needRefresh = true;
        }
    }
    
    // Center button - execute only (no stop functionality)
    if(!digitalRead(BTN_CENTER)) {
        if(!isRunning && fileCount > 0) {
            isRunning = true;
            needRefresh = true;
            executeFile(files[selectedFile]);
        }
    }
}

void updateScroll() {
    const int visibleItems = 3;
    if(selectedFile < scrollOffset) {
        scrollOffset = selectedFile;
    } else if(selectedFile >= scrollOffset + visibleItems) {
        scrollOffset = selectedFile - visibleItems + 1;
    }
}

void drawMenu() {
    oled.clear();
    oled.setCursorXY(0, 0);
    
    // Header
    oled.print("Files: ");
    oled.print(fileCount);
    oled.print("/");
    oled.print(MAX_FILES);
    
    // Files list
    const int visibleItems = 3;
    for(int i = 0; i < visibleItems; i++) {
        int fileIndex = scrollOffset + i;
        if(fileIndex >= fileCount) break;
        
        int yPos = 15 + i*15;
        if(fileIndex == selectedFile) {
            oled.roundRect(0, yPos-2, 127, yPos+10, OLED_FILL);
            oled.invertText(true);
        } else {
            oled.invertText(false);
        }
        
        oled.setCursorXY(5, yPos);
        String name = files[fileIndex].substring(1);
        if(name.length() > 15) name = name.substring(0, 12) + "...";
        oled.print(name);
    }
    
    // Status bar
    oled.invertText(false);
    oled.setCursorXY(0, 55);
    if(isRunning) {
        oled.print("<< RUNNING >>");
    } else if(fileCount > 0) {
        oled.print("[CENT] Start   ");
    } else {
        oled.print(" No files found ");
    }
    
    oled.update();
}

void executeFile(String filename) {
    File file = SD.open(filename);
    if(!file) {
        showError("File error!");
        return;
    }
    
    while(file.available() && isRunning) {
        String line = file.readStringUntil('\n');
        commandParser.parse(line);
        delay(10);
    }
    
    file.close();
    isRunning = false;
    needRefresh = true;
}

void showError(String message) {
    oled.clear();
    oled.setCursorXY(0, 0);
    oled.print("ERROR:");
    oled.setCursorXY(0, 15);
    oled.print(message);
    oled.update();
    while(1) delay(1000);
}