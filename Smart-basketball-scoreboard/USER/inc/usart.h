#ifndef  _USART_H_
#define _USART_H_
#include "stm32f10x.h"

void USART1_Init(u32 bound);		//����1��ʼ������


void USART1_SendByte(uint8_t Data);						//�����ֽں���
void USART1_SendString(uint8_t* Data,u8 len);	//�����ַ�������
#endif



