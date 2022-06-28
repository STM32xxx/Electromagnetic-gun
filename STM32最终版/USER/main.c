/********************************************************************************
       ______________________________
      / 														 \
     /                                \
    /   ________            _______    \
   /   /||||    \          /||||   \    \
  /   / ||||     \        / ||||    \    \
 /   /  ||||___   \      /  ||||___  \    \
 |  /__/       \__ \    /__/       \__\   |
 |                                        | 
 |                                        |
 |                                        |
 |															  	      |
 |	   \  									    / 		    |
 \ 	    \		 						 	     / 		  	 	/                                            
  \      \                    /          /
   \	 	  \									 /		  		/
	  \      \________________/          /
		 \																/	
		  \______________________________/

******************滑稽护体********************>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

故里草木深
最后更新日期:2019/8/3
包含内容：
1·SYN6288语音模块控制
2·MPU6050加速度计陀螺仪数据读取（软件模拟IIC）
3·直流电PWM控制，编码器正交解码
4·SPWM互补波形产生，调频，调占空比
5·SPI接口OLED（软件模拟SPI，待修改为硬件SPI）
6·2路超声波模块测距（待优化）
7·hmi串口屏通信
8·2路ADC采集（待扩展为多路）
9·步进电机高速脉冲
10·ESP8266wifi模块AT指令（待加入）
11·虚拟示波器
12·内部RTC时钟
13·USB虚拟串口调试（待加入）
14·2路DAC输出（大容量芯片才有此功能）
15·TF卡内容读取（待加入）
17·内部温度传感器
18·flash模拟eeprom，存储数据掉电不丢失
19·卡尔曼滤波，姿态融合解算（待加入）
20·HMC5883L电子罗盘数据读取（软件模拟IIC）（原iic通信存在bug）（结构较乱，待整理）
21·PCF8591的IIC通信，可同时在两根总线挂接多个设备
22·USART3与openmv的通信
23·
*******************************************************************************/
#include "stdio.h"
#include "string.h"
#include "math.h"

#include "led.h"
#include "systick.h"
#include "outputdata.h"
#include "usart.h"
#include "motor.h"
#include "adc.h"
#include "ultrasound.h"
#include "dac.h"
#include "hmi.h"
#include "steering.h"
#include "oled.h"
#include "rtc.h"
#include "flash.h"
#include "i2c.h"
#include "mpu6050.h"
#include "hmc5883l.h"
#include "delay.h"
#include "pcf8591.h"
#include "boom.h"

float Distance1=0.0;//设定的距离值
float Angle1=0.0;//设定的角度值（-30~30）

void SWDConfig(void)//使能SWD调试，失能JTAG，PA15,PB3,PB4作为普通IO口使用
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
}

