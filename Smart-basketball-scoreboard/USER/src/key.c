#include "key.h"


Key_Flag Key_flag={0};		//������־λ


/*****************************************************************
�������ƣ�static void Key_Init(void);
�������ܣ���ʼ������I/O
��ڲ�������
���ڲ�������
��		�ţ�PC5  - PC8
					PC10 - PC14
˵		����ֻ�б��ļ����Ե����������
��		�ڣ�2021.3.24
*******************************************************************/
static void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);			//����RCC����GPIOC��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);			//����RCC����GPIOA��ʱ��
	GPIO_InitStruct.GPIO_Pin			= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | EXTI_Line15;
	GPIO_InitStruct.GPIO_Pin		 |= GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;	//ѡ������8��������9
	GPIO_InitStruct.GPIO_Mode			= GPIO_Mode_IPU;	//�ڲ�����
	GPIO_Init(GPIOC,&GPIO_InitStruct);							//��ʼ��GPIOC
	
	GPIO_InitStruct.GPIO_Mode			= GPIO_Mode_Out_PP;		//�������	
	GPIO_InitStruct.GPIO_Pin 			= GPIO_Pin_8;					//����8
	GPIO_InitStruct.GPIO_Speed		= GPIO_Speed_2MHz; 		//2MHz�ٶ�
	GPIO_Init(GPIOA,&GPIO_InitStruct);				//��ʼ��GPIOA
	GPIO_SetBits(GPIOA, GPIO_Pin_8);					//Ĭ�ϸߵ�ƽ��Ϩ��LED��
	GPIO_SetBits(GPIOC, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);		//�ڲ�����
}

/*****************************************************************
�������ƣ�void Exti_Init(void)
�������ܣ��ⲿ�жϳ�ʼ��
��ڲ�������
���ڲ�������
��		�ڣ�2021.3.24
*******************************************************************/
void Exti_Init(void)
{
	Key_Init();
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	EXTI_InitStruct.EXTI_Line				= EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9 | EXTI_Line10 | EXTI_Line11 | EXTI_Line12 | EXTI_Line13 | EXTI_Line14;		//��8��9�ж�
	EXTI_InitStruct.EXTI_LineCmd		= ENABLE;
	EXTI_InitStruct.EXTI_Mode				= EXTI_Mode_Interrupt;	//ѡ���ж�
	EXTI_InitStruct.EXTI_Trigger		= EXTI_Trigger_Rising;	//ѡ���½�����Ч
	EXTI_Init(&EXTI_InitStruct);
	
	//GPIO_EXTILineConfig���������û�����
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5 );		//��PC5 �����ж���ӳ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6 );		//��PC6 �����ж���ӳ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7 );		//��PC7 �����ж���ӳ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8 );		//��PC8 �����ж���ӳ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource9 );		//��PC9 �����ж���ӳ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);		//��PC10�����ж���ӳ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);		//��PC11�����ж���ӳ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);		//��PC12�����ж���ӳ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);		//��PC13�����ж���ӳ������
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource14);		//��PC14�����ж���ӳ������
	
	NVIC_InitStruct.NVIC_IRQChannel										=	EXTI9_5_IRQn;		//��5~9�ж�
	NVIC_InitStruct.NVIC_IRQChannelCmd								=	ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;		//�������ȼ�Ϊ0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority				= 1;		//��Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel			=	EXTI15_10_IRQn;	//��10~15�ж�
	NVIC_Init(&NVIC_InitStruct);
}


/*********************************************************************************
˵		����		��5 ->	��ʼ/��ͣ
							��6 ->	�л�������мӷּ��ֲ���
							��7 ->	�ӷ�
							��8 ->	����
							��9 ->	24���־
**********************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)){					//�ж��Ƿ�����5�ж�
		Key_flag.Stop_Start = 1 - Key_flag.Stop_Start;		//	��ͣ/��ʼ	
		EXTI_ClearITPendingBit(EXTI_Line5);			//�����5�жϱ�־
	}
	if(EXTI_GetITStatus(EXTI_Line6)){
		Key_flag.Switch  = 1 - Key_flag.Switch;	//ÿ��һ���жϾͻ��л�һ��״̬
		EXTI_ClearITPendingBit(EXTI_Line6);			//�����6�жϱ�־
	}
	if(EXTI_GetITStatus(EXTI_Line7)){
		Key_flag.Bonus_Point = 1;					//�ӷֱ�־
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line8)){
		Key_flag.Deduction	= 1;					//���ֱ�־
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line9)){
		Key_flag.Second24	= 1;					//24���־
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10)){
		Key_flag.Penalty_Shot		= 1;			//�����־
		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line11)){
		Key_flag.Voice_Broadcast	= 1;			//��������
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line12)){
		Key_flag.Zero_Clearing		= 1;			//������
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	if(EXTI_GetITStatus(EXTI_Line13)){
		Key_flag.Time_UP		= 1;					//���ϵ���ʱ��
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
	if(EXTI_GetITStatus(EXTI_Line14)){
		Key_flag.Time_DOWN	= 1;					//���µ���ʱ��
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}

/*********************************************************************************
˵		����		��10 ->	����
							��11 ->	����������־
							��12 ->	������
							��13 ->	���ϵ���ʱ��		ֻ�ܵ��ڷ�
							��14 ->	���µ���ʱ��
**********************************************************************************/



