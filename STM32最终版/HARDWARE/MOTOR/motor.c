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

/*PWM输出配置说明，*/
/* - Prescaler = (TIM3CLK / TIM3 counter clock) - 1 选择TIMER工作在36MHZ
    The TIM3 is running at 36 MHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
                                                  = 36 MHz / 3600 = 10 KHz
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 */
	/*转换周期10K*/
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
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;//打开互补输出
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); //这句的功能是让改变CCR2之后马上有效
	
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); //这句的功能是让改变CCR2之后马上有效  
	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); //这句的功能是让改变CCR2之后马上有效  
	
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
	TIM_CtrlPWMOutputs(TIM1, ENABLE);//TIM1需要加上这句,貌似低级定时器不需要
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
 * 函数名：TIM3_GPIO_Init
 * 描述  ：配置TIM3复用输出PWM时用到的I/O
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

/*
 * 函数名：TIM3_Mode_Config
 * 描述  ：配置TIM3输出的PWM信号的模式，如周期、极性、占空比
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void TIM3_Mode_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

  /* Time base configuration */		 
	/*直流电机pwm*/
//  TIM_TimeBaseStructure.TIM_Period = 999;//定时器从0计数到该值为一个pwm周期
//  TIM_TimeBaseStructure.TIM_Prescaler = 71;//分频值，该值和TIM_Period一块影响pwm周期长度
	
	/*舵机pwm*/
  TIM_TimeBaseStructure.TIM_Period = 9999;//20ms一个周期
  TIM_TimeBaseStructure.TIM_Prescaler = 143;
	
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM2 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//配置为PWM模式1,定时器值小于比较值输出高电平
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//当定时器计数值小于CCR1_Val时为高电平

	TIM_OCInitStructure.TIM_Pulse = 0;//设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);//使能通道1
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_Pulse = 0;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //使能通道3
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_Pulse = 0;   
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_OCInitStructure.TIM_Pulse = 0;	  //
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	  //使能通道4
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM3重载寄存器ARR

  TIM_Cmd(TIM3, ENABLE);                   //使能定时器2	
}

void TIM3_PWM_Init(void)
{
//	MOTOR_GPIO_Config();
	TIM3_GPIO_Init();
	TIM3_Mode_Init();	
}

/****************************************
	正交解码初始化(电机测速)
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
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // TIM4时钟预分频值
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  //设定计时器重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
  //TIM_ICPolarity_Rising上升沿捕获
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 6; //无滤波器
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
 // Clear all pending interrupts
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);	 //清除TIM4的更新标志位
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	  //开启TIM4的更新标志位
  
  TIM_Cmd(TIM4, ENABLE);  	   //使能定时器3
}

void TIM4_IRQHandler(void)//定时器4中断函数
{ 		    		  			    
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	    
}


