#include "usart.h"
#include <stdio.h>

/******************************************************
�������ܣ���ʼ������3
��ڲ�����  bound-> ������
���ڲ�������
˵		��������
��		�ڣ�2021.3.10
*******************************************************/
void USART1_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStruct;			//����һ��GPIO_InitTypeDef�Ľṹ�����
	USART_InitTypeDef USART_InitStruct;		//����һ��USART_InitTypeDef�Ľṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//����RCC����GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);	//��������1��ʱ��
	
	//PA9-> TX 		���ó����ģʽ
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_9;				//ѡ������9
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF_PP;	//�������ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_2MHz;	//2M���ٶ�
	GPIO_Init(GPIOA,&GPIO_InitStruct);							//��ʼ��GPIOA
	
	//PA10-> RX 		���ó�����ģʽ
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_10;						//ѡ������10
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN_FLOATING;	//��������ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_2MHz;				//2M���ٶ�
	GPIO_Init(GPIOA,&GPIO_InitStruct);										//��ʼ��GPIOA
	
	//����USART1
	USART_InitStruct.USART_BaudRate						= bound;								//������
	USART_InitStruct.USART_WordLength					= USART_WordLength_8b;//����λ����
	USART_InitStruct.USART_StopBits						= USART_StopBits_1;		//һλֹͣλ
	USART_InitStruct.USART_Parity							= USART_Parity_No;		//����żУ��λ
	USART_InitStruct.USART_Mode								= USART_Mode_Tx | USART_Mode_Rx;	//���պͷ���ģʽ
	USART_InitStruct.USART_HardwareFlowControl= USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);	//ʹ��UART1 
}


//�����ֽں���
void USART1_SendByte(uint8_t Data)
{
	while (!USART_GetFlagStatus(USART1,USART_FLAG_TC));
	USART_SendData(USART1,Data);
}

//�����ַ�������
void USART1_SendString(uint8_t* Data,u8 len)
{
	u8 i;
	for(i = 0; i < len; i++)
	{
		USART1_SendByte(*Data++);
	}
}

/******************************************
��дprintf����
******************************************/
int fputc(int ch ,FILE *f)
{
	 USART1_SendByte(ch);
	 return ch;
}


