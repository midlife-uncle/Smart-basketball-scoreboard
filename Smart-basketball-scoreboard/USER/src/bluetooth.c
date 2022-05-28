#include "bluetooth.h"
#include <string.h>
#include <stdio.h>
#include "key.h"

BluetoothData bluetoothdata;

/*****************************************************************
函数名称：void Bluetooth_Init(void)
函数功能：蓝牙初始化
入口参数：无
出口参数：无
说		明：使用USART3与蓝牙模块进行通信
引		脚：PB10 -> TX			PB11 -> RX
日		期：2021.3.29
*******************************************************************/
void Bluetooth_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;				//定义一个GPIO_InitTypeDef的结构体变量
	USART_InitTypeDef USART_InitStruct;			//定义一个USART_InitTypeDef的结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//开启RCC——GPIOB的时钟
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_10;			//选择引脚10
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF_PP;	//复用推挽输出模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_2MHz;	//2M的速度
	GPIO_Init(GPIOB,&GPIO_InitStruct);	//初始化GPIOB
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_11;					//选择引脚11
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN_FLOATING;//浮空输入模式
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
	//开启串口3的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART_InitStruct.USART_BaudRate						= 115200;								//波特率
	USART_InitStruct.USART_WordLength					= USART_WordLength_8b;	//数据位长度
	USART_InitStruct.USART_StopBits						= USART_StopBits_1;			//一位停止位
	USART_InitStruct.USART_Parity							= USART_Parity_No;			//无奇偶校验位
	USART_InitStruct.USART_Mode								= USART_Mode_Tx | USART_Mode_Rx;	//接收和发送模式
	USART_InitStruct.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART_Init(USART3,&USART_InitStruct);
	
	USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);	//开启串口接收中断--串口分的中断
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);	//开启串口空闲中断--串口分的中断	
	
	//开启NVIC中断控制
	NVIC_InitTypeDef  NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel    								= USART3_IRQn;	//选择串口3中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; 						//抢占优先级设置为1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority 				= 1; 						//响应优先级为1
	NVIC_InitStruct.NVIC_IRQChannelCmd 								= ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART3,ENABLE);		//使能UART3
}


//使用串口接收中断接收蓝牙发送过来的数据
void USART3_IRQHandler (void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET) 	//串口接收中断	
	{
		bluetoothdata.recbuf[bluetoothdata.reclen++] = USART_ReceiveData(USART3); //收到的数据不是结束标志，就把它存到数组中
	}
	
	if(USART_GetITStatus(USART3,USART_IT_IDLE) == SET) //串口空闲中断
	{
		USART3->SR;
		USART3->DR;
		bluetoothdata.recbuf[bluetoothdata.reclen] = '\0';	//在后面补上一个结束符
	}
}

/*****************************************************************
函数名称：void Bluetooth_SetFlag(void)
函数功能：设置标志位
入口参数：无
出口参数：无
说		明：比较串口中断结收到的数据
引		脚：无
日		期：2021.3.30
*******************************************************************/
void Bluetooth_SetFlag(void)
{
	if(strcmp((const char *)bluetoothdata.recbuf, "Stop_Start") == 0){
		Key_flag.Stop_Start = 1 - Key_flag.Stop_Start;			// 开始/暂停
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}//清楚数组里面的内容
	if(strcmp((const char *)bluetoothdata.recbuf, "Switch") == 0){
		Key_flag.Switch = 1 -  Key_flag.Switch;						//切换球队
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Bonus_Point") == 0){
		Key_flag.Bonus_Point = 1;						//加分
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Deduction") == 0){
		Key_flag.Deduction = 1;							//减分
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Penalty_Shot") == 0){
		Key_flag.Penalty_Shot = 1;					//犯规
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Voice_Broadcast") == 0){
		Key_flag.Voice_Broadcast = 1;				//语音播报
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Zero_Clearing") == 0){
		Key_flag.Zero_Clearing = 1;			//秒清零
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Time_DOWN") == 0){
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));
		Key_flag.Time_DOWN = 1;					}//调低分					
	if(strcmp((const char *)bluetoothdata.recbuf, "Time_UP") == 0){
		Key_flag.Time_UP = 1;						//调高分
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
	if(strcmp((const char *)bluetoothdata.recbuf, "Second24") == 0){
		Key_flag.Second24 = 1;						//24秒
		memset(&bluetoothdata, 0, sizeof(bluetoothdata));		}
}



