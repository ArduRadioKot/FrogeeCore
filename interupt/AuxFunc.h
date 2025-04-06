#ifndef AUXFUNC_H
#define AUXFUNC_H

#include <Arduino.h>

class Aux {
public:
    static bool isNumber(String str);
    static String trim_space(String str);
    
    // Упрощаем функцию и делаем её static inline
    static inline uint32_t strHash(const char* str) {
        uint32_t hash = 5381;
        while (char c = *str++) {
            hash = ((hash << 5) + hash) ^ c;
        }
        return hash;
    }
};

extern Aux aux;

#endif