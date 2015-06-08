/*
* Life Catch - Fall Detection and Alerting
* copyright 2014 George Braxton IV
*
* LifeCatch_v008.ino
*
* Changelog:
* (02/22/2014) v008
*    Script for sending alerts is now sendAlert.sh and it requires one argument of "fall" or "button"
*      "fall" argument means the alert was triggered by fall detection
*      "button" argument indicates the alert was triggered by the user pressing the emergency button.
*    Alerts can also be sent to email.
*    Configuration of messages and emergency contact information are set through web interface ****TODO webpage name******        
* (02/09/2014) v007
*    Fall detection optimizations added:
*        Jump detection. Doesn't really detect jumps but jumps do lower the sensitivity of the determining thresholds
*        Orientation considerations
* (02/02/2014) v006
*     Circuit layout changed during this version so many pin assignments changed in config.h
*      Added #ifdef SERIALOUTPUT around all serial println statements.
* (02/01/2014) v005
*     Sketch name changed to LifeCatch_vXXX.ino
*     Added task to verify internet connectivity and light led to indicate such. (InternetCheck.h)
*     Added Emergency button functionality
*     Added Speaker and alert tone functionality
*     Added vibration motor alert functionality
* (01/24/2014) v004
*     Name changed to Life Catch
*     Accelerometer data now read directly instead of from DMP
*     Fall detection based on vector magnitude of all 3-axis
*     SendSMS file removed. Now sent with script file on SD card: sendSMS.sh
*     Temboo/Twilio account data also now stored on SD card in file lifecatch.conf
* (01/20/2014) v003 
*     Added timer interrupt based task scheduling
*     Moved many functions into separate header files
*     Reincorporated a dismiss period.
*     Some performance optimizations including changing
*     the sample rate in the MotionAxis library header.
* (01/19/2014) v002 
*      Added sending of SMS through Temboo/Twilio
*      Dismiss button no longer relevant
*      LED not turned off for 5 seconds after SMS sent
*      Arduino-1.5.5(Beta) has bug in bridge affecting Choreo.
*      Use latest nightly build which has fixed the bug.
* (01/18/2014) v001 
*      Initial Version Detects Falls
*      Outputs alerts to serial and LED
*      Alert dismissed by button
*/

/*
*  Dev Notes:
*    Create new script to send different sms message for emergency button
*    Still need to tweak fall detection thresholds
*    Need automated way to set accelerometer offsets
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Bridge.h>
#include <Wire.h>

#include "AccelerometerTask.h"
#include "Config.h"
#include "FallDetectTask.h"
#include "InternetCheck.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "SerialSetup.h"
#include "TaskScheduler.h"

// Shared Globals
volatile bool runTasks = false; //Set by timer ISR
int16_t accel_x, accel_y, accel_z, accel_vect_magnitude; //global mpu readings

TaskScheduler* scheduler;
//Scheduler Task List -- Add task pointers here --
task* acTask;
task* fdTask;
task* icTask;

void setup() {
    scheduler = new TaskScheduler;
    Bridge.begin();
    #ifdef SERIALOUTPUT
      SerialSetup();
    #endif
    icCheck();
    accelSetup();
    fallDetectSetup();
    fdTask = new task( S_FD_FF_COUNT,
                       FALLDETECTTASKPERIOD_MS,
                       FALLDETECTTASKPERIOD_MS/2,
                       &fallDetectTask);
    acTask = new task( S_AC_GETACC,
                       ACCELTASKPERIOD_MS,
                       ACCELTASKPERIOD_MS,
                       &accelTask);
    icTask = new task( S_IC_WAIT,
                       INTERNETCHECKTASKPERIOD_MS,
                       INTERNETCHECKTASKPERIOD_MS,
                       &internetCheckTask);
    scheduler->tsAddTask(icTask);
    scheduler->tsAddTask(acTask);
    scheduler->tsAddTask(fdTask);
    scheduler->tsStartTimer();
}

void loop(){
  if(runTasks){
    scheduler->tsISR();
    runTasks = false;
  }
}
