#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <GyverOLED.h>
#include "Modules.h"  // Подключаем ваш парсер команд
// Определение статического члена класса Display
GyverOLED<SSD1306_128x64, OLED_BUFFER> Display::oled;
// Конфигурация пинов
const uint8_t SD_CS = 7;
const uint8_t BTN_LEFT = 10;
const uint8_t BTN_CENTER = 1;
const uint8_t BTN_RIGHT = 0;

// Настройки дисплея
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
const uint8_t MAX_VISIBLE_ITEMS = 3;

// Меню
const uint8_t MAX_FILES = 20;
String fileList[MAX_FILES];
uint8_t fileCount = 0;
uint8_t selectedIndex = 0;
uint8_t scrollOffset = 0;

void setup() {
    Serial.begin(115200);
    initDisplay();
    initButtons();
    
    if (!initSDCard()) {
        showFatalError("SD Card Fail");
    }
    
    if (!scanFiles()) {
        showFatalError("No Files");
    }
    
    // Основной цикл меню
    while (true) {
        drawMenu();
        handleButtons();
    }
}

void loop() {
    // Пустой loop - всё выполняется в setup()
}

// Инициализация дисплея
void initDisplay() {
    Wire.begin(8, 9); // SDA, SCL
    oled.init();
    oled.clear();
    oled.setScale(1);
}

// Инициализация кнопок
void initButtons() {
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_CENTER, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
}

// Инициализация SD-карты
bool initSDCard() {
    return SD.begin(SD_CS, SPI, 4000000);
}

// Сканирование файлов
bool scanFiles() {
    File root = SD.open("/");
    if (!root) return false;
    
    while (File file = root.openNextFile()) {
        if (!file.isDirectory() && file.name()[0] != '.' && fileCount < MAX_FILES) {
            fileList[fileCount++] = "/" + String(file.name());
        }
    }
    return fileCount > 0;
}

// Отрисовка меню
void drawMenu() {
    oled.clear();
    
    // Заголовок
    oled.setCursorXY(0, 0);
    oled.print("Boot Menu");
    oled.setCursorXY(0, 10);
    oled.printf("Select: %d/%d", selectedIndex + 1, fileCount);
    
    // Список файлов
    uint8_t itemsToShow = min((int)MAX_VISIBLE_ITEMS, fileCount - scrollOffset);
    for (uint8_t i = 0; i < itemsToShow; i++) {
        uint8_t yPos = 25 + i * 12;
        
        if (i + scrollOffset == selectedIndex) {
            oled.rect(0, yPos - 2, 127, yPos + 10, OLED_FILL);
            oled.invertText(true);
        }
        
        String shortName = fileList[i + scrollOffset].substring(1);
        if (shortName.length() > 15) shortName = shortName.substring(0, 12) + "...";
        
        oled.setCursorXY(2, yPos);
        oled.print(shortName);
        oled.invertText(false);
    }
    
    // Статус бар
    oled.setCursorXY(0, 55);
    oled.print("[CENT] Run program");
    oled.update();
}

// Обработка кнопок
void handleButtons() {
    static uint32_t lastPress = 0;
    if (millis() - lastPress < 200) return;
    
    if (!digitalRead(BTN_LEFT)) {
        selectedIndex = (selectedIndex - 1 + fileCount) % fileCount;
        updateScroll();
        lastPress = millis();
    }
    
    if (!digitalRead(BTN_RIGHT)) {
        selectedIndex = (selectedIndex + 1) % fileCount;
        updateScroll();
        lastPress = millis();
    }
    
    if (!digitalRead(BTN_CENTER)) {
        executeSelectedFile();
        // После выполнения программа не вернётся в меню
    }
}

// Выполнение выбранного файла
void executeSelectedFile() {
    String filename = fileList[selectedIndex];
    File file = SD.open(filename);
    
    if (!file) {
        showError("File open error");
        return;
    }
    
    oled.clear();
    oled.setCursorXY(0, 0);
    oled.print("Running:");
    oled.setCursorXY(0, 10);
    oled.print(filename.substring(1));
    oled.update();
    
    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        
        if (line.length() > 0) {
            // Выводим текущую команду на экран
            oled.setCursorXY(0, 25);
            oled.print("> ");
            oled.print(line.substring(0, 20));
            oled.update();
            
            // Исполняем команду через ваш парсер
            commandParser.Parse(line);
        }
    }
    
    file.close();
    // После выполнения программа останавливается
    while(true); // Бесконечный цикл
}

// Обновление скролла
void updateScroll() {
    if (selectedIndex < scrollOffset) {
        scrollOffset = selectedIndex;
    } else if (selectedIndex >= scrollOffset + MAX_VISIBLE_ITEMS) {
        scrollOffset = selectedIndex - MAX_VISIBLE_ITEMS + 1;
    }
}

// Вывод ошибок
void showError(String msg) {
    oled.clear();
    oled.setCursorXY(0, 0);
    oled.print("ERROR:");
    oled.setCursorXY(0, 15);
    oled.print(msg);
    oled.update();
    delay(2000);
}

void showFatalError(String msg) {
    oled.clear();
    oled.setCursorXY(0, 0);
    oled.print("FATAL ERROR:");
    oled.setCursorXY(0, 15);
    oled.print(msg);
    oled.update();
    while (true);
}