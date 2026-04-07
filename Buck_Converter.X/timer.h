/*
 * File:   timer.h
 * Author: aaditya
 *
 * Timer1 ISR for fixed-rate control loop with soft-start
 */

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Converter operating states
typedef enum {
    STATE_SOFTSTART,
    STATE_RUN
} ConverterState;

// ISR frequency
#define ISR_FREQ_HZ       20000UL
// Soft-start duration in ISR ticks (100ms * 20kHz = 2000)
#define SOFTSTART_TICKS   2000UL
// Timer1 period register value: Fcy / ISR_FREQ - 1
#define TIMER1_PERIOD     2499U

void Timer1_Init(void);

extern volatile ConverterState converter_state;

#endif /* TIMER_H */
