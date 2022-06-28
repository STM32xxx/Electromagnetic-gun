#ifndef _I2C_H_
#define _I2C_H_

#include "stm32f10x.h"

#define SCL_H    GPIOB->BSRR = GPIO_Pin_10 /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCL_L    GPIOB->BRR  = GPIO_Pin_10 /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */

#define SDA_H    GPIOB->BSRR = GPIO_Pin_11 /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SDA_L    GPIOB->BRR  = GPIO_Pin_11 /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCL_read GPIOB->IDR  & GPIO_Pin_10 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA_read GPIOB->IDR  & GPIO_Pin_11 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */

#define true 1
#define false 0 
#define bool uint8_t

void I2C_delay(void);
bool I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NoAck(void);
bool I2C_WaitAck(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_ReceiveByte(void);

void i2cInit(void);

#endif




