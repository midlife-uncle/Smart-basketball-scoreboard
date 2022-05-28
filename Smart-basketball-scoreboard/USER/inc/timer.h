#ifndef _TIMER_H_
#define _TIMER_H_
#include "stm32f10x.h"

//ʱ��ṹ��
typedef struct
{
	u8 	minute;			//��
	u8 	second;			//��
	u16 millisecond;//����
	u8 	second24;			//��
	u16 millisecond24;//����
}Time;

//�����ṹ�壬������Ӻ�����
typedef struct
{
	u8 Red_Score;						//��ӷ���
	u8 Red_Penalty_Shot;		//��ӷ�����
	
	u8 Blue_Score;					//���ӷ���
	u8 Blue_Penalty_Shot;		//���ӷ�����
}Score;		

extern Time time;		//����ʱ��
extern Score score;
void Timer_Init(u32 arr, u16 psc);		//��ʱ��6��ʼ��
void Show_Time(void);		//��ʾʱ��
void Show_Score(void);	//��ʾ����

#endif



