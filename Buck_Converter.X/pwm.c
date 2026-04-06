/* 
 * File:   PWM.c
 * Author: izakd
 *
 * Created on December 17, 2025, 8:59 AM
 */

#include <xc.h>
#include "macros.h"
#include "pwm.h"

uint32_t Target_Duty_Period_us; // kept for legacy reference
uint16_t PWM_Period_Counts;

// HSPWM  = 4 * Fosc. Assumes Fosc = 100 mHz -> HSPWM = 400 MHz
#define HSPWM_CLOCK_HZ  400000000UL

void Period_Calc(void) {
    Period_Calc(); // ensure counts are ready
    
    // PG1: Stage 1 buck (80V -> output)
    PG1CONLbits.ON = 0; // off during config
    PG1CONLbits.CLKSEL = 0b01; // FOSC as clock source (HSPWM scales internally)
    PG1CONLbits.MODSEL = 0b000; // Independent edge PWM
    PG1CONHbits.MSTEN = 0; // Not a master
    PG1IOCONL = 0x0000;
    PG1IOCONHbits.PMOD  = 0b00;     // Complementary output (high + low side)
    PG1IOCONHbits.PENH  = 1;        // PWM1H pin owned by PWM generator
    PG1IOCONHbits.PENL  = 1;        // PWM1L pin owned by PWM generator
    PG1IOCONHbits.POLH  = 0;        // Active high
    PG1IOCONHbits.POLL  = 1;        // Low side active low (complementary)
    PG1DTL              = 30;       // Dead time low  (75 ns @ 400 MHz)
    PG1DTH              = 30;       // Dead time high
    PG1PER              = PWM_Period_Counts;
    PG1DC               = PWM_Period_Counts / 5;  // Start at 20% duty — safe startup
    PG1PHASE            = 0;
    PG1CONLbits.ON      = 1;        // Enable

    // PG2: Stage 2 buck (80V -> 12V) 
    PG2CONLbits.ON      = 0;
    PG2CONLbits.CLKSEL  = 0b01;
    PG2CONLbits.MODSEL  = 0b000;
    PG2CONHbits.MSTEN   = 0;
    PG2IOCONL           = 0x0000;
    PG2IOCONHbits.PMOD  = 0b00;
    PG2IOCONHbits.PENH  = 1;
    PG2IOCONHbits.PENL  = 1;
    PG2IOCONHbits.POLH  = 0;
    PG2IOCONHbits.POLL  = 1;
    PG2DTL              = 30;
    PG2DTH              = 30;
    PG2PER              = PWM_Period_Counts;
    PG2DC               = PWM_Period_Counts / 5;  // 20% startup
    PG2PHASE            = 0;
    PG2CONLbits.ON      = 1;
}

void PWM_SetDuty_Stage1(uint16_t duty_counts) {
    // Clamp to [0, PER] — never exceed period
    if (duty_counts > PG1PER) duty_counts = PG1PER;
    PG1DC = duty_counts;
}

void PWM_SetDuty_Stage2(uint16_t duty_counts) {
    if (duty_counts > PG2PER) duty_counts = PG2PER;
    PG2DC = duty_counts;
}

void Period_Calc(void){
    Target_Duty_Period_us = 1000000UL / FREQUENCY;
}
