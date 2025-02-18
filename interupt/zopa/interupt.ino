#include <stdlib.h>
#include <Arduino.h>
#include <math.h>
#define MAX_VARIABLES 20  // Ограничение на количество переменных
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
//void handlePinCommand(String input);
void handleAssignment(String command);
void handlePrintSerial(String args);
void handleCast(String args);
void printVariable(String varName);  // Добавлен прототип функции printVariable
bool isNumeric(String str);
void registerCommand(String name, void (*handler)(String args));
void processCommand(String input);
float calculate(String func);
float calcCommand(String expression);
void pinInclude(String command);
void setup() {
  Serial.begin(115200);

  // Регистрация команд
  registerCommand("print.serial", handlePrintSerial);
  registerCommand("pinMode", pinInclude);
  
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processCommand(command);
    pinInclude(command);
    calcCommand(command);
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

// Функция для обработки команд
void processCommand(String input) {
  int parenPos = input.indexOf('(');

  // Проверка на присвоение переменной с использованием "c(...)"
  if (input.indexOf('=') != -1 && input.indexOf("c(") != -1) {
    handleAssignment(input);  // Обработка присвоения с выражением "c(...)"
  } else if (parenPos != -1) {
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
    handleAssignment(input);  // Обработка обычного присвоения переменной
  } else {
    Serial.println("Unknown command: " + input);
  }
}

// Регистрация команды
// Изменяем calcCommand для работы с float, возвращаемым из calculate
float calcCommand(String expression) {
  int startPar = expression.indexOf('(');
  int endPar = expression.indexOf(')');
  
  if (startPar == -1 || endPar == -1 || startPar >= endPar) {
    Serial.println("Синтаксическая ошибка в команде.");
    return 0;
  }

  String massage = expression.substring(startPar + 1, endPar);
  float result = calculate(massage);  // Теперь calculate возвращает float

  return result;  // Возвращаем результат для использования в print.serial
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

// Обработка команды print.serial
void handlePrintSerial(String args) {
  args.trim();
  
  // Если аргумент начинается с "c(", вызываем calcCommand для вычисления значения
  if (args.startsWith("c(") && args.endsWith(")")) {
    float result = calcCommand(args);  // Получаем результат выражения c(...)
    
    // Отображаем результат
    if (result == (int)result) {
      Serial.println((int)result);  // Вывод как целое число, если результат целый
    } else {
      Serial.println(result, 4);  // Показываем до 4 знаков после запятой для float
    }
  } else {
    // Если аргумент не является выражением c(...), выводим как обычный текст или переменную
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
}


// Обработка присвоения переменной
void handleAssignment(String command) {
  int equalPos = command.indexOf('=');
  
  if (equalPos != -1) {
    String varName = command.substring(0, equalPos);
    varName.trim();

    if (isNumeric(varName)) {
      //Serial.println("Error: Variable name cannot be a number.");
      return;
    }

    String valueString = command.substring(equalPos + 1);
    valueString.trim();

    // Определение типа значения (int, float или строка)
    if(valueString.indexOf('c(') != -1){
      float result = calcCommand(command);  // Получаем результат выражения c(...)
    
    // Отображаем результат
    if (result == (int)result) {
      int intValue = (int)result;
      setVariable(varName, intValue);
    } else {
      float floatValue = ((float)result, 4);
      setVariable(varName, floatValue);
    }
    }else if (valueString.indexOf('"') != -1) {
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



float calculate(String func) {
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
      //Serial.println("Ошибка: Неверный операнд.");
      return 0;  // Возвращаем 0, чтобы указать на ошибку
    }

    // Выполняем арифметическую операцию
    float result = 0;
    if (command == "+") {
      result = operand_1 + operand_2;
    } else if (command == "-") {
      result = operand_1 - operand_2;
    } else if (command == "*") {
      result = operand_1 * operand_2;
    } else if (command == "/") {
      result = operand_1 / operand_2;
    } else if (command == "^") {
      result = pow(operand_1, operand_2);
    } else if (command == "%") {
      result = fmod(operand_1, operand_2);
    } else {
      //Serial.println("Ошибка: Неизвестная операция.");
      return 0;  // Указываем на ошибку операции
    }

    return result;  // Возвращаем результат вычисления
  } else {
    //Serial.println("Синтаксическая ошибка в команде.");
    return 0;  // Указываем на ошибку синтаксиса
  }
}


void pinInclude(String command){
  command.trim();
  int dot = command.indexOf('.');

  if(dot != -1){
    String pinStr = command.substring(0, dot);
    String mode = command.substring(dot + 1);

    int pin = pinStr.toInt();
    if (mode == "In"){
      pinMode(pin, INPUT);
    }else if (mode == "Out"){
      pinMode(pin, OUTPUT);
    }else{
      Serial.print ("Syntax error" + mode);
    }
  }
}







