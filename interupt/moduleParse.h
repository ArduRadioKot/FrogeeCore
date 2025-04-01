#ifndef MODULEPARSE_H
#define MODULEPARSE_H
#include "Assign.h"
#include "AuxFunc.h"
//#include "Pin.h"
class ModuleParse{
public:
    static String check(const String &in){
      if(aux.isNumber(in)){
        return in;
      }
      else if(Assign::find_var(in) != -1){
        return (variable[Assign::find_var(in)].var);
      }else{
        return module(in);
      }

    }
private:
    static String module(const String &in){
      
      return "0";
      
    }
};

ModuleParse mdp;

#endif