#include "hmc5883l.h"
#include "delay.h"
#include "i2c.h"

void hmc_write_reg(u8 reg,u8 data)
{
	I2C_Start();
	I2C_SendByte(WRITE_ADDRESS);
	I2C_WaitAck();
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_SendByte(data);
	I2C_WaitAck();
	I2C_Stop();
	//delay_ms(5);
}

u8 hmc_read_reg(u8 reg)
{
	u8 data;
	I2C_Start();
	I2C_SendByte(WRITE_ADDRESS);
	I2C_WaitAck();
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_Stop();
	I2C_Start();
	I2C_SendByte(READ_ADDRESS);
	I2C_WaitAck();
	data=I2C_ReceiveByte();
	I2C_NoAck();
	I2C_Stop();
	return data;
}

void hmc_read_XYZ(short int *data)
{
	u16 temp;
	temp=hmc_read_reg(DATAX_M);
	*data++=(temp<<8)+hmc_read_reg(DATAX_L);
	temp=hmc_read_reg(DATAY_M);
	*data++=(temp<<8)+hmc_read_reg(DATAY_L);
	temp=hmc_read_reg(DATAZ_M);
	*data++=(temp<<8)+hmc_read_reg(DATAZ_L);
}

void HMC5883L_Init(void)
{
	hmc_write_reg(CONFIGA,0x14);
	hmc_write_reg(CONFIGB,0x80);
	hmc_write_reg(MODE,0x00);
}

//		hmc_read_XYZ(data);
//		angle=atan2((double)data[2],(double)data[0])*(180 / 3.14159265)+180;
//		printf("%.3f\n",angle);
//		delay_ms(200);

