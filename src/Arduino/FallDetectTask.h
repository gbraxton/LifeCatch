/*
* Life Catch - Fall Detection and Alerting
* copyright 2014 George Braxton IV
*
* FallDetectTask.h
*
* Changelog:
*  (02/09/2014)
*      Optimizations to reduce false positives
* (02/02/2014)
*      Added #ifdef SERIALOUTPUT around serial print statements
* (02/01/2014)
*      Added alert tone using tone() function
*        elapsed_tone used to cycle alert tone on and off at 1/2 s intervals
*      Added vibration alert. Also toggles on and off at 1/2 s intervals using elapsed_vibration.
* (01/24/2014)
*      State names changed
*      Thresholds now checked against vector magnitude
*      Active alert sends SMS through shell command
* (01/20/2014)
*      Initial Version. Fall Detection Task routines.
*
* FallDetectTask.h implements the state machine that serves as the fall
* detection algorithm using the data in the global variable accel_vect_magnitude.
*/

#ifndef FALLDETECTTASK_H
#define FALLDETECTTASK_H

#include "MPU6050.h"
#include "Config.h"
#include "TaskScheduler.h"

//Shared Globals from LifeCatch_XXX.ino
extern int16_t accel_x, accel_y, accel_z, accel_vect_magnitude;
//extern bool activeAlert;
extern TaskScheduler* scheduler;

// *****************************************************************
// *                        SETUP                                  *
// *****************************************************************

void fallDetectSetup(){
  #ifdef SERIALOUTPUT
    Serial.println(F("Fall Detect Setup Running"));
  #endif
  // configure output pins
  pinMode(ALERT_LED_PIN, OUTPUT);
  pinMode(INTERNET_LED_PIN, OUTPUT);
  pinMode(VIBRATION_MOTOR_PIN, OUTPUT);
  pinMode(DISMISS_ALERT_PIN, INPUT_PULLUP);
  pinMode(EMERGENCY_ALERT_PIN, INPUT_PULLUP);
}

// *****************************************************************
// *                      STATE MACHINE                            *
// *****************************************************************

