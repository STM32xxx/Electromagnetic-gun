#include "boom.h"
#include "usart.h"
#include "led.h"
#include "adc.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "steering.h"
#include "hmi.h"

#define X_ZERO (85.0f)
#define Y_ZERO (63.0f)

#define _180V (2400)
#define _220V (2900)
#define _250V (3200)
#define _265V (3400)
#define _270V (3480)
#define _285V (3600)
#define _300V (3800)
#define _310V (4000)

extern unsigned char HmiRxBuff[USART2_REC_LEN];
extern unsigned char HMI_USART_Receive;
unsigned char distance1[20] = "\0";//接收串口屏发送距离字符串
unsigned char angle1[20]="\0" ;//接收串口屏发送角度字符串
extern unsigned char clear[];

extern float Distance1;//设定的距离值
extern float Angle1;//设定的角度值（-30~30）

void steer_test(void)
{
	steerSet(0,X_ZERO);
	steerSet(1,Y_ZERO);//最大143.5，小63
	
//	delay_ms(1000);
//	
//	while(1)
//	{
//		steerSet(0,X_ZERO+30);
//		steerSet(1,Y_ZERO);//最大143.5，小63
//		delay_ms(1000);
//		
//		steerSet(0,X_ZERO-30);
//		steerSet(1,Y_ZERO);//最大143.5，小63
//		delay_ms(1000);
//	}
}

void sheding()
{
	int i=0,h=0,j=0,hmi_len;
	
		hmi_len = strlen((const char*)HmiRxBuff);
	      angle1[h]='#';
			hmi_len = strlen((const char*)HmiRxBuff);
				for(i=2;i<hmi_len - j ;i++)
				{
					
					if(HmiRxBuff[i]==0x64)
					{
						angle1[h+1]='*';
						angle1[h+2]='\0';
						distance1[j]='#';
						for(h=i;h<hmi_len;h++)
						{
							j++;
							distance1[j]=HmiRxBuff[h];	
						}
					}
					h++;
					angle1[h] = HmiRxBuff[i];				
				}
				j = 0;
				
				Distance1 = HMI_ReceiveTxt(distance1);
				HMI_SendTxt("2","t3",Distance1);
		    Angle1 = HMI_ReceiveTxt(angle1);
				HMI_SendTxt("2","t4",Angle1);	
				
//				printf("%f,%f\n",Angle1,Distance1);
				
				hmi_len = strlen((const char*)distance1);
				for(i=0;i<hmi_len;i++)
				{
					distance1[i]=clear[i];
				}	
				
				hmi_len = strlen((const char*)angle1);			
				for(i=0;i<hmi_len;i++)
				{
					angle1[i]=clear[i];
				}	
}

void hmi_receive(void)
{
	int i=0,h=0,j=0,hmi_len;
	float d;
	if(HMI_USART_Receive==1)
		{
			HMI_USART_Receive=0;
			
			if(HmiRxBuff[1]==0x01)//模式1
			{
				mode1();
			}
			else if(HmiRxBuff[1]==0x02)//模式2
			{
				sheding();//32返回一个值显示，确定已接收到
				mode2();
			}
			else if(HmiRxBuff[1]==0x03)//模式3
			{
				sheding();
				mode3();
			}
			else if(HmiRxBuff[1]==0x04)//模式4
			{
				mode4();
			}
			else if(HmiRxBuff[1]==0x05)//模式5
			{
				mode5();
			}
			hmi_len = strlen((const char*)HmiRxBuff);
			for(i=0;i<hmi_len;i++)
			{
				HmiRxBuff[i]=clear[i];
			}		
		}
}

void mode1(void)//基本要求第一步
{
	short num;
	unsigned int ad;
	
	steer_test();
	
	delay_ms(500);
	
	steerSet(0,X_ZERO);
	steerSet(1,Y_ZERO+30);//最大143.5，小64
	
	BEEP_ON;//蜂鸣器提示
	delay_ms(1000);
	BEEP_OFF;
	
	RELAY_ON;//开始充电
	while(1)
	{
		for(num=0;num<5;num++)//5次求平均值
			ad += Get_ADC_IN0();
		ad /= 5;
		if(ad >= _265V)
		{
			RELAY_OFF;
			break;
		}
		delay_ms(20);
	}
	
	delay_ms(2000);
	BEEP_ON;
	delay_ms(1000);
	BEEP_OFF;
	FIRE;
	delay_ms(500);
	STOP;
	
	steer_test();
}

