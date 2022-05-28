#include "spi.h"


/********************************************************************************
�������ƣ�void SPI2_Init(void);
�������ܣ���ʼ��SPI2
��ڲ�������
���ڲ�������
����˵����PA7 -> MOSI 			PA5 -> SCK 
˵		�����˺��������ڳ�ʼ��OLED
��		�ڣ�2021.3.10
*********************************************************************************/
void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;			//����һ��GPIO_InitTypeDef�Ľṹ�����
	SPI_InitTypeDef  SPI_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	//����RCC����GPIOB��ʱ��

	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_15 | GPIO_Pin_13;	//ѡ������13��15
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_AF_PP;	//�����������ģʽ
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;	//50M���ٶ�
	GPIO_Init(GPIOB,&GPIO_InitStruct);							//��ʼ��GPIOB
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);		//ʹ��SPI2
  SPI_InitStruct.SPI_Direction 				 = SPI_Direction_2Lines_FullDuplex;	//2��ȫ˫��
  SPI_InitStruct.SPI_Mode 		 				 = SPI_Mode_Master;					//���ó�����ģʽ
  SPI_InitStruct.SPI_DataSize  				 = SPI_DataSize_8b;					//����֡8λ
  SPI_InitStruct.SPI_CPOL 		 				 = SPI_CPOL_High;						//ѡȡ��λ��ַ
  SPI_InitStruct.SPI_CPHA 		 				 = SPI_CPHA_2Edge;					//�ڶ�������
  SPI_InitStruct.SPI_NSS			 				 = SPI_NSS_Soft;						//���ģʽ
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;//��������Ƶ
  SPI_InitStruct.SPI_FirstBit			 		 = SPI_FirstBit_MSB;				//��λ�ȷ�
  SPI_InitStruct.SPI_CRCPolynomial 		 = 7;
	SPI_Init(SPI2,&SPI_InitStruct);
	
	SPI_Cmd(SPI2,ENABLE);		//ʹ��SPI2ʱ��
}

u16  SPI2_SendByte(SPI_TypeDef* SPIx, uint16_t Data)
{
	while (!SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE));	//�ȴ���������
	SPI_I2S_SendData(SPIx,Data);
	while (!SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE));
	return SPI_I2S_ReceiveData(SPIx);
}




