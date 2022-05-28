#include "usart.h"
#include <stdio.h>

/******************************************************
函数功能：初始化串口3
入口参数：  bound-> 波特率
出口参数：无
说		明：保留
日		期：2021.3.10
*******************************************************/
void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStruct;			//定义一个GPIO_InitTypeDef的结构体变量
	USART_InitTypeDef USART_InitStruct;		//定义一个USART_InitTypeDef的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//开启RCC——GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//开启串口1的时钟
	
	//PA9-> TX 		配置成输出模式
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_9;				//选择引脚9
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF_PP;	//推挽输出模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_2MHz;	//2M的速度
	GPIO_Init(GPIOA,&GPIO_InitStruct);							//初始化GPIOA
	
	//PA10-> RX 		配置成输入模式
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_10;						//选择引脚10
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN_FLOATING;	//浮空输入模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_2MHz;				//2M的速度
	GPIO_Init(GPIOA,&GPIO_InitStruct);										//初始化GPIOA
	
	//配置USART1
	USART_InitStruct.USART_BaudRate						= bound;								//波特率
	USART_InitStruct.USART_WordLength					= USART_WordLength_8b;//数据位长度
	USART_InitStruct.USART_StopBits						= USART_StopBits_1;		//一位停止位
	USART_InitStruct.USART_Parity							= USART_Parity_No;		//无奇偶校验位
	USART_InitStruct.USART_Mode								= USART_Mode_Tx | USART_Mode_Rx;	//接收和发送模式
	USART_InitStruct.USART_HardwareFlowControl= USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);	//使能UART1 
}


//发送字节函数
void USART1_SendByte(uint8_t Data)
{
	while (!USART_GetFlagStatus(USART1,USART_FLAG_TC));
	USART_SendData(USART1,Data);
}

//发送字符串函数
void USART1_SendString(uint8_t* Data,u8 len)
{
	u8 i;
	for(i = 0; i < len; i++)
	{
		USART1_SendByte(*Data++);
	}
}

/******************************************
重写printf函数
******************************************/
int fputc(int ch ,FILE *f)
{
	 USART1_SendByte(ch);
	 return ch;
}


