#ifndef _SYN6288_H_
#define _SYN6288_H_
#include "stm32f10x.h"

extern u8 Voice_Broadcast_buf[34];


void SYN_FrameInfo(u8 Music, u8 *HZdata);		//�������ֺ���
void YS_SYN_Set(u8 *Info_data);
void Voice_Broadcast(void);		//������������

#endif


