#ifndef DEL_H
#define DEL_H
#include "moduleParse.h"
class Del{
public:
    void Delay(const String &param){
     int del = mdp.check(param).toInt();
     delay(del);
    }
};
Del del;
#endif