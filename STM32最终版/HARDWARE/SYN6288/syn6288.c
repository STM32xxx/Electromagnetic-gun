#include "syn6288.h"
#include "string.h"
#include "usart.h"

/**************************************************************
技术手册请参考SYN6288--数据手册
**************************************************************/

/***********************************************************
* 名    称：  YS-SYN6288 文本合成函数
* 功    能：  发送合成文本到SYN6288芯片进行合成播放
* 入口参数：Music(背景音乐选择):0无背景音乐。1-15：相关背景音乐
*HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： SYN_FrameInfo（0，“乐声电子科技”）；
**********************************************************/
void SYN_FrameInfo(unsigned char Music,unsigned char *HZdata)
{
	/****************需要发送的文本**********************************/ 
	unsigned char Frame_Info[50];
	unsigned char HZ_Length;  
	unsigned char ecc  = 0;  			//定义校验字节
	unsigned int i = 0; 
	HZ_Length = strlen((const char*)HZdata); 			//需要发送文本的长度

	/*****************帧固定配置信息**************************************/           
	Frame_Info[0] = 0xFD ; 			//构造帧头FD
	Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
	Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
	Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
	Frame_Info[4] = 0x01 | Music<<4 ;  //构造命令参数：背景音乐设定

	/*******************校验码计算***************************************/		 
	for(i = 0; i<5; i++)   				//依次发送构造好的5个帧头字节
	{  
		ecc=ecc^(Frame_Info[i]);		//对发送的字节进行异或校验	
	}

	for(i= 0; i<HZ_Length; i++)   		//依次发送待合成的文本数据
	{  
		ecc=ecc^(HZdata[i]); 				//对发送的字节进行异或校验		
	}		 
	/*******************发送帧信息***************************************/		  
	memcpy(&Frame_Info[5], HZdata, HZ_Length);
	Frame_Info[5+HZ_Length]=ecc;
//	PrintCom(Frame_Info,5+HZ_Length+1);
	Uart3SendBuff((char*)Frame_Info,5+HZ_Length+1);
}






