#include "stepping.h"

//unsigned int pulse_0,pulse_1,pulse_2;
//unsigned int target_0,target_1,target_2;
//char complete_0,complete_1,complete_2;

//void stepping_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB,ENABLE);		//ʹ��GPIOBʱ��

//	//��ʼ��������������������
//	GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//PB15����ʹ�ܣ�PB14,PB13���Ʒ���
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14);//������ߵ�ƽ���趨һ���������
//	GPIO_ResetBits(GPIOB,GPIO_Pin_15);//���ʹ��

////	TIM2_Init(1500-1, 72-1);//��ʼ����ʱ��2
//	TIM3_Init(1500-1, 72-1);//��ʼ����ʱ��3
//	TIM4_Init(1500-1, 72-1);//��ʼ����ʱ��4

//	pulse_0 = 0;target_0 = 0;complete_0 = 0;
//	pulse_1 = 0;target_1 = 0;complete_1 = 0;
//	pulse_2 = 0;target_2 = 0;complete_2 = 0;
//}

///*��ʱ��3��ʼ��Ϊ������������������������������ʹ��ͨ��3��PB0�������*/
//void TIM3_Init(u16 arr, u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//ʹ��TIM3ʱ��	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//ʹ��GPIOBʱ�Ӻ͸���ʱ��

//	GPIO_StructInit(&GPIO_InitStructure);				//��GPIO_InitStruct�еĲ�����ȱʡֵ����
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//PB0
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//                      
//	TIM_DeInit(TIM3);	//�ѼĴ���ֵ����Ϊȱʡֵ
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//	TIM_TimeBaseStructure.TIM_Period = arr; 					//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler = psc; 					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ�� PWM ������˿�
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //������Ը�
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
//	TIM_OCInitStructure.TIM_Pulse = arr/2; //CH3 PWM ��ֵ
//	TIM_OC3Init(TIM3,&TIM_OCInitStructure);//��ʼ�� TIM3 OC3
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); //ʹ��Ԥװ�ؼĴ���

//	//��ʱ��3�ж�����
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );

//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	TIM_ARRPreloadConfig(TIM3,ENABLE);//ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ
//	TIM_Cmd(TIM3,DISABLE);//ʧ�ܶ�ʱ��
//}

///*��ʱ��4��ʼ��Ϊ������������������������������ʹ��ͨ��3��PB8�������*/
//void TIM4_Init(u16 arr, u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ��TIM4ʱ��	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ�Ӻ�  X����ʱ�� | RCC_APB2Periph_AFIO

//	GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	TIM_DeInit(TIM4);											//�ѼĴ���ֵ����Ϊȱʡֵ
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//	TIM_TimeBaseStructure.TIM_Period = arr; 					//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler = psc;	 				//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 				//����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
//	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;  			//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure);

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set; 
//	TIM_OCInitStructure.TIM_Pulse = arr/2;						//CCR3 ��ֵ
//	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);			//ʹ��Ԥװ�ؼĴ���

//	//��ʱ��2�ж�����
//	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );

//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	TIM_ARRPreloadConfig(TIM4,ENABLE);//ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ
//	TIM_Cmd(TIM4,DISABLE);//ʧ�ܶ�ʱ��
//}

//void TIM3_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//����жϱ�־λ
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




