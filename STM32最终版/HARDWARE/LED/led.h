#ifndef _LED_H_
#define _LED_H_	

#include "stm32f10x.h"

#define BEEP_ON  {GPIO_ResetBits(GPIOB, GPIO_Pin_3);}
#define BEEP_OFF {GPIO_SetBits(GPIOB, GPIO_Pin_3);}

#define RELAY_ON  {GPIO_SetBits(GPIOB, GPIO_Pin_4);}
#define RELAY_OFF {GPIO_ResetBits(GPIOB, GPIO_Pin_4);}

#define FIRE {GPIO_SetBits(GPIOC, GPIO_Pin_13);}
#define STOP {GPIO_ResetBits(GPIOC, GPIO_Pin_13);}

void LEDInit(void);

#endif

