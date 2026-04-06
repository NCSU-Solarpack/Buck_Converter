#include "pid.h"

// PID_Init: Call once at startup to cinfigure your controller
void PID_Init(PID_t *pid, float Kp, float Ki, float Kd, float out_min, float out_max, float dt) {
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->out_min = out_min;
    pid->out_max = out_max;
    pid->dt = dt;
    pid->integral = 0.0f;
    pid->prev_error= 0.0f;
}

// PID_Compute: Call every control loop tick. Returns PWM duty value.
// setpoint = desired output voltage
// measured = ADC-sampled actual output voltage
float PID_Compute(PID_t *pid, float setpoint, float measured) {
    // Step 1: Calculate error
    float error = setpoint - measured;
    // Step 2: Proportional term
    float P = pid->Kp * error;
    // Step 3: Integral term 
    pid->integral += error * pid->dt;
    // clamp integral so it can't grow unbounded
    // when the output is already saturated
    float integral_max = pid->out_max / pid->Ki;
    float integral_min = pid->out_min / pid->Ki;
    if (pid->integral > integral_max) pid->integral = integral_max;
    if (pid->integral < integral_min) pid->integral = integral_min;

    float I = pid->Ki *pid->integral;

    // Step 4: Derivative term 
    // You should do this one on measureement, not error
    // acoids a spike in D when the setppont suddenly changes
    float D = pid->Kd * (error - pid->prev_error) / pid->dt;
    pid->prev_error = error;

    // Step 5: Summ all three terms
    float output = P + I + D;

    // Step 6: Calmp output to valid PWM range
    if (output > pid->out_max) output = pid->out_max;
    if (output < pid->out_min) output = pid->out_min;
}


// PID_Reset: Call if you cahnge setpoint drastically or re-enable
void PID_Reset(PID_t *pid) {
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}
