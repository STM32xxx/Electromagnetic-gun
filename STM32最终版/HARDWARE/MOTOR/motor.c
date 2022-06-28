#include "motor.h"

#define KP (1.50f*2.0)

uint16_t Duty_1[] = {1024,1088,1152,1216,1278,1340,1401,1460,1517,1572,
											1625,1676,1724,1770,1812,1852,1888,1920,1950,1975,
											1997,2015,2029,2039,2045,2047,2045,2039,2029,2015,
											1997,1975,1950,1920,1888,1852,1812,1770,1724,1676,
											1625,1572,1517,1460,1401,1340,1278,1216,1152,1088,
											1024, 960, 896, 832,0770, 708,0647, 588,0531,0476,
											0423,0372,0324, 278,0236, 196,0160, 128,  98,0073,
											0051,0033,  19,   9,0003,0001,0003,   9,  19,0033,
											0051,0073,  98, 128,0160, 196,0236, 278,0324,0372,
											0423,0476,0531, 588,0647, 708,0770, 832, 896, 960};

uint16_t Duty_2[] = {1920,1888,1852,1812,1770,1724,1676,
											1625,1572,1517,1460,1401,1340,1278,1216,1152,1088,
											1024, 960, 896, 832,0770, 708,0647, 588,0531,0476,
											0423,0372,0324, 278,0236, 196,0160, 128,  98,0073,
											0051,0033,  19,   9,0003,0001,0003,   9,  19,0033,
											0051,0073,  98, 128,0160, 196,0236, 278,0324,0372,
											0423,0476,0531, 588,0647, 708,0770, 832, 896, 960,
											1024,1088,1152,1216,1278,1340,1401,1460,1517,1572,
											1625,1676,1724,1770,1812,1852,1888,1920,1950,1975,
											1997,2015,2029,2039,2045,2047,2045,2039,2029,2015,
											1997,1975,1950};

uint16_t Duty_3[] = {0160, 128,  98,0073,
											0051,0033,  19,   9,0003,0001,0003,   9,  19,0033,
											0051,0073,  98, 128,0160, 196,0236, 278,0324,0372,
											0423,0476,0531, 588,0647, 708,0770, 832, 896, 960,
											1024,1088,1152,1216,1278,1340,1401,1460,1517,1572,
											1625,1676,1724,1770,1812,1852,1888,1920,1950,1975,
											1997,2015,2029,2039,2045,2047,2045,2039,2029,2015,
											1997,1975,1950,1920,1888,1852,1812,1770,1724,1676,
											1625,1572,1517,1460,1401,1340,1278,1216,1152,1088,
											1024, 960, 896, 832,0770, 708,0647, 588,0531,0476,
											0423,0372,0324, 278,0236, 196,};

uint16_t count_1 = 0,count_2 = 0,count_3 = 0;
uint16_t num_1 = sizeof(Duty_1)/(sizeof(Duty_1[0]));
uint16_t num_2 = sizeof(Duty_2)/(sizeof(Duty_2[0]));
uint16_t num_3 = sizeof(Duty_3)/(sizeof(Duty_3[0]));

static uint16_t CCR1_Val = 1800;
//static uint16_t CCR2_Val = 1800;
//static uint16_t CCR3_Val = 1800;
//static uint16_t PrescalerValue = 0;

/*PWM�������˵����*/
/* - Prescaler = (TIM3CLK / TIM3 counter clock) - 1 ѡ��TIMER������36MHZ
    The TIM3 is running at 36 MHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
                                                  = 36 MHz / 3600 = 10 KHz
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 */
	/*ת������10K*/
void SPWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	GPIO_InitTypeDef         GPIO_InitStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	/* TIM3 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/* GPIOA and GPIOB clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	/* GPIOA Configuration:TIM3 Channel1, 2, 3 and 4 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//TIM1_CH1N
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Compute the prescaler value */
//	PrescalerValue = (uint16_t) (SystemCoreClock / 36000000) - 1; //TIMER2 - 36MHZ
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 7199;
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
//	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;//TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//�򿪻������
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //���Ĺ������øı�CCR2֮��������Ч
	
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); //���Ĺ������øı�CCR2֮��������Ч  
	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); //���Ĺ������øı�CCR2֮��������Ч  
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM1, ENABLE);
	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//TIM1��Ҫ�������,ò�Ƶͼ���ʱ������Ҫ
}

//void TIM1_CC_IRQHandler(void)
//{
//  if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)
//  {
//    TIM1->SR = (uint16_t)~TIM_IT_CC1;
//    TIM1->CCR1 = Duty_1[count_1] * KP;
//    count_1++;
//    if(count_1==num_1)
//    {
//        count_1=0;
//    }
//  }
//	
//	if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
//  {
//    TIM1->SR = (uint16_t)~TIM_IT_CC2;
//    TIM1->CCR2 = Duty_2[count_2] * KP;
//    count_2++;
//    if(count_2==num_2)
//    {
//        count_2=0;
//    }
//  }
//	
//	if (TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
//  {
//    TIM1->SR = (uint16_t)~TIM_IT_CC3;
//    TIM1->CCR3 = Duty_3[count_3] * KP;
//    count_3++;
//    if(count_3==num_3)
//    {
//        count_3=0;
//    }
//  }
//}

//static void MOTOR_GPIO_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

//	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);		  

//	GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4);
//	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
//}

/*
 * ��������TIM3_GPIO_Init
 * ����  ������TIM3�������PWMʱ�õ���I/O
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM3_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM2 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

/*
 * ��������TIM3_Mode_Config
 * ����  ������TIM3�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
static void TIM3_Mode_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* Time base configuration */		 
	/*ֱ�����pwm*/
//  TIM_TimeBaseStructure.TIM_Period = 999;//��ʱ����0��������ֵΪһ��pwm����
//  TIM_TimeBaseStructure.TIM_Prescaler = 71;//��Ƶֵ����ֵ��TIM_Periodһ��Ӱ��pwm���ڳ���
	
	/*���pwm*/
  TIM_TimeBaseStructure.TIM_Period = 9999;//20msһ������
  TIM_TimeBaseStructure.TIM_Prescaler = 143;
	
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM2 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//����ΪPWMģʽ1,��ʱ��ֵС�ڱȽ�ֵ����ߵ�ƽ
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

	TIM_OCInitStructure.TIM_Pulse = 0;//��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);//ʹ��ͨ��1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_Pulse = 0;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��3
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_Pulse = 0;   
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_Pulse = 0;	  //
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��4
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��2	
}

void TIM3_PWM_Init(void)
{
//	MOTOR_GPIO_Config();
	TIM3_GPIO_Init();
	TIM3_Mode_Init();	
}

/****************************************
	���������ʼ��(�������)
****************************************/
void TIM4_Encoder_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_ICInitTypeDef TIM_ICInitStructure;
  
/* Encoder unit connected to TIM3, 4X mode */    
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* TIM3 clock source enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  /* Enable GPIOA, clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
  GPIO_StructInit(&GPIO_InitStructure);
  /* Configure PA.06,07 as encoder input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Enable the TIM3 Update Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Timer configuration in Encoder mode */
  TIM_DeInit(TIM4);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // TIM4ʱ��Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  //�趨��ʱ����װֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
  //TIM_ICPolarity_Rising�����ز���
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6; //���˲���
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
 // Clear all pending interrupts
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);	 //���TIM4�ĸ��±�־λ
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	  //����TIM4�ĸ��±�־λ
  
  TIM_Cmd(TIM4, ENABLE);  	   //ʹ�ܶ�ʱ��3
}

void TIM4_IRQHandler(void)//��ʱ��4�жϺ���
{ 		    		  			    
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	    
}


