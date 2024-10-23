String variableName = "";  // Имя переменной для хранения значений
int variableValue = 0;     // Значение переменной

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // Чтение строки с последовательного порта
    String command = Serial.readStringUntil('\n');
    command.trim();  // Убираем лишние пробелы и символы новой строки

    // Проверка, является ли строка вызовом функции или присвоением
    if (command.startsWith("print.serial(") && command.endsWith(")")) {
      // Вызов функции print.serial("...")
      handlePrintSerial(command);
    } 
    else if (command.indexOf('=') != -1) {
      // Присвоение переменной: val = <number>
      handleAssignment(command);
    } 
    else {
      // Команда не распознана
      Serial.println("Unknown command");
    }
  }
}

// Функция для обработки команды print.serial("...")
void handlePrintSerial(String command) {
  // Извлечение текста между кавычками
  int startIndex = command.indexOf('"');
  int endIndex = command.lastIndexOf('"');
  
  if (startIndex != -1 && endIndex != -1 && startIndex < endIndex) {
    String message = command.substring(startIndex + 1, endIndex);
    Serial.println(message);
  } else {
    Serial.println("Syntax error in print.serial");
  }
}

// Функция для обработки присвоения переменной (например, val = 42)
void handleAssignment(String command) {
  int equalPos = command.indexOf('=');
  
  if (equalPos != -1) {
    String varName = command.substring(0, equalPos);
    varName.trim();  // Убираем пробелы вокруг имени переменной

    String valueString = command.substring(equalPos + 1);
    valueString.trim();  // Убираем пробелы вокруг значения

    // Преобразование строки в число
    int value = valueString.toInt();
    
    // Сохраняем значение в "переменной"
    variableName = varName;
    variableValue = value;
    
    Serial.print("Assigned ");
    Serial.print(variableName);
    Serial.print(" = ");
    Serial.println(variableValue);
  } else {
    Serial.println("Syntax error in assignment");
  }
}
