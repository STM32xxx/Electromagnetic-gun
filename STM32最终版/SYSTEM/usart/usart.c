#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "sys.h"

#define PRINTF USART1    //改变此处串口号可以改变printf函数绑定的串口

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE
{
	int handle;
};

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x)
{
	x = x;
}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{
	while((PRINTF->SR&0X40)==0);//循环发送,直到发送完毕   
	PRINTF->DR = (u8)ch;
	return ch;
}
#endif 

/********************************************************************************
                                  串口1
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
void Uart1SendByte(char byte)   //串口发送一个字节
{
   USART_SendData(USART1, byte);        //通过库函数  发送数据
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
                                  串口3
********************************************************************************/
void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	//定义串口初始化结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	/* config USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* USART3 GPIO config */
	/* Configure USART3 Tx (PB.10) as alternate function push-pull 推拉输出模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART3 Rx (PB.11) as input floating 浮点输入模式*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//禁用RTSCTS硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//使能发送接收
	USART_Init(USART3, &USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//使能接收中断
	USART_Cmd(USART3, ENABLE);
}
void Uart3SendByte(char byte)   //串口发送一个字节
{
  USART_SendData(USART3, byte);        //通过库函数  发送数据
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
                                  串口2
********************************************************************************/
void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;	 //声明一个结构体变量，用来初始化GPIO
	USART_InitTypeDef USART_InitStructure;	 //串口结构体定义
	NVIC_InitTypeDef NVIC_InitStructure;
	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TX-485	//串口输出PA2
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;		  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);				 	/* 初始化串口输入IO */   

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//RX-485	   //串口输入PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	    //模拟输入
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);/* 初始化GPIO */
	
	/* Enable the USART2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);    

	USART_InitStructure.USART_BaudRate = bound;			   //波特率设置为9600	//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	   //数据长8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		   //无效验
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //失能硬件流
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		 //开启发送和接受模式
	
	USART_Init(USART2, &USART_InitStructure);	   /* 初始化USART2 */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);  //使能或者失能指定的USART中断 接收中断
//	USART_ClearFlag(USART2,USART_FLAG_TC);	 //清除USARTx的待处理标志位
	USART_Cmd(USART2,ENABLE);
}

void Uart2SendByte(char byte)   //串口发送一个字节
{
  USART_SendData(USART2, byte);        //通过库函数  发送数据
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
		HmiRxBuff[Usart2Index] = USART_ReceiveData(USART2);//读取接收到的数据	
    if(HmiRxBuff[0]!=0x23){Usart2Index=0;return;}
		else if(HmiRxBuff[Usart2Index++]==0x2A)
		{
			Usart2Index = 0;
			HMI_USART_Receive = 1;
		}
	}
}


