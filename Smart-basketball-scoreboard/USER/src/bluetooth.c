#include "bluetooth.h"
#include <string.h>
#include <stdio.h>
#include "key.h"

BluetoothData bluetoothdata;

/*****************************************************************
�������ƣ�void Bluetooth_Init(void)
�������ܣ�������ʼ��
��ڲ�������
���ڲ�������
˵		����ʹ��USART3������ģ�����ͨ��
��		�ţ�PB10 -> TX			PB11 -> RX
��		�ڣ�2021.3.29
*******************************************************************/
void Bluetooth_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;				//����һ��GPIO_InitTypeDef�Ľṹ�����
	USART_InitTypeDef USART_InitStruct;			//����һ��USART_InitTypeDef�Ľṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//����RCC����GPIOB��ʱ��
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_10;			//ѡ������10
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF_PP;	//�����������ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_2MHz;	//2M���ٶ�
	GPIO_Init(GPIOB,&GPIO_InitStruct);	//��ʼ��GPIOB
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_11;					//ѡ������11
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN_FLOATING;//��������ģʽ
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	//��������3��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART_InitStruct.USART_BaudRate						= 115200;								//������
	USART_InitStruct.USART_WordLength					= USART_WordLength_8b;	//����λ����
	USART_InitStruct.USART_StopBits						= USART_StopBits_1;			//һλֹͣλ
	USART_InitStruct.USART_Parity							= USART_Parity_No;			//����żУ��λ
	USART_InitStruct.USART_Mode								= USART_Mode_Tx | USART_Mode_Rx;	//���պͷ���ģʽ
	USART_InitStruct.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_Init(USART3,&USART_InitStruct);
	
	USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);	//�������ڽ����ж�--���ڷֵ��ж�
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);	//�������ڿ����ж�--���ڷֵ��ж�	
	
	//����NVIC�жϿ���
	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel    								= USART3_IRQn;	//ѡ�񴮿�3�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; 						//��ռ���ȼ�����Ϊ1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 				= 1; 						//��Ӧ���ȼ�Ϊ1
	NVIC_InitStruct.NVIC_IRQChannelCmd 								= ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART3,ENABLE);		//ʹ��UART3
}


//ʹ�ô��ڽ����жϽ����������͹���������
void USART3_IRQHandler (void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET) 	//���ڽ����ж�	
	{
		bluetoothdata.recbuf[bluetoothdata.reclen++] = USART_ReceiveData(USART3); //�յ������ݲ��ǽ�����־���Ͱ����浽������
	}
	
	if(USART_GetITStatus(USART3,USART_IT_IDLE) == SET) //���ڿ����ж�
	{
		USART3->SR;
		USART3->DR;
		bluetoothdata.recbuf[bluetoothdata.reclen] = '\0';	//�ں��油��һ��������
	}
}

/*****************************************************************
�������ƣ�void Bluetooth_SetFlag(void)
�������ܣ����ñ�־λ
��ڲ�������
���ڲ�������
˵		�����Ƚϴ����жϽ��յ�������
��		�ţ���
��		�ڣ�2021.3.30
*******************************************************************/
void Bluetooth_SetFlag(void)
{
	if(strcmp((const char *)bluetoothdata.recbuf, "Stop_Start") == 0){
		Key_flag.Stop_Start = 1 - Key_flag.Stop_Start;			// ��ʼ/��ͣ
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}//����������������
	if(strcmp((const char *)bluetoothdata.recbuf, "Switch") == 0){
		Key_flag.Switch = 1 -  Key_flag.Switch;						//�л����
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Bonus_Point") == 0){
		Key_flag.Bonus_Point = 1;						//�ӷ�
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Deduction") == 0){
		Key_flag.Deduction = 1;							//����
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Penalty_Shot") == 0){
		Key_flag.Penalty_Shot = 1;					//����
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Voice_Broadcast") == 0){
		Key_flag.Voice_Broadcast = 1;				//��������
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Zero_Clearing") == 0){
		Key_flag.Zero_Clearing = 1;			//������
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Time_DOWN") == 0){
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));
		Key_flag.Time_DOWN = 1;					}//���ͷ�					
	if(strcmp((const char *)bluetoothdata.recbuf, "Time_UP") == 0){
		Key_flag.Time_UP = 1;						//���߷�
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Second24") == 0){
		Key_flag.Second24 = 1;						//24��
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
}



