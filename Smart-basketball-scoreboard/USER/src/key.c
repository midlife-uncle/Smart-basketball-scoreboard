#include "key.h"


Key_Flag Key_flag={0};		//按键标志位


/*****************************************************************
函数名称：static void Key_Init(void);
函数功能：初始化按键I/O
入口参数：无
出口参数：无
引		脚：PC5  - PC8
					PC10 - PC14
说		明：只有本文件可以调用这个函数
日		期：2021.3.24
*******************************************************************/
static void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);			//开启RCC——GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);			//开启RCC——GPIOA的时钟
	GPIO_InitStruct.GPIO_Pin			= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | EXTI_Line15;
	GPIO_InitStruct.GPIO_Pin		 |= GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;	//选择引脚8，和引脚9
	GPIO_InitStruct.GPIO_Mode			= GPIO_Mode_IPU;	//内部上拉
	GPIO_Init(GPIOC,&GPIO_InitStruct);							//初始化GPIOC
	
	GPIO_InitStruct.GPIO_Mode			= GPIO_Mode_Out_PP;		//推挽输出	
	GPIO_InitStruct.GPIO_Pin 			= GPIO_Pin_8;					//引脚8
	GPIO_InitStruct.GPIO_Speed		= GPIO_Speed_2MHz; 		//2MHz速度
	GPIO_Init(GPIOA,&GPIO_InitStruct);				//初始化GPIOA
	GPIO_SetBits(GPIOA, GPIO_Pin_8);					//默认高电平，熄灭LED灯
	GPIO_SetBits(GPIOC, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);		//内部供电
}

/*****************************************************************
函数名称：void Exti_Init(void)
函数功能：外部中断初始化
入口参数：无
出口参数：无
日		期：2021.3.24
*******************************************************************/
void Exti_Init(void)
{
	Key_Init();
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	EXTI_InitStruct.EXTI_Line				= EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9 | EXTI_Line10 | EXTI_Line11 | EXTI_Line12 | EXTI_Line13 | EXTI_Line14;		//线8，9中断
	EXTI_InitStruct.EXTI_LineCmd		= ENABLE;
	EXTI_InitStruct.EXTI_Mode				= EXTI_Mode_Interrupt;	//选择中断
	EXTI_InitStruct.EXTI_Trigger		= EXTI_Trigger_Rising;	//选择下降沿有效
	EXTI_Init(&EXTI_InitStruct);
	
	//GPIO_EXTILineConfig函数不能用或运算
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5 );		//将PC5 口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6 );		//将PC6 口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7 );		//将PC7 口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8 );		//将PC8 口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9 );		//将PC9 口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);		//将PC10口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);		//将PC11口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);		//将PC12口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);		//将PC13口与中断线映射起来
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);		//将PC14口与中断线映射起来
	
	NVIC_InitStruct.NVIC_IRQChannel										=	EXTI9_5_IRQn;		//线5~9中断
	NVIC_InitStruct.NVIC_IRQChannelCmd								=	ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//抢断优先级为0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority				= 1;		//响应优先级为1
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel			=	EXTI15_10_IRQn;	//线10~15中断
	NVIC_Init(&NVIC_InitStruct);
}


/*********************************************************************************
说		明：		线5 ->	开始/暂停
							线6 ->	切换队伍进行加分减分操作
							线7 ->	加分
							线8 ->	减分
							线9 ->	24秒标志
**********************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)){					//判断是否是线5中断
		Key_flag.Stop_Start = 1 - Key_flag.Stop_Start;		//	暂停/开始	
		EXTI_ClearITPendingBit(EXTI_Line5);			//清除线5中断标志
	}
	if(EXTI_GetITStatus(EXTI_Line6)){
		Key_flag.Switch  = 1 - Key_flag.Switch;	//每进一次中断就会切换一次状态
		EXTI_ClearITPendingBit(EXTI_Line6);			//清除线6中断标志
	}
	if(EXTI_GetITStatus(EXTI_Line7)){
		Key_flag.Bonus_Point = 1;					//加分标志
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line8)){
		Key_flag.Deduction	= 1;					//减分标志
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line9)){
		Key_flag.Second24	= 1;					//24秒标志
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10)){
		Key_flag.Penalty_Shot		= 1;			//罚球标志
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line11)){
		Key_flag.Voice_Broadcast	= 1;			//语音播报
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line12)){
		Key_flag.Zero_Clearing		= 1;			//秒清零
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if(EXTI_GetITStatus(EXTI_Line13)){
		Key_flag.Time_UP		= 1;					//向上调节时间
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14)){
		Key_flag.Time_DOWN	= 1;					//向下调节时间
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}

/*********************************************************************************
说		明：		线10 ->	罚球
							线11 ->	语音播报标志
							线12 ->	秒清零
							线13 ->	向上调节时间		只能调节分
							线14 ->	向下调节时间
**********************************************************************************/



