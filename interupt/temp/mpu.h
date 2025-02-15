#ifndef MPU_COMMAND_H
#define MPU_COMMAND_H

#include <Adafruit_ADXL345_U.h> 
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345); 

class mpu{
  public:
    void mpu_init(){
         accel.begin();
         accel.setRange(ADXL345_RANGE_16_G);
    }

    void mpu_update(){
          sensors_event_t event;
          accel.getEvent(&event);
    }

    void mpuAccelerationX(){
          return event.acceleration.x;
    }

    void mpuAccelerationY(){
          return event.acceleration.y;
    }

    void mpuAccelerationZ(){
          return event.acceleration.y;
    }

};
mpu mpuComm;
#endif
