#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f10x.h"

void Delay_init(void);	//系统滴答延时初始化
void delay_us(u32 nus);	//微秒级延时
void delay_ms(u16 nms);	//毫秒级延时

#endif

