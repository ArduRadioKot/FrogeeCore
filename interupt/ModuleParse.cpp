#include "moduleParse.h"
#include "CommandParse.h"

String ModuleParse::check(const String &in) {
    if (aux.isNumber(in)) {
        return in;
    } else if (Assign::find_var(in) != -1) {
        return variable[Assign::find_var(in)].var;
    } else {
        return mod(in);
    }
}

String ModuleParse::mod(String in) {
    return ::commandParser.find(in);  // Используем глобальный объект через ::
}