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

******************��������********************>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

�����ľ��
����������:2019/8/3
�������ݣ�
1��SYN6288����ģ�����
2��MPU6050���ٶȼ����������ݶ�ȡ�����ģ��IIC��
3��ֱ����PWM���ƣ���������������
4��SPWM�������β�������Ƶ����ռ�ձ�
5��SPI�ӿ�OLED�����ģ��SPI�����޸�ΪӲ��SPI��
6��2·������ģ���ࣨ���Ż���
7��hmi������ͨ��
8��2·ADC�ɼ�������չΪ��·��
9�����������������
10��ESP8266wifiģ��ATָ������룩
11������ʾ����
12���ڲ�RTCʱ��
13��USB���⴮�ڵ��ԣ������룩
14��2·DAC�����������оƬ���д˹��ܣ�
15��TF�����ݶ�ȡ�������룩
17���ڲ��¶ȴ�����
18��flashģ��eeprom���洢���ݵ��粻��ʧ
19���������˲�����̬�ںϽ��㣨�����룩
20��HMC5883L�����������ݶ�ȡ�����ģ��IIC����ԭiicͨ�Ŵ���bug�����ṹ���ң�������
21��PCF8591��IICͨ�ţ���ͬʱ���������߹ҽӶ���豸
22��USART3��openmv��ͨ��
23��
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

float Distance1=0.0;//�趨�ľ���ֵ
float Angle1=0.0;//�趨�ĽǶ�ֵ��-30~30��

void SWDConfig(void)//ʹ��SWD���ԣ�ʧ��JTAG��PA15,PB3,PB4��Ϊ��ͨIO��ʹ��
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
	USART2_Init(9600);//�봮����ͨ��
	USART3_Init(9600);//��openmvͨ��
//	TIM4_Encoder_Init();
	TIM3_PWM_Init();
//	SPWM_Init();
//	i2cInit();
//	MPU6050_Init();
//	HMC5883L_Init();
	ADC_IN0_Init();
//	ADC_IN5_Init();
//	SR04_Trig_Init();//����������io��
//	HC_SR04_Init();//���������岶��
//	Dac1_Init();//������stm32����dac����
//	OLED_Init();
//	rtc_init();//RTC���ó�ʼ��,�����趨ʱ��ȡ��ע�ͣ������ϵ�������ʱ,���˺���ע��
//	clockinit_RTC();
//	TempSensor_Init();//�ڲ��¶ȴ�������ʹ��ADC1ͨ��16
//	FlashDataSave();//�����ݴ���flash
	LEDInit();
	SysTick_Init();
	
	STOP;//�ضϿɿع�
	
	delay_ms(200);

	steer_test();//�����λ

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

//	while(1)//openmvͨ�ų���
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
///******************���ܵ����������ݽ��д���************************/
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