void mode2(void)//基本要求第二步
{
	float d = Distance1;
	float ang = 0;
	short num;
	unsigned int ad;
	
	steer_test();
	delay_ms(500);
	
	if(d>=200 && d<=222)
		ang = 125+(d-200)*0.25f;
	
	if(d>222 && d<=235)
		ang = 130+(d-222)*0.23f;
	
	if(d>235 && d<=260)
		ang = 135+(d-235)*0.18f;
	
	if(d>260 && d<=300)
		ang = 140+(d-260)*0.088f;
	
	//调节仰角
	if(ang < Y_ZERO) ang = Y_ZERO;
	if(ang > 143.5) ang = 143.5;
	
	steerSet(1,ang);
	
	BEEP_ON;//蜂鸣器提示
	delay_ms(1000);
	BEEP_OFF;
	
	RELAY_ON;//开始充电
	while(1)
	{
		for(num=0;num<5;num++)//5次求平均值
			ad += Get_ADC_IN0();
		ad /= 5;
		if(ad >= _265V)
		{
			RELAY_OFF;
			break;
		}
		delay_ms(20);
	}
	
	delay_ms(2000);
	BEEP_ON;
	delay_ms(1000);
	BEEP_OFF;
	FIRE;
	delay_ms(500);
	STOP;
	
	steer_test();
}

void mode3(void)//基本要求第三步
{
	float a = Angle1;
	float d = Distance1;
	float ang = 0;
	short num;
	unsigned int ad;
	
	steer_test();
	
	delay_ms(500);
	
	if(d>=200 && d<=222)
		ang = 125+(d-200)*0.25f;
	
	if(d>222 && d<=235)
		ang = 130+(d-222)*0.23f;
	
	if(d>235 && d<=260)
		ang = 135+(d-235)*0.18f;
	
	if(d>260 && d<=300)
		ang = 140+(d-260)*0.088f;
	
	a=X_ZERO-a;
//	a = -(a + X_ZERO);
	
	if(a > X_ZERO+33) a=X_ZERO+33;//限幅
	if(a < X_ZERO-33) a=X_ZERO-33;
	
	if(ang < Y_ZERO) ang = Y_ZERO;
	if(ang > 143.5) ang = 143.5;
	
	steerSet(0,a);
	steerSet(1,ang);
	
	BEEP_ON;//蜂鸣器提示
	delay_ms(1000);
	BEEP_OFF;
	
	RELAY_ON;//开始充电
	while(1)
	{
		for(num=0;num<5;num++)//5次求平均值
			ad += Get_ADC_IN0();
		ad /= 5;
		if(ad >= _265V)
		{
			RELAY_OFF;
			break;
		}
		delay_ms(20);
	}
	
	delay_ms(2000);
	BEEP_ON;
	delay_ms(1000);
	BEEP_OFF;
	FIRE;
	delay_ms(500);
	STOP;
	
	steer_test();
}

void mode4(void)//发挥部分第一步
{
	short num = 0;//循环计数变量
	unsigned int ad = 0;
	unsigned char buff[6];
	short ball_x,ball_y,ball_dis;
	float angle_x = X_ZERO;
	unsigned char flag = 0;
	short count = 0;
	float ang = 0;

	steer_test();
	delay_ms(200);
	
	while(1)
	{
		if(USART3_Receive == 1)//接收到摄像头数据
		{
			USART3_Receive = 0;
			buff[0]=USART3_RX_BUF[1];buff[1]=USART3_RX_BUF[2];buff[2]=USART3_RX_BUF[3];buff[3]=USART3_RX_BUF[4];
			buff[4]=USART3_RX_BUF[5];buff[5]=USART3_RX_BUF[6];
			memset(USART3_RX_BUF,0,USART3_REC_LEN);
			
			if(buff[0] == 0xfe)//未找到定标点
			{
				if(angle_x < (X_ZERO+33) && flag==0)
				{
					angle_x += 0.8;
//					angle_x += 0.3;
					steerSet(0,angle_x);
					if(angle_x > X_ZERO+30-5)
						flag = 1;
				}
				if(angle_x > (X_ZERO-33) && flag==1)//摄像头主动扫描寻找
				{
					angle_x -= 0.8;
//					angle_x -= 0.3;
					steerSet(0,angle_x);
					if(angle_x < X_ZERO-30+5)
						flag = 0;
				}
			}
			else//找到定标点
			{
				ball_x = buff[0]<<8 | buff[1];//定标点x坐标
				ball_y = buff[2]<<8 | buff[3];//定标点y坐标
				ball_dis = buff[4]<<8 | buff[5];//定标点距离
				
				ball_x -= 160;//计算目标位置与终点的偏差

				if(ball_x>-3 && ball_x<3)
					count++;
		
				angle_x += ball_x*0.01f;//比例控制，比例系数为0.05比较合适
			
				if(angle_x > (X_ZERO+33)) angle_x = X_ZERO+33;//输出限幅
				if(angle_x < (X_ZERO-33)) angle_x = X_ZERO-33;//输出限幅
				
				steerSet(0,angle_x);
				
				if(count > 40)//确定已经瞄准
				{
					break;
				}
			}
		}
	}
	
	BEEP_ON;delay_ms(100);BEEP_OFF;delay_ms(100);
	BEEP_ON;delay_ms(100);BEEP_OFF;delay_ms(100);
	
//调节y轴部分代码
	
	ball_dis -= 20;
	
	ang = 135;//不符合距离值直接往两米五打
	
	if(ball_dis>=200 && ball_dis<=222)
		ang = 125+(ball_dis-200)*0.25f;
	
	if(ball_dis>222 && ball_dis<=235)
		ang = 130+(ball_dis-222)*0.23f;
	
	if(ball_dis>235 && ball_dis<=260)
		ang = 135+(ball_dis-235)*0.18f;
	
	if(ball_dis>260 && ball_dis<=300)
		ang = 140+(ball_dis-260)*0.088f;
	
	if(ang < Y_ZERO) ang = Y_ZERO;
	if(ang > 143.5) ang = 143.5;
	
	steerSet(1,ang);
	
	BEEP_ON;//蜂鸣器提示
	delay_ms(200);
	BEEP_OFF;

	RELAY_ON;//开始充电
	while(1)
	{
		for(num=0;num<5;num++)//5次求平均值
			ad += Get_ADC_IN0();
		ad /= 5;
		if(ad >= _265V)
		{
			RELAY_OFF;
			break;
		}
		delay_ms(20);
	}
	
	delay_ms(500);
	BEEP_ON;
	delay_ms(100);
	BEEP_OFF;
	FIRE;
	delay_ms(800);
	STOP;

	steer_test();
}

