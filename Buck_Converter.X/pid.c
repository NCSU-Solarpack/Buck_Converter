/*
 * File:   pid.c
 * Author: aaditya
 *
 * Integer PID controller with anti-windup and output clamping
 */

#include "pid.h"

void PID_Init(PID_Controller *pid, int32_t kp, int32_t ki, int32_t kd,
              int32_t out_min, int32_t out_max, int32_t int_max) {
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->integral = 0;
    pid->prev_error = 0;
    pid->out_min = out_min;
    pid->out_max = out_max;
    pid->int_max = int_max;
}

void PID_Reset(PID_Controller *pid) {
    pid->integral = 0;
    pid->prev_error = 0;
}

uint16_t PID_Compute(PID_Controller *pid, int32_t error) {
    // Proportional
    int32_t p_term = pid->Kp * error;

    // Integral with anti-windup
    pid->integral += error;
    if (pid->integral > pid->int_max)
        pid->integral = pid->int_max;
    else if (pid->integral < -(pid->int_max))
        pid->integral = -(pid->int_max);
    int32_t i_term = pid->Ki * pid->integral;

    // Derivative
    int32_t d_term = pid->Kd * (error - pid->prev_error);
    pid->prev_error = error;

    // Sum and scale
    int32_t output = (p_term + i_term + d_term) / PID_SCALE;

    // Output clamping
    if (output > pid->out_max)
        output = pid->out_max;
    else if (output < pid->out_min)
        output = pid->out_min;

    return (uint16_t)output;
}
