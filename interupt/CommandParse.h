#ifndef COMMANDPARSE_H
#define COMMANDPARSE_H

#include "AuxFunc.h"
static_assert(noexcept(aux.strHash("test")), "strHash must be constexpr");
namespace CommandHashes {
    const uint32_t PRINT = aux.strHash("print");
    const uint32_t PIN = aux.strHash("pin");
    const uint32_t VAR = aux.strHash("var");
    const uint32_t DELAY = aux.strHash("delay");
    const uint32_t DISPLAY_CMD = aux.strHash("display");
    const uint32_t MPU = aux.strHash("mpu");
    // ... остальные хеши
    // Подкоманды
    constexpr uint32_t PRINT_SERIAL = aux.strHash("Serial");
    constexpr uint32_t PRINT_LNSERIAL = aux.strHash("lnSerial");
    constexpr uint32_t PIN_READ = aux.strHash("read");
    constexpr uint32_t PIN_WRITE = aux.strHash("write");
    constexpr uint32_t PIN_INIT = aux.strHash("init");
    constexpr uint32_t DISPLAY_BEGIN = aux.strHash("begin");
    constexpr uint32_t DISPLAY_TEST = aux.strHash("test");
    constexpr uint32_t DISPLAY_CLEAR = aux.strHash("clear");
    constexpr uint32_t DISPLAY_PRINT = aux.strHash("print");
    constexpr uint32_t DISPLAY_PRINTLN = aux.strHash("println");
    constexpr uint32_t DISPLAY_PIXEL = aux.strHash("pixel");
    constexpr uint32_t DISPLAY_LINE = aux.strHash("line");
    constexpr uint32_t DISPLAY_CIRCLE = aux.strHash("circle");
    constexpr uint32_t DISPLAY_SETSCALE = aux.strHash("setScale");
    constexpr uint32_t AX = aux.strHash("ax");
    constexpr uint32_t AY = aux.strHash("ay");
    constexpr uint32_t AZ = aux.strHash("az");
    constexpr uint32_t GX = aux.strHash("gx");
    constexpr uint32_t GY = aux.strHash("gy");
    constexpr uint32_t GZ = aux.strHash("gz");
    constexpr uint32_t BEGIN = aux.strHash("begin");
    constexpr uint32_t UPDBIAS = aux.strHash("updBias");
}

#endif