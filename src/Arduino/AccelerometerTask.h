/*
* Life Catch - Fall Detection and Alerting
* copyright 2014 George Braxton IV
*
* AccelerometerTask.h
*
* Changelog:
*  (02/09/2014)
*      Accel variables now defined in ino file and declared extern in this file
* (02/02/2014)
*      Added #ifdef SERAILOUTPUT around print statements
* (01/24/2014)
*      File name changed from Accelerometer.h to AccelerometerTask.h.
*      DMP on 6050 no longer used. Accelerometer values read directly;
*      Much of the code was trimmed so that all accel data is updated every tick.
* (01/20/2014)
*      Initial Version. Accelerometer functions moved from Fall_Detector_XXX.ino
*      to its own header file.
*
* Accelerometer.h implements the state machine that communicates with the MPU6050
* to get the current values of the accelerometer and calculating the vector 
* magnitude and stores it in the global variable accel_vect_magnitude.
*/

#ifndef ACCELEROMETER_TASK_H
#define ACCELEROMETER_TASK_H

#include "I2Cdev.h"
#include "MPU6050.h"

#include "Config.h"

MPU6050 mpu;

extern int16_t accel_x, accel_y, accel_z, accel_vect_magnitude;

int16_t getMagnitude(int16_t x, int16_t y, int16_t z){
 return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

// *****************************************************************
// *                        SETUP                                  *
// *****************************************************************

void accelSetup(){
  #ifdef SERIALOUTPUT
    Serial.println(F("Accelerometer Setup Running"));
  #endif
  Wire.begin();
  TWBR = 24;
  mpu.initialize();
}

// *****************************************************************
// *                      STATE MACHINE                            *
// *****************************************************************

enum AccelStates{S_AC_GETACC};
uint8_t accelTask(uint8_t ac_state){
  switch(ac_state){
    case S_AC_GETACC:
      mpu.getAcceleration(&accel_x, &accel_y, &accel_z);
      #ifdef ACCELOUTPUT
        Serial.print(accel_x);
        Serial.print(F(", "));
        Serial.print(accel_y);
        Serial.print(F(", "));
        Serial.println(accel_z);
      #endif
      accel_vect_magnitude = getMagnitude(accel_x, accel_y, accel_z);
      ac_state = S_AC_GETACC;
      break;
    default:
      break;
  }
  return ac_state;
}

#endif //ACCELEROMETER_TASK_H
