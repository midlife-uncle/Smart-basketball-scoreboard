#ifndef _BLUETOOTH_H
#define _BLUETOOTH_H
#include "stm32f10x.h"

typedef struct{	
	u8 recbuf[20];		//接收数据缓冲数组
	u8 reclen;				//接收数据长度	
}BluetoothData;				//串口3的接收状态结构体

extern BluetoothData bluetoothdata;

void Bluetooth_Init(void);	//蓝牙初始化
void Bluetooth_SetFlag(void);




#endif