// elapsed time for wait states
uint32_t elapsed_ticks = 0;
uint32_t freefall_count = 0;
uint32_t rotation_count = 0;
uint32_t nojump_count = 0;
uint32_t elapsed_tone = 0;
uint32_t elapsed_vibration = 0;
int16_t jump_x, jump_y, jump_z;
bool jump_fl = false;
enum Orientation{x_up, y_up, z_up} last_orientation;
enum FDSTATES{ S_FD_FF_COUNT, S_FD_WAITIMPACT, S_FD_FALLDETECT, S_FD_ACTIVEALERT };
uint8_t fallDetectTask(uint8_t fd_state){ 
  switch(fd_state){
    case S_FD_FF_COUNT:
      if( (accel_x > accel_y) && (accel_x > accel_z)){
          last_orientation = x_up;
        } else if( (accel_y > accel_x) && (accel_y > accel_z)){
          last_orientation = y_up;
        } else {
          last_orientation = z_up;
        }
      if(accel_vect_magnitude > JUMP_THRESHOLD){ //jump detection
        if(last_orientation==x_up){
          jump_x = accel_x; jump_y = 0; jump_z = 0;
        } else if(last_orientation==y_up){
          jump_y = accel_y; jump_x = 0; jump_z = 0;
        } else {
          jump_z = accel_z; jump_y = 0; jump_x = 0;
        }
        jump_fl = true;
      } else if(nojump_count > (JUMP_TIMEOUT_MS/FALLDETECTTASKPERIOD_MS)){
        jump_x = jump_y = jump_z = 0;
        nojump_count = 0;
        jump_fl = false;
      } else {
        nojump_count++;
      }
      if(digitalRead(EMERGENCY_ALERT_PIN) == LOW){
        elapsed_ticks = 0;
        freefall_count = 0;
        digitalWrite(ALERT_LED_PIN, HIGH);
        scheduler->tsStopTimer();
        Process sendsms;
        if(sendsms.runShellCommand("/mnt/sda1/LifeCatch/sendAlert.sh button") == 0){
          #ifdef SERIALOUTPUT
            Serial.println(F("SMS Sent"));
          #endif
        } else {
          #ifdef SERIALOUTPUT
            Serial.println(F("SMS could not be Sent"));
          #endif
        }
        scheduler->tsStartTimer();
        tone(SPEAKER_PIN, ALERT_TONE);
        fd_state = S_FD_ACTIVEALERT;
      } else if(accel_vect_magnitude < FFTHRESHOLD){
        #ifdef SERIALOUTPUT
          Serial.println(F("FREEFALL DETECTED"));
        #endif
        freefall_count++;
      } else if(freefall_count >= FFDURATION_MS/FALLDETECTTASKPERIOD_MS){
        elapsed_ticks = 0;
        freefall_count = 0;
        fd_state = S_FD_WAITIMPACT;
      } else {
        fd_state = S_FD_FF_COUNT;
        freefall_count = 0;
      }
      break;
    case S_FD_WAITIMPACT:
      if(digitalRead(EMERGENCY_ALERT_PIN) == LOW){
        elapsed_ticks = 0;
        freefall_count = 0;
        digitalWrite(ALERT_LED_PIN, HIGH);
        scheduler->tsStopTimer();
        Process sendsms;
        if(sendsms.runShellCommand("/mnt/sda1/LifeCatch/sendAlert.sh button") == 0){
          #ifdef SERIALOUTPUT
            Serial.println(F("SMS Sent"));
          #endif
        } else {
          #ifdef SERIALOUTPUT
            Serial.println(F("SMS could not be Sent"));
          #endif
        }
        scheduler->tsStartTimer();
        tone(SPEAKER_PIN, ALERT_TONE);
        fd_state = S_FD_ACTIVEALERT;
      } else if(accel_vect_magnitude > IMPLOWERTHRESHOLD && last_orientation == x_up && accel_x > accel_y && accel_x > accel_z && !jump_fl){
        fd_state = S_FD_FALLDETECT;
        elapsed_ticks = 0;
        digitalWrite(ALERT_LED_PIN, HIGH);
        #ifdef SERIALOUTPUT
          Serial.print(F("**********A FALL HAS BEEN DETECTED**********\n"));
        #endif
          elapsed_tone = 0;
          elapsed_vibration = 0;
          tone(SPEAKER_PIN, ALERT_TONE, 500);
          digitalWrite(VIBRATION_MOTOR_PIN, HIGH);
       } else if(accel_vect_magnitude > IMPLOWERTHRESHOLD && last_orientation == y_up && accel_y > accel_x && accel_y > accel_z && !jump_fl){
        fd_state = S_FD_FALLDETECT;
        elapsed_ticks = 0;
        digitalWrite(ALERT_LED_PIN, HIGH);
        #ifdef SERIALOUTPUT
          Serial.print(F("**********A FALL HAS BEEN DETECTED**********\n"));
        #endif
          elapsed_tone = 0;
          elapsed_vibration = 0;
          tone(SPEAKER_PIN, ALERT_TONE, 500);
          digitalWrite(VIBRATION_MOTOR_PIN, HIGH);
      } else if(accel_vect_magnitude > IMPLOWERTHRESHOLD && last_orientation == z_up && accel_z > accel_y && accel_z > accel_x && !jump_fl){
        fd_state = S_FD_FALLDETECT;
        elapsed_ticks = 0;
        digitalWrite(ALERT_LED_PIN, HIGH);
        #ifdef SERIALOUTPUT
          Serial.print(F("**********A FALL HAS BEEN DETECTED**********\n"));
        #endif
          elapsed_tone = 0;
          elapsed_vibration = 0;
          tone(SPEAKER_PIN, ALERT_TONE, 500);
          digitalWrite(VIBRATION_MOTOR_PIN, HIGH);
      } else if(accel_vect_magnitude > IMPTHRESHOLD){
        fd_state = S_FD_FALLDETECT;
        elapsed_ticks = 0;
        digitalWrite(ALERT_LED_PIN, HIGH);
        #ifdef SERIALOUTPUT
          Serial.print(F("**********A FALL HAS BEEN DETECTED**********\n"));
        #endif
          elapsed_tone = 0;
          elapsed_vibration = 0;
          tone(SPEAKER_PIN, ALERT_TONE, 500);
          digitalWrite(VIBRATION_MOTOR_PIN, HIGH);
      } else if(elapsed_ticks < IMPTIMEOUT_MS/FALLDETECTTASKPERIOD_MS){
        #ifdef SERIALOUTPUT
          Serial.println(F("...no impact"));
        #endif
        fd_state = S_FD_WAITIMPACT;
        elapsed_ticks++;;
      } else {
        #ifdef SERIALOUTPUT
          Serial.println(F("Impact Timeout"));
        #endif
        fd_state = S_FD_FF_COUNT;
      }
      break;
    case S_FD_FALLDETECT:
      if(elapsed_vibration == 500/FALLDETECTTASKPERIOD_MS){
        digitalWrite(VIBRATION_MOTOR_PIN, LOW);
        elapsed_vibration++;
      } else if(elapsed_vibration == 1000/FALLDETECTTASKPERIOD_MS){
        digitalWrite(VIBRATION_MOTOR_PIN, HIGH);
        tone(SPEAKER_PIN, ALERT_TONE, 500);
        elapsed_vibration = 0;
      } else {
        elapsed_vibration++;
      }
      if(digitalRead(EMERGENCY_ALERT_PIN) == LOW){
        elapsed_ticks = 0;
        freefall_count = 0;
        digitalWrite(ALERT_LED_PIN, HIGH);
        scheduler->tsStopTimer();
        Process sendsms;
        if(sendsms.runShellCommand("/mnt/sda1/LifeCatch/sendAlert.sh button") == 0){
          #ifdef SERIALOUTPUT
            Serial.println(F("SMS Sent"));
          #endif
        } else {
          #ifdef SERIALOUTPUT
            Serial.println(F("SMS could not be Sent"));
          #endif
        }
        scheduler->tsStartTimer();
        tone(SPEAKER_PIN, ALERT_TONE);
        digitalWrite(VIBRATION_MOTOR_PIN, LOW);
        fd_state = S_FD_ACTIVEALERT;
      } else if(digitalRead(DISMISS_ALERT_PIN) == LOW){
        digitalWrite(ALERT_LED_PIN, LOW);
        digitalWrite(VIBRATION_MOTOR_PIN, LOW);
        freefall_count = 0;
        fd_state = S_FD_FF_COUNT;
        digitalWrite(VIBRATION_MOTOR_PIN, LOW);
      } else if(elapsed_ticks < DISMISSTIMEOUT_MS/FALLDETECTTASKPERIOD_MS){
        elapsed_ticks++;
        fd_state = S_FD_FALLDETECT;
      } else {
        fd_state = S_FD_ACTIVEALERT;
        #ifdef SIMULATESMS
          Serial.println(F("(Sim) SMS Sent"));
        #else
          scheduler->tsStopTimer();
          Process sendsms;
          if(sendsms.runShellCommand("/mnt/sda1/LifeCatch/sendAlert.sh fall") == 0){
            #ifdef SERIALOUTPUT
              Serial.println(F("SMS Sent"));
            #endif
          } else {
            #ifdef SERIALOUTPUT
              Serial.println(F("SMS could not be Sent"));
            #endif
          }
          scheduler->tsStartTimer();
          tone(SPEAKER_PIN, ALERT_TONE);
          digitalWrite(VIBRATION_MOTOR_PIN, LOW);
        #endif
      }
      break;
    case S_FD_ACTIVEALERT:
      if(digitalRead(DISMISS_ALERT_PIN) == LOW){
        digitalWrite(ALERT_LED_PIN, LOW);
        digitalWrite(VIBRATION_MOTOR_PIN, LOW);
        freefall_count = 0;
        noTone(SPEAKER_PIN);
        fd_state = S_FD_FF_COUNT;
      } else {
        fd_state = S_FD_ACTIVEALERT;
      }
      break;
    default:
      fd_state = S_FD_FF_COUNT;
      break;
  }
  return fd_state;
}

#endif //FALLDETECTTASK_H
