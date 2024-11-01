#include <U8g2lib.h>

// Инициализация дисплея с U8g2
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);
  u8g2.begin(); // Инициализация дисплея
  u8g2.clearDisplay(); // Очистка дисплея при старте
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n'); // Читаем до конца строки для уменьшения задержек
    parser(command);
  }
}

bool isNumber(const String &str) {
  return str.length() > 0 && (str.toInt() != 0 || str == "0");
}

void parser(const String &args) {
  int startParams = args.indexOf('(');
  int endParams = args.lastIndexOf(')');
  if (startParams == -1 || endParams == -1) return; // Проверка на корректный формат команды

  String command = args.substring(0, startParams);
  String param = args.substring(startParams + 1, endParams);
  
  if (command == "printSerial") {
    printSerial(param);
  } else if (command == "printlnSerial") {
    printlnSerial(param);
  } else if (command == "displayText") {
    displayText(param);
  } else if (command == "displayRect") {
    displayRect(param);
  } else if (command == "displayClear") {
    displayClear();
  } else if(command == "displayPixel"){
    displayPixel(param);
  }else{
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
  //if (firstSpace == -1 || secondSpace == -1) return 0;

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
  //return 0;
}

// Отображение текста на OLED
void displayText(const String &params) {
  int startQuo = params.indexOf('"');
  int endQuo = params.indexOf('"', startQuo + 1);
  int firstDot = params.indexOf(',', endQuo + 1);
  int secondDot = params.indexOf(',', firstDot + 1);
  if (startQuo == -1 || endQuo == -1 || startQuo >= endQuo) return;
  int x = params.substring(firstDot + 2, secondDot).toInt();
  int y = params.substring(secondDot + 2).toInt();
  

  String text = params.substring(startQuo + 1, endQuo);
  u8g2.setFont(u8g2_font_ncenB08_tr); // Устанавливаем читаемый шрифт
  u8g2.drawStr(x, y, text.c_str()); // Отображаем текст по координатам (0,12)
  u8g2.sendBuffer(); // Отправка буфера на дисплей
}

// Рисование прямоугольника на OLED
void displayRect(const String &params) {
  int comma1 = params.indexOf(',');
  int comma2 = params.indexOf(',', comma1 + 1);
  int comma3 = params.indexOf(',', comma2 + 1);

  if (comma1 == -1 || comma2 == -1 || comma3 == -1) return;

  int x = params.substring(0, comma1).toInt();
  int y = params.substring(comma1 + 2, comma2).toInt();
  int width = params.substring(comma2 + 2, comma3).toInt();
  int height = params.substring(comma3 + 2).toInt();

  u8g2.drawBox(x, y, width, height); // Рисуем прямоугольник с заданными размерами
  u8g2.sendBuffer(); // Обновляем дисплей
}

void displayClear(){

  //u8g2.clear();
  u8g2.clearDisplay();
  u8g2.sendBuffer();

}

void displayPixel(const String &params){

  int Dot = params.indexOf(',');
  int x = params.substring(0, Dot).toInt();
  int y = params.substring(Dot + 2).toInt();

  u8g2.drawPixel(x, y);
  u8g2.sendBuffer(); // Обновляем дисплей

}
