#ifndef _TIMER_H_
#define _TIMER_H_
#include "stm32f10x.h"

//时间结构体
typedef struct
{
	u8 	minute;			//分
	u8 	second;			//秒
	u16 millisecond;//毫秒
	u8 	second24;			//秒
	u16 millisecond24;//毫秒
}Time;

//分数结构体，包含红队和蓝队
typedef struct
{
	u8 Red_Score;						//红队分数
	u8 Red_Penalty_Shot;		//红队犯规数
	
	u8 Blue_Score;					//蓝队分数
	u8 Blue_Penalty_Shot;		//蓝队犯规数
}Score;		

extern Time time;		//比赛时间
extern Score score;
void Timer_Init(u32 arr, u16 psc);		//定时器6初始化
void Show_Time(void);		//显示时间
void Show_Score(void);	//显示分数

#endif



