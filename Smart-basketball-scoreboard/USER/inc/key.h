#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f10x.h"

/*************************************************************************

按键中断的优先级大于定时器中断

*******************************************************************************/


/******************************************************
说		明：这个结构体用来存放按键的标志位
					可以用蓝牙控制，也能用按键控制
*******************************************************/
typedef struct
{
	u8 Stop_Start;			//开始/暂停
	u8 Switch;					//切换队伍进行加分减分操作
	u8 Bonus_Point;			//加分
	u8 Deduction;				//减分
	u8 Penalty_Shot;		//犯规
	u8 Voice_Broadcast;	//语音播报
	u8 Zero_Clearing;		//秒清零
	u8 Time_UP;					//向上调节时间
	u8 Time_DOWN;				//向下调节时间
	u8 Second24;				//向下调节时间
}Key_Flag;


extern Key_Flag Key_flag;


void Exti_Init(void);		//外部中断初始化

#endif

