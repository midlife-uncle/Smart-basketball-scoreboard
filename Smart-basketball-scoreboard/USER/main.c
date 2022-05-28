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
	USART1_Init(9600);//串口1初始化，波特率设置为9600，语音播报的波特率设置成9600
	Bluetooth_Init();		//蓝牙初始化
	Delay_init();				//延时初始化
	OLED_Init();				//OLED初始化
	Exti_Init();				//外部中断初始化
	Timer_Init(10000,72);	//定时器初始化，10毫秒进一中断
	OLED_ShowCHinese(0 , 0, "蓝");
	OLED_ShowCHinese(16, 0, "队");
	OLED_ShowCHinese(40, 0, "红");
	OLED_ShowCHinese(56, 0, "队");
	
	OLED_ShowChar(96,    0, ':');
	OLED_ShowChar(32,    2, ':');
	OLED_ShowChar(32,    4, ':');
	OLED_ShowChar(16,    6, ':');
	OLED_ShowChar(40,    6, ':');
	memset(&Key_flag,0,sizeof(Key_flag));			//使用之前先将数组清空
	while(1)
	{
		Bluetooth_SetFlag();	//蓝牙设置标志位
		Show_Score();					//显示分数
		Show_Time();					//显示时间
		Voice_Broadcast();		//语音播报函数	
	}
}




