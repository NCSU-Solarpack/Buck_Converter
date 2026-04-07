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
void PID_Init(PID_t * pid, int16_t Kp, int16_t Ki, int16_t Kd, int16_t out_min, int16_t out_max, int16_t dt);
int16_t PID_Compute(PID_t * pid, int16_t setpoint, int16_t measured);
void PID_Reset(PID_t *pid);

#endif // PID_H
