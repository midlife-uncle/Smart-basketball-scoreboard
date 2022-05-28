#ifndef _OLED_H_
#define _OLED_H_
#include "stm32f10x.h"


#define Max_Column	128	//��Ļ�128������

#define GB_NUM 10				//���ֿ�����

/********		����˵��    ********************************

PB7 :CS		  PB13:SCL		PB15:SI
PA4 :RES		PA15:D/C

*********************************************************/

#define OLED_DC_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_15)	//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOA,GPIO_Pin_15)
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_7)	//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define OLED_RST_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)	//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define OLED_CMD  0			//д����
#define OLED_DATA 1			//д����


void OLED_Clear(void);	//��������
void OLED_Init(void);		//OLED��ʼ������
void OLED_ShowChar(u8 x,u8 y,u8 chr);						//��ʾASCII�ַ�
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len);		//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,const char c[]);//��ʾ����


#endif

