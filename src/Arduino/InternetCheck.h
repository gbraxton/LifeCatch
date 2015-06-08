/*
* Life Catch - Fall Detection and Alerting
* copyright 2014 George Braxton IV
*
* InternetCheck.h
*
* Changelog:
* (02/02/2014)
*      Added #ifdef SERIALOUTPUT around serial print statements
* (02/01/2014)
*      Initial Version. Check for internet connectivity using curl command over bridge.
*
*  InternetCheck.h implements a state machine that repeatedly checks for internet access
*  by issuing a curl command over the Linino Bridge.
*/

#ifndef INTERNETCHECK_H
#define INTERNETCHECK_H

#include "Config.h"
#include "I2Cdev.h"
#include "TaskScheduler.h"

extern TaskScheduler* scheduler;
Process icc;

void icCheck(){
  #ifdef SERIALOUTPUT
    Serial.println(F("Testing Connection to Temboo Server"));
  #endif
  if( icc.runShellCommand("curl -k -s https://www.temboo.com > /dev/null") == 0 ){
    digitalWrite(INTERNET_LED_PIN, HIGH);
    #ifdef SERIALOUTPUT
      Serial.println(F("Internet Access Detected"));
    #endif
  } else {
    digitalWrite(INTERNET_LED_PIN, LOW);
    #ifdef SERIALOUTPUT
      Serial.println(F("Warning - Internet Access Not Detected"));
    #endif
  }
}

// *****************************************************************
// *                      STATE MACHINE                            *
// *****************************************************************

enum IC_STATES{ S_IC_RUN_CHECK, S_IC_WAIT_EXIT, S_IC_WAIT };
uint32_t ic_ticks = 0;
uint8_t internetCheckTask(uint8_t state){
  switch(state){
    case S_IC_RUN_CHECK:
      icc.runShellCommandAsynchronously("curl -k -s https://www.temboo.com > /dev/null");
      state = S_IC_WAIT_EXIT;
      break;
    case S_IC_WAIT_EXIT:
      if(icc.running()){
        state = S_IC_WAIT_EXIT;
      } else if(icc.exitValue() == 0){
        digitalWrite(INTERNET_LED_PIN, HIGH);
        #ifdef SERIALOUTPUT
          Serial.println(F("Internet Access Detected"));
        #endif
        state = S_IC_WAIT;
        ic_ticks = 0;
      } else {
        digitalWrite(INTERNET_LED_PIN, LOW);
        #ifdef SERIALOUTPUT
          Serial.println(F("Warning - No Internet Access"));
        #endif
        state = S_IC_WAIT;
        ic_ticks = 0;
      }
      break;
    case S_IC_WAIT:
      if(ic_ticks < INTERNETCHECKPERIOD_MS/INTERNETCHECKTASKPERIOD_MS){
        ic_ticks++;
        state = S_IC_WAIT;
      } else {
        state = S_IC_RUN_CHECK;
      }
      break;
    default:
      break;
  }
  return state;
}

#endif //INTERNETCHECK_H
