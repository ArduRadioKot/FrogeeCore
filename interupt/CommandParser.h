// CommandParser.h
namespace CommandHashes {
    constexpr uint32_t PRINT = aux.strHash("print");
    constexpr uint32_t PIN = aux.strHash("pin");
    constexpr uint32_t VAR = aux.strHash("var");
    constexpr uint32_t DELAY = aux.strHash("delay");
    constexpr uint32_t DISPLAY_CMD = aux.strHash("display");  // Changed from DISPLAY
    constexpr uint32_t SD = aux.strHash("sd");
    constexpr uint32_t MPU = aux.strHash("mpu");


    // Подкоманды для "print"
    constexpr uint32_t PRINT_SERIAL = aux.strHash("Serial");
    constexpr uint32_t PRINT_LNSERIAL = aux.strHash("lnSerial");

    // Подкоманды для "pin"
    constexpr uint32_t PIN_READ = aux.strHash("read");
    constexpr uint32_t PIN_WRITE = aux.strHash("write");
    constexpr uint32_t PIN_INIT = aux.strHash("init");

    // Подкоманды для "display"
    constexpr uint32_t DISPLAY_BEGIN = aux.strHash("begin");
    constexpr uint32_t DISPLAY_TEST = aux.strHash("test");
    constexpr uint32_t DISPLAY_CLEAR = aux.strHash("clear");
    constexpr uint32_t DISPLAY_PRINT = aux.strHash("print");
    constexpr uint32_t DISPLAY_PRINTLN = aux.strHash("println");
    constexpr uint32_t DISPLAY_PIXEL = aux.strHash("pixel");
    constexpr uint32_t DISPLAY_LINE = aux.strHash("line");
    constexpr uint32_t DISPLAY_CIRCLE = aux.strHash("circle");
    constexpr uint32_t DISPLAY_SETSCALE = aux.strHash("setScale");

    // Подкоманды для "sd"
    constexpr uint32_t SD_START = aux.strHash("start");

    // Подкоманды для "mpu"
    constexpr uint32_t MPU_BEGIN = aux.strHash("begin");
    // 
    constexpr uint32_t LOOP = aux.strHash("loop");
}