/* 
 * File:   main.c
 * Author: izakd
 *
 * Created on December 17, 2025, 8:59 AM
 */

#include <xc.h>
#include "macros.h"
#include "Control.h"
#include "Init.h"
#include "pwm.h"


int main(int argc, char** argv) {
    
    System_Init();
    ADC_Init();
    Period_Calc();
    
    while(true){
        Get_ADC_Raw_Stage_1();
        Get_ADC_Raw_Stage_2();
        Scale_Convert_ADC_Stage_1();
        Scale_Convert_ADC_Stage_2();
        Error_Calc_Stage_1();  
        Error_Calc_Stage_2();
    }

    return (EXIT_SUCCESS);
}

