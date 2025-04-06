#include "AuxFunc.h"

bool Aux::isNumber(String str) {
    return str.length() > 0 && (str.toInt() != 0 || str == "0");
}

String Aux::trim_space(String str) {
    String result = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] != ' ') {
            result += str[i];
        }
    }
    return result;
}

// Функция strHash теперь полностью определена в заголовочном файле