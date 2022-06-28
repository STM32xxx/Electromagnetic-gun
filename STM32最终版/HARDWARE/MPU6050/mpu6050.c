#include "mpu6050.h"
#include "i2c.h"

//static bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
//{
//	if (!I2C_Start())
//		return false;
//	I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
//	if (!I2C_WaitAck())
//	{
//		I2C_Stop();
//		return false;
//	}
//	I2C_SendByte(reg);
//	I2C_WaitAck();
//	I2C_SendByte(data);
//	I2C_WaitAck();
//	I2C_Stop();
//	return true;
//}

static bool i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	int i;
	if (!I2C_Start())
	return false;
	I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
	if (!I2C_WaitAck())
	{
		I2C_Stop();
		return false;
	}
	I2C_SendByte(reg);
	I2C_WaitAck();
	for (i = 0; i < len; i++)
	{
		I2C_SendByte(data[i]);
		if (!I2C_WaitAck())
		{
			I2C_Stop();
			return false;
		}
	}
	I2C_Stop();
	return true;
}


static bool i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	if (!I2C_Start())
		return false;
	I2C_SendByte(addr << 1 | I2C_Direction_Transmitter);
	if (!I2C_WaitAck())
	{
		I2C_Stop();
		return false;
	}
	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(addr << 1 | I2C_Direction_Receiver);
	I2C_WaitAck();
	while(len)
	{
		*buf = I2C_ReceiveByte();
		if (len == 1)
		I2C_NoAck();
		else
		I2C_Ack();
		buf++;
		len--;
	}
	I2C_Stop();
	return true;
}

static int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
	if(i2cWriteBuffer(addr,reg,len,data))
		return 0;
	else
		return -1;
}

static int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	if(i2cRead(addr,reg,len,buf))
		return 0;
	else
		return -1;
}

void MPU6050_Init(void)
{
	unsigned char buff[5] = {0x00,0x07,0x04,0x18,0x01};
	i2cwrite(MPU6050_ADDR,PWR_MGMT_1, 1,&buff[0]);   //½â³ıĞİÃß×´Ì¬
	i2cwrite(MPU6050_ADDR,SMPLRT_DIV, 1,&buff[1]); //ÍÓÂİÒÇ125hz
	i2cwrite(MPU6050_ADDR,CONFIG, 1,&buff[2]); //Accelerometer:21hz 8.5ms ; Gyroscope:20hz 8.3ms
	i2cwrite(MPU6050_ADDR,GYRO_CONFIG, 1,&buff[3]); //¡À2000¡ã/s  
	i2cwrite(MPU6050_ADDR,ACCEL_CONFIG, 1,&buff[4]); //¡À2g
}

short DataSynthesis(unsigned char REG_Address)
{
	unsigned char uByte[2]; 

	i2cread(MPU6050_ADDR,REG_Address,2,uByte)  ;

	return ((uByte[0] << 8) | uByte[1]);   /*·µ»ØºÏ³ÉÊı¾İ*/
}




