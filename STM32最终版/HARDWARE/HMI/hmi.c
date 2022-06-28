#include "hmi.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

unsigned char hmi_number[] = {'0','1','2','3','4','5','6','7','8','9','\0'};//
unsigned char clear[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";//在进入串口接收中断后处理完数据将中断保存数组的值清空
unsigned char str[10] = "\0";//sprintf转换中间数组
unsigned char hmi_command[]="\0";//保存要给串口屏发送指令的字符串
unsigned char *lastpage;//保存上一次是哪个页面

extern float Distance1;


/*************************************************************************************************************************
*函数名：HMI_SendTxt
*参数：page_id---串口第几页（从0开始根据软件页面顺序依次增加）
*      control_id---本页控件的名称（设定的type）
*      value--要发送的数据
*功能：向串口屏的文本控件发送数据
*发送文本格式为t0.txt="abc"结束符为0xff 0xff 0xff结果在串口屏文本控件tape为t0的文本显示abc
例：HMI_SendTxt("0","t0f",123)
   在串口屏第0页的t0f文本控件显示123
***************************************************************************************************************************/
void HMI_SendTxt(unsigned char *page_id,unsigned char *control_id,float value)
{
	int i = 0,hmi_len = 0;
	u32 temp=0;
	float temp_value;

	/********选择页********************************************/
	if(strcmp((const char*)page_id,(const char*)lastpage)!=0)
	{
		Uart2SendStr("page ");//后面必须有空格，格式为page 0
		Uart2SendStr((char *)page_id);
		Uart2SendByte(0xff);
		Uart2SendByte(0xff);
		Uart2SendByte(0xff);
	}

	//	/******选择控件****************************************/
	
//  printf("t=%f\n",Distance1);
	temp_value=value;
	if(temp_value<0)
		value = value*(-1);
	value=value*100;//*100为了显示小数点后两位
//	printf("t=%f\n",Distance1);
	temp = value;//因为hmi_number[temp]  []中的数不能是浮点数取余
	sprintf((char *)str,"%c%c%c%c%c%c%c",hmi_number[temp%1000000/100000],hmi_number[temp%100000/10000],
	hmi_number[temp%10000/1000],hmi_number[temp%1000/100],'.',hmi_number[temp%100/10],hmi_number[temp%10]); 
	strcat((char *)hmi_command,(const char*)control_id);
	
	
	if(temp_value<0)
		strcat((char *)hmi_command,".txt=\"-");
	else
		strcat((char *)hmi_command,".txt=\"");
	
	strcat((char *)hmi_command,(const char*)str);
	strcat((char *)hmi_command,"\"");	
	
	
	Uart2SendStr((char *)hmi_command);//后面必须有空格
	Uart2SendByte(0xff);
	Uart2SendByte(0xff);
	Uart2SendByte(0xff);
	
	

	hmi_len = strlen((const char*)hmi_command);
	for(i=0;i<hmi_len;i++)
	{
		hmi_command[i]=clear[i];
	}
	lastpage=page_id;
}

/***************************************************************************************************************
*函数名：HMI_ReceiveTxt
*功能：将串口屏传送到32数据（eg:HMI_USART_RX_BUF[]={0x23 0x66 0x34 0x35,0x38 x2A}）数值458取出
*例：float h;
		h=HMI_ReceiveTxt(HMI_USART_RX_BUF);
		得到h=458
*****************************************************************************************************************/
float HMI_ReceiveTxt(unsigned char *DATA)
{
	u8 decimals_places=0;//decimals_places小数位数*0.1
	u32 hmi_len=0,places=1;//places整个数据的位数*10
	float hmi_data=0,decimals=1;//decimals小数，hmi_data保存发送数据转换后的值
	u16 i,j;
	hmi_len=strlen((const char*)DATA);//使用sizeof时得不到正确的数值
	
	if(DATA[2]=='-')
	{
		for(i=hmi_len-2;i>2;i--)
		{
			decimals_places++;
			if(DATA[i]==0x2e)//0x2e--'.'如果遇到小数点记录小数点后数据的位数decimals_places，并将其乘以对应的值将其变为对应小数
			{
				for(j=0;j<decimals_places-1;j++)
				{
					decimals *=0.1;
				}
			}
			else
			{
				hmi_data += (DATA[i]-0x30)*places;
				places=places*10;
			}
		}
		hmi_data *= decimals;//乘以 decimals将对应数变成小数
		hmi_data = hmi_data*(-1);
	}
	
	
	else
	{
		for(i=hmi_len-2;i>1;i--)
		{
			decimals_places++;
			if(DATA[i]==0x2e)//0x2e--'.'如果遇到小数点记录小数点后数据的位数decimals_places，并将其乘以对应的值将其变为对应小数
			{
				for(j=0;j<decimals_places-1;j++)
				{
					decimals *=0.1;
				}
			}
			else
			{
				hmi_data += (DATA[i]-0x30)*places;
				places=places*10;
			}
		}
	
		hmi_data *= decimals;//乘以 decimals将对应数变成小数
	}
	
//	printf("%f\n",hmi_data);
	return hmi_data;
}

