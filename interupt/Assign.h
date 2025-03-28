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
        int equal = params.indexOf('=');
        String name = params.substring(0, equal);
        String n_var = params.substring(equal + 1);
        int firstQuo = params.indexOf('"');
        int secondQuo = params.indexOf(firstQuo, '"');

        if (find_var(n_var) != -1) { // Если n_var - другая переменная.
            n_var = variable[find_var(n_var)].var;
        } else if (find_var(name) != -1) { // Если переменная с таким именем уже существует.
            reWrite_var(find_var(name), n_var);
        } else {
            // Создание новой переменной с определением типа.
            if ((firstQuo != -1) && (secondQuo != -1)) {
                assign_var(name, "str", n_var);
            } else if (Aux::isNumber(n_var)) {
                if (n_var.indexOf('.') == -1) {
                    assign_var(name, "int", n_var);
                } else {
                    assign_var(name, "float", n_var);
                }
            }
        }
    }
};

Assign assign;

#endif