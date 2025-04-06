#include "Modules.h"
#include "Print.h"
#include "Pin.h"
#include "Del.h"
#include "Display.h"
#include "MPU6050.h"
#include "AuxFunc.h"
#include "Assign.h"

// Реализация статических методов
String CommandParser::find(String in) {
    if (aux.isNumber(in)) {
        return in;
    }
    
    int varIndex = Assign::find_var(in);
    if (varIndex != -1) {
        return variable[varIndex].var;
    }
    
    if (in.indexOf('.') != -1) {
        String cmd = in.substring(0, in.indexOf('.'));
        String subcmd = in.substring(in.indexOf('.') + 1);
        
        if (cmd == "mpu") {
            commandParser.Pmpu(subcmd, "");  // Вызываем через объект
            return "";
        }
        else if (cmd == "pin") {
            return commandParser.Ppin(subcmd, "");  // Вызываем через объект
        }
    }
    
    return "";
}

void CommandParser::Parse(String args) {
    int startParams = args.indexOf('(');
    int endParams = args.lastIndexOf(')');
    
    if (startParams == -1 || endParams == -1) return;
    
    String commands = args.substring(0, startParams);
    String params = args.substring(startParams + 1, endParams);
    
    if (commands.indexOf('.') != -1) {
        int dot = commands.indexOf('.');
        String cmd1 = commands.substring(0, dot);
        String cmd2 = commands.substring(dot + 1);
        
        switch (aux.strHash(cmd1.c_str())) {
            case CommandHashes::PRINT:
                Pprint(cmd2, params);
                break;
            case CommandHashes::PIN:
                Ppin(cmd2, params);
                break;
            case CommandHashes::VAR:
                Passign(cmd2, params);
                break;
            case CommandHashes::DELAY:
                Pdel(cmd2, params);
                break;
            case CommandHashes::DISPLAY_CMD:
                Pdisplay(cmd2, params);
                break;
            case CommandHashes::MPU:
                Pmpu(cmd2, params);
                break;
        }
    }
}

// Приватные методы обработки команд
void CommandParser::Pprint(String cmd, String arg) {
    switch (aux.strHash(cmd.c_str())) {
        case CommandHashes::PRINT_SERIAL:
            myPrint.printSerial(arg);
            break;
        case CommandHashes::PRINT_LNSERIAL:
            myPrint.printlnSerial(arg);
            break;
    }
}

String CommandParser::Ppin(String cmd, String arg) {
    arg = aux.trim_space(arg);
    switch (aux.strHash(cmd.c_str())) {
        case CommandHashes::PIN_READ:
            return String(Pin::read(arg));
        case CommandHashes::PIN_WRITE:
            Pin::write(arg);
            break;
        case CommandHashes::PIN_INIT:
            Pin::init(arg);
            break;
    }
    return "";
}

void CommandParser::Passign(String cmd, String arg) {
    Assign::tramit_var(arg);
}

void CommandParser::Pdel(String cmd, String arg) {
    arg = aux.trim_space(arg);
    del.Delay(arg);
}

void CommandParser::Pdisplay(String cmd, String arg) {
    switch (aux.strHash(cmd.c_str())) {
        case CommandHashes::DISPLAY_BEGIN:
            display.DisplayBegin();
            break;
        case CommandHashes::DISPLAY_TEST:
            display.DisplayTest();
            break;
        case CommandHashes::DISPLAY_CLEAR:
            display.DisplayClear();
            break;
        case CommandHashes::DISPLAY_PRINT:
            display.DisplayPrint(arg);
            break;
        case CommandHashes::DISPLAY_PIXEL:
            display.DisplayPixel(arg);
            break;
        case CommandHashes::DISPLAY_LINE:
            display.DisplayLine(arg);
            break;
        case CommandHashes::DISPLAY_CIRCLE:
            display.DisplayCircle(arg);
            break;
        case CommandHashes::DISPLAY_PRINTLN:
            display.DisplayPrintLN(arg);
            break;
        case CommandHashes::DISPLAY_SETSCALE:
            display.DisplaySetScale(arg);
            break;
    }
}

String CommandParser::Pmpu(String cmd, String arg) {
    switch (aux.strHash(cmd.c_str())) {
        case CommandHashes::AX:
            return String(mpu6050.getAccelerationX());
        case CommandHashes::AY:
            return String(mpu6050.getAccelerationY());
        case CommandHashes::AZ:
            return String(mpu6050.getAccelerationZ());
        case CommandHashes::GX:
            return String(mpu6050.getGyroX());
        case CommandHashes::GY:
            return String(mpu6050.getGyroY());
        case CommandHashes::GZ:
            return String(mpu6050.getGyroZ());
        case CommandHashes::BEGIN:
            mpu6050.begin();
            break;
        case CommandHashes::UPDBIAS:
            mpu6050.updateBias();
            break;
    }
    return "";
}

// Инициализация глобальных объектов
CommandParser commandParser;