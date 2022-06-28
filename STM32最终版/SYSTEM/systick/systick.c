#include "systick.h"
#include "stdio.h"
#include "steering.h"
#include "usart.h"
#include "string.h"
#include "adc.h"

#define PWM_MAX 0
#define DEADLINE_CONTROL 0

//unsigned char buff[4];
//volatile short ball_x,ball_y;//����Ŀ��ĵ�ǰ����
//float angle_x = 90,angle_y = 90;
unsigned int i;
//short xNext = 160,yNext = 120;//����Ŀ����´�Ŀ������
//float angle_x=90,angle_y=90;

//typedef struct
//{
//	float Kp;
//	float Ki;
//	float Kd;
//	float Error;
//	float Last_Error;
//	float Target;
//}steer;

//steer steerX,steerY;

void SysTick_Init(void)
{
	/* SystemFrequency / 100     10ms�ж�һ��
	   SystemFrequency / 200     5ms�ж�һ��
	   SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	SystemCoreClockUpdate();
	if (SysTick_Config(SystemCoreClock / 50))// ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;// �����δ�ʱ��
}

void SysTick_Handler(void)//���ʱ���жϳ���
{
//	i = Get_ADC_IN0();
//	printf("v=%d\n",i);
	
//	static char i = 0;
//	if(i++ == 50){i=0;GPIOC->ODR ^= 0x2000;}//ָʾ�Ƽ��һ����˸
	
//	if(USART3_Receive == 1)
//	{
//		USART3_Receive = 0;
//		buff[0]=USART3_RX_BUF[1];buff[1]=USART3_RX_BUF[2];buff[2]=USART3_RX_BUF[3];buff[3]=USART3_RX_BUF[4];
//		memset(USART3_RX_BUF,0,USART3_REC_LEN);
//		
//		if(buff[0] == 0xfe)//δ��⵽Ŀ��
//		{
////			printf("none4\n");
//		}
//		else
//		{
//			ball_x = buff[0]<<8 | buff[1];//�õ�Ŀ���x����
//			ball_y = buff[2]<<8 | buff[3];//�õ�Ŀ���y����
//			
//			ball_x -= 160;ball_y -= 120;//����Ŀ��λ�����յ��ƫ��
//			angle_x += ball_x*0.04;//�������ƣ�����ϵ��Ϊ0.05�ȽϺ���
//			angle_y += ball_y*0.04;//��������
//			
//			if(angle_x>180) angle_x=180;//����޷�
//			if(angle_x<0) angle_x=0;//����޷�
//			if(angle_y>180) angle_y=180;//����޷�
//			if(angle_y<0) angle_y=0;//����޷�
//			
////			printf("%.2f,%.2f\n",angle_x,angle_y);
//			
//			steerSet(0,angle_x);
//			steerSet(1,angle_y);
//		}
//	}
	
//	if(buff[0] == 0xfe)
//	{
//		printf("none4\n");
//	}
//	else
//	{
//		ball_x -= 160;
//		ball_y -= 120;
//		printf("%d,%d\n",ball_x,ball_y);
//	}
	
//	if(ball_y == 0x4000)//����ͷδ��⵽Ŀ��
//	{
//		printf("none2\n");
//	}
//	else//����ͷ��⵽Ŀ��
//	{
//		ball_x -= 160;
//		ball_y -= 120;
//		printf("p:%d,%d\n",ball_x,ball_y);
		
		//����ʱball_x��ball_y�������Ŀ��������ͷ��Ұ�е�����
//		Select_Mode(0);
		//�����޸�PID�����ĺ���
//		pwm_x = underpropX(ball_x);
//		pwm_y = underpropY(ball_y);
//		ball_x-=160;ball_y-=120;
////		printf("%d,%d\n",ball_x,ball_y);
//		
//		steerX.Target = 0;steerY.Target = 0;//�趨Ŀ��ֵ
//		
//		if(ball_x>steerX.Target) ball_x-=3;
//		else if(ball_x<steerX.Target) ball_x+=3;
//		if(ball_y>steerY.Target) ball_y-=3;
//		else if(ball_y<steerY.Target) ball_y+=3;
//		
//		pwm_x = underpropX(ball_x);
//		pwm_y = underpropY(ball_y);
//		
//		printf("%d,%d\n",pwm_x,pwm_y);
//	}	
}


