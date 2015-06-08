/*
* Life Catch - Fall Detection and Alerting
* copyright 2014 George Braxton IV
*
* TaskScheduler.cpp
*
* Changelog:
* (01/24/2014)
*      TaskScheduler no longer a singleton class.
*      Removed priority, enable, runTask, InitFct from task structure.
*      Functions trimmed to be more efficient.
*      ISR now sets a flag thats checked in loop().
* (01/20/2014)
*      Initial Version. Definitions for Task and TaskScheduler classes.
*/

#include "TaskScheduler.h"

//task class constructor
task::task(uint8_t state, uint32_t period, uint32_t elapsedTime, uint8_t (*TickFct)(uint8_t))
  : state(state), period(period), elapsedTime(elapsedTime), TickFct(TickFct) {}

TaskScheduler::TaskScheduler() : numTasks(0), tasks() {
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = SCHEDULER_PERIOD_MS * 250;
  TCCR1B = 0x0B;   //CTC &  64 prescaler; 16MHz/64=250KHz
  TCNT1=0;
}

void TaskScheduler::tsAddTask(task* tsk){
  if(numTasks < MAX_NUM_TASKS){
    tasks[numTasks] = tsk;
    numTasks++;
  }
}

void TaskScheduler::tsStartTimer(){
  TIMSK1 = (1<<OCIE1A);
}

void TaskScheduler::tsStopTimer(){
  TIMSK1 = 0;
}

void TaskScheduler::tsISR(){
  for (int i = 0; i < numTasks; i++) { 
    if ( tasks[i]->elapsedTime >= tasks[i]->period) { // Ready
      tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
      tasks[i]->elapsedTime = 0;
    }
    tasks[i]->elapsedTime += SCHEDULER_PERIOD_MS;
  }
}

// Timer ISR for scheduler
extern bool runTasks;
ISR(TIMER1_COMPA_vect) {
  runTasks = true;
}
