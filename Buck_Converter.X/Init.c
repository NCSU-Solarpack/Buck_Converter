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
    ADCON1Lbits.ADON = 0;

    ADCON3Hbits.CLKSEL  = 0;    // FOSC/2
    ADCON3Hbits.CLKDIV  = 1;
    ADCON1Hbits.FORM    = 0;    // Unsigned integer
    ADCON1Hbits.SHRRES  = 3;    // 12-bit
    ADCON2Lbits.SHRADCS = 10;

    ADCON3Lbits.SHRPWR = 1;
    while(ADCON3Lbits.SHRRDY == 0);

    // AN2 on RB7 — Stage 1 (80V rail, divided down)
    ADMOD0Lbits.SIGN2 = 0;
    ADMOD0Lbits.DIFF2 = 0;
    ADIELbits.IE2      = 0;

    // AN3 on RA3 — Stage 2 (12V rail, divided down)
    ADMOD0Lbits.SIGN3 = 0;
    ADMOD0Lbits.DIFF3 = 0;
    ADIELbits.IE3      = 0;

    ADCON1Lbits.ADON = 1; // turn on adc
}

