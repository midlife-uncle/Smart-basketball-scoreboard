#include "syn6288.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "key.h"
#include "delay.h"
#include "timer.h"

u8 Voice_Broadcast_buf[34] = "[v10][m1][t3]现在的比分是";		//构造语音播报数据

/**************************************************************
函数名称：void SYN_FrameInfo(u8 Music, u8 *HZdata)
函数功能：文字转语音播报
入口参数： Music:选择背景音乐。 0:无背景音乐；1~15：选择背景音乐
					 HZdata：需要播报的内容
出口参数：无
说    明：HZdata的格式："[v4][m5][t2]123456";
					m[0~16]:0背景音乐为静音，16背景音乐音量最大
					v[0~16]:0朗读音量为静音，16朗读音量最大
					t[0~5]:0朗读语速最慢，5朗读语速最快
					播报的内容：123456
日		期：2021.3.10
***************************************************************/
void SYN_FrameInfo(u8 Music, u8 *HZdata)
{
  /****************需要发送的文本**********************************/
  unsigned  char  Frame_Info[50];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  				 //定义校验字节
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); //需要发送文本的长度
  /*****************帧固定配置信息**************************************/
  Frame_Info[0] = 0xFD ; 					//构造帧头FD
  Frame_Info[1] = 0x00 ; 					//构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 3; 	//构造数据区长度的低字节
  Frame_Info[3] = 0x01 ; 					//构造命令字：合成播放命令
  Frame_Info[4] = 0x01 | Music << 4 ; //构造命令参数：背景音乐设定
  /*******************校验码计算***************************************/
  for(i = 0; i < 5; i++){   				//依次发送构造好的5个帧头字节
    ecc = ecc ^ (Frame_Info[i]);	//对发送的字节进行异或校验
  }
  for(i = 0; i < HZ_Length; i++){  //依次发送待合成的文本数据
    ecc = ecc ^ (HZdata[i]); 			//对发送的字节进行异或校验
  }
  /*******************发送帧信息***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  USART1_SendString(Frame_Info, 5 + HZ_Length + 1);		//将构造好的文本发送通过串口发送到SYN6288中播报
}


/***********************************************************
* 名    称： YS_SYN_Set(u8 *Info_data)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
* 调用方法：通过调用已经定义的相关数组进行配置。
日期：2021.3.10
**********************************************************/
void YS_SYN_Set(u8 *Info_data)
{
  u8 Com_Len;
  Com_Len = strlen((char*)Info_data);
  USART1_SendString(Info_data, Com_Len);
}

/***********************************************************
* 名    称： void Voice_Broadcast(void)
* 功    能： 主函数	程序入口
* 入口参数： 无
* 出口参数：无
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
* 调用方法：通过调用已经定义的相关数组进行配置。
日期：2021.3.30
**********************************************************/
void Voice_Broadcast(void)
{
	u8 redbuf[3]; 			//红队分数
	u8 bluebuf[3];			//蓝队分数
	if((Key_flag.Stop_Start == 0) && (Key_flag.Voice_Broadcast == 1)){	//比赛暂停且语音播报标志位为1
		Key_flag.Voice_Broadcast = 0;										//清除语音播报标志位
		sprintf((char*)redbuf, "%3d",score.Red_Score);	//将整型数据转成字符串
		sprintf((char*)bluebuf,"%3d",score.Blue_Score);
		strcat((char *)Voice_Broadcast_buf, (char *)bluebuf);	//将蓝队的分数复制到播报数组中
		strcat((char *)Voice_Broadcast_buf, "比");
		strcat((char *)Voice_Broadcast_buf, (char *)redbuf);
		SYN_FrameInfo(0,Voice_Broadcast_buf);				//语音播报
		strcpy((char *)Voice_Broadcast_buf,"[v10][m1][t3]现在的比分是");		//重新对播报数组进行初始化
	}
}


