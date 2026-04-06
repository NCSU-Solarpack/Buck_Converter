/* 
 * File:   adc.c
 * Author: izakd
 *
 * Created on December 18, 2025, 2:39 PM
 */

#include <xc.h>
#include "adc.h"

uint16_t ADC_Read(channel) {
    // Set the channel in the shared ADC trigger source select
    ADCON3Lbits.CNVCHSEL = channel;   // Select AN2 or AN3
    ADCON3Lbits.CNVRTCH  = 1;         // Trigger single conversion
    while(ADCON3Lbits.CNVRTCH == 1);  // Wait for conversion complete
    return ADCBUF2;  // Both channels land in their own buffer (ADCBUF2 / ADCBUF3)
                     // For AN3 return ADCBUF3 — split into two wrappers below
}

uint16_t ADC_Read_AN2(void) {
    ADCON3Lbits.CNVCHSEL = 2;
    ADCON3Lbits.CNVRTCH  = 1;
    while(ADCON3Lbits.CNVRTCH == 1);
    return ADCBUF2;
}

uint16_t ADC_Read_AN3(void) {
    ADCON3Lbits.CNVCHSEL = 3;
    ADCON3Lbits.CNVRTCH  = 1;
    while(ADCON3Lbits.CNVRTCH == 1);
    return ADCBUF3;
}
