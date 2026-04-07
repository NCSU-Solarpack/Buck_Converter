/*
 * File:   timer.c
 * Author: aaditya
 *
 * Timer1 ISR: 20 kHz control loop with soft-start ramp
 */

#include <xc.h>
#include "timer.h"
#include "macros.h"
#include "adc.h"
#include "Control.h"
#include "pwm.h"
#include "pid.h"

// PID controllers for each stage
static PID_Controller pid_stage1;
static PID_Controller pid_stage2;

// State tracking
volatile ConverterState converter_state = STATE_SOFTSTART;
static uint16_t softstart_count = 0;

// Initial duty targets for soft-start ramp endpoint
// Stage 1: D1 = Vout/Vin = 80/420 ~ 0.19 -> 0.19 * 2000 = 380 counts
// Stage 2: D2 = Vout/Vin = 14/80 ~ 0.175 -> 0.175 * 2000 = 350 counts
#define SOFTSTART_TARGET_1  380U
#define SOFTSTART_TARGET_2  350U

void Timer1_Init(void) {
    // Init PID controllers with conservative gains
    // Gains are scaled by PID_SCALE (1000). Start low, tune on hardware.
    // Stage 1: 250-420V -> 80V (large input range, be gentle)
    PID_Init(&pid_stage1, 2, 1, 0, 0, (int32_t)PWM_Period_Counts, 50000L);
    // Stage 2: 80V -> 14V (tighter regulation needed)
    PID_Init(&pid_stage2, 5, 2, 0, 0, (int32_t)PWM_Period_Counts, 50000L);

    // Configure Timer1
    T1CONbits.TON = 0;       // Disable during config
    T1CONbits.TCKPS = 0b00;  // 1:1 prescaler
    T1CONbits.TCS = 0;       // Internal clock (Fcy)
    TMR1 = 0;                // Clear counter
    PR1 = TIMER1_PERIOD;     // 20 kHz

    // Configure interrupt
    IFS0bits.T1IF = 0;       // Clear flag
    IPC0bits.T1IP = 4;       // Priority 4 (mid-level)
    IEC0bits.T1IE = 1;       // Enable interrupt

    // Start timer
    T1CONbits.TON = 1;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;  // Clear interrupt flag first

    // --- Read and scale voltages ---
    Get_ADC_Raw_Stage_1();
    Get_ADC_Raw_Stage_2();
    Scale_Convert_ADC_Stage_1();
    Scale_Convert_ADC_Stage_2();
    Error_Calc_Stage_1();
    Error_Calc_Stage_2();

    switch (converter_state) {
        case STATE_SOFTSTART:
            // Linear ramp from 0 to initial duty over SOFTSTART_TICKS
            softstart_count++;

            uint16_t ramp_duty_1 = (uint16_t)((uint32_t)SOFTSTART_TARGET_1 * softstart_count / SOFTSTART_TICKS);
            uint16_t ramp_duty_2 = (uint16_t)((uint32_t)SOFTSTART_TARGET_2 * softstart_count / SOFTSTART_TICKS);

            PWM_SetDuty_Stage1(ramp_duty_1);
            PWM_SetDuty_Stage2(ramp_duty_2);

            // Hold PID in reset during ramp
            PID_Reset(&pid_stage1);
            PID_Reset(&pid_stage2);

            if (softstart_count >= SOFTSTART_TICKS) {
                converter_state = STATE_RUN;
            }
            break;

        case STATE_RUN: {
            // Closed-loop PID control
            uint16_t duty1 = PID_Compute(&pid_stage1, Voltage_Error_Stage_1);
            uint16_t duty2 = PID_Compute(&pid_stage2, Voltage_Error_Stage_2);

            PWM_SetDuty_Stage1(duty1);
            PWM_SetDuty_Stage2(duty2);
            break;
        }
    }
}
