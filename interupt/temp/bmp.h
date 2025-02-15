#ifndef BMP_COMM_H
#define BMP_COMM_H

#include <Adafruit_BMP280.h>   

Adafruit_BMP280 bmp; 

class bmp{
    public:
    void bmp_init(){
        bmp.begin();
    }
    float read_Pressure(){
        return bmp.readPressure();
    }
    float read_Temperature(){
        bmp.readTemperature();
    }
    float read_altitude(){
        return bmp.readAltitude();
        }

};
bmp bmpComm;
#endif // BMP_COMM_H
