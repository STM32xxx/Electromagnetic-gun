#include "delay.h"

//����ʾ�������ԣ���ʱ�ȽϾ�ȷ 2019.8.4
void delay_us(u32 nTimer)
{
	while(nTimer--){
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}

//��ʱʱ��Խ�������Խ����ʮ������ʱ�ȽϽӽ�
void delay_ms(u32 nTimer)
{
  while(nTimer--)
		delay_us(980);
}


