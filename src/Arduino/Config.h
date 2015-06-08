/*
* Life Catch - Fall Detection and Alerting
* copyright 2014 George Braxton IV
*
* Config.h
*
* Changelog:
*  (02/09/2014)
*      Added defines for new jump and orientation thresholds/timeouts
* (02/02/2014)
*      Changed most of pins for layout efficiency
* (02/01/2014)
*      defines for speaker pin, alert tone, and vibration motor pin added
* (01/24/2014)
*      MPUINT define removed
* (01/20/2014)
*      Initial Version. Consolidation of many of the configuration macros.
*/

#ifndef CONFIG_H
#define CONFIG_H

//led pin. the onboard pin on arduino yun is 13
#define ALERT_LED_PIN 11
//internet connectivity pin
#define INTERNET_LED_PIN 13
//speaker pin
#define SPEAKER_PIN 8
//vibration motor pin
#define VIBRATION_MOTOR_PIN 9
//pin for dismiss alert button
#define DISMISS_ALERT_PIN 12
//pin for emergency button
#define EMERGENCY_ALERT_PIN 10

#define ALERT_TONE 440

//Uncomment to output serial messages
#define SERIALOUTPUT

//Uncomment to output accelerometer values to serial
#define ACCELOUTPUT

//Uncomment to output fall detection alerts to serial
//Alerts include FREEFALL, IMPACT, FALL DETECTED
//#define FALLOUTPUT

//Uncomment to force device to wait for serial enumeration
//before finishing setup
//#define HOLDFORSERIALENUM

//Uncomment to wait for a char to be sent over serial
//before beginning to reading dmp
//#define HOLDFORSEND

//Uncomment to simulate sending SMS while testing
//#define SIMULATESMS

#define INTERNETCHECKPERIOD_MS 120000UL

//Scheduler Constants
#define SCHEDULER_PERIOD_MS 40UL
#define ACCELTASKPERIOD_MS 40UL
#define FALLDETECTTASKPERIOD_MS 40UL
#define INTERNETCHECKTASKPERIOD_MS 200UL
#define MAX_NUM_TASKS 8

//Freefall Detection
// maximum value for freefall event
#define FFTHRESHOLD 1500
// min consecutive samples to indicate freefall event
#define FFDURATION_MS 40UL
// minimum value for impact event
#define IMPTHRESHOLD 14000
#define IMPLOWERTHRESHOLD 10000
// maximum samples after freefall for which impact must occur
#define IMPTIMEOUT_MS 500UL
// ticks before send alert is activated after fall is detected
#define DISMISSTIMEOUT_MS 10000UL
// the threshold required to note a posible jump condition
#define JUMP_THRESHOLD 5000
#define JUMP_TIMEOUT_MS 200UL
// min val for rotation values to influence fall detection sensitivity
#define ROTATION_THRESHOLD

#endif //CONFIG_H
