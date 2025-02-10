#ifndef BMP_COMM_H
#define BMP_COMM_H

#include <Adafruit_BMP280.h>   

Adafruit_BMP280 bmp; 

class bmp{
    private:
    void bmp_init(){
        bmp.begin();
    }
    void read_Pressure(){
        return bmp.readPressure();
    }
    void read_Temperature(){
        bmp.readTemperature();
    }
    void read_altitude(){
        return bmp.readAltitude();
        }

};
bmp bmpComm;
#endif // BMP_COMM_H
