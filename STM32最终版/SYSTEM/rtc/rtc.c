#include "rtc.h"

u8 timebz;

void RTC_NVIC_Config()	  //Ϊ��ʹ�󱸵���ڶϵ��ʱ����Ȼ��ʱ�����ж����÷�������
{
	NVIC_InitTypeDef NVIC_InitStructure; 	//�жϽṹ�嶨��

	/* ����NVIC���� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn; 	   //��RTC��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 	 //��Ӧ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	 //ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* �� �� ��         : rtc_init
* ��������		   : RTCʱ�ӡ��жϳ�ʼ��	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void rtc_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//�򿪵�Դʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);//�򿪴洢��ʱ��

	PWR_BackupAccessCmd(ENABLE);//ʹ�ܻ���ʧ��RTC�ͺ󱸼Ĵ�������
	BKP_DeInit();//������BKP��ȫ���Ĵ�������Ϊȱʡֵ
	RCC_LSEConfig(RCC_LSE_ON);//�����ⲿ���پ���LSE��
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//���ָ����RCC��־λ�������
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//����RTCʱ�ӣ�RTCCLK��
	RCC_RTCCLKCmd(ENABLE);//ʹ�ܻ���ʧ��RTCʱ��
	RTC_WaitForSynchro(); //�ȴ�RTC�Ĵ���ͬ��
	RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_ITConfig(RTC_IT_SEC,ENABLE);//ʹ�ܻ���ʧ��ָ����RTC�ж�
	RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	RTC_SetPrescaler(32767);//����Ԥ��Ƶ ʹ���ⲿ����Ϊ32.768K��Ҫ��1s�ж���Ԥ��Ƶ������Ϊ32767��ϵͳ���ڴ����ֻ����ϼ�1
	RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������	
}

void clockinit_RTC()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);//�򿪺󱸱�������ʱ�Ӻ͵�Դʱ��
	PWR_BackupAccessCmd(ENABLE); //ʹ��RTC�ͺ�����Ĵ�������
	if(BKP_ReadBackupRegister(BKP_DR1)!=0XA5A5)//��ָ���ĺ󱸼Ĵ����ж�������
	{
		//��һ������  ��ʼ������
		//RTC��ʼ��
		rtc_init();
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_SetCounter(0xd020);//����RTC��������ֵ11:20:50,���㷽��:0x9f92=50+20*60+11*60*60
		RTC_WaitForLastTask();
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
	}
	else
	{	
		RTC_WaitForSynchro();//�ȴ�RTC�Ĵ���ͬ��	
		RTC_WaitForLastTask();//�ȴ�дRTC�Ĵ������		
	  RTC_ITConfig(RTC_IT_SEC,ENABLE);//ʹ��RTC���ж�    
		RTC_WaitForLastTask();//�ȴ�дRTC�Ĵ������	
	}
	RTC_NVIC_Config();
	RCC_ClearFlag();//�����λ��־��		
}

void RTC_IRQHandler()//RTCʵʱʱ��
{
	if(RTC_GetITStatus((RTC_IT_SEC))!=RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);
		timebz=1;	
	}
}



