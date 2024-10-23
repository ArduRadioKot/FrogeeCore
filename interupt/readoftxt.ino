#include <FS.h> // Подключите библиотеку файловой системы

void setup() {
    Serial.begin(115200);
    if (!SPIFFS.begin()) {
        Serial.println("Ошибка при инициализации SPIFFS");
        return;
    }

    File file = SPIFFS.open("/code.txt", "r");
    if (!file) {
        Serial.println("Не удалось открыть файл");
        return;
    }

    String code = "";
    while (file.available()) {
        code += String((char)file.read());
    }
    file.close();

    // Теперь вы можете использовать переменную 'code' для выполнения
    Serial.println(code); // Просто выводим код в Serial Monitor
}

void loop() {
    // Ваш основной код
}