/* 
 * File:   adc.h
 * Author: izakd
 *
 * Created on December 18, 2025, 2:39 PM
 */


#ifndef ADC_H
#define	ADC_H
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

//Function prototypes
uint16_t ADC_Read(uint8_t channel);
uint16_t ADC_Read_AN2(void);
uint16_t ADC_Read_AN3(void);

#ifdef	__cplusplus
}
#endif

#endif

