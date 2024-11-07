/*void executeCommand(String command) {
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
    } else if (command.startsWith("PWM_")) {
        int pin = command.substring(4, 5).toInt();
        int value = command.substring(6).toInt();
        ledcWrite(pin, value); // Используйте ledcWrite для ESP32
        Serial.print("PWM на пине ");
        Serial.print(pin);
        Serial.print(" значение ");
        Serial.println(value);
    } else if (command.startsWith("MATH_ADD_")) {
        int a = command.substring(9, command.indexOf('_', 9)).toInt();
        int b = command.substring(command.indexOf('_', 9) + 1).toInt();
        Serial.print("Сумма ");
        Serial.print(a);
        Serial.print(" и ");
        Serial.print(b);
        Serial.print(" равна ");
        Serial.println(a + b);
    } else if (command.startsWith("MATH_SUB_")) {
        int a = command.substring(9, command.indexOf('_', 9)).toInt();
        int b = command.substring(command.indexOf('_', 9) + 1).toInt();
        Serial.print("Разность ");
        Serial.print(a);
        Serial.print(" и ");
        Serial.print(b);
        Serial.print(" равна ");
        Serial.println(a - b);
    } else if (command.startsWith("MATH_MUL_")) {
        int a = command.substring(9, command.indexOf('_', 9)).toInt();
        int b = command.substring(command.indexOf('_', 9) + 1).toInt();
        Serial.print("Произведение ");
        Serial.print(a);
        Serial.print(" и ");
        Serial.print(b);
        Serial.print(" равно ");
        Serial.println(a * b);
    } else if (command.startsWith("MATH_DIV_")) {
        int a = command.substring(9, command.indexOf('_', 9)).toInt();
        int b = command.substring(command.indexOf('_', 9) + 1).toInt();
        if (b != 0) {
            Serial.print("Частное ");
            Serial.print(a);
            Serial.print(" и ");
            Serial.print(b);
            Serial.print(" равно ");
            Serial.println((float)a / b);
        } else {
            Serial.println("Ошибка: Деление на ноль");
        }
    } else {
        Serial.println("Неизвестная команда: " + command);
    }
}*/