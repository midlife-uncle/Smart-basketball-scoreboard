#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "syn6288.h"
#include "oled.h"
#include <stdio.h>
#include <string.h>
#include "key.h"
#include "timer.h"
#include "bluetooth.h"

//git test
int main()
{
	USART1_Init(9600);//����1��ʼ��������������Ϊ9600�����������Ĳ��������ó�9600
	Bluetooth_Init();		//������ʼ��
	Delay_init();				//��ʱ��ʼ��
	OLED_Init();				//OLED��ʼ��
	Exti_Init();				//�ⲿ�жϳ�ʼ��
	Timer_Init(10000,72);	//��ʱ����ʼ����10�����һ�ж�
	OLED_ShowCHinese(0 , 0, "��");
	OLED_ShowCHinese(16, 0, "��");
	OLED_ShowCHinese(40, 0, "��");
	OLED_ShowCHinese(56, 0, "��");
	
	OLED_ShowChar(96,    0, ':');
	OLED_ShowChar(32,    2, ':');
	OLED_ShowChar(32,    4, ':');
	OLED_ShowChar(16,    6, ':');
	OLED_ShowChar(40,    6, ':');
	memset(&Key_flag,0,sizeof(Key_flag));			//ʹ��֮ǰ�Ƚ��������
	while(1)
	{
		Bluetooth_SetFlag();	//�������ñ�־λ
		Show_Score();					//��ʾ����
		Show_Time();					//��ʾʱ��
		Voice_Broadcast();		//������������	
	}
}




