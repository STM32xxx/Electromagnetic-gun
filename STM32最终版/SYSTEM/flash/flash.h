#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm32f10x.h"

//stm32f103c8t6��64kflash(MDK��⵽��128???)
//��ַ��Χ0x08000000-0x0800ffff(��0x08000000-0x0801ffff)
//Ҫ��ҳ���֣�1k�ֽڷ�ҳ,32��flash��nand flash???
//ʹ��flash���һҳģ��eeprom
//�洢������Ҫ4�ֽڶ���???
//0x0800ffff-1024+1=0x0800fc00

#define FLASH_ADR 0x0800fc00//�������洢���ı������ݵ���ʼ��ַ,

void FlashDataSave(void);


#endif



