/* 
 * File:   PWM.c
 * Author: izakd
 *
 * Created on December 17, 2025, 8:59 AM
 */

#include "macros.h"
#include "pwm.h"

uint32_t Target_Duty_Period_us;


void Period_Calc(void){
    Target_Duty_Period_us = 1000000UL / FREQUENCY;
}
