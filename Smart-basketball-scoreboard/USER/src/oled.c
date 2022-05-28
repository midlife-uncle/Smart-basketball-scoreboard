#include "oled.h"
#include "spi.h"
#include "delay.h"
#include "oledfont.h"

/*****************************************************************
函数名称：static void OLED_WR_Byte(u8 dat,u8 cmd);
函数功能：向SSD1306写入一个字节。
入口参数：dat:要写入的数据/命令
					cmd:数据/命令标志。		0,表示命令;1,表示数据;
出口参数：无
说		明：只有本文件可以调用这个函数
日		期：2021.3.10
*******************************************************************/
static void OLED_WR_Byte(u8 dat, u8 cmd)
{
	OLED_CS_Clr();	//拉低片选
	if(cmd)					//如果发送的是数据
	  OLED_DC_Set();//拉高DC
	else 
	  OLED_DC_Clr();//如果发送的是命令，拉低DC
	SPI2_SendByte(SPI2, dat);
	OLED_CS_Set();		//发送完数据或者命令，拉高片选
	OLED_DC_Set();		//拉高DC，默认发送命令
}


/*****************************************************************
函数名称：void OLED_Clear(void) 
函数功能：清除OLED的所有内容。
入口参数：无
出口参数：无
说		明：清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
日		期：2021.3.10
*******************************************************************/  
void OLED_Clear(void)  
{  
	u8 i, n;		    
	for(i = 0; i < 8; i++)  
	{  
		OLED_WR_Byte (0xb0 + i, OLED_CMD);   //设置页地址（0~7）
		OLED_WR_Byte (0x00, OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10, OLED_CMD);      //设置显示位置—列高地址   
		for(n = 0; n < 128; n++)	OLED_WR_Byte(0 , OLED_DATA); 
	} //更新显示
}

/*****************************************************************
函数名称：void OLED_Set_Pos(unsigned char x, unsigned char y) 
函数功能：设置坐标。
入口参数：x -> 横坐标；	y-> 纵坐标
出口参数：
说		明：只能在本文件调用
日		期：2021.3.10
*******************************************************************/ 
static void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y, OLED_CMD);
	OLED_WR_Byte(((x&0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x&0x0f) | 0x01, OLED_CMD);
}


/*****************************************************************
函数名称：void OLED_Init(void)
函数功能：初始化OLED。
入口参数：dat:要写入的数据/命令
					cmd:数据/命令标志。		0,表示命令;1,表示数据;
出口参数：无
日		期：2021.3.10
*******************************************************************/
void OLED_Init(void)
{
	SPI2_Init();		//硬件SPI初始化
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB口时钟
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	 					//初始化GPIO
 	GPIO_SetBits(GPIOB, GPIO_Pin_7);		//默认是高电平
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能GPIOA口时钟
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				//初始化GPIOA
 	GPIO_SetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_15);			//默认是高电平
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 		 //开启复用功能时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭PA15的JTAG下载功能，PA15才能当作通用IO使用

	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set();
	
	/***************  	以下是启动程序  	**************************/
	/***************  具体需参考芯片手册  **************************/
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
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
函数名称：u32 oled_pow(u8 m,u8 n)
函数功能：计算m的n次方/幂
入口参数：m -> 底数		n -> 幂
出口参数：无
说		明：只能在本文件中调用
日		期：2021.3.10
*****************************************/
static u32 oled_pow(u8 m,u8 n)
{
	u32 result = 1;	 
	while(n--)		result *= m;    
	return result;
}

/*****************************************************************
函数名称：void OLED_ShowChar(u8 x,u8 y,u8 chr) 
函数功能：在指定位置显示字符，ASCII码。
入口参数：起始坐标(x , y)
					chr ->	显示的字符
出口参数：无
说		明：x:0~127		y:0~63
					只能显示部分字符
日		期：2021.3.10
*******************************************************************/ 
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{    	
	unsigned char c = 0, i = 0;	
	c = chr - ' ';		//得到偏移后的值			
	if(x > Max_Column - 1)	{		x = 0; y = y + 2;		}		//如果这一行已显示完，换下一行显示
			OLED_Set_Pos(x , y);	
			for( i = 0; i < 8; i++)
			OLED_WR_Byte(ASCII_F8X16[c*16 + i] , OLED_DATA);
			OLED_Set_Pos(x , y+1);
			for(i = 0; i < 8; i++)
			OLED_WR_Byte(ASCII_F8X16[c*16+i+8] , OLED_DATA);
}

/*****************************************************
函数名称：void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len)
函数功能：显示数字
入口参数：起始坐标(x, y)		num -> 需要显示的数字
					len -> 数字长度
出口参数：无
日		期：2021.3.10
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
函数名称：void OLED_ShowCHinese(u8 x,u8 y,const char c[])
函数功能：显示汉字
入口参数：起始坐标(x, y)		c[] -> 需要显示的汉字
出口参数：无
说		明：只能显示大小为16字节的汉字
					直接输入汉字即可
日		期：2021.3.10
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
		for(j = 0; j < 32; j++)	  			//一个中文32个字节的字模
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

