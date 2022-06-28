#include "rtc.h"

u8 timebz;

void RTC_NVIC_Config()	  //为了使后备电池在断电的时候仍然走时，将中断配置放在外面
{
	NVIC_InitTypeDef NVIC_InitStructure; 	//中断结构体定义

	/* 设置NVIC参数 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn; 	   //打开RTC的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 	 //响应优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	 //使能
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函 数 名         : rtc_init
* 函数功能		   : RTC时钟、中断初始化	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void rtc_init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//打开电源时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);//打开存储器时钟

	PWR_BackupAccessCmd(ENABLE);//使能或者失能RTC和后备寄存器访问
	BKP_DeInit();//将外设BKP的全部寄存器重设为缺省值
	RCC_LSEConfig(RCC_LSE_ON);//设置外部低速晶振（LSE）
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//检查指定的RCC标志位设置与否
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//设置RTC时钟（RTCCLK）
	RCC_RTCCLKCmd(ENABLE);//使能或者失能RTC时钟
	RTC_WaitForSynchro(); //等待RTC寄存器同步
	RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成
	RTC_ITConfig(RTC_IT_SEC,ENABLE);//使能或者失能指定的RTC中断
	RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成
	RTC_SetPrescaler(32767);//设置预分频 使用外部晶振为32.768K，要想1s中断则预分频数设置为32767，系统会在此数字基础上加1
	RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成	
}

void clockinit_RTC()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);//打开后备备份区域时钟和电源时钟
	PWR_BackupAccessCmd(ENABLE); //使能RTC和后备区域寄存器访问
	if(BKP_ReadBackupRegister(BKP_DR1)!=0XA5A5)//从指定的后备寄存器中读出数据
	{
		//第一次运行  初始化设置
		//RTC初始化
		rtc_init();
		RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成
		RTC_SetCounter(0xd020);//设置RTC计数器的值11:20:50,计算方法:0x9f92=50+20*60+11*60*60
		RTC_WaitForLastTask();
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
	}
	else
	{	
		RTC_WaitForSynchro();//等待RTC寄存器同步	
		RTC_WaitForLastTask();//等待写RTC寄存器完成		
	  RTC_ITConfig(RTC_IT_SEC,ENABLE);//使能RTC秒中断    
		RTC_WaitForLastTask();//等待写RTC寄存器完成	
	}
	RTC_NVIC_Config();
	RCC_ClearFlag();//清除复位标志；		
}

void RTC_IRQHandler()//RTC实时时钟
{
	if(RTC_GetITStatus((RTC_IT_SEC))!=RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);
		timebz=1;	
	}
}



