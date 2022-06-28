#include "stepping.h"

//unsigned int pulse_0,pulse_1,pulse_2;
//unsigned int target_0,target_1,target_2;
//char complete_0,complete_1,complete_2;

//void stepping_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB,ENABLE);		//使能GPIOB时钟

//	//初始化步进电机方向控制引脚
//	GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PB15控制使能，PB14,PB13控制方向
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14);//先输出高电平，设定一个电机方向
//	GPIO_ResetBits(GPIOB,GPIO_Pin_15);//电机使能

////	TIM2_Init(1500-1, 72-1);//初始化定时器2
//	TIM3_Init(1500-1, 72-1);//初始化定时器3
//	TIM4_Init(1500-1, 72-1);//初始化定时器4

//	pulse_0 = 0;target_0 = 0;complete_0 = 0;
//	pulse_1 = 0;target_1 = 0;complete_1 = 0;
//	pulse_2 = 0;target_2 = 0;complete_2 = 0;
//}

///*定时器3初始化为高速脉冲输出，用于驱动步进电机，使用通道3，PB0输出脉冲*/
//void TIM3_Init(u16 arr, u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能TIM3时钟	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//使能GPIOB时钟和复用时钟

//	GPIO_StructInit(&GPIO_InitStructure);				//将GPIO_InitStruct中的参数按缺省值输入
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//PB0
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//                      
//	TIM_DeInit(TIM3);	//把寄存器值设置为缺省值
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//	TIM_TimeBaseStructure.TIM_Period = arr; 					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler = psc; 					//设置用来作为TIMx时钟频率除数的预分频值 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
//	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能 PWM 输出到端口
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
//	TIM_OCInitStructure.TIM_Pulse = arr/2; //CH3 PWM 的值
//	TIM_OC3Init(TIM3,&TIM_OCInitStructure);//初始化 TIM3 OC3
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能预装载寄存器

//	//定时器3中断配置
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );

//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	TIM_ARRPreloadConfig(TIM3,ENABLE);//使能自动重装载的预装载寄存器允许位
//	TIM_Cmd(TIM3,DISABLE);//失能定时器
//}

///*定时器4初始化为高速脉冲输出，用于驱动步进电机，使用通道3，PB8输出脉冲*/
//void TIM4_Init(u16 arr, u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能TIM4时钟	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟和  X复用时钟 | RCC_APB2Periph_AFIO

//	GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	TIM_DeInit(TIM4);											//把寄存器值设置为缺省值
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//	TIM_TimeBaseStructure.TIM_Period = arr; 					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler = psc;	 				//设置用来作为TIMx时钟频率除数的预分频值 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
//	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;  			//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//	TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure);

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
//	TIM_OCInitStructure.TIM_Pulse = arr/2;						//CCR3 的值
//	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);			//使能预装载寄存器

//	//定时器2中断配置
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );

//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	TIM_ARRPreloadConfig(TIM4,ENABLE);//使能自动重装载的预装载寄存器允许位
//	TIM_Cmd(TIM4,DISABLE);//失能定时器
//}

//void TIM3_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除中断标志位
//		
//		pulse_0++;
//		if(pulse_0>=target_0)
//		{
//			TIM_Cmd(TIM3,DISABLE);
//			complete_0=1;
//		}
//	}
//}

//void TIM4_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

//		pulse_1++;
//		if(pulse_1>=target_1)
//		{
//			TIM_Cmd(TIM4,DISABLE);
//			complete_1=1;
//		}
//	}
//}




