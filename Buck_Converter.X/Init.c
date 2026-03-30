/* 
 * File:   Init.c
 * Author: izakd
 *
 * Created on December 17, 2025, 1:48 PM
 */

#include "macros.h"
#include "Init.h"

//Numerator and denominator variables for each stage voltage division calculation
uint32_t Stage_1_NUM;
uint32_t Stage_1_DEN;
uint32_t Stage_2_NUM;
uint32_t Stage_2_DEN;

void System_Init(void){
    
    Stage_1_NUM = (RTOP_STAGE_1_K + RBOT_STAGE_1_K);
    Stage_1_DEN = (RBOT_STAGE_1_K);
    Stage_2_NUM = (RTOP_STAGE_2_K + RBOT_STAGE_2_K);
    Stage_2_DEN = (RBOT_STAGE_2_K);
    
}

void ADC_Init(void){
    // Turn on ADC module
    ADCON1Lbits.ADON = 0;      // Disable ADC before config

    // Clock setup
    ADCON3Hbits.CLKSEL = 0;    // FOSC/2
    ADCON3Hbits.CLKDIV = 1;    // Clock divider

    // Sampling and conversion
    ADCON1Hbits.FORM = 0;      // Integer format
    ADCON1Hbits.SHRRES = 3;    // 12-bit resolution
    ADCON2Lbits.SHRADCS = 10;  // Sample time

    // Enable shared core
    ADCON3Lbits.SHRPWR = 1;    // Power shared core
    while(ADCON3Lbits.SHRRDY == 0); // Wait until ready

    // Enable channel AN2
    ADMOD0Lbits.SIGN2 = 0;     // Unsigned
    ADMOD0Lbits.DIFF2 = 0;     // Single-ended

    // Select AN2 for input
    ADIELbits.IE2 = 0;         // No interrupt needed

    // Turn ADC on
    ADCON1Lbits.ADON = 1;  
}