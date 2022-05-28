#include "oled.h"
#include "spi.h"
#include "delay.h"
#include "oledfont.h"

/*****************************************************************
�������ƣ�static void OLED_WR_Byte(u8 dat,u8 cmd);
�������ܣ���SSD1306д��һ���ֽڡ�
��ڲ�����dat:Ҫд�������/����
					cmd:����/�����־��		0,��ʾ����;1,��ʾ����;
���ڲ�������
˵		����ֻ�б��ļ����Ե����������
��		�ڣ�2021.3.10
*******************************************************************/
static void OLED_WR_Byte(u8 dat, u8 cmd)
{
	OLED_CS_Clr();	//����Ƭѡ
	if(cmd)					//������͵�������
	  OLED_DC_Set();//����DC
	else 
	  OLED_DC_Clr();//������͵����������DC
	SPI2_SendByte(SPI2, dat);
	OLED_CS_Set();		//���������ݻ����������Ƭѡ
	OLED_DC_Set();		//����DC��Ĭ�Ϸ�������
}


/*****************************************************************
�������ƣ�void OLED_Clear(void) 
�������ܣ����OLED���������ݡ�
��ڲ�������
���ڲ�������
˵		������������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
��		�ڣ�2021.3.10
*******************************************************************/  
void OLED_Clear(void)  
{  
	u8 i, n;		    
	for(i = 0; i < 8; i++)  
	{  
		OLED_WR_Byte (0xb0 + i, OLED_CMD);   //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00, OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10, OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n = 0; n < 128; n++)	OLED_WR_Byte(0 , OLED_DATA); 
	} //������ʾ
}

/*****************************************************************
�������ƣ�void OLED_Set_Pos(unsigned char x, unsigned char y) 
�������ܣ��������ꡣ
��ڲ�����x -> �����ꣻ	y-> ������
���ڲ�����
˵		����ֻ���ڱ��ļ�����
��		�ڣ�2021.3.10
*******************************************************************/ 
static void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y, OLED_CMD);
	OLED_WR_Byte(((x&0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x&0x0f) | 0x01, OLED_CMD);
}


/*****************************************************************
�������ƣ�void OLED_Init(void)
�������ܣ���ʼ��OLED��
��ڲ�����dat:Ҫд�������/����
					cmd:����/�����־��		0,��ʾ����;1,��ʾ����;
���ڲ�������
��		�ڣ�2021.3.10
*******************************************************************/
void OLED_Init(void)
{
	SPI2_Init();		//Ӳ��SPI��ʼ��
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOB��ʱ��
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	 					//��ʼ��GPIO
 	GPIO_SetBits(GPIOB, GPIO_Pin_7);		//Ĭ���Ǹߵ�ƽ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��GPIOA��ʱ��
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				//��ʼ��GPIOA
 	GPIO_SetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_15);			//Ĭ���Ǹߵ�ƽ
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 		 //�������ù���ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //�ر�PA15��JTAG���ع��ܣ�PA15���ܵ���ͨ��IOʹ��

	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set();
	
	/***************  	��������������  	**************************/
	/***************  ������ο�оƬ�ֲ�  **************************/
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0, 0); 	
}

/****************************************
�������ƣ�u32 oled_pow(u8 m,u8 n)
�������ܣ�����m��n�η�/��
��ڲ�����m -> ����		n -> ��
���ڲ�������
˵		����ֻ���ڱ��ļ��е���
��		�ڣ�2021.3.10
*****************************************/
static u32 oled_pow(u8 m,u8 n)
{
	u32 result = 1;	 
	while(n--)		result *= m;    
	return result;
}

/*****************************************************************
�������ƣ�void OLED_ShowChar(u8 x,u8 y,u8 chr) 
�������ܣ���ָ��λ����ʾ�ַ���ASCII�롣
��ڲ�������ʼ����(x , y)
					chr ->	��ʾ���ַ�
���ڲ�������
˵		����x:0~127		y:0~63
					ֻ����ʾ�����ַ�
��		�ڣ�2021.3.10
*******************************************************************/ 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{    	
	unsigned char c = 0, i = 0;	
	c = chr - ' ';		//�õ�ƫ�ƺ��ֵ			
	if(x > Max_Column - 1)	{		x = 0; y = y + 2;		}		//�����һ������ʾ�꣬����һ����ʾ
			OLED_Set_Pos(x , y);	
			for( i = 0; i < 8; i++)
			OLED_WR_Byte(ASCII_F8X16[c*16 + i] , OLED_DATA);
			OLED_Set_Pos(x , y+1);
			for(i = 0; i < 8; i++)
			OLED_WR_Byte(ASCII_F8X16[c*16+i+8] , OLED_DATA);
}

/*****************************************************
�������ƣ�void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len)
�������ܣ���ʾ����
��ڲ�������ʼ����(x, y)		num -> ��Ҫ��ʾ������
					len -> ���ֳ���
���ڲ�������
��		�ڣ�2021.3.10
******************************************************/ 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len)
{         	
	u8 t, temp;
	u8 enshow = 0;						   
	for( t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if(enshow == 0 && t < (len - 1))
		{
			if(temp == 0)
			{
				OLED_ShowChar(x + 8 * t, y, ' ');
				continue;
			}else enshow = 1; 
		 	 
		}
	 	OLED_ShowChar(x + 8 * t, y, temp + '0'); 
	}
}

/*****************************************************
�������ƣ�void OLED_ShowCHinese(u8 x,u8 y,const char c[])
�������ܣ���ʾ����
��ڲ�������ʼ����(x, y)		c[] -> ��Ҫ��ʾ�ĺ���
���ڲ�������
˵		����ֻ����ʾ��СΪ16�ֽڵĺ���
					ֱ�����뺺�ּ���
��		�ڣ�2021.3.10
******************************************************/
void OLED_ShowCHinese(u8 x, u8 y, const char c[])
{      			    
	u8 temp[32], j, k;
	u16 gb_num;			 	
	for(gb_num = 0; gb_num < GB_NUM; gb_num++)
	{
		if((c[0] == GB_1616[gb_num].index[0])  &&  (c[1] == GB_1616[gb_num].index[1]))
		{
			break;	
		}
	}
	if(gb_num < GB_NUM)
	{
		for(j = 0; j < 32; j++)	  			//һ������32���ֽڵ���ģ
  	{ 					  
			temp[j]=GB_1616[gb_num].matrix[j];
		}
		OLED_Set_Pos(x, y);	
    for(k = 0; k < 16; k++)
		{
				OLED_WR_Byte(temp[k], OLED_DATA);
    }	
		OLED_Set_Pos(x, y+1);	
    for(k = 0; k < 16; k++)
		{	
				OLED_WR_Byte(temp[k+16], OLED_DATA);
     }		
	}			
}

