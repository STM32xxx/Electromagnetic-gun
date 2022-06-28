#ifndef _PCF8591_H_
#define _PCF8591_H_

#include "stm32f10x.h"

#define  PCF8591_1 0x90//PCF8591 µÿ÷∑
#define  PCF8591_2 0x92//PCF8591 µÿ÷∑

#define _ADC0 0x41
#define _ADC1 0x42
#define _ADC2 0x43
#define _ADC3 0x40
#define _DAC0 0x40

void PCF8591_Send(u8 add,u8 reg,u8 data);
u8 PCF8591_Read(u8 add,u8 reg);

#endif


