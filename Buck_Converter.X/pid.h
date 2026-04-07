/*
 * File:   pid.h
 * Author: aaditya
 *
 * PID controller for buck converter voltage regulation
 */

#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef struct {
    int32_t Kp;           // Proportional gain (scaled by PID_SCALE)
    int32_t Ki;           // Integral gain (scaled by PID_SCALE)
    int32_t Kd;           // Derivative gain (scaled by PID_SCALE)
    int32_t integral;     // Accumulated integral term
    int32_t prev_error;   // Previous error for derivative
    int32_t out_min;      // Output clamp minimum (typically 0)
    int32_t out_max;      // Output clamp maximum (typically PWM_Period_Counts)
    int32_t int_max;      // Integral anti-windup clamp
} PID_Controller;

// PID_SCALE: gains are multiplied by this. E.g., Kp=50 with PID_SCALE=1000 means effective Kp=0.05
#define PID_SCALE 1000L

void PID_Init(PID_Controller *pid, int32_t kp, int32_t ki, int32_t kd,
              int32_t out_min, int32_t out_max, int32_t int_max);
void PID_Reset(PID_Controller *pid);
uint16_t PID_Compute(PID_Controller *pid, int32_t error);

#endif /* PID_H */
