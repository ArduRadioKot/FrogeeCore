void setup() {
  // put your setup code here, to run once:

}

void loop() {
  Serial.begin(115200);
  String command = Serial.readString();
  parser(command);
}

void parser(String agrs){

  int startParams = agrs.indexOf('(');
  int endParams = agrs.indexOf(')');
  String command = agrs.substring(0, startParams);
  String param = agrs.substring(startParams + 1, endParams);
  /////
  if(command == "printSerial"){
    printSerial(param);
  }
  /////
  if(command == "printlnSerial"){
    printlnSerial(param);
  }
  /////
}


void printSerial(String params){

  if(params == (String)params){
  int startQuo = params.indexOf('"');
  int endQuo = params.lastIndexOf('"');
  String paramsStr = params.substring(startQuo + 1, endQuo);
  Serial.print(paramsStr);
 }else{
  Serial.print(params);
 }
}

void printlnSerial(String params){
 
  if(params == (String)params){
  int startQuo = params.indexOf('"');
  int endQuo = params.lastIndexOf('"');
  String paramsStr = params.substring(startQuo + 1, endQuo);
  Serial.println(paramsStr);
 }else{
  Serial.println(params);
 }
}