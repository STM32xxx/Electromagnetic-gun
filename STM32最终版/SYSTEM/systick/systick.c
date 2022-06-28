#include "systick.h"
#include "stdio.h"
#include "steering.h"
#include "usart.h"
#include "string.h"
#include "adc.h"

#define PWM_MAX 0
#define DEADLINE_CONTROL 0

//unsigned char buff[4];
//volatile short ball_x,ball_y;//保存目标的当前坐标
//float angle_x = 90,angle_y = 90;
unsigned int i;
//short xNext = 160,yNext = 120;//保存目标的下次目标坐标
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
	/* SystemFrequency / 100     10ms中断一次
	   SystemFrequency / 200     5ms中断一次
	   SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	SystemCoreClockUpdate();
	if (SysTick_Config(SystemCoreClock / 50))// ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;// 开启滴答定时器
}

void SysTick_Handler(void)//嘀嗒时钟中断程序
{
//	i = Get_ADC_IN0();
//	printf("v=%d\n",i);
	
//	static char i = 0;
//	if(i++ == 50){i=0;GPIOC->ODR ^= 0x2000;}//指示灯间隔一秒闪烁
	
//	if(USART3_Receive == 1)
//	{
//		USART3_Receive = 0;
//		buff[0]=USART3_RX_BUF[1];buff[1]=USART3_RX_BUF[2];buff[2]=USART3_RX_BUF[3];buff[3]=USART3_RX_BUF[4];
//		memset(USART3_RX_BUF,0,USART3_REC_LEN);
//		
//		if(buff[0] == 0xfe)//未检测到目标
//		{
////			printf("none4\n");
//		}
//		else
//		{
//			ball_x = buff[0]<<8 | buff[1];//得到目标的x坐标
//			ball_y = buff[2]<<8 | buff[3];//得到目标的y坐标
//			
//			ball_x -= 160;ball_y -= 120;//计算目标位置与终点的偏差
//			angle_x += ball_x*0.04;//比例控制，比例系数为0.05比较合适
//			angle_y += ball_y*0.04;//比例控制
//			
//			if(angle_x>180) angle_x=180;//输出限幅
//			if(angle_x<0) angle_x=0;//输出限幅
//			if(angle_y>180) angle_y=180;//输出限幅
//			if(angle_y<0) angle_y=0;//输出限幅
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
	
//	if(ball_y == 0x4000)//摄像头未检测到目标
//	{
//		printf("none2\n");
//	}
//	else//摄像头检测到目标
//	{
//		ball_x -= 160;
//		ball_y -= 120;
//		printf("p:%d,%d\n",ball_x,ball_y);
		
		//到此时ball_x和ball_y保存的是目标在摄像头视野中的坐标
//		Select_Mode(0);
		//加入修改PID参数的函数
//		pwm_x = underpropX(ball_x);
//		pwm_y = underpropY(ball_y);
//		ball_x-=160;ball_y-=120;
////		printf("%d,%d\n",ball_x,ball_y);
//		
//		steerX.Target = 0;steerY.Target = 0;//设定目标值
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


