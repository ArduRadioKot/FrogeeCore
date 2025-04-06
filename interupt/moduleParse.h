#ifndef MODULEPARSE_H
#define MODULEPARSE_H

#include "AuxFunc.h"
#include "Assign.h"

class CommandParser;

class ModuleParse {
public:
    String check(const String &in);
    String mod(String in);
};

extern ModuleParse mdp;

#endif