#include "flash.h"

void FlashDataSave(void)
{
	FLASH_Unlock();//����	
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);//�����־λ	
	FLASH_ErasePage(FLASH_ADR);//Ҫ����ҳ����ʼ��ַ	
	
//	FLASH_ProgramWord();
//	FLASH_ProgramHalfWord();
//	FLASH_ProgramOptionByteData();
	
	FLASH_Lock();//����
}

//��ȡ����
//	FlashDataSave(FLASH_ADR,value);
//	printf("flash data=%d\r\n",*(u32*)(FLASH_ADR));





