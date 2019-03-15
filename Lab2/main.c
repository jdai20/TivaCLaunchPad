/*
 * ECE 266 Lab 2, Spring 2019
 * main.c
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>
#include "launchpad.h"
#include "seg7.h"

// 7-segment coding table. See https://en.wikipedia.org/wiki/Seven-segment_display. The segments
// are named as A, B, C, D, E, F, G. In this coding table, segments A-G are mapped to bits 0-7.
// Bit 7 is not used in the coding. This display uses active high signal, in which '1' turns ON a
// segment, and '0' turns OFF a segment.
static uint8_t seg7Coding[10] = {
        0b00111111, 		// digit 0
        0b00000110, 		// digit 1
        0b01011011,			// digit 2
        0b01001111,			// digit 3
        0b01100110,         // digit 4
        0b01101101,         // digit 5
        0b01111101,         // digit 6
        0b00000111,         // digit 7
        0b01111111,         // digit 8
        0b01101111,         // digit 9
        // MORE CODINGS
        };


// The colon status: if colon == 0b10000000, then the colon is on,
// otherwise it is off.
static uint8_t colon = 0;

// global variables to keep in check the overflow for the seconds and minutes.
static unsigned short secondsLSB = 0;
static unsigned short secondsCarry = 0;
static unsigned short minutesLSB = 0;
static unsigned short minutesCarry = 0;


// function updateDigits(): Used conditional statements to check for overflow of bits and increment the seconds and minutes

void updateDigits(){

    if (secondsLSB == 10){
        secondsLSB = 0;
        secondsCarry = secondsCarry + 1;
    }

    if(secondsCarry == 6){
        secondsCarry = 0;
        minutesLSB = minutesLSB + 1;
    }
    if(minutesLSB == 10){
        minutesLSB = 0;
        minutesCarry = minutesCarry + 1;
    }
    if(minutesCarry == 6){
        minutesCarry = 0;
    }
}

// Update the clock display
void clockUpdate(uint32_t time)                     // pointer to a 4-byte array
{
    uint8_t code[4];            // The 7-segment code for the four clock digits

    // Display 01:23 on the 7-segment displays
    // The colon ':' will flash on and off every 0.5 seconds
    code[0] = seg7Coding[secondsLSB] + colon;
    code[1] = seg7Coding[secondsCarry] + colon;
    code[2] = seg7Coding[minutesLSB] + colon;
    code[3] = seg7Coding[minutesCarry] + colon;
    seg7Update(code);




    // Calculate the display digits and colon setting for the next update
    if (colon == 0b00000000)
    {
        colon = 0b10000000;
        secondsLSB = secondsLSB + 1;
    }
    else
    {
        colon = 0b00000000;
    }

    // Call back after 1 second
    schdCallback(clockUpdate, time + 500);
}

void checkPushButton(uint32_t time){
    int code = pbRead();
    uint32_t delay = 10;

    switch (code){
    case 1:
        minutesLSB +=1;
        updateDigits();
        delay = 250;
        break;

    case 2:
        secondsLSB +=1;
        updateDigits();
        delay = 250;
        break;

    }

    schdCallback(checkPushButton, time + delay);

}

int main(void)
{
    lpInit();
    seg7Init();

    uprintf("%s\n\r", "Lab 2: Wall clock");

    // Schedule the first callback eventsc for LED flashing and push button checking.
    // Those trigger callback chains. The time unit is millisecond.
    schdCallback(clockUpdate, 1000);
    schdCallback(checkPushButton, 1000);

    // Loop forever
    while (true)
    {
        schdExecute();
    }
}
