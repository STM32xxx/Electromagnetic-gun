#include "i2c.h"

void I2C_delay(void)
{
	volatile int i = 7;
	while (i)
			i--;
}

bool I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if (!SDA_read)
		return false;
	SDA_L;
	I2C_delay();
	if (SDA_read)
		return false;
	SDA_L;
	I2C_delay();
	return true;
}

void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

void I2C_Ack(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

void I2C_NoAck(void)
{
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

bool I2C_WaitAck(void)
{
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
		SCL_L;
		return false;
	}
	SCL_L;
	return true;
}

void I2C_SendByte(uint8_t byte)
{
	uint8_t i = 8;
	while (i--)
	{
		SCL_L;
		I2C_delay();
		if(byte & 0x80) SDA_H;
		else SDA_L;
		byte <<= 1;
		I2C_delay();
		SCL_H;
		I2C_delay();
	}
	SCL_L;
}

uint8_t I2C_ReceiveByte(void)
{
	uint8_t i = 8;
	uint8_t byte = 0;

	SDA_H;
	while (i--)
	{
		byte <<= 1;
		SCL_L;
		I2C_delay();
		SCL_H;
		I2C_delay();
		if (SDA_read) byte |= 0x01;
	}
	SCL_L;
	return byte;
}

void i2cInit(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}






