#ifndef _OLED_H_
#define _OLED_H_

#include "sys.h"
//#include "stm32f10x_pwr.h"
//#include "stm32f10x_bkp.h"

//-----------------OLED端口定义---------------- 
#define OLED_RST_Clr() PBout(7)=0   //RST
#define OLED_RST_Set() PBout(7)=1   //RST

#define OLED_RS_Clr() PBout(6)=0    //DC
#define OLED_RS_Set() PBout(6)=1    //DC

#define OLED_SCLK_Clr()  PBout(9)=0  //SCL
#define OLED_SCLK_Set()  PBout(9)=1   //SCL

#define OLED_SDIN_Clr()  PBout(8)=0   //SDA
#define OLED_SDIN_Set()  PBout(8)=1   //SDA

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
//void OLED_Display_On(void);
//void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   				   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p);	

#endif




