#ifndef ASSIGN_H
#define ASSIGN_H

#include "AuxFunc.h"
#define Var_cnt 10  // Максимальное количество переменных.
int varCount = 0;   // Счетчик текущих переменных.

struct Mem {
    String name; // Имя переменной.
    String type; // Тип переменной (например, "int", "str", "float").
    String var;  // Значение переменной.
};

Mem variable[Var_cnt]; // Массив для хранения переменных.

// Класс для работы с переменными.
class Assign {
public:
    // Создает новую переменную.
    static void assign_var(const String &n_name, const String &n_type, const String &n_var) {
        if (varCount <= Var_cnt) {
            variable[varCount].name = n_name;
            variable[varCount].type = n_type;
            variable[varCount].var = n_var;
            varCount++;
        }
    }

    // Перезаписывает значение существующей переменной.
    static void reWrite_var(const int &id, const String &n_var) {
        variable[id].var = n_var;
    }

    // Находит индекс переменной по имени.
    static int find_var(const String &name_var) {
        for (int i = 0; i <= Var_cnt; i++) {
            if (variable[i].name == name_var) {
                return i;
            }
        }
        return -1;
    }

    // Обрабатывает строку с назначением переменной.
static void tramit_var(const String &params) {
    int firstQuo = params.indexOf('"');
    int secondQuo = params.indexOf('"', firstQuo + 1);  // Исправлено: ищем вторую кавычку после первой
    
    String params1 = Aux::trim_space(params.substring(0, firstQuo != -1 ? firstQuo : params.length()));
    int equal = params1.indexOf('=');
    
    if (equal == -1) return;  // Нет знака равенства - некорректный синтаксис
    
    String name = params1.substring(0, equal);
    String n_var = params1.substring(equal + 1);
    
    // Если есть кавычки - это строковая переменная
    if (firstQuo != -1 && secondQuo != -1) {
        String strValue = params.substring(firstQuo + 1, secondQuo);
        assign_var(name, "str", strValue);
    }
    // Если переменная уже существует
    else if (find_var(n_var) != -1) {
        String existingValue = variable[find_var(n_var)].var;
        reWrite_var(find_var(name), existingValue);
    }
    // Числовые переменные
    else if (Aux::isNumber(n_var)) {
        if (n_var.indexOf('.') == -1) {
            assign_var(name, "int", n_var);
        } else {
            assign_var(name, "float", n_var);
        }
    }
    // Если ничего не подошло, возможно, это имя другой переменной
    else {
        int varIndex = find_var(n_var);
        if (varIndex != -1) {
            n_var = variable[varIndex].var;
            reWrite_var(find_var(name), n_var);
        }
    }
}
};

Assign assign;

#endif