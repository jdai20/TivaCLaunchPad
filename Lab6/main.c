/*
 * main.c for ECE 266 Lab 6, Knob Control (ADC)
 *
 *      Author: zzhang
 *      Edited by: Alex Dai, Hamza Waseem
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "seg7digit.h"
#include "ras.h"

// The initial state of the 7-segment display: "00:00" with colon on
seg7Display_t seg7Display = { 0, 0, 0, 0, 1 };


void updateDisplay (uint32_t time) {

    uint32_t delay = 100;

    uint32_t rasData[2] = {0, 0};

    rasRead(rasData);

    uint32_t seg12 = rasData[0];
    uint32_t seg34 = rasData[1];

    uprintf("%d\n\r", seg12);
    uprintf("%d\n\r", seg34);

    seg12 = rasData[0] / 41;
    seg7Display.d1 = seg12 % 10;
    seg7DigitUpdate(&seg7Display);
    seg7Display.d2 = (seg12 / 10) % 10;

    seg34 = rasData[1] / 41;
    seg7Display.d3 = seg34 % 10;
    seg7DigitUpdate(&seg7Display);
    seg7Display.d4 = (seg34 / 10) % 10;

    seg7DigitUpdate(&seg7Display);
    schdCallback(updateDisplay, time + delay);
}


int main(void)
{
    lpInit();
    seg7Init();
    rasInit();

    uprintf("%s\n\r", "Lab 6: Knob Control (ADC)");

    // Update the clock display
    seg7DigitUpdate(&seg7Display);

    schdCallback(updateDisplay, 1010);

    // Run the event scheduler to process callback events
    while (true) {
        schdExecute();
    }
}
