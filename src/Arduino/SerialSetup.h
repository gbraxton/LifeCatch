/*
* Life Catch - Fall Detection and Alerting
* copyright 2014 George Braxton IV
*
* SerialSetup.h
*
* Changelog:
* (01/24/2014)
*      Removed macros defining SERIALOUTPUT
* (01/20/2014)
*      Initial Version. This header deals with serial initialization
*      and preprocessor task for enabling/disabling serial output.
*/

#ifndef SERIALSETUP_H
#define SERIALSETUP_H

#include "Config.h"

void SerialSetup(){
  #ifdef SERIALOUTPUT
    Serial.begin(115200);
    #ifdef HOLDFORSERIALENUM
      while (!Serial);     // hold until serial enumerated
    #endif
    Serial.println(F("Life Catch - Fall Detection and Alerting v008"));
    Serial.println(F("copyright 2014 George Braxton IV"));
  #endif
  
  #ifdef HOLDFORSEND
    //wait for char sent over serial before beginning
    Serial.println(F("\nSend a character to begin\n"));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again
  #endif
}

#endif //SERIALSETUP_H
