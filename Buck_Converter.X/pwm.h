/* 
 * File:   PWM.h
 * Author: izakd
 *
 * Created on December 17, 2025, 11:30 AM
 */

#ifndef PWM_H
#define	PWM_H
#include <stdint.h>


void Period_Calc(void);
void PWM_Init(void);
void PWM_SetDuty_Stage1(uint16_t duty_counts);
void PWM_SetDuty_STage2(uint16_t duty_counts);

// Precomputed period in HSPWM  counts (set by Period_Calc)
extern uint16_t PWM_Period_Counts;


#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

