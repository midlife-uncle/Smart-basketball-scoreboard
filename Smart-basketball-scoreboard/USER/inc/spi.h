#ifndef _SPI_H_
#define _SPI_H_
#include "stm32f10x.h"

void SPI2_Init(void);
u16  SPI2_SendByte(SPI_TypeDef* SPIx, uint16_t Data);


#endif
