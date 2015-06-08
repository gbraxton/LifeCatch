/*
* Life Catch - Fall Detection and Alerting
* copyright 2014 George Braxton IV
*
* TaskScheduler.h
*
* Changelog:
* (01/24/2014)
*      TaskScheduler no longer a singleton class.
*      Removed priority, enable, runTask, InitFct from task structure.
*      Functions trimmed to be more efficient.
*      ISR now sets a flag thats checked in loop().
* (01/20/2014)
*      Initial Version. Task and TaskScheduler
*/

#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <avr/interrupt.h>
#include "Config.h"
#include "I2Cdev.h"


// ********************************************************************
// *                             SCHEDULER                            *
// ********************************************************************

//task object
class task {
public:
  task(uint8_t state, uint32_t period, uint32_t elapsedTime, uint8_t (*TickFct)(uint8_t));
  uint8_t state;
  uint32_t period;
  uint32_t elapsedTime;
  uint8_t (*TickFct)(uint8_t);
 };

class TaskScheduler{
  private:
    unsigned char numTasks;
    task* tasks[MAX_NUM_TASKS];            //holds pointer to array of task
  public:
    TaskScheduler();
    void tsAddTask(task*);
    void tsStart();
    void tsStartTimer();
    void tsStopTimer();
    void tsISR();
};

#endif //TASKSCHEDULER_H
