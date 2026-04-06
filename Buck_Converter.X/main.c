/* 
 * File:   main.c
 * Author: izakd
 *
 * Created on December 17, 2025, 8:59 AM
 * 
 * Wiring Summary: 
    Stage 1 sense (80V rail  divider  RB7/AN2)  ADC_Read_AN2()
    Stage 2 sense (12V rail  divider  RA3/AN3)  ADC_Read_AN3()

    Stage 1 PWM high-side  RB14 (PWM1H)  gate driver IN+
    Stage 1 PWM low-side   RB15 (PWM1L)  gate driver IN-
    Stage 2 PWM high-side  RB12 (PWM2H)  gate driver IN+
    Stage 2 PWM low-side   RB13 (PWM2L)  gate driver IN-
 */

#include <xc.h>
#include "macros.h"
#include "Control.h"
#include "Init.h"
#include "pwm.h"
#include "adc.h"

int main(void) {
    System_Init();
    ADC_Init();
    PWM_Init(); // rplacers bare Period_Calc()
    
    while(true){
        Get_ADC_Raw_Stage_1();
        Get_ADC_Raw_Stage_2();
        Scale_Convert_ADC_Stage_1();
        Scale_Convert_ADC_Stage_2();
        Error_Calc_Stage_1();  
        Error_Calc_Stage_2();
        // Step 4: PI_Compute() calls go here
        // Step 6: this whole block moves into a Timer ISR
    }

    return 0;
}

