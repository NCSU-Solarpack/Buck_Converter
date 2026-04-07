#ifndef PID_H
#define PID_H

// PID Controller State
typedef struct {
    // Tuning parameters  //Reece: int16_t used because it uses less memory but still gives a good amount of precision (compared to float).
    int16_t Kp; // Proportional gain
    int16_t Ki; // Integral gain
    int16_t Kd; // Derivative gain

    // Output limits
    int16_t out_min;
    int16_t out_max;

    // Internal state
    int16_t integral; // accum
    int16_t prev_error;
    int16_t dt;  // time step (s)
} PID_t;

// Func Prototypes
void PID_Init(PID_t * pid, int16_t Kp, int16_t Ki, int16_t Kd, int16_t out_min, int16_t out_max, int16_t dt);
int16_t PID_Compute(PID_t * pid, int16_t setpoint, int16_t measured);
void PID_Reset(PID_t *pid);

#endif // PID_H
