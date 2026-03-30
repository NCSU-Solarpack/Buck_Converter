/* 
 * File:   Control.h
 * Author: izakd
 *
 * Created on December 17, 2025, 12:03 PM
 */

#ifndef CONTROL_H
#define	CONTROL_H

//Definitions


//Function prototypes


void Get_ADC_Raw_Stage_1(void);
void Scale_Convert_ADC_Stage_1(void);
void Error_Calc_Stage_1(void);

void Get_ADC_Raw_Stage_2(void);
void Scale_Convert_ADC_Stage_2(void);
void Error_Calc_Stage_2(void);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* CONTROL_H */

