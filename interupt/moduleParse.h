#ifndef MODULEPARSE_H
#define MODULEPARSE_H
#include "Assign.h"
#include "AuxFunc.h"
#include "MPU6050.h"
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
    static String module(const String &args){
        int startParams = args.indexOf('(');
        int endParams = args.lastIndexOf(')');
        String commands = args.substring(0, startParams);
        String command_1, command_2;

        // Разделение команды на основные и подкоманды.
        if (commands.indexOf('.') != -1) {
            int dot = commands.indexOf('.');
            command_1 = commands.substring(0, dot);
            command_2 = commands.substring(dot + 1);
        } else {
            command_1 = commands;
        }

        String params = args.substring(startParams + 1, endParams);


        if(command_1 == "mpu"){
          if(command_2 == "ax"){
            return String(mpu6050.getAccelerationX());
          } else if(command_2 == "ay"){
            return String(mpu6050.getAccelerationY());
          } else if(command_2 == "az"){
            return String(mpu6050.getAccelerationZ());
          }
        }
      
    }
};

ModuleParse mdp;

#endif