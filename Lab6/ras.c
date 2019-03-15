/*
 * Created by Alex Dai and Hamza Waseem
 */

#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/adc.h>
#include "launchpad.h"
#include "ras.h"

//--------------------------------------------------------------------------------------------------------

void rasInit() {

    // Enable the ADC0 module.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    // Configure ADC0, sequencer #0
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    // Configure the sequencer, can have multiple steps

    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH7);
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH6);  // Configure a single step of the sequencer

    ADCSequenceEnable(ADC0_BASE, 0);
}

//--------------------------------------------------------------------------------------------------------
/*
void rasRead(uint32_t data[]) {

    // Trigger the sample sequence.
    ADCProcessorTrigger(ADC0_BASE, 0);
    // Wait until the sample sequence has completed.
    while(!ADCIntStatus(ADC0_BASE, 0, false))
    { }
    // Read the value from the ADC.
    ADCSequenceDataGet(ADC0_BASE, 0, data[]);
    // Clear interrupt signal
    ADCIntClear(ACD0_BASE, 0);
}
*/
