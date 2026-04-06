#ifndef PID_H
#define PID_H

// PID Controller State
typedef struct {
    // Tuning parameters
    float Kp; // Proportional gain
    float Ki; // Integral gain
    float Kd; // Derivative gain

    // Output limits
    float out_min;
    float out_max;

    // Internal state
    float integral; // accum
    float prev_error;
    float dt;  // time step (s)
} PID_t;

// Func Prototypes
void PID_Init(PID_t * pid, float Kp, float Ki, float Kd, float out_min, float out_max, float dt);
float PID_Compute(PID_t * pid, float setpoint, float measured);
void PID_Reset(PID_t *pid);

#endif // PID_H
