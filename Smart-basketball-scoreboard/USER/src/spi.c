#include "spi.h"


/********************************************************************************
函数名称：void SPI2_Init(void);
函数功能：初始化SPI2
入口参数：无
出口参数：无
引脚说明：PA7 -> MOSI 			PA5 -> SCK 
说		明：此函数仅用于初始化OLED
日		期：2021.3.10
*********************************************************************************/
void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;			//定义一个GPIO_InitTypeDef的结构体变量
	SPI_InitTypeDef  SPI_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//开启RCC——GPIOB的时钟

	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_15 | GPIO_Pin_13;	//选择引脚13和15
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF_PP;	//复用推挽输出模式
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;	//50M的速度
	GPIO_Init(GPIOB,&GPIO_InitStruct);							//初始化GPIOB
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);		//使能SPI2
  SPI_InitStruct.SPI_Direction 				 = SPI_Direction_2Lines_FullDuplex;	//2线全双工
  SPI_InitStruct.SPI_Mode 		 				 = SPI_Mode_Master;					//设置成主机模式
  SPI_InitStruct.SPI_DataSize  				 = SPI_DataSize_8b;					//数据帧8位
  SPI_InitStruct.SPI_CPOL 		 				 = SPI_CPOL_High;						//选取高位地址
  SPI_InitStruct.SPI_CPHA 		 				 = SPI_CPHA_2Edge;					//第二个边沿
  SPI_InitStruct.SPI_NSS			 				 = SPI_NSS_Soft;						//软件模式
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;//主机二分频
  SPI_InitStruct.SPI_FirstBit			 		 = SPI_FirstBit_MSB;				//高位先发
  SPI_InitStruct.SPI_CRCPolynomial 		 = 7;
	SPI_Init(SPI2,&SPI_InitStruct);
	
	SPI_Cmd(SPI2,ENABLE);		//使能SPI2时钟
}

u16  SPI2_SendByte(SPI_TypeDef* SPIx, uint16_t Data)
{
	while (!SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE));	//等待发送数据
	SPI_I2S_SendData(SPIx,Data);
	while (!SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPIx);
}




