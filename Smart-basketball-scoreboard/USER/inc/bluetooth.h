#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H
#include "stm32f10x.h"

typedef struct{	
	u8 recbuf[20];		//�������ݻ�������
	u8 reclen;				//�������ݳ���	
}BluetoothData;				//����3�Ľ���״̬�ṹ��

extern BluetoothData bluetoothdata;

void Bluetooth_Init(void);	//������ʼ��
void Bluetooth_SetFlag(void);




#endif

