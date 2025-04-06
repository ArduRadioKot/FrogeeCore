#ifndef ASSIGN_H
#define ASSIGN_H

#include "AuxFunc.h"

#define Var_cnt 10

struct Mem {
    String name;
    String type;
    String var;
};

extern Mem variable[Var_cnt];
extern int varCount;

class Assign {
public:
    static void assign_var(const String &n_name, const String &n_type, const String &n_var);
    static void reWrite_var(const int &id, const String &n_var);
    static int find_var(const String &name_var);
    static void tramit_var(const String &params);
};

extern Assign assign;

#endif