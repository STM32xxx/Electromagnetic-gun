#ifndef __HMC5883L_H_
#define __HMC5883L_H_

#include "sys.h"

//#define IIC_SCL    PBout(10) //SCL
//#define IIC_SDA    PBout(11) //SDA	 
//#define READ_SDA   PBin(11)  // ‰»ÎSDA
#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  // ‰»ÎSDA

#define WRITE_ADDRESS  0x3c
#define READ_ADDRESS 0x3d
#define CONFIGA 0x00
#define CONFIGB 0x01
#define MODE 0x02
#define DATAX_M 0x03
#define DATAX_L 0x04
#define DATAY_M 0x05
#define DATAY_L 0x06
#define DATAZ_M 0x07
#define DATAZ_L 0x08
#define STATUS 0x09

void hmc_write_reg(u8 reg,u8 data);
u8 hmc_read_reg(u8 reg);
void hmc_read_XYZ(short int *data);
void HMC5883L_Init(void);

#endif



