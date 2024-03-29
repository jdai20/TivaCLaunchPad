/*
 * ECE 266, Spring 2019, Lab 4 Buzzer
 *
 * main.c
 * Created by Zhao Zhang
 *
 * Edited By: Alex Dai sp2019
 */
//----------------------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"

// ON/Off state type
typedef enum {On, Off} OnOff_t;

// System running state
static OnOff_t sysState = Off;

// Alarm state
static OnOff_t alarmState = Off;

// Buzzer state
static OnOff_t buzzerState = Off;

// Motion Sensor state
int32_t motionState = 0;

/*
 * Task 1: Flash LED
 */

//----------------------------------------------------------------------------------------

// Callback function for playing the buzzer

void callbackBuzzerPlay(uint32_t time)                    // the scheduled time
{
    uint32_t delay = 10;

    if (alarmState == On)
    {
        assert(sysState == On);

        // Turn the buzzer on and off alternatively
        // Adjust the time values to control the sound intensity
        switch (buzzerState)
        {
        case On:
            buzzerOff();
            buzzerState = Off;
            delay = 2988;                           // off for 2988 ms
            break;

        case Off:
            buzzerOn();
            buzzerState = On;
            delay = 12;                            // on for 12 ms
            break;
        }
    }

    // schedule the next callback
    schdCallback(callbackBuzzerPlay, time + delay);
}

//----------------------------------------------------------------------------------------
/*
 * Task 2: Check push buttons
 */

// Callback function for checking the push buttons

void callbackCheckPushButton(uint32_t time)
{
    uint32_t delay = 10;        // the default delay for the next checking

    int code = pbRead();        // read the pushbutton
    switch (code)
    {
    case 1:                     // SW1: Turn on the system and the alarm
        sysState = On;
        //alarmState = On;
        ledTurnOnOff(false , false , true );
        delay = 250;
        break;

    case 2:                     // SW2: Turn off the system and the alarm
        sysState = Off;
        //alarmState = Off;
        ledTurnOnOff(false , false , false );
        buzzerOff();
        delay = 250;
        break;
    }

    // schedule the next callback
    schdCallback(callbackCheckPushButton, time + delay);
}

//----------------------------------------------------------------------------------------

/*
 * Task 3: YOUR CODE, check the PIR motion sensor
 */
void callbackCheckMotionSensor(uint32_t time){

    uint32_t delay = 10;
    motionState = motionOnOff();

    // switches motion sensor state on if motion is detected
    // off, if else
    if (sysState == On) {
        if(motionState == 0) {
        // no motion detected, turn alarm off
            alarmState = Off;
            ledTurnOnOff(false /* red */, false /* blue */, true /* green */);
            delay = 250;
            uprintf("%s\n\r", "No Motion Detected");

        } else {
             // motion detected, turns alarm on
            alarmState = On;
            ledTurnOnOff(true /* red */, false /* blue */, false /* green */);
            delay = 250;
            uprintf("%s\n\r", "Motion Detected");

        }
    }

    // schedule next callback
    schdCallback(callbackCheckMotionSensor, time + delay);
}

//----------------------------------------------------------------------------------------

/*
 * The main function
 */
int main(void)
{
    lpInit();
    motionInit();
    buzzerInit();

    // Print out a start message
    uprintf("%s\n\r", "Lab 4 starts");

    // Schedule the first callback events
    schdCallback(callbackBuzzerPlay, 1000);
    schdCallback(callbackCheckPushButton, 1002);
    schdCallback(callbackCheckMotionSensor, 1005);

    // Turn on the green sub-LED
    //ledTurnOnOff(false /* red */, false /* blue */, true /* green */);

    // Run the callback scheduler
    while (true)
    {
        schdExecute();
    }
}
