#include <stdlib.h>

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

// Динамический массив для переменных
Variable* variables = NULL;
int varCount = 0;  // Количество переменных

void setup() {
  Serial.begin(115200);
}

void loop(){
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("print.serial(") && command.endsWith(")")) {
      handlePrintSerial(command);
    } 
    else if (command.indexOf('=') != -1) {
      handleAssignment(command);
    } 
    else {
      Serial.println("Unknown command");
    }
  }
}

// void executeCommand(String command) {
//     command.trim(); // Удаляем пробелы
//     if (command == "LED_ON") {
//         digitalWrite(ledPin, HIGH);
//         Serial.println("LED включен");
//     } else if (command == "LED_OFF") {
//         digitalWrite(ledPin, LOW);
//         Serial.println("LED выключен");
//     } else if (command.startsWith("DELAY_")) {
//         int delayTime = command.substring(6).toInt();
//         delay(delayTime);
//         Serial.print("Задержка на ");
//         Serial.print(delayTime);
//         Serial.println(" мс");
//     } else {
//         Serial.println("Неизвестная команда: " + command);
//     }
// }

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
  // Увеличиваем массив на одну переменную
  variables = (Variable*) realloc(variables, (varCount + 1) * sizeof(Variable));
  
  // Присваиваем новое имя и тип переменной
  variables[varCount].name = name;
  variables[varCount].type = type;
  
  // Увеличиваем количество переменных
  varCount++;
}
