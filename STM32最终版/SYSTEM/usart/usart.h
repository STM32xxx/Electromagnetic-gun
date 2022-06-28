#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

#define USART1_REC_LEN 30
#define USART2_REC_LEN 30
#define USART3_REC_LEN 30

void USART2_Init(u32 bound);
void Uart2SendByte(char byte);
void Uart2SendBuff(char *buf,unsigned short len);
void Uart2SendStr(char *str);

void USART1_Init(u32 bound);
void Uart1SendByte(char byte);
void Uart1SendBuff(char *buf,unsigned short len);
void Uart1SendStr(char *str);

void USART3_Init(u32 bound);
void Uart3SendByte(char byte);
void Uart3SendBuff(char *buf,u16 len);
void Uart3SendStr(char *str);

/*串口3的接收数据相关变量*/
extern unsigned char USART3_RX_BUF[USART3_REC_LEN];
extern short USART3_RX_STA;
extern unsigned char USART3_Receive;

/*串口2的接收数据相关变量*/
extern unsigned char HMI_USART_Receive;
extern unsigned char HmiRxBuff[USART2_REC_LEN];

#endif


