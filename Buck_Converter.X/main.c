/* 
 * File:   main.c
 * Author: izakd
 *
 * Created on December 17, 2025, 8:59 AM
 * 
 * Wiring Summary: 
    Stage 1 sense (80V rail  divider  RB7/AN2)  ADC_Read_AN2()
    Stage 2 sense (12V rail  divider  RA3/AN3)  ADC_Read_AN3()

    Stage 1 PWM high-side  RB14 (PWM1H)  gate driver IN+
    Stage 1 PWM low-side   RB15 (PWM1L)  gate driver IN-
    Stage 2 PWM high-side  RB12 (PWM2H)  gate driver IN+
    Stage 2 PWM low-side   RB13 (PWM2L)  gate driver IN-
 */

#include <xc.h>
#include "macros.h"
#include "Control.h"
#include "Init.h"
#include "pwm.h"
#include "adc.h"
#include "timer.h"

int main(void) {
    System_Init();
    ADC_Init();
    PWM_Init();
    Timer1_Init();  // starts 20 kHz control loop with soft-start

    while(true) {
        // Control loop runs in Timer1 ISR (timer.c)
        // Main loop is idle - can add telemetry/diagnostics here later
    }

    return 0;
}

