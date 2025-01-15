#ifndef AUXFUNC_H
#define AUXFUNC_H
class Aux{
public:
    static bool isNumber(String str) {
        return str.length() > 0 && (str.toInt() != 0 || str == "0");
    }
    // Функция для обрезки пробелов
    static String trim_space(String str) {
      String result = "";
      for (int i = 0; i < str.length(); i++) {
        if (str[i] != ' ') {
          result += str[i];
        }
      }
      return result;
    }
};
Aux aux;
#endif