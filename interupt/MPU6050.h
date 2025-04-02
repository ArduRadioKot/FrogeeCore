// MPU6050.h
#include <basicMPU6050.h>

class MPU6050 {
public:
    void begin() {
        mpu.setup();
        mpu.setBias();
    }

    float getAccelerationX() {
        return mpu.ax();
    }

    float getAccelerationY() {
        return mpu.ay();
    }

    float getAccelerationZ() {
        return mpu.az();
    }

    float getGyroX() {
        return mpu.gx();
    }

    float getGyroY() {
        return mpu.gy();
    }

    float getGyroZ() {
        return mpu.gz();
    }

    float getTemperature() {
        return mpu.temp();
    }

    void updateBias() {
        mpu.updateBias();
    }

private:
    basicMPU6050<> mpu;
};

// Глобальный объект MPU6050
MPU6050 mpu6050; 