#ifndef _DELAY_H_
#define _DELAY_H_
#include "stm32f10x.h"

void Delay_init(void);	//ϵͳ�δ���ʱ��ʼ��
void delay_us(u32 nus);	//΢�뼶��ʱ
void delay_ms(u16 nms);	//���뼶��ʱ

#endif

