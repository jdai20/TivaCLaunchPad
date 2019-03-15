; Lab 6, ECE 266, Fall 2018
; Created by Alex Dai and Hamza Waseem

; include C header files
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/sysctl.h","driverlib/adc.h","driverlib/gpio.h","driverlib/pin_map.h","driverlib/timer.h","launchpad.h","ras.h"
                .text

ADC0			.field  ADC0_BASE

				.global rasRead
                .asmfunc
rasRead 		PUSH    {lr, r0}
				; Trigger the sample sequence.
    			; ADCProcessorTrigger(ADC0_BASE, 0);
				LDR r0, ADC0
				MOV r1, #0
				BL	ADCProcessorTrigger
				; Wait until the sample sequence has completed.
    			; while(!ADCIntStatus(ADC0_BASE, 0, false)) {}
while_loop		LDR		r0, ADC0
				MOVS	r1, #0
				MOVS	r2, #0 ;boolean
				BL		ADCIntStatus
				CMP		r0, #0
				BEQ		while_loop
				; Read the value from the ADC.
    			; ADCSequenceDataGet(ADC0_BASE, 0, data[]);
				LDR		r0, ADC0
				MOV		r1, #0
				POP		{r2}
				BL		ADCSequenceDataGet
				; Clear interrupt signal
    			; ADCIntClear(ACD0_BASE, 0);
    			LDR		r0, ADC0
    			MOV		r1, #0
    			BL 		ADCIntClear

                POP     {pc}
                .endasmfunc

