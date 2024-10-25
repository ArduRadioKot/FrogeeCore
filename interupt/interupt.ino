#include <stdlib.h>
#include <Arduino.h>
#include <math.h>
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
  bool isFloat;
};

// Массив доступных пинов



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
void calculate(String func);

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
    handlePinCommand(command);
    calculate(command);
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
    //Serial.println("Syntax error");
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
  int startIndex = args.indexOf('(');
  int endIndex = args.lastIndexOf(')');
  
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
    //Serial.println("Syntax error in assignment");
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


// Поиск переменной по имени
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


// Функция для работы с пинами
/*void handlePinCommand(String input) {
  // Проверка на запись пина (writePin.номер.состояние)
  if (input.startsWith("writePin.")) {
    int dotPos = input.indexOf('.', 9);  // Поиск второй точки
    if (dotPos != -1) {
      // Извлечение номера пина и состояния
      String pinNumberStr = input.substring(9, dotPos);
      String pinStateStr = input.substring(dotPos + 1);
      
      int pinNumber = pinNumberStr.toInt();
      int pinState = pinStateStr.toInt();
      
      // Проверка допустимости номера пина и состояния
      if (pinNumber >= 0 && (pinState == HIGH || pinState == LOW)) {
        pinMode(pinNumber, OUTPUT);
        digitalWrite(pinNumber, pinState);
        Serial.print("Pin ");
        Serial.print(pinNumber);
        Serial.print(" set to ");
        Serial.println(pinState == HIGH ? "HIGH" : "LOW");
      } else {
        Serial.println("Invalid pin number or state.");
      }
    } else {
      Serial.println("Syntax error in writePin command.");
    }
  }
  
  // Проверка на чтение пина (readPin.номер)
  else if (input.startsWith("readPin.")) {
    String pinNumberStr = input.substring(8);
    int pinNumber = pinNumberStr.toInt();
    
    if (pinNumber >= 0) {
      pinMode(pinNumber, INPUT);
      int pinValue = digitalRead(pinNumber);
      Serial.print("Pin ");
      Serial.print(pinNumber);
      Serial.print(" is ");
      Serial.println(pinValue == HIGH ? "HIGH" : "LOW");
    } else {
      Serial.println("Invalid pin number.");
    }
  }
}*/
bool parseOperand(String operandStr, float &value) {
  int index = findVariable(operandStr);
  if (index != -1) {
    if (variables[index].isFloat) {
      value = variables[index].floatValue;
    } else {
      value = (float)variables[index].intValue;
    }
    return true;
  } else if (operandStr.indexOf('.') != -1) {
    value = operandStr.toFloat();
    return true;
  } else {
    value = (float)operandStr.toInt();
    return true;
  }
}



void calculate(String func) {
  int first_space = func.indexOf(' ');
  int second_space = func.indexOf(' ', first_space + 1);

  if (first_space != -1 && second_space != -1) {
    String operand_1Str = func.substring(0, first_space);
    String command = func.substring(first_space + 1, second_space);
    String operand_2Str = func.substring(second_space + 1);

    float operand_1 = 0;
    float operand_2 = 0;

    // Преобразуем операнды
    if (!parseOperand(operand_1Str, operand_1) || !parseOperand(operand_2Str, operand_2)) {
      Serial.println("Error: Invalid operand.");
      return;
    }

    // Выполнение арифметической операции
    float result = 0;
    if (command == "+") {
      result = operand_1 + operand_2;
    } else if (command == "-") {
      result = operand_1 - operand_2;
    } else if (command == "*") {
      result = operand_1 * operand_2;
    } else if (command == "/") {
      if (operand_2 != 0) {
        result = operand_1 / operand_2;
      } else {
        Serial.println("Error: Division by zero.");
        return;
      }
    } else if (command == "^") {
      result = pow(operand_1, operand_2);
    } else if (command == "%") {
      if ((int)operand_2 != 0) {
        result = fmod(operand_1, operand_2);
      } else {
        Serial.println("Error: Division by zero in modulo.");
        return;
      }
    } else {
      //Serial.println("Error: Unknown operation.");
      return;
    }

    // Выводим результат с проверкой на тип (целое или дробное)
    Serial.print("Result: ");
    if (result == (int)result) {
      Serial.println((int)result);
    } else {
      Serial.println(result, 4);  // 4 знака после запятой для вещественных чисел
    }
  } else {
    //Serial.println("Syntax error in calculate command.");
  }
}


// Функция для работы с пинами по номерам (без массива)
void handlePinCommand(String input) {
  Serial.print("Received command: ");
  Serial.println(input);

  // Разделим строку на части по точкам
  int firstDot = input.indexOf('.');
  int secondDot = input.indexOf('.', firstDot + 1);

  // Проверим, что команда имеет правильный формат с двумя точками
  if (firstDot != -1 && secondDot != -1) {
    // Извлекаем команду (writePin или readPin)
    String command = input.substring(0, firstDot);
    // Извлекаем номер пина
    String pinNumberStr = input.substring(firstDot + 1, secondDot);
    // Извлекаем состояние пина (только для writePin)
    String pinStateStr = input.substring(secondDot + 1);

    int pinNumber = pinNumberStr.toInt();  // Преобразуем номер пина в число

    // Если это команда для записи на пин
    if (command == "writePin") {
      Serial.println("Detected writePin command.");

      int pinState = (pinStateStr == "HIGH") ? HIGH : (pinStateStr == "LOW") ? LOW : -1;

      if (pinNumber >= 0 && (pinState == HIGH || pinState == LOW)) {
        Serial.print("Setting pin ");
        Serial.print(pinNumber);
        Serial.print(" to state ");
        Serial.println(pinState == HIGH ? "HIGH" : "LOW");

        pinMode(pinNumber, OUTPUT);
        digitalWrite(pinNumber, pinState);
      } else {
        Serial.println("Invalid pin number or state.");
      }
    }
    // Если это команда для чтения с пина
    else if (command == "readPin") {
      Serial.println("Detected readPin command.");

      if (pinNumber >= 0) {
        pinMode(pinNumber, INPUT);
        int pinValue = digitalRead(pinNumber);

        Serial.print("Pin ");
        Serial.print(pinNumber);
        Serial.print(" is ");
        Serial.println(pinValue == HIGH ? "HIGH" : "LOW");
      } else {
        Serial.println("Invalid pin number.");
      }
    }
    // Если команда не распознана
    else {
      Serial.println("Unknown command.");
    }
  } else {
    //Serial.println("Syntax error in command.");
  }
}