int main(void)
{
	unsigned int i;
	short num;
	
	SWDConfig();
	NVIC_Configuration();

//	USART1_Init(115200);
	USART2_Init(9600);//与串口屏通信
	USART3_Init(9600);//与openmv通信
//	TIM4_Encoder_Init();
	TIM3_PWM_Init();
//	SPWM_Init();
//	i2cInit();
//	MPU6050_Init();
//	HMC5883L_Init();
	ADC_IN0_Init();
//	ADC_IN5_Init();
//	SR04_Trig_Init();//超声波输入io口
//	HC_SR04_Init();//超声波脉冲捕获
//	Dac1_Init();//大容量stm32才有dac功能
//	OLED_Init();
//	rtc_init();//RTC配置初始化,初次设定时间取消注释，如果想断电后继续走时,将此函数注释
//	clockinit_RTC();
//	TempSensor_Init();//内部温度传感器，使用ADC1通道16
//	FlashDataSave();//将数据存入flash
	LEDInit();
	SysTick_Init();
	
	STOP;//关断可控硅
	
	delay_ms(200);

	steer_test();//舵机复位

	while(1)
	{
		hmi_receive();
	}

//	steerSet(0,X_ZERO);
//	steerSet(1,Y_ZERO);

//	mode4();

//		RELAY_ON;
//		delay_ms(2000);
//		RELAY_OFF;

//	BEEP_ON;
//	delay_ms(1000);
//	BEEP_OFF;

//	RELAY_ON;

//	while(1)
//	{
//		for(num=0;num<5;num++)
//			i += Get_ADC_IN0();
//		i /= 5;
////		printf("i=%d\n",i);
//		if(i >= _250V)
//		{
//			RELAY_OFF;
//			break;
//		}
//		delay_ms(20);
//	}

//	delay_ms(2000);
//	BEEP_ON;
//	delay_ms(1000);
//	BEEP_OFF;
//	FIRE;
//	delay_ms(500);
//	STOP;

////////////////////////////////////////
	
//		RELAY_ON;
//		delay_ms(2000);
//		RELAY_OFF;
		
//		delay_ms(2000);
//		FIRE;
//		delay_ms(500);
//		STOP;
//		delay_ms(1000);

//	steerSet(0,X_ZERO);
//	steerSet(1,Y_ZERO);

//	while(1)
//	{
//		RELAY_ON;
//		delay_ms(10000);
//		RELAY_OFF;
//		delay_ms(1000);
//	}

//	while(1)
//	{
//		for(i=0;i<180;i+=0.1)
//		{
//			steerSet(1,i);
//			delay_ms(1);
//		}
//		for(i=180;i>0;i-=0.1)
//		{
//			steerSet(1,i);
//			delay_ms(1);
//		}
//	}

//	while(1)
//	{
//		RELAY_ON;
//		delay_ms(10000);
//		RELAY_OFF;
//		delay_ms(1000);
//	}

//	while(1)
//	{
//		i = Get_ADC_IN0();
//		printf("%d\n",i);
//		delay_ms(200);
//	}

//	steerSet(0,90);
//	steerSet(1,90);

//	while(1)
//	{
//		for(i=0;i<180;i+=0.1)
//		{
//			steerSet(0,i);
//			delay_ms(5);
//		}
//		for(i=180;i>0;i-=0.1)
//		{
//			steerSet(0,i);
//			delay_ms(5);
//		}
//	}

//	while(1)//openmv通信程序
//	{
//		if(USART3_Receive == 1)
//		{
//			USART3_Receive = 0;
//			buff[0]=USART3_RX_BUF[1];buff[1]=USART3_RX_BUF[2];buff[2]=USART3_RX_BUF[3];buff[3]=USART3_RX_BUF[4];
//			memset(USART3_RX_BUF,0,USART3_REC_LEN);
//			
//			if(buff[0] == 0xfe)
//				ball_y = 0x4000;
//			else
//			{
//				ball_x = buff[0]<<8 | buff[1];
//				ball_y = buff[2]<<8 | buff[3];
//			}
//		}
//	}

//	while(1)
//	{
//		dis = Read_Distane_4();
//		if(dis < 300)
//			printf("%d\n",dis);
//		else
//			printf("none\n");
//		delay_ms(100);
//	}

	
//	while(1)
//	{
//		i[0] = PCF8591_Read(PCF8591_1,_ADC0);delay_us(20);
//		i[1] = PCF8591_Read(PCF8591_1,_ADC1);delay_us(20);
//		i[2] = PCF8591_Read(PCF8591_1,_ADC2);delay_us(20);
//		i[3] = PCF8591_Read(PCF8591_1,_ADC3);delay_us(20);
//		PCF8591_Send(PCF8591_1,_DAC0,200);
//		printf("%d %d %d %d\n",i[0],i[1],i[2],i[3]);
//		
//		i[0] = PCF8591_Read(PCF8591_2,_ADC0);delay_us(20);
//		i[1] = PCF8591_Read(PCF8591_2,_ADC1);delay_us(20);
//		i[2] = PCF8591_Read(PCF8591_2,_ADC2);delay_us(20);
//		i[3] = PCF8591_Read(PCF8591_2,_ADC3);delay_us(20);
//		PCF8591_Send(PCF8591_2,_DAC0,100);
//		printf("%d %d %d %d\n",i[0],i[1],i[2],i[3]);

//		i[0] = DataSynthesis(ACCEL_XOUT_H);
//		i[1] = DataSynthesis(ACCEL_YOUT_H);
//		i[2] = DataSynthesis(ACCEL_ZOUT_H);
//		printf("%d %d %d\n",i[0],i[1],i[2]);
//	
//		hmc_read_XYZ(data);
//		angle=atan2((double)data[2],(double)data[0])*(180 / 3.14159265)+180;
//		printf("%.3f\n",angle);
//		delay_ms(200);
//	}

//	Set_PID(0,10,0,0);
	
//	while(1)
//	{
//		printf("%.2f\t%d\n",Get_ADC_IN4(),Get_ADC_IN5());
//		delay_ms(100);
//	}
	
//	while(1)
//	{
//		Read_Distane();
//		printf("%d\n",Distance);
//		delay_ms(300);
//	}

//	while(1)
//	{
///******************接受到串口屏数据进行处理************************/
//		if(HMI_USART_Receive)
//		{
//			HMI_USART_Receive = 0;
//			s = HMI_ReceiveTxt(HmiRxBuff);
//			memset(HmiRxBuff,0,sizeof(HmiRxBuff));
//			
//			printf("%.3f\n",s);
////			printf("%.3f\n",s);
//		}
///******************************************************************/
//	}

//	HMI_SendTxt("0","t0v",45.6);
	
	while(1);
}



