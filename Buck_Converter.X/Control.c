/* 
 * File:   Control.c
 * Author: izakd
 *
 * Created on December 17, 2025, 8:59 AM
 */

#include "macros.h"
#include "adc.h"
#include "Control.h"

uint32_t ADC_Raw_Stage_1;
uint32_t ADC_Raw_Stage_2;
uint32_t Scaled_ADC_Stage_1;
uint32_t Scaled_ADC_Stage_2;
uint32_t Voltage_mv_Stage_1;
uint32_t Voltage_mv_Stage_2;
int32_t Voltage_Error_Stage_1;
int32_t Voltage_Error_Stage_2;

//Functions to grab raw ADC values from the two enabled pins
void Get_ADC_Raw_Stage_1(void){
    ADC_Raw_Stage_1 =  ADC_Read_AN2(); // replaces hardcoded 1
}

void Get_ADC_Raw_Stage_2(void){
    ADC_Raw_Stage_2 = ADC_Read_AN3(); // replaces hardcoded 2     
}

//Functions to scale the raw ADC value from voltage divider and then convert it to a mV value
void Scale_Convert_ADC_Stage_1(void){
    Scaled_ADC_Stage_1 = (ADC_Raw_Stage_1 * ADC_REF) / ADC_RES; // pin voltage in mV
    Voltage_mv_Stage_1 = (Scaled_ADC_Stage_1 * Stage_1_NUM) / Stage_1_DEN;
}

void Scale_Convert_ADC_Stage_2(void){
    Scaled_ADC_Stage_2 = (ADC_Raw_Stage_2 * ADC_REF) / ADC_RES;
    Voltage_mv_Stage_2 = (Scaled_ADC_Stage_2 * Stage_2_NUM) / Stage_2_DEN;
}

//Functions to get error from voltage setpoint and measured voltage
void Error_Calc_Stage_1(void) {
    Voltage_Error_Stage_1 = (int32_t)V_TARGET_STAGE_1 - (int32_t)Voltage_mv_Stage_1;
}

void Error_Calc_Stage_2(void) {
    Voltage_Error_Stage_2 = (int32_t)V_TARGET_STAGE_2 - (int32_t)Voltage_mv_Stage_2;
}








