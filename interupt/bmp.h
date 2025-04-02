#ifndef BMP_COMM_H
#define BMP_COMM_H

#include <GyverBME280.h>
#include "moduleParse.h"

GyverBME280 bme;

class BME {
public:
    void bme_init() {
        bme.begin();
    }

    float read_Pressure() {
        return bme.readPressure();
    }

    float read_Temperature() {
        return bme.readTemperature();
    }

    float read_Humidity() {
        return bme.readHumidity();
    }

    float read_Altitude() {
        float pressure = bme.readPressure();
        return pressureToAltitude(pressure);
    }

    float read_Pressure_hPa() {
        return bme.readPressure() / 100.0F;
    }

    float read_Pressure_mmHg() {
        float pressure = bme.readPressure();
        return pressureToMmHg(pressure);
    }
};

BME bmeComm;

#endif // BMP_COMM_H
