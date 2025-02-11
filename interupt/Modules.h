#include "AuxFunc.h"
#include "Assign.h"
#include "Del.h"
#include "Pin.h"
#include "Print.h"
#include "Display.h"
#include "Arduino.h"


// Парсер команд.
class CommandParser {
public:
    void parse(String args) {
        int startParams = args.indexOf('(');
        int endParams = args.lastIndexOf(')');
        String commands = args.substring(0, startParams);
        String command_1, command_2;

        // Разделение команды на основные и подкоманды.
        if (commands.indexOf(".") != 1) {
            int dot = commands.indexOf('.');
            command_1 = commands.substring(0, dot);
            command_2 = commands.substring(dot + 1);
        } else {
            command_1 = commands;
        }

        String params = args.substring(startParams + 1, endParams);
        // Обработка команд.
        //Print.h
        if (command_1 == "print") {
            if (command_2 == "Serial") {
                MyPrint::printSerial(params);
            } else if (command_2 == "lnSerial") {
                MyPrint::printlnSerial(params);
            } else {
                //Serial.println("Error: command not found");
            }
        //Pin.h
        } else if (command_1 == "pin") {
          params = Aux::trim_space(params);
            if (command_2 == "read") {
                Pin::read(params);
            } else if (command_2 == "write") {
                Pin::write(params);
            } else if (command_2 == "init") {
                Pin::init(params);
            } else {
                //Serial.println("Error: command not found");
            }
        //Assign.h
        } else if (command_1 == "assign") {
          params = Aux::trim_space(params);
            Assign::tramit_var(params);
        //Del.h
        } else if (command_1 == "delay"){
          params = Aux::trim_space(params);
            del.Delay(params); 
        } //Display.h
        else if (command_1 == "display"){
          if(command_2 == "begin"){
              display.DisplayBegin();
          }
          else if(command_2 == "test"){
              display.DisplayTest();
          }
          else if(command_2 == "clear"){
              display.DisplayClear();
          }
          else if(command_2 == "print"){
              display.DisplayPrint(params);
          }
        }
        else {
          //Serial.println("Error: command not found");
        }
    }
};

// Глобальные объекты.
CommandParser commandParser;


