/*#include <FS.h> // Подключите библиотеку файловой системы
#include <SD.h> // Подключите библиотеку SD
#include <SPI.h> // Подключите библиотеку SPI
#include <Arduino.h>

const int ledPin = 2; // Пин для LED
const int chipSelect = 10; // Пин выбора чипа для SD-карты

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);

    if (!SPIFFS.begin()) {
        Serial.println("Ошибка при инициализации SPIFFS");
        return;
    }

    File file = SPIFFS.open("/commands.txt", "r");
    if (!file) {
        Serial.println("Не удалось открыть файл");
        return;
    }

    while (file.available()) {
        String command = file.readStringUntil('\n');
        executeCommand(command);
    }
    file.close();
}
    // Инициализация SD-карты
    if (!SD.begin(chipSelect)) {
        Serial.println("Ошибка при инициализации SD-карты");
        return;
    }

    File file = SD.open("/commands.txt", FILE_READ);
    if (!file) {
        Serial.println("Не удалось открыть файл");
        return;
    }

}

void loop() {
    // Ваш основной код
    if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("print.serial(") && command.endsWith(")")) {
      handlePrintSerial(command);
    } 
    else if (command.indexOf('=') != -1) {
      handleAssignment(command);
    }
    else if (command.startsWith("cast(") && command.endsWith(")")) {
      handleCast(command);
    }
    else {
      Serial.println("Unknown command");
    }
  }
    File file = SD.open("/commands.txt", FILE_READ);
    if (!file) {
        Serial.println("Не удалось открыть файл");
        return;
    }

    // Читаем команды из файла и выполняем их
    while (file.available()) {
        String command = file.readStringUntil('\n');
        executeCommand(command);
    }
    file.close();

    // После выполнения всех команд, можно добавить небольшую задержку перед повторным чтением
    delay(1000); // Задержка перед повторным чтением файла
}

*/