#include "timer.h"
#include <stdio.h>
#include "key.h"
#include "oled.h"

Time time={12,0,0,24,0};		//����ʱ��
Score score;



/***********************************************************
��    �ƣ�void Timer_Init(u32 arr, u16 psc)
��    �ܣ���ʱ����ʼ��
��ڲ�����arr����װ��ֵ  	psc����Ƶϵ��	
���ڲ�������
˵    ����������������ʾ�Ƿ��Ƶ�ʱ��ĳ�ʼ��
���÷�����
��		�ڣ�2021.3.31
**********************************************************/
void Timer_Init(u32 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	//������ʱ��6ʱ��
	TIM_TimeBaseInitStruct.TIM_Period 	 		 = arr - 1; //������װֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler 		 = psc - 1; //���÷�Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 	//ʱ�ӷ�Ƶ���ӡ�������ʱ��û��
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000 ; 	//�ظ���������������ʱ��û��
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel    = TIM1_UP_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 4; //��ռ���ȼ�����Ϊ2
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3 ; 			 //��Ӧ���ȼ�Ϊ3
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE); //�����ж�
}

//��ʱ���жϷ�����
void TIM1_UP_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);		//����жϱ�־
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
		if(time.second24 == 0 &&  time.millisecond24 == 0){			//Υ��ֹͣ��ʱ
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
				time.second--;  //���0���59
			}
		}
		if((time.minute == 0) && (time.second == 0) && (time.millisecond == 0)){			//ʱ�䵽ֹͣ��ʱ
				Key_flag.Stop_Start = 0;
		}
	}
}

/***********************************************************
��    �ƣ�void Show_Time(void)
��    �ܣ���ʾʱ��
��ڲ�������	
���ڲ�������
˵    ����
���÷�����
��		�ڣ�2021.3.31
**********************************************************/
void Show_Time(void)
{
/**************  ������ ����ʱ��  ************************/
	if(Key_flag.Zero_Clearing){			//������
		Key_flag.Zero_Clearing = 0;
		time.millisecond = 0;
		time.second = 0;
	}
	if(Key_flag.Time_UP){					//���ϵ���ʱ��
		Key_flag.Time_UP = 0;		
		time.minute++;
	}
	if(Key_flag.Time_DOWN){				//���µ���ʱ��
		Key_flag.Time_DOWN = 0;
		time.minute--;
	}
/*********************************************************/
/**************   24��Υ��  ******************************/
	if(Key_flag.Second24){				//24��Υ��
		Key_flag.Second24  = 0;
		time.millisecond24 = 0;
		time.second24 		 = 24;
	}
/*********************************************************/
	if(Key_flag.Stop_Start){			
		TIM_Cmd(TIM1,ENABLE);		//����ʱ��ʹ��
	}
	else if(Key_flag.Stop_Start == 0){
		TIM_Cmd(TIM1,DISABLE);		//�ر�ʱ��ʹ��
	}
	
	OLED_ShowNum(80 ,0,time.second24, 2);
	OLED_ShowNum(104,0,time.millisecond24, 2);
	OLED_ShowNum(0 ,6,time.minute, 2);
	OLED_ShowNum(24,6,time.second, 2);
	OLED_ShowNum(48,6,time.millisecond, 2);
}

/***********************************************************
* ��    �ƣ�void Show_Score(void)
* ��    �ܣ���ʾ����
* ��ڲ�������	
* ���ڲ�������
* ˵    ����
* ���÷�����
���ڣ�2021.3.31
**********************************************************/
void Show_Score(void)
{
	if(Key_flag.Switch == 1){
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
		if(Key_flag.Bonus_Point == 1){		//�ӷ�
			Key_flag.Bonus_Point = 0;
			score.Blue_Score ++;
		}
		if(Key_flag.Deduction  == 1){			//����
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
			if(Key_flag.Bonus_Point == 1){		//�ӷ�
			Key_flag.Bonus_Point = 0;
			score.Red_Score ++;
		}
		if(Key_flag.Deduction  == 1){			//����
			score.Red_Score --;
			Key_flag.Deduction  = 0;
		}
		if(Key_flag.Penalty_Shot == 1){
			Key_flag.Penalty_Shot = 0;
			score.Red_Penalty_Shot++;
		}
	}
	
	OLED_ShowNum(0 ,2,score.Blue_Score, 3);		//���ӷ���
	OLED_ShowNum(40,2,score.Red_Score , 3);		//��ӷ���
	OLED_ShowNum(0 ,4,score.Blue_Penalty_Shot, 3);	//���ӷ�����
	OLED_ShowNum(40,4,score.Red_Penalty_Shot , 3);	//��ӷ�����
}




