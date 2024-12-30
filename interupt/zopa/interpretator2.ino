#include <SD.h> // Подключите библиотеку SD
#include <SPI.h> // Подключите библиотеку SPI
#include <Arduino.h>

const int ledPin = 2; // Пин для LED
const int chipSelect = 15; // Пин выбора чипа для SD-карты

void setup() {
    Serial.begin(115200);
    pinMode(ledPin, OUTPUT);

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
        
        // Оценка условия
        if (evaluateCondition(conditionValue)) {
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
}

// Метод для оценки условия
private boolean evaluateCondition(String condition) {
    // Пример: условие может быть "x > 10", "y == 5" и т.д.
    // Здесь нужно реализовать парсинг и оценку условия.
    // Для простоты, можно использовать eval или написать свой парсер.
    
    // Предположим, что у нас есть метод getVariableValue, который возвращает значение переменной
    // и метод compare для сравнения значений.
    
    // Пример реализации:
    String[] parts = condition.split(" ");
    if (parts.length == 3) {
        String variable = parts[0];
        String operator = parts[1];
        String value = parts[2];

        // Получаем значение переменной
        int variableValue = getVariableValue(variable);
        int comparisonValue = Integer.parseInt(value); // или другой тип, в зависимости от контекста

        // Сравниваем значения
        switch (operator) {
            case ">":
                return variableValue > comparisonValue;
            case "<":
                return variableValue < comparisonValue;
            case "==":
                return variableValue == comparisonValue;
            case "!=":
                return variableValue != comparisonValue;
            // Добавьте другие операторы по мере необходимости
            default:
                return false;
        }
    }
    return false; // Если условие не распознано
}

// Метод для получения значения переменной
private int getVariableValue(String variable) {
    // Здесь должна быть логика для получения значения переменной
    // Например, если у вас есть карта переменных, вы можете сделать что-то вроде:
    // return variableMap.get(variable);
    return 0; // Заглушка, замените на вашу реализацию
}
    } else {
        Serial.println("Неизвестная команда: " + command);
    }
}