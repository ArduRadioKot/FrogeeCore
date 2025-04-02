// Modules.h
#include "AuxFunc.h"
#include "Assign.h"
#include "Del.h"
#include "Pin.h"
#include "Print.h"
#include "Display.h"
#include "Arduino.h"
#include "CommandParser.h"
#include "bmp.h"
#include "MPU6050.h"

// Парсер команд.
class CommandParser {
public:
    void Parse(String args) {
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

        // Обработка command_1 через switch-case
        switch (aux.strHash(command_1.c_str())) {
            case (CommandHashes::PRINT):
                // Обработка подкоманд print
                Pprint(command_2, params);
                break;

            case CommandHashes::PIN:
                // Обработка подкоманд pin
                Ppin(command_2, params);
                break;

            case CommandHashes::VAR:
                Passign(command_2, params);
            case CommandHashes::DELAY:
                Pdel(command_2, params);

            case (CommandHashes::DISPLAY_CMD):  // Changed from DISPLAY
                // Обработка подкоманд display
                Pdisplay(command_2, params);
                break;

            case (CommandHashes::BME):
                // Обработка подкоманд bme
                Pbme(command_2, params);
                break;

            case (CommandHashes::MPU):
                // Обработка подкоманд mpu
                Pmpu(command_2, params);
                break;
        }
    }
    private:
      void Pprint(String cmd, String arg){
        switch (aux.strHash(cmd.c_str())) {
            case CommandHashes::PRINT_SERIAL:
                MyPrint::printSerial(arg);
                break;
            case CommandHashes::PRINT_LNSERIAL:
                MyPrint::printlnSerial(arg);
                break;
            }
      }
      static void Ppin(String cmd, String arg){
        arg = Aux::trim_space(arg);
        switch (aux.strHash(cmd.c_str())) {
           case CommandHashes::PIN_READ:
               Pin::read(arg);
               break;
           case CommandHashes::PIN_WRITE:
               Pin::write(arg);
               break;
           case CommandHashes::PIN_INIT:
               Pin::init(arg);
               break;
        }
      }
      void Passign(String cmd, String arg){
        //arg = Aux::trim_space(arg);
        Assign::tramit_var(arg);
      }
      void Pdel(String cmd, String arg){
        arg = Aux::trim_space(arg);
        del.Delay(arg);
      }
      void Pdisplay(String cmd, String arg){
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
      void Pbme(String cmd, String arg) {
        switch (aux.strHash(cmd.c_str())) {
            case CommandHashes::BME_INIT:
                bmeComm.bme_init();
                break;
            case CommandHashes::BME_TEMP:
                Serial.println(bmeComm.read_Temperature());
                break;
            case CommandHashes::BME_PRES:
                Serial.println(bmeComm.read_Pressure());
                break;
            case CommandHashes::BME_HUM:
                Serial.println(bmeComm.read_Humidity());
                break;
            case CommandHashes::BME_ALT:
                Serial.println(bmeComm.read_Altitude());
                break;
            case CommandHashes::BME_PRES_HPA:
                Serial.println(bmeComm.read_Pressure_hPa());
                break;
            case CommandHashes::BME_PRES_MMHG:
                Serial.println(bmeComm.read_Pressure_mmHg());
                break;
        }
      }
      void Pmpu(String cmd, String arg) {
        switch (aux.strHash(cmd.c_str())) {
            case CommandHashes::MPU_INIT:
                mpu6050.begin();
                break;
            case CommandHashes::MPU_ACCX:
                Serial.println(mpu6050.getAccelerationX());
                break;
            case CommandHashes::MPU_ACCY:
                Serial.println(mpu6050.getAccelerationY());
                break;
            case CommandHashes::MPU_ACCZ:
                Serial.println(mpu6050.getAccelerationZ());
                break;
            case CommandHashes::MPU_GYROX:
                Serial.println(mpu6050.getGyroX());
                break;
            case CommandHashes::MPU_GYROY:
                Serial.println(mpu6050.getGyroY());
                break;
            case CommandHashes::MPU_GYROZ:
                Serial.println(mpu6050.getGyroZ());
                break;
            case CommandHashes::MPU_TEMP:
                Serial.println(mpu6050.getTemperature());
                break;
        }
      }
};  // Added missing semicolon here

// Глобальные объекты.
CommandParser commandParser;
