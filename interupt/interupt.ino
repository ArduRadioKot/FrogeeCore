#include <stdlib.h>

#define MAX_VARIABLES   // Ограничение на количество переменных

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

// Фиксированный массив для переменных
Variable variables[MAX_VARIABLES];
int varCount = 0;  // Количество переменных

void setup() {
  Serial.begin(115200);
}

void loop() {
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
}

// Обработка print.serial()
void handlePrintSerial(String command) {
  int startIndex = command.indexOf('"');
  int endIndex = command.lastIndexOf('"');

  if (startIndex != -1 && endIndex != -1 && startIndex < endIndex) {
    // Если есть строка, выводим её
    String message = command.substring(startIndex + 1, endIndex);
    Serial.println(message);
  } else {
    // Если строки нет, ищем переменную
    String varName = command.substring(13, command.length() - 1);
    varName.trim();
    printVariable(varName);
  }
}

// Поиск и вывод переменной
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
      // Это строка
      String stringValue = valueString.substring(1, valueString.length() - 1);
      setVariable(varName, stringValue);
    } else if (valueString.indexOf('.') != -1) {
      // Это float
      float floatValue = valueString.toFloat();
      setVariable(varName, floatValue);
    } else {
      // Это int
      int intValue = valueString.toInt();
      setVariable(varName, intValue);
    }
  } else {
    Serial.println("Syntax error in assignment");
  }
}

// Функция для обработки cast
void handleCast(String command) {
  int commaIndex = command.indexOf(',');
  if (commaIndex != -1) {
    String type = command.substring(5, commaIndex);
    String value = command.substring(commaIndex + 1, command.length() - 1);
    type.trim();
    value.trim();

    if (type == "int") {
      int intValue = value.toInt();  // Преобразование в int
      Serial.println(intValue);
    } else if (type == "float") {
      float floatValue = value.toFloat();  // Преобразование в float
      Serial.println(floatValue);
    } else if (type == "string") {
      Serial.println(value);  // Преобразование в строку
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
    // Переменная уже существует, обновляем
    variables[index].intValue = value;
    variables[index].type = "int";
    Serial.print("Updated ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  } else {
    // Проверка на ограничение переменных
    if (varCount >= MAX_VARIABLES) {
      Serial.println("Error: Variable limit reached.");
      return;
    }

    // Переменная новая, добавляем
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
    // Переменная уже существует, обновляем
    variables[index].floatValue = value;
    variables[index].type = "float";
    Serial.print("Updated ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  } else {
    // Проверка на ограничение переменных
    if (varCount >= MAX_VARIABLES) {
      Serial.println("Error: Variable limit reached.");
      return;
    }

    // Переменная новая, добавляем
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
    // Переменная уже существует, обновляем
    variables[index].stringValue = value;
    variables[index].type = "string";
    Serial.print("Updated ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  } else {
    // Проверка на ограничение переменных
    if (varCount >= MAX_VARIABLES) {
      Serial.println("Error: Variable limit reached.");
      return;
    }

    // Переменная новая, добавляем
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
