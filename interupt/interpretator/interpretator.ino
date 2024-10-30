void setup() {
  // put your setup code here, to run once:

}

void loop() {
  Serial.begin(115200);
  String command = Serial.readString();
  parser(command);
}

bool isNumber(String str) {
    return str.toInt() != 0 || str == "0";
}

void parser(String agrs){

  int startParams = agrs.indexOf('(');
  int endParams = agrs.indexOf(')');
  String command = agrs.substring(0, startParams);
  String param = agrs.substring(startParams + 1, endParams);
  /////
  if(command == "printSerial"){
    printSerial(param);
  }else if(command == "printlnSerial"){
    printlnSerial(param);
  }
}


void printSerial(String params){
  if(params.startsWith("c(") && params.endsWith(")")){
    int start = params.indexOf('(');
    int end = params.lastIndexOf(')');
    String exp = params.substring(start + 1, end);
    float result = calculate(exp);
    if(result == (int)result){
      int resultInt = int(result);
      Serial.print(resultInt);
    }else{
      Serial.print(result);
    }
  }else if(params == (String)params){
    int startQuo = params.indexOf('"');
    int endQuo = params.lastIndexOf('"');
    String paramsStr = params.substring(startQuo + 1, endQuo);
    Serial.print(paramsStr);
 }else{
    Serial.print(params);
 }
}

void printlnSerial(String params){
  if(params.startsWith("c")){
    int start = params.indexOf('(');
    int end = params.lastIndexOf(')');
    String exp = params.substring(start + 1, end);
    float result = calculate(exp);
    if(result == (int)result){
      int resultInt = int(result);
      Serial.println(resultInt);
    }else{
      Serial.println(result);
    }
  }else if(params == (String)params){
    int startQuo = params.indexOf('"');
    int endQuo = params.lastIndexOf('"');
    String paramsStr = params.substring(startQuo + 1, endQuo);
    Serial.println(paramsStr);
  }else{
    Serial.println(params);
  }
}

float calculate(String params){
  float result = 0;
  int firstSpace = params.indexOf(' ');
  int secondSpace = params.indexOf(' ' + 2);
  String firstParamStr = params.substring(0, firstSpace);
  String operand = params.substring(firstSpace + 1, firstSpace + 2);
  String secondParamStr = params.substring(firstSpace + 2);
  if(isNumber(firstParamStr) && isNumber(secondParamStr)){
    float firstParam = firstParamStr.toFloat();
    float secondParam = secondParamStr.toFloat();
    if(operand == "+"){
      float result = firstParam + secondParam;
      return result;
    }
    if(operand == "-"){
      float result = firstParam - secondParam;
      return result;
    }
    if(operand == "*"){
      float result = firstParam * secondParam;
      return result;
    }
    if(operand == "/"){
      float result = firstParam / secondParam;
      return result;
    }

  
  }

}