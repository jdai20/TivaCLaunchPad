;
; Lab 4, ECE 266, Spring 2019
;
; Assembly functions related to motion operations.
;
; Edited by: Hamza Waseem
;

; To include names declared in C
                .cdecls "stdint.h","stdbool.h","inc/hw_memmap.h","driverlib/pin_map.h","driverlib/sysctl.h","driverlib/gpio.h","motion.h"

                .text
                .global motionOnOff
 ;               .global motionOff

; GPIO peripheral, port base and pin mask value for the motion sensor
; The motion is assumed to be connected to J16, and then the motion sensor's
; signal pin is connected to PC4.
MOTION_PORT     .field  GPIO_PORTC_BASE
MOTION_PIN      .equ    GPIO_PIN_4

;
; uint32_t motionOnOff(): Turn on/off the motion sensor. It calls GPIOPinRead() to read 0 or 1 from the signal pin.
;
motionOnOff        PUSH    {LR}            ; save the return address

                ; Write 1 to the GPIO pin that the buzzer uses:
                ;   GPIOPinRead(MOTION_PORT, MOTION_PIN, MOTION_PIN)
                LDR     r0, MOTION_PORT
                MOV     r1, #MOTION_PIN
                BL      GPIOPinRead

                POP     {PC}            ; return


