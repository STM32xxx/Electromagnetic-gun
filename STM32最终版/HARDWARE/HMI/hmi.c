#include "hmi.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

unsigned char hmi_number[] = {'0','1','2','3','4','5','6','7','8','9','\0'};//
unsigned char clear[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";//�ڽ��봮�ڽ����жϺ��������ݽ��жϱ��������ֵ���
unsigned char str[10] = "\0";//sprintfת���м�����
unsigned char hmi_command[]="\0";//����Ҫ������������ָ����ַ���
unsigned char *lastpage;//������һ�����ĸ�ҳ��

extern float Distance1;


/*************************************************************************************************************************
*��������HMI_SendTxt
*������page_id---���ڵڼ�ҳ����0��ʼ�������ҳ��˳���������ӣ�
*      control_id---��ҳ�ؼ������ƣ��趨��type��
*      value--Ҫ���͵�����
*���ܣ��򴮿������ı��ؼ���������
*�����ı���ʽΪt0.txt="abc"������Ϊ0xff 0xff 0xff����ڴ������ı��ؼ�tapeΪt0���ı���ʾabc
����HMI_SendTxt("0","t0f",123)
   �ڴ�������0ҳ��t0f�ı��ؼ���ʾ123
***************************************************************************************************************************/
void HMI_SendTxt(unsigned char *page_id,unsigned char *control_id,float value)
{
	int i = 0,hmi_len = 0;
	u32 temp=0;
	float temp_value;

	/********ѡ��ҳ********************************************/
	if(strcmp((const char*)page_id,(const char*)lastpage)!=0)
	{
		Uart2SendStr("page ");//��������пո񣬸�ʽΪpage 0
		Uart2SendStr((char *)page_id);
		Uart2SendByte(0xff);
		Uart2SendByte(0xff);
		Uart2SendByte(0xff);
	}

	//	/******ѡ��ؼ�****************************************/
	
//  printf("t=%f\n",Distance1);
	temp_value=value;
	if(temp_value<0)
		value = value*(-1);
	value=value*100;//*100Ϊ����ʾС�������λ
//	printf("t=%f\n",Distance1);
	temp = value;//��Ϊhmi_number[temp]  []�е��������Ǹ�����ȡ��
	sprintf((char *)str,"%c%c%c%c%c%c%c",hmi_number[temp%1000000/100000],hmi_number[temp%100000/10000],
	hmi_number[temp%10000/1000],hmi_number[temp%1000/100],'.',hmi_number[temp%100/10],hmi_number[temp%10]); 
	strcat((char *)hmi_command,(const char*)control_id);
	
	
	if(temp_value<0)
		strcat((char *)hmi_command,".txt=\"-");
	else
		strcat((char *)hmi_command,".txt=\"");
	
	strcat((char *)hmi_command,(const char*)str);
	strcat((char *)hmi_command,"\"");	
	
	
	Uart2SendStr((char *)hmi_command);//��������пո�
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
*��������HMI_ReceiveTxt
*���ܣ������������͵�32���ݣ�eg:HMI_USART_RX_BUF[]={0x23 0x66 0x34 0x35,0x38 x2A}����ֵ458ȡ��
*����float h;
		h=HMI_ReceiveTxt(HMI_USART_RX_BUF);
		�õ�h=458
*****************************************************************************************************************/
float HMI_ReceiveTxt(unsigned char *DATA)
{
	u8 decimals_places=0;//decimals_placesС��λ��*0.1
	u32 hmi_len=0,places=1;//places�������ݵ�λ��*10
	float hmi_data=0,decimals=1;//decimalsС����hmi_data���淢������ת�����ֵ
	u16 i,j;
	hmi_len=strlen((const char*)DATA);//ʹ��sizeofʱ�ò�����ȷ����ֵ
	
	if(DATA[2]=='-')
	{
		for(i=hmi_len-2;i>2;i--)
		{
			decimals_places++;
			if(DATA[i]==0x2e)//0x2e--'.'�������С�����¼С��������ݵ�λ��decimals_places����������Զ�Ӧ��ֵ�����Ϊ��ӦС��
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
		hmi_data *= decimals;//���� decimals����Ӧ�����С��
		hmi_data = hmi_data*(-1);
	}
	
	
	else
	{
		for(i=hmi_len-2;i>1;i--)
		{
			decimals_places++;
			if(DATA[i]==0x2e)//0x2e--'.'�������С�����¼С��������ݵ�λ��decimals_places����������Զ�Ӧ��ֵ�����Ϊ��ӦС��
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
	
		hmi_data *= decimals;//���� decimals����Ӧ�����С��
	}
	
//	printf("%f\n",hmi_data);
	return hmi_data;
}

