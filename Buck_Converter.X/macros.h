/* 
 * File:   macros.h
 * Author: izakd
 *
 * Created on December 17, 2025, 9:00 AM
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifndef MACROS_H
#define	MACROS_H

//Definitions
#define FREQUENCY         200000UL    //Operating frequency of buck converter(Hz))
#define V_TARGET_STAGE_1  80000UL     //Target output voltage stage 1 (mV)
#define V_TARGET_STAGE_2  14000UL     //Target output voltage stage 2 (mV)
#define RTOP_STAGE_1_K    390UL       //Top resistor value for first stage voltage division(Ohm)
#define RBOT_STAGE_1_K    10UL        //Bottom resistor value for first stage voltage division(Ohm)
#define RTOP_STAGE_2_K    56UL       //Top resistor value for second stage voltage division(Ohm)
#define RBOT_STAGE_2_K    10UL        //Bottom resistor value for first stage voltage division(Ohm)
#define ADC_RES           4095UL      //ADC resolution value n               
#define ADC_REF           3300UL      //ADC voltage reference 3300mV

//Global variables

//Variable for numerator and denominator values for resistor divider
extern uint32_t Stage_1_NUM;
extern uint32_t Stage_1_DEN;
extern uint32_t Stage_2_NUM;
extern uint32_t Stage_2_DEN;

//Variable for raw ADC values
extern uint32_t ADC_Raw_Stage_1;
extern uint32_t ADC_Raw_Stage_2;

//Variables for the scaled values of the ADC
extern uint32_t Scaled_ADC_Stage_1;
extern uint32_t Scaled_ADC_Stage_2;

//Variables for mV values of ADC
extern uint32_t Voltage_mv_Stage_1;
extern uint32_t Voltage_mv_Stage_2;

//Variables for error from setpoint
extern int32_t Voltage_Error_Stage_1;
extern int32_t Voltage_Error_Stage_2;

//Variable for target duty cycle
extern uint32_t Target_Duty_Period_us;

//Function prototypes


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MACROS_H */

