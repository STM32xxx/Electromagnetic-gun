#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "sys.h"

#define PRINTF USART1    //�ı�˴����ںſ��Ըı�printf�����󶨵Ĵ���

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE
{
	int handle;
};

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x)
{
	x = x;
}
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{
	while((PRINTF->SR&0X40)==0);//ѭ������,ֱ���������   
	PRINTF->DR = (u8)ch;
	return ch;
}
#endif 

/********************************************************************************
                                  ����1
********************************************************************************/
void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);
}
void Uart1SendByte(char byte)   //���ڷ���һ���ֽ�
{
   USART_SendData(USART1, byte);        //ͨ���⺯��  ��������
   while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
}
void Uart1SendBuff(char *buf, unsigned short len)
{
	unsigned short i;
	for(i=0; i<len; i++)Uart1SendByte(*buf++);
}
void Uart1SendStr(char *str)
{
	unsigned short i,len;
	len = strlen(str);
	for(i=0; i<len; i++)Uart1SendByte(*str++);
}

unsigned char USART1_RX_BUF[USART1_REC_LEN];
int USART1_RX_STA=0;
unsigned char USART1_Receive = 0;

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE ) != RESET )
	{
		USART1_RX_BUF[USART1_RX_STA] = USART_ReceiveData(USART1);
		if(USART1_RX_BUF[0] != '#') {USART1_RX_STA=0;return;}
		if(USART1_RX_BUF[USART1_RX_STA++] == '*') 
		{
			USART1_RX_STA = 0;
			USART1_Receive = 1;
		}
	}
}

/********************************************************************************
                                  ����3
********************************************************************************/
void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	//���崮�ڳ�ʼ���ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	/* config USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB.10) as alternate function push-pull �������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART3 Rx (PB.11) as input floating ��������ģʽ*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����RTSCTSӲ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ʹ�ܷ��ͽ���
	USART_Init(USART3, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
	USART_Cmd(USART3, ENABLE);
}
void Uart3SendByte(char byte)   //���ڷ���һ���ֽ�
{
  USART_SendData(USART3, byte);        //ͨ���⺯��  ��������
  while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);  
}
void Uart3SendBuff(char *buf,u16 len)
{
	unsigned short i;
	for(i=0; i<len; i++)Uart3SendByte(*buf++);
}
void Uart3SendStr(char *str)
{
	unsigned short i,len;
	len = strlen(str);
	for(i=0; i<len; i++)Uart3SendByte(*str++);
}

unsigned char USART3_RX_BUF[USART3_REC_LEN] = "\0";
short USART3_RX_STA=0;
unsigned char USART3_Receive = 0;

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)
	{
		USART3_RX_BUF[USART3_RX_STA]=USART_ReceiveData(USART3);
		if(USART3_RX_BUF[0]!='#'){USART3_RX_STA=0;return;}
		if(USART3_RX_BUF[USART3_RX_STA++]=='*')
		{
			USART3_RX_STA = 0;
			USART3_Receive = 1;
		}
	}
}

/********************************************************************************
                                  ����2
********************************************************************************/
void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;	 //����һ���ṹ�������������ʼ��GPIO
	USART_InitTypeDef USART_InitStructure;	 //���ڽṹ�嶨��
	NVIC_InitTypeDef NVIC_InitStructure;
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TX-485	//�������PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);				 	/* ��ʼ����������IO */   

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//RX-485	   //��������PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //ģ������
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);/* ��ʼ��GPIO */
	
	/* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);    

	USART_InitStructure.USART_BaudRate = bound;			   //����������Ϊ9600	//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	   //���ݳ�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;		   //��Ч��
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //ʧ��Ӳ����
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		 //�������ͺͽ���ģʽ
	
	USART_Init(USART2, &USART_InitStructure);	   /* ��ʼ��USART2 */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  //ʹ�ܻ���ʧ��ָ����USART�ж� �����ж�
//	USART_ClearFlag(USART2,USART_FLAG_TC);	 //���USARTx�Ĵ������־λ
	USART_Cmd(USART2,ENABLE);
}

void Uart2SendByte(char byte)   //���ڷ���һ���ֽ�
{
  USART_SendData(USART2, byte);        //ͨ���⺯��  ��������
  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
}
void Uart2SendBuff(char *buf,u16 len)
{
	unsigned short i;
	for(i=0; i<len; i++)Uart2SendByte(*buf++);
}
void Uart2SendStr(char *str)
{
	unsigned short i,len;
	len = strlen(str);
	for(i=0; i<len; i++)Uart2SendByte(*str++);
}

unsigned char HmiRxBuff[USART2_REC_LEN] = "\0";
short Usart2Index = 0;
unsigned char HMI_USART_Receive = 0;

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		HmiRxBuff[Usart2Index] = USART_ReceiveData(USART2);//��ȡ���յ�������	
    if(HmiRxBuff[0]!=0x23){Usart2Index=0;return;}
		else if(HmiRxBuff[Usart2Index++]==0x2A)
		{
			Usart2Index = 0;
			HMI_USART_Receive = 1;
		}
	}
}


