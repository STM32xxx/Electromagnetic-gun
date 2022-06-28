#include "pcf8591.h"
#include "i2c.h"

void PCF8591_Send(u8 add,u8 reg,u8 data)
{
	I2C_Start();
	I2C_SendByte(add);
	I2C_WaitAck();
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_SendByte(data);
	I2C_WaitAck();
	I2C_Stop();
}

u8 PCF8591_Read(u8 add,u8 reg)
{
	u8 c;
	
	I2C_Start();
	I2C_SendByte(add);
	I2C_WaitAck();
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_Stop();
	
	I2C_Start();
	I2C_SendByte(add+1);
	I2C_WaitAck();
	c = I2C_ReceiveByte();
	I2C_NoAck();
	I2C_Stop();
	return c;
}

//i[0] = PCF8591_Read(PCF8591,_ADC0);delay_us(20);//读去完成一次加适当延时，否则会出错
//PCF8591_Send(PCF8591,_DAC0,200);