void mode5(void)//发挥部分第一步
{
	short num = 0;//循环计数变量
	unsigned int ad = 0;
	unsigned char buff[6];
	short ball_x,ball_y;
	float angle_x = X_ZERO+30;
	char flag = 1;
	short count = 0;
	float ang = 0;

	steerSet(0,X_ZERO+30);
	steerSet(1,Y_ZERO);//最大143.5，小63
	delay_ms(100);
	
	while(1)
	{
		if(USART3_Receive == 1)//接收到摄像头数据
		{
			USART3_Receive = 0;
			buff[0]=USART3_RX_BUF[1];buff[1]=USART3_RX_BUF[2];buff[2]=USART3_RX_BUF[3];buff[3]=USART3_RX_BUF[4];
			buff[4]=USART3_RX_BUF[5];buff[5]=USART3_RX_BUF[6];
			memset(USART3_RX_BUF,0,USART3_REC_LEN);
			
			if(buff[0] == 0xfe)//未找到定标点
			{
				if(angle_x < (X_ZERO+33) && flag==0)
				{
					angle_x += 0.8;
					steerSet(0,angle_x);
					if(angle_x > X_ZERO+30-8)
						flag = 1;
				}
				if(angle_x > (X_ZERO-33) && flag==1)//摄像头主动扫描寻找
				{
					angle_x -= 0.8;
					steerSet(0,angle_x);
					if(angle_x < X_ZERO-30+8)
						flag = 0;
				}
			}
			else//找到定标点
			{
				ball_x = buff[0]<<8 | buff[1];//定标点x坐标
				ball_y = buff[2]<<8 | buff[3];//定标点y坐标
				
				ball_x -= 160;//计算目标位置与终点的偏差

				if(ball_x>-3 && ball_x<3)
					count++;
				
				angle_x += ball_x*0.01f;//比例控制，比例系数为0.05比较合适
				
				if(angle_x > (X_ZERO+33)) angle_x = X_ZERO+33;//输出限幅
				if(angle_x < (X_ZERO-33)) angle_x = X_ZERO-33;//输出限幅
				
				steerSet(0,angle_x);
				
				if(count > 20)//确定已经瞄准
				{
					break;
				}
			}
		}
	}
	
	BEEP_ON;delay_ms(100);BEEP_OFF;
	
//调节y轴部分代码
	ang = 138.5;//y轴固定250cm

	if(ang < Y_ZERO) ang = Y_ZERO;
	if(ang > 143.5) ang = 143.5;
	
	steerSet(1,ang);
	
	BEEP_ON;//蜂鸣器提示
	delay_ms(100);
	BEEP_OFF;

	RELAY_ON;//开始充电
	while(1)
	{
		for(num=0;num<5;num++)//5次求平均值
			ad += Get_ADC_IN0();
		ad /= 5;
		if(ad >= _265V)
		{
			RELAY_OFF;
			break;
		}
		delay_ms(20);
	}
	
	delay_ms(100);
	BEEP_ON;
	delay_ms(100);
	BEEP_OFF;
	FIRE;
	delay_ms(500);
	STOP;

	steerSet(0,X_ZERO+30);
	steerSet(1,Y_ZERO);//最大143.5，小63
}

void chongdian(short V)
{
	unsigned int vol;
	short num;
	
	RELAY_ON;
	while(1)
	{
		for(num=0;num<5;num++)
			vol += Get_ADC_IN0();
		vol /= 5;
		if(vol >= V)
		{
			RELAY_OFF;
			break;
		}
		delay_ms(20);
	}
}

void HMI_Scan(void)
{
	if(HMI_USART_Receive==1)
	{
		HMI_USART_Receive=0;

		if(HmiRxBuff[1]==0x01)//模式1
		{
			mode1();
		}
		else if(HmiRxBuff[1]==0x02)//模式2
		{
//			mode2();
		}
		else if(HmiRxBuff[1]==0x03)//模式3
		{
			mode3();
		}
		else if(HmiRxBuff[1]==0x04)//模式4
		{
			mode4();
		}
		else if(HmiRxBuff[1]==0x05)//模式5
		{
			mode5();
		}
	}
}



