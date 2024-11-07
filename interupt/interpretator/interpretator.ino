#include <U8g2lib.h>
#define Val_Int 10

// Инициализация дисплея с U8g2
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

int int_val[Val_Int];
String intVal_name[Val_Int];
int varCount = 0;  

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.clearDisplay();
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n'); 
    parser(command);
  }
}

bool isNumber(const String &str) {
  return str.length() > 0 && (str.toInt() != 0 || str == "0");
}

void parser(const String &args) {
  if (args.indexOf('.') != -1) {
    int dot = args.indexOf('.');
    int startParams = args.indexOf('(');
    int endParams = args.lastIndexOf(')');

    String command_1 = args.substring(0, dot);
    String command_2 = args.substring(dot + 1, startParams);
    String params = args.substring(startParams + 1, endParams);

    if (command_1 == "print") {
      if (command_2 == "Serial") {
        printSerial(params);
      } else if (command_2 == "lnSerial") {
        printlnSerial(params);
      } else if (command_2 == "displayText") {
        displayText(params);
      } else if (command_2 == "displayRect") {
        displayRect(params);
      } else if (command_2 == "displayPixel") {
        displayPixel(params);
      } else {
        Serial.println("Error: command not found");
      }
    } else if (command_1 == "pin") {
      if(command_2 == "read"){
        pinRead(params);
      }else if(command_2 == "write"){
        pinWrite(params);
      }else if(command_2 == "init"){
        pinInit(params);
      }
    } else {
      Serial.println("Error: command not found");
    }
  } else if (args.substring(0, args.indexOf('(')) == "displayClear") {
    displayClear();
  } else if (args.indexOf(' = ') != -1) {
    int equal = args.indexOf('=');
    String name = args.substring(0, equal);
    String value = args.substring(equal + 2);
    var_Set(name, value);
  } else {
    Serial.println("Error: command not found");
  }
}

void printSerial(const String &params) {
  if (params.startsWith("c")) {
    float result = calculate(params.substring(params.indexOf('(') + 1, params.lastIndexOf(')')));
    Serial.print(result == (int)result ? (int)result : result);
  } else if (params.startsWith("\"") && params.endsWith("\"")) {
    Serial.print(params.substring(1, params.length() - 1));
  } else {
    Serial.print(params);
  }
}

void printlnSerial(const String &params) {
  if (params.startsWith("c")) {
    float result = calculate(params.substring(params.indexOf('(') + 1, params.lastIndexOf(')')));
    Serial.println(result == (int)result ? (int)result : result);
  } else if (params.startsWith("\"") && params.endsWith("\"")) {
    Serial.println(params.substring(1, params.length() - 1));
  } else {
    Serial.println(params);
  }
}

float calculate(const String &params) {
  int firstSpace = params.indexOf(' ');
  int secondSpace = params.indexOf(' ', firstSpace + 1);

  String firstParamStr = params.substring(0, firstSpace);
  String operand = params.substring(firstSpace + 1, secondSpace);
  String secondParamStr = params.substring(secondSpace + 1);

  if (isNumber(firstParamStr) && isNumber(secondParamStr)) {
    float firstParam = firstParamStr.toFloat();
    float secondParam = secondParamStr.toFloat();

    if (operand == "+") return firstParam + secondParam;
    if (operand == "-") return firstParam - secondParam;
    if (operand == "*") return firstParam * secondParam;
    if (operand == "/") return secondParam != 0 ? firstParam / secondParam : 0;
  }
}

void displayText(const String &params) {
  int startQuo = params.indexOf('"');
  int endQuo = params.indexOf('"', startQuo + 1);
  int firstDot = params.indexOf(',', endQuo + 1);
  int secondDot = params.indexOf(',', firstDot + 1);

  if (startQuo == -1 || endQuo == -1 || startQuo >= endQuo) return;

  int x = params.substring(firstDot + 2, secondDot).toInt();
  int y = params.substring(secondDot + 2).toInt();
  String text = params.substring(startQuo + 1, endQuo);

  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(x, y, text.c_str()); 
  u8g2.sendBuffer();
}

void displayRect(const String &params) {
  int comma1 = params.indexOf(',');
  int comma2 = params.indexOf(',', comma1 + 1);
  int comma3 = params.indexOf(',', comma2 + 1);

  if (comma1 == -1 || comma2 == -1 || comma3 == -1) return;

  int x = params.substring(0, comma1).toInt();
  int y = params.substring(comma1 + 2, comma2).toInt();
  int width = params.substring(comma2 + 2, comma3).toInt();
  int height = params.substring(comma3 + 2).toInt();

  u8g2.drawBox(x, y, width, height); 
  u8g2.sendBuffer(); 
}

void displayClear() {
  u8g2.clearDisplay();
  u8g2.sendBuffer();
}

void displayPixel(const String &params) {
  int dot = params.indexOf(',');
  int x = params.substring(0, dot).toInt();
  int y = params.substring(dot + 2).toInt();

  u8g2.drawPixel(x, y);
  u8g2.sendBuffer();
}

void var_Set(const String &name, const String &value) {
  int value_int = value.toInt();

  if (find_Var(name) == -1) {
    int_val[varCount] = value_int;
    intVal_name[varCount] = name;
    varCount++;
  } else {
    int varLoca = find_Var(name);
    int_val[varLoca] = value_int;
  }
}

int find_Var(const String &name) {
  for (int i = 0; i < Val_Int; i++) {
    if (intVal_name[i] == name) {
      return i;
    }
  }
  return -1;
}

void pinInit(const String &params) {
  int dot = params.indexOf(',');
  String pinStr = params.substring(0, dot);
  String mode = params.substring(dot + 2);
  int pin = pinStr.toInt();
  if(mode == "input"){
    pinMode(pin, INPUT);
  }else if(mode == "output"){
    pinMode(pin, OUTPUT);
  }
  Serial.println("init " + String(pin));
}

void pinRead(const String &params){
  int dot = params.indexOf(',');
  String type = params.substring(0, dot);
  String pinStr = params.substring(dot + 2);
  int pin = pinStr.toInt();
  
  if(type == "an"){
    int value = analogRead(pin);
    Serial.println(value);
  } else if(type == "dg"){
    int value = digitalRead(pin);
    Serial.println(value);
  }
}

void pinWrite(const String &params){
  int firstDot = params.indexOf(',');
  int secondDot = params.indexOf(',', firstDot + 1);
  
  String type = params.substring(0, firstDot);
  String pinStr = params.substring(firstDot + 2, secondDot);
  String valueStr = params.substring(secondDot + 2);
  
  int pin = pinStr.toInt();
  int value = valueStr.toInt();
  
  if(type == "an"){
    analogWrite(pin, value);
  } else if(type == "dg"){
    digitalWrite(pin, value == 1 ? HIGH : LOW);
    Serial.println(value);
  }
}
