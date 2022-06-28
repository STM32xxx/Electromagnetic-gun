#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm32f10x.h"

//stm32f103c8t6有64kflash(MDK检测到是128???)
//地址范围0x08000000-0x0800ffff(或0x08000000-0x0801ffff)
//要按页划分，1k字节分页,32的flash是nand flash???
//使用flash最后一页模拟eeprom
//存储的数据要4字节对齐???
//0x0800ffff-1024+1=0x0800fc00

#define FLASH_ADR 0x0800fc00//定义主存储器的保存数据的起始地址,

void FlashDataSave(void);


#endif



