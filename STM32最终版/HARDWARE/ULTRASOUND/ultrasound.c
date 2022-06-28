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
	����ģʽ��ʼ��(���������������)
*/
void HC_SR04_Init(void)
{
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//PB0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//��ʼ����ʱ��3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 						//�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; 						//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 			//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM3���벶�����
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; 			//CC1S=03 	ѡ������� IC3ӳ�䵽TI1��
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//���������Ƶ,����Ƶ 
 	TIM_ICInitStructure.TIM_ICFilter = 0x00;							//���������˲��� ���˲�
  TIM_ICInit(TIM1, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; 			//CC1S=03 	ѡ������� IC3ӳ�䵽TI1��
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//���������Ƶ,����Ƶ 
 	TIM_ICInitStructure.TIM_ICFilter = 0x00;							//���������˲��� ���˲�
  TIM_ICInit(TIM1, &TIM_ICInitStructure);
	

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn|TIM1_UP_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 	
	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC4|TIM_IT_CC1,ENABLE);//��������ж� ,����CC3IE�����ж�	
 	TIM_Cmd(TIM1,ENABLE ); 	//ʹ�ܶ�ʱ��3
}

/*
	����һ�γ�������࣬����ȡ�ϴβ������
*/
int Read_Distane_1(void)
{
	int dis;
	
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
	delay_us(20);  
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
	if(TIM1CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
	{
		dis = TIM1CH1_CAPTURE_STA&0X3F;
		dis *= 65536;					 //���ʱ���ܺ�
		dis = TIM1CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
		dis = dis*170/10000;
		TIM1CH1_CAPTURE_STA=0;				//������һ�β���
	}
	return dis;
}

int Read_Distane_4(void)
{
	int dis;
	
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
	delay_us(20);  
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	if(TIM1CH4_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
	{
		dis = TIM1CH4_CAPTURE_STA&0X3F;
		dis *= 65536;					 //���ʱ���ܺ�
		dis = TIM1CH4_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
		dis = dis*170/10000;
		TIM1CH4_CAPTURE_STA=0;				//������һ�β���
	}
	return dis;
}

/*
	�����жϴ�����
*/
void TIM1_CC_IRQHandler(void)
{
	if((TIM1CH1_CAPTURE_STA&0X80)==0)//��δ���һ�����岶��	
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
		if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)//ͨ��4�����¼�
		{	
			if(TIM1CH1_CAPTURE_STA&0X40){	  			
				TIM1CH1_CAPTURE_STA|=0X80;		//����Ѿ������½���
				TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); //����Ϊ�����ز���
			}
			else{
				TIM1CH1_CAPTURE_STA=0;			
				TIM1CH1_CAPTURE_VAL=0;
				TIM_SetCounter(TIM1,0);
				TIM1CH1_CAPTURE_STA|=0X40;													//����Ѿ�����������
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);	// ����Ϊ�½��ز���
			}		    
		}			     	    					   
  }
	
	if((TIM1CH4_CAPTURE_STA&0X80)==0)//��δ���һ�����岶��	
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
		if (TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)//ͨ��4�����¼�
		{	
			if(TIM1CH4_CAPTURE_STA&0X40){	  			
				TIM1CH4_CAPTURE_STA|=0X80;		//����Ѿ������½���
				TIM1CH4_CAPTURE_VAL=TIM_GetCapture4(TIM1);
				TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Rising); //����Ϊ�����ز���
			}
			else{
				TIM1CH4_CAPTURE_STA=0;			
				TIM1CH4_CAPTURE_VAL=0;
				TIM_SetCounter(TIM1,0);
				TIM1CH4_CAPTURE_STA|=0X40;													//����Ѿ�����������
				TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Falling);	// ����Ϊ�½��ز���
			}		    
		}			     	    					   
  }
	
  TIM_ClearITPendingBit(TIM1, TIM_IT_CC4|TIM_IT_CC1|TIM_IT_Update); 
}




