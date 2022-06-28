#include "ultrasound.h"
#include "delay.h"

static unsigned int TIM1CH1_CAPTURE_STA;
static unsigned int TIM1CH1_CAPTURE_VAL;
static unsigned int TIM1CH4_CAPTURE_STA;
static unsigned int TIM1CH4_CAPTURE_VAL;

void SR04_Trig_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4);
}

/*
	捕获模式初始化(超声波测距脉宽捕获)
*/
void HC_SR04_Init(void)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//使能TIM3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOB时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//PB0 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//初始化定时器3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 						//设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; 						//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 			//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM3输入捕获参数
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; 			//CC1S=03 	选择输入端 IC3映射到TI1上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//配置输入分频,不分频 
 	TIM_ICInitStructure.TIM_ICFilter = 0x00;							//配置输入滤波器 不滤波
  TIM_ICInit(TIM1, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 			//CC1S=03 	选择输入端 IC3映射到TI1上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//配置输入分频,不分频 
 	TIM_ICInitStructure.TIM_ICFilter = 0x00;							//配置输入滤波器 不滤波
  TIM_ICInit(TIM1, &TIM_ICInitStructure);
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn|TIM1_UP_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 	
	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC4|TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC3IE捕获中断	
 	TIM_Cmd(TIM1,ENABLE ); 	//使能定时器3
}

/*
	触发一次超声波测距，并读取上次测量结果
*/
int Read_Distane_1(void)
{
	int dis;
	
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	delay_us(20);  
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	if(TIM1CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
	{
		dis = TIM1CH1_CAPTURE_STA&0X3F;
		dis *= 65536;					 //溢出时间总和
		dis = TIM1CH1_CAPTURE_VAL;		//得到总的高电平时间
		dis = dis*170/10000;
		TIM1CH1_CAPTURE_STA=0;				//开启下一次捕获
	}
	return dis;
}

int Read_Distane_4(void)
{
	int dis;
	
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	delay_us(20);  
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	if(TIM1CH4_CAPTURE_STA&0X80)//成功捕获到了一次高电平
	{
		dis = TIM1CH4_CAPTURE_STA&0X3F;
		dis *= 65536;					 //溢出时间总和
		dis = TIM1CH4_CAPTURE_VAL;		//得到总的高电平时间
		dis = dis*170/10000;
		TIM1CH4_CAPTURE_STA=0;				//开启下一次捕获
	}
	return dis;
}

/*
	捕获中断处理函数
*/
void TIM1_CC_IRQHandler(void)
{
	if((TIM1CH1_CAPTURE_STA&0X80)==0)//还未完成一次脉冲捕获	
	{
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
		{
			if(TIM1CH1_CAPTURE_STA&0X40)
			{
				if((TIM1CH1_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM1CH1_CAPTURE_STA|=0X80;
					TIM1CH1_CAPTURE_VAL=0XFFFF;
				}
				else TIM1CH1_CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)//通道4捕获到事件
		{	
			if(TIM1CH1_CAPTURE_STA&0X40){	  			
				TIM1CH1_CAPTURE_STA|=0X80;		//标记已经捕获到下降沿
				TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); //设置为上升沿捕获
			}
			else{
				TIM1CH1_CAPTURE_STA=0;			
				TIM1CH1_CAPTURE_VAL=0;
				TIM_SetCounter(TIM1,0);
				TIM1CH1_CAPTURE_STA|=0X40;													//标记已经捕获到上升沿
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);	// 设置为下降沿捕获
			}		    
		}			     	    					   
  }
	
	if((TIM1CH4_CAPTURE_STA&0X80)==0)//还未完成一次脉冲捕获	
	{
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
		{
			if(TIM1CH4_CAPTURE_STA&0X40)
			{
				if((TIM1CH4_CAPTURE_STA&0X3F)==0X3F)
				{
					TIM1CH4_CAPTURE_STA|=0X80;
					TIM1CH4_CAPTURE_VAL=0XFFFF;
				}
				else TIM1CH4_CAPTURE_STA++;
			}	 
		}
		if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)//通道4捕获到事件
		{	
			if(TIM1CH4_CAPTURE_STA&0X40){	  			
				TIM1CH4_CAPTURE_STA|=0X80;		//标记已经捕获到下降沿
				TIM1CH4_CAPTURE_VAL=TIM_GetCapture4(TIM1);
				TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Rising); //设置为上升沿捕获
			}
			else{
				TIM1CH4_CAPTURE_STA=0;			
				TIM1CH4_CAPTURE_VAL=0;
				TIM_SetCounter(TIM1,0);
				TIM1CH4_CAPTURE_STA|=0X40;													//标记已经捕获到上升沿
				TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Falling);	// 设置为下降沿捕获
			}		    
		}			     	    					   
  }
	
  TIM_ClearITPendingBit(TIM1, TIM_IT_CC4|TIM_IT_CC1|TIM_IT_Update); 
}




