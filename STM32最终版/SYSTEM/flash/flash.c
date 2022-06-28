#include "flash.h"

void FlashDataSave(void)
{
	FLASH_Unlock();//解锁	
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//清除标志位	
	FLASH_ErasePage(FLASH_ADR);//要擦出页的起始地址	
	
//	FLASH_ProgramWord();
//	FLASH_ProgramHalfWord();
//	FLASH_ProgramOptionByteData();
	
	FLASH_Lock();//锁定
}

//读取函数
//	FlashDataSave(FLASH_ADR,value);
//	printf("flash data=%d\r\n",*(u32*)(FLASH_ADR));





