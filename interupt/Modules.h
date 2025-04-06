#ifndef MODULES_H
#define MODULES_H

#include "CommandParse.h"
#include "AuxFunc.h"
#include "Assign.h"
#include "Del.h"
#include "Pin.h"
#include "Print.h"
#include "Display.h"
#include "MPU6050.h"
extern CommandParser commandParser;
class CommandParser {
public:
    static String find(String in);
    void Parse(String args);
    
private:
    void Pprint(String cmd, String arg);
    String Ppin(String cmd, String arg);
    void Passign(String cmd, String arg);
    void Pdel(String cmd, String arg);
    void Pdisplay(String cmd, String arg);
    String Pmpu(String cmd, String arg);
};

extern CommandParser commandParser;

#endif