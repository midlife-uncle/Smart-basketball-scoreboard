#ifndef  _USART_H_
#define _USART_H_
#include "stm32f10x.h"

void USART1_Init(u32 bound);		//串口1初始化函数


void USART1_SendByte(uint8_t Data);						//发送字节函数
void USART1_SendString(uint8_t* Data,u8 len);	//发送字符串函数
#endif



