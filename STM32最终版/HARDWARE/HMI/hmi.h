#ifndef _HMI_H_
#define _HMI_H_

void HMI_SendTxt(unsigned char *page_id,unsigned char *control_id,float value);
float HMI_ReceiveTxt(unsigned char *DATA);

#endif

