// Структура для хранения переменной с типом и значением
struct Variable {
  String name;
  String type;  // Тип переменной: "int", "float", "string"
  union {
    int intValue;
    float floatValue;
  };
  String stringValue;
};

// Массив для хранения переменных (максимум 10 переменных)
Variable variables[10];
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
  for (int i = 0; i < varCount; i++) {
    if (variables[i].name == name) {
      variables[i].intValue = value;
      variables[i].type = "int";
      Serial.print("Updated ");
      Serial.print(name);
      Serial.print(" = ");
      Serial.println(value);
      return;
    }
  }

  if (varCount < 10) {
    variables[varCount].name = name;
    variables[varCount].intValue = value;
    variables[varCount].type = "int";
    varCount++;
    Serial.print("Assigned ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  } else {
    Serial.println("Variable limit reached");
  }
}

// Установка переменной (float)
void setVariable(String name, float value) {
  for (int i = 0; i < varCount; i++) {
    if (variables[i].name == name) {
      variables[i].floatValue = value;
      variables[i].type = "float";
      Serial.print("Updated ");
      Serial.print(name);
      Serial.print(" = ");
      Serial.println(value);
      return;
    }
  }

  if (varCount < 10) {
    variables[varCount].name = name;
    variables[varCount].floatValue = value;
    variables[varCount].type = "float";
    varCount++;
    Serial.print("Assigned ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  } else {
    Serial.println("Variable limit reached");
  }
}

// Установка переменной (string)
void setVariable(String name, String value) {
  for (int i = 0; i < varCount; i++) {
    if (variables[i].name == name) {
      variables[i].stringValue = value;
      variables[i].type = "string";
      Serial.print("Updated ");
      Serial.print(name);
      Serial.print(" = ");
      Serial.println(value);
      return;
    }
  }

  if (varCount < 10) {
    variables[varCount].name = name;
    variables[varCount].stringValue = value;
    variables[varCount].type = "string";
    varCount++;
    Serial.print("Assigned ");
    Serial.print(name);
    Serial.print(" = ");
    Serial.println(value);
  } else {
    Serial.println("Variable limit reached");
  }
}
