#include <stdlib.h>

#define MAX_VARIABLES 10  // Ограничение на количество переменных
#define MAX_COMMANDS 20   // Максимальное количество команд

// Структура для хранения переменной
struct Variable {
  String name;
  String type;  // Тип переменной: "int", "float", "string"
  union {
    int intValue;
    float floatValue;
  };
  String stringValue;
};

// Массив доступных пинов
int pinArray[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};  // Массив пинов на плате
int pinCount = sizeof(pinArray) / sizeof(pinArray[0]);       // Количество пинов


// Структура для хранения команды
struct Command {
  String name;
  void (*handler)(String args);  // Указатель на функцию обработки
};

// Фиксированные массивы для переменных и команд
Variable variables[MAX_VARIABLES];
Command commands[MAX_COMMANDS];

int varCount = 0;    // Количество переменных
int cmdCount = 0;    // Количество зарегистрированных команд

// Прототипы функций
void handlePinArrayCommand(String input);
void handleAssignment(String command);
void handlePrintSerial(String args);
void handleCast(String args);
void printVariable(String varName);  // Добавлен прототип функции printVariable
bool isNumeric(String str);
void registerCommand(String name, void (*handler)(String args));
void processCommand(String input);

void setup() {
  Serial.begin(115200);

  // Регистрация команд
  registerCommand("print.serial", handlePrintSerial);
  registerCommand("cast", handleCast);
  
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processCommand(command);
    handlePinArrayCommand(command);
  }
}

// Функция для обработки команд
void processCommand(String input) {
  int parenPos = input.indexOf('(');
  
  if (parenPos != -1) {
    String cmdName = input.substring(0, parenPos);
    cmdName.trim();
    String args = input.substring(parenPos + 1, input.length() - 1);  // Извлекаем аргументы
    
    for (int i = 0; i < cmdCount; i++) {
      if (commands[i].name == cmdName) {
        commands[i].handler(args);  // Вызываем обработчик команды
        return;
      }
    }
    Serial.println("Unknown command: " + cmdName);
  } else if (input.indexOf('=') != -1) {
    handleAssignment(input);  // Обработка присвоения переменной
  } else {
    Serial.println("Syntax error");
  }
}

// Регистрация команды
void registerCommand(String name, void (*handler)(String args)) {
  if (cmdCount < MAX_COMMANDS) {
    commands[cmdCount].name = name;
    commands[cmdCount].handler = handler;
    cmdCount++;
  }
}

// Обработка команды print.serial
void handlePrintSerial(String args) {
  int startIndex = args.indexOf('"');
  int endIndex = args.lastIndexOf('"');
  
  if (startIndex != -1 && endIndex != -1 && startIndex < endIndex) {
    // Если аргумент является строкой
    String message = args.substring(startIndex + 1, endIndex);
    Serial.println(message);
  } else {
    // Если аргумент — это переменная
    args.trim();
    printVariable(args);  // Теперь функция printVariable объявлена корректно
  }
}

// Функция для вывода значения переменной
void printVariable(String varName) {
  for (int i = 0; i < varCount; i++) {
    if (variables[i].name == varName) {
      if (variables[i].type == "int") {
        Serial.println(variables[i].intValue);
      } else if (variables[i].type == "float") {
        Serial.println(variables[i].floatValue);
      } else if (variables[i].type == "string") {
        Serial.println(variables[i].stringValue);
      }
      return;
    }
  }
  Serial.println("Variable not found");
}

// Обработка присвоения переменной
void handleAssignment(String command) {
  int equalPos = command.indexOf('=');
  
  if (equalPos != -1) {
    String varName = command.substring(0, equalPos);
    varName.trim();

    if (isNumeric(varName)) {
      Serial.println("Error: Variable name cannot be a number.");
      return;
    }

    String valueString = command.substring(equalPos + 1);
    valueString.trim();

    // Определение типа значения (int, float или строка)
    if (valueString.indexOf('"') != -1) {
      String stringValue = valueString.substring(1, valueString.length() - 1);
      setVariable(varName, stringValue);
    } else if (valueString.indexOf('.') != -1) {
      float floatValue = valueString.toFloat();
      setVariable(varName, floatValue);
    } else {
      int intValue = valueString.toInt();
      setVariable(varName, intValue);
    }
  } else {
    Serial.println("Syntax error in assignment");
  }
}

