#ifndef _ADC_H_
#define _ADC_H_	

#include "stm32f10x.h"
#include "stm32f10x_adc.h"

void TempSensor_Init(void);
float Get_TempVal(void);
void ADC_IN0_Init(void);
void ADC_IN5_Init(void);
unsigned short Get_ADC_IN0(void);
unsigned short Get_ADC_IN5(void);

#endif

