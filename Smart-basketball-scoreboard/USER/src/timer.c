#include "timer.h"
#include <stdio.h>
#include "key.h"
#include "oled.h"

Time time={12,0,0,24,0};		//比赛时间
Score score;



/***********************************************************
名    称：void Timer_Init(u32 arr, u16 psc)
功    能：定时器初始化
入口参数：arr：重装载值  	psc：分频系数	
出口参数：无
说    明：本函数用于显示记分牌的时间的初始化
调用方法：
日		期：2021.3.31
**********************************************************/
void Timer_Init(u32 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	//开启定时器6时钟
	TIM_TimeBaseInitStruct.TIM_Period 	 		 = arr - 1; //设置重装值
	TIM_TimeBaseInitStruct.TIM_Prescaler 		 = psc - 1; //设置分频系数
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 	//时钟分频因子。基本定时器没有
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000 ; 	//重复计数器，基本定时器没有
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel    = TIM1_UP_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4; //抢占优先级设置为2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3 ; 			 //响应优先级为3
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE); //更新中断
}

//定时器中断服务函数
void TIM1_UP_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);		//清除中断标志
	if(Key_flag.Stop_Start)
	{
		if(time.millisecond24 == 0){
			time.millisecond24 = 99;
		}		
		else{
			time.millisecond24--;
		}
		if(time.millisecond24 == 99){
			time.second24--;	
		}
		if(time.second24 == 0 &&  time.millisecond24 == 0){			//违例停止计时
				Key_flag.Stop_Start = 0;
		}
/**********************************************************************/		
		if(time.millisecond == 0){
			time.millisecond = 99;
		}
		else{
			time.millisecond--;
		}
		if(time.millisecond == 99){
			if(time.second == 0){
				time.second = 59;
				time.minute--;
			}
			else{
				time.second--;  //秒从0变成59
			}
		}
		if((time.minute == 0) && (time.second == 0) && (time.millisecond == 0)){			//时间到停止计时
				Key_flag.Stop_Start = 0;
		}
	}
}

/***********************************************************
名    称：void Show_Time(void)
功    能：显示时间
入口参数：无	
出口参数：无
说    明：
调用方法：
日		期：2021.3.31
**********************************************************/
void Show_Time(void)
{
/**************  秒清零 调节时间  ************************/
	if(Key_flag.Zero_Clearing){			//秒清零
		Key_flag.Zero_Clearing = 0;
		time.millisecond = 0;
		time.second = 0;
	}
	if(Key_flag.Time_UP){					//向上调节时间
		Key_flag.Time_UP = 0;		
		time.minute++;
	}
	if(Key_flag.Time_DOWN){				//向下调节时间
		Key_flag.Time_DOWN = 0;
		time.minute--;
	}
/*********************************************************/
/**************   24秒违例  ******************************/
	if(Key_flag.Second24){				//24秒违例
		Key_flag.Second24  = 0;
		time.millisecond24 = 0;
		time.second24 		 = 24;
	}
/*********************************************************/
	if(Key_flag.Stop_Start){			
		TIM_Cmd(TIM1,ENABLE);		//开启时钟使能
	}
	else if(Key_flag.Stop_Start == 0){
		TIM_Cmd(TIM1,DISABLE);		//关闭时钟使能
	}
	
	OLED_ShowNum(80 ,0,time.second24, 2);
	OLED_ShowNum(104,0,time.millisecond24, 2);
	OLED_ShowNum(0 ,6,time.minute, 2);
	OLED_ShowNum(24,6,time.second, 2);
	OLED_ShowNum(48,6,time.millisecond, 2);
}

/***********************************************************
* 名    称：void Show_Score(void)
* 功    能：显示分数
* 入口参数：无	
* 出口参数：无
* 说    明：
* 调用方法：
日期：2021.3.31
**********************************************************/
void Show_Score(void)
{
	if(Key_flag.Switch == 1){
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
		if(Key_flag.Bonus_Point == 1){		//加分
			Key_flag.Bonus_Point = 0;
			score.Blue_Score ++;
		}
		if(Key_flag.Deduction  == 1){			//减分
			score.Blue_Score --;
			Key_flag.Deduction  = 0;
		}
		if(Key_flag.Penalty_Shot == 1){
			Key_flag.Penalty_Shot = 0;
			score.Blue_Penalty_Shot++;
		}
	}
	
	if(Key_flag.Switch == 0){
			GPIO_SetBits(GPIOA, GPIO_Pin_8);
			if(Key_flag.Bonus_Point == 1){		//加分
			Key_flag.Bonus_Point = 0;
			score.Red_Score ++;
		}
		if(Key_flag.Deduction  == 1){			//减分
			score.Red_Score --;
			Key_flag.Deduction  = 0;
		}
		if(Key_flag.Penalty_Shot == 1){
			Key_flag.Penalty_Shot = 0;
			score.Red_Penalty_Shot++;
		}
	}
	
	OLED_ShowNum(0 ,2,score.Blue_Score, 3);		//蓝队分数
	OLED_ShowNum(40,2,score.Red_Score , 3);		//红队分数
	OLED_ShowNum(0 ,4,score.Blue_Penalty_Shot, 3);	//蓝队犯规数
	OLED_ShowNum(40,4,score.Red_Penalty_Shot , 3);	//红队犯规数
}