// Обработка команды cast
void handleCast(String args) {
  int commaIndex = args.indexOf(',');
  if (commaIndex != -1) {
    String type = args.substring(0, commaIndex);
    String value = args.substring(commaIndex + 1);
    type.trim();
    value.trim();

    if (type == "int") {
      int intValue = value.toInt();
      Serial.println(intValue);
    } else if (type == "float") {
      float floatValue = value.toFloat();
      Serial.println(floatValue);
    } else if (type == "string") {
      Serial.println(value);
    } else {
      Serial.println("Unknown type for cast");
    }
  } else {
    Serial.println("Syntax error in cast");
  }
}

// Установка переменной (int)
void setVariable(String name, int value) {
  int index = findVariable(name);
  
  if (index != -1) {
    variables[index].intValue = value;
    variables[index].type = "int";
    Serial.print("Updated ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  } else {
    if (varCount >= MAX_VARIABLES) {
      Serial.println("Error: Variable limit reached.");
      return;
    }
    addVariable(name, "int");
    variables[varCount - 1].intValue = value;
    Serial.print("Assigned ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  }
}

// Установка переменной (float)
void setVariable(String name, float value) {
  int index = findVariable(name);
  
  if (index != -1) {
    variables[index].floatValue = value;
    variables[index].type = "float";
    Serial.print("Updated ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  } else {
    if (varCount >= MAX_VARIABLES) {
      Serial.println("Error: Variable limit reached.");
      return;
    }
    addVariable(name, "float");
    variables[varCount - 1].floatValue = value;
    Serial.print("Assigned ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  }
}

// Установка переменной (string)
void setVariable(String name, String value) {
  int index = findVariable(name);
  
  if (index != -1) {
    variables[index].stringValue = value;
    variables[index].type = "string";
    Serial.print("Updated ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  } else {
    if (varCount >= MAX_VARIABLES) {
      Serial.println("Error: Variable limit reached.");
      return;
    }
    addVariable(name, "string");
    variables[varCount - 1].stringValue = value;
    Serial.print("Assigned ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  }
}

// Функция для поиска переменной по имени
int findVariable(String name) {
  for (int i = 0; i < varCount; i++) {
    if (variables[i].name == name) {
      return i;
    }
  }
  return -1;
}

// Функция для добавления новой переменной
void addVariable(String name, String type) {
  if (varCount < MAX_VARIABLES) {
    variables[varCount].name = name;
    variables[varCount].type = type;
    varCount++;
  }
}

// Проверка, что строка является числом
bool isNumeric(String str) {
  for (int i = 0; i < str.length(); i++) {
    if (isDigit(str.charAt(i)) == 0) {
      return false;
    }
  }
  return true;
}

void handlePinArrayCommand(String input) {
  // Проверка на запись пина (writePinArray.индекс.состояние)
  if (input.startsWith("writePinArray.")) {
    int dotPos = input.indexOf('.', 13);  // Поиск второй точки после "writePinArray."
    if (dotPos != -1) {
      // Извлечение индекса массива и состояния пина
      String pinIndexStr = input.substring(13, dotPos);
      String pinStateStr = input.substring(dotPos + 1);
      
      int pinIndex = pinIndexStr.toInt();
      int pinState = pinStateStr.toInt();
      
      // Проверка допустимости индекса и состояния
      if (pinIndex >= 0 && pinIndex < pinCount && (pinState == HIGH || pinState == LOW)) {
        int realPin = pinArray[pinIndex];  // Получаем реальный номер пина
        pinMode(realPin, OUTPUT);
        digitalWrite(realPin, pinState);
        Serial.print("Pin ");
        Serial.print(realPin);
        Serial.print(" (index ");
        Serial.print(pinIndex);
        Serial.print(") set to ");
        Serial.println(pinState == HIGH ? "HIGH" : "LOW");
      } else {
        Serial.println("Invalid pin index or state.");
      }
    } else {
      Serial.println("Syntax error in writePinArray command.");
    }
  }
  
  // Проверка на чтение пина (readPinArray.индекс)
  else if (input.startsWith("readPinArray.")) {
    String pinIndexStr = input.substring(13);
    int pinIndex = pinIndexStr.toInt();
    
    if (pinIndex >= 0 && pinIndex < pinCount) {
      int realPin = pinArray[pinIndex];  // Получаем реальный номер пина
      pinMode(realPin, INPUT);
      int pinValue = digitalRead(realPin);
      Serial.print("Pin ");
      Serial.print(realPin);
      Serial.print(" (index ");
      Serial.print(pinIndex);
      Serial.print(") is ");
      Serial.println(pinValue == HIGH ? "HIGH" : "LOW");
    } else {
      Serial.println("Invalid pin index.");
    }
  }
}

