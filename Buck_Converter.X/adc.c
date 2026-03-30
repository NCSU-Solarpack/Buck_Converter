/* 
 * File:   adc.c
 * Author: izakd
 *
 * Created on December 18, 2025, 2:39 PM
 */

#include "macros.h"

uint16_t ADC_Read_AN2(void)
{
    ADCORE0Hbits.SAMC = 10;    // Sample time
    ADCORE0Hbits.ADCS = 2;     // Conversion clock

    // Select channel
    ADCHS0Lbits.CH0SA = 2;     // Select AN2

    // Start sampling
    ADCON3Lbits.GSWTRG = 1;

    // Wait for conversion
    while(ADSTATLbits.AN2RDY == 0);

    // Read result
    return ADCBUF2;
}
