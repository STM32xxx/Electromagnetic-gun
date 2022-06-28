#include "steering.h"

void steerSet(char num,float angle)
{
	unsigned short i = angle * 5.5556f;
	i += 250.0f;
	switch(num)
	{
		case 0:TIM_SetCompare1(TIM3,i);break;
		case 1:TIM_SetCompare2(TIM3,i);break;
		case 2:break;
		case 3:break;
		default:break;
	}
}




