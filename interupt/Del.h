#ifndef DEL_H
#define DEL_H
class Del{
public:
    void Delay(const String &param){
     int del = param.toInt();
     delay(del);
    }
};
Del del;
#endif