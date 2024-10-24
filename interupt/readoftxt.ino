#include <FS.h> // Подключите библиотеку файловой системы
#include <Arduino.h>

const int ledPin = 2; // Пин для LED

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

void loop() {
    // Ваш основной код
}

void executeCommand(String command) {
    command.trim(); // Удаляем пробелы
    if (command == "LED_ON") {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED включен");
    } else if (command == "LED_OFF") {
        digitalWrite(ledPin, LOW);
        Serial.println("LED выключен");
    } else if (command.startsWith("DELAY_")) {
        int delayTime = command.substring(6).toInt();
        delay(delayTime);
        Serial.print("Задержка на ");
        Serial.print(delayTime);
        Serial.println(" мс");
    } else if (command == "SERIAL_PRINT") {
        Serial.println("Привет, мир!");
    } else if (command.startsWith("ANALOG_WRITE_")) {
        int pin = command.substring(13, 14).toInt();
        int value = command.substring(15).toInt();
        analogWrite(pin, value);
        Serial.print("Аналоговый вывод на пине ");
        Serial.print(pin);
        Serial.print(" значение ");
        Serial.println(value);
    } else if (command.startsWith("DIGITAL_WRITE_")) {
        int pin = command.substring(14, 15).toInt();
        int value = command.substring(16).toInt();
        digitalWrite(pin, value);
        Serial.print("Цифровой вывод на пине ");
        Serial.print(pin);
        Serial.print(" значение ");
        Serial.println(value);
    } else if (command.startsWith("READ_ANALOG_")) {
        int pin = command.substring(12, 13).toInt();
        int value = analogRead(pin);
        Serial.print("Аналоговый вход на пине ");
        Serial.print(pin);
        Serial.print(" значение ");
        Serial.println(value);
    } else if (command.startsWith("READ_DIGITAL_")) {
        int pin = command.substring(13, 14).toInt();
        int value = digitalRead(pin);
        Serial.print("Цифровой вход на пине ");
        Serial.print(pin);
        Serial.print(" значение ");
        Serial.println(value);
    } else if (command.startsWith("IF_")) {
        String condition = command.substring(3);
        int openParenIndex = condition.indexOf("(");
        int closeParenIndex = condition.indexOf(")");

        int openBraceIndex = condition.indexOf("{");
        int closeBraceIndex = condition.indexOf("}");

        if (openParenIndex != -1 && closeParenIndex != -1 &&
            openBraceIndex != -1 && closeBraceIndex != -1 && 
            closeBraceIndex > openBraceIndex) {
            
            String conditionValue = condition.substring(openParenIndex + 1, closeParenIndex).trim();
            String commandsBlock = condition.substring(openBraceIndex + 1, closeBraceIndex).trim();
            
            if (conditionValue == "TRUE") {
                // Разделяем команды по новой строке и выполняем каждую
                int startIndex = 0;
                int newlineIndex;
                while ((newlineIndex = commandsBlock.indexOf('\n', startIndex)) != -1) {
                    String cmd = commandsBlock.substring(startIndex, newlineIndex).trim();
                    if (cmd.length() > 0) {
                        executeCommand(cmd);
                    }
                    startIndex = newlineIndex + 1;
                }
                // Выполняем последнюю команду, если она есть
                String lastCmd = commandsBlock.substring(startIndex).trim();
                if (lastCmd.length() > 0) {
                    executeCommand(lastCmd);
                }
            }
        } else {
            Serial.println("Неправильный формат команды IF");
        }
    } else {
        Serial.println("Неизвестная команда: " + command);
    }
}