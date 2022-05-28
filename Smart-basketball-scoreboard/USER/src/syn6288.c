#include "syn6288.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "key.h"
#include "delay.h"
#include "timer.h"

u8 Voice_Broadcast_buf[34] = "[v10][m1][t3]���ڵıȷ���";		//����������������

/**************************************************************
�������ƣ�void SYN_FrameInfo(u8 Music, u8 *HZdata)
�������ܣ�����ת��������
��ڲ����� Music:ѡ�񱳾����֡� 0:�ޱ������֣�1~15��ѡ�񱳾�����
					 HZdata����Ҫ����������
���ڲ�������
˵    ����HZdata�ĸ�ʽ��"[v4][m5][t2]123456";
					m[0~16]:0��������Ϊ������16���������������
					v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
					t[0~5]:0�ʶ�����������5�ʶ��������
					���������ݣ�123456
��		�ڣ�2021.3.10
***************************************************************/
void SYN_FrameInfo(u8 Music, u8 *HZdata)
{
  /****************��Ҫ���͵��ı�**********************************/
  unsigned  char  Frame_Info[50];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  				 //����У���ֽ�
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); //��Ҫ�����ı��ĳ���
  /*****************֡�̶�������Ϣ**************************************/
  Frame_Info[0] = 0xFD ; 					//����֡ͷFD
  Frame_Info[1] = 0x00 ; 					//�������������ȵĸ��ֽ�
  Frame_Info[2] = HZ_Length + 3; 	//�������������ȵĵ��ֽ�
  Frame_Info[3] = 0x01 ; 					//���������֣��ϳɲ�������
  Frame_Info[4] = 0x01 | Music << 4 ; //����������������������趨
  /*******************У�������***************************************/
  for(i = 0; i < 5; i++){   				//���η��͹���õ�5��֡ͷ�ֽ�
    ecc = ecc ^ (Frame_Info[i]);	//�Է��͵��ֽڽ������У��
  }
  for(i = 0; i < HZ_Length; i++){  //���η��ʹ��ϳɵ��ı�����
    ecc = ecc ^ (HZdata[i]); 			//�Է��͵��ֽڽ������У��
  }
  /*******************����֡��Ϣ***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  USART1_SendString(Frame_Info, 5 + HZ_Length + 1);		//������õ��ı�����ͨ�����ڷ��͵�SYN6288�в���
}


/***********************************************************
* ��    �ƣ� YS_SYN_Set(u8 *Info_data)
* ��    �ܣ� ������	�������
* ��ڲ����� *Info_data:�̶���������Ϣ����
* ���ڲ�����
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��
* ���÷�����ͨ�������Ѿ�������������������á�
���ڣ�2021.3.10
**********************************************************/
void YS_SYN_Set(u8 *Info_data)
{
  u8 Com_Len;
  Com_Len = strlen((char*)Info_data);
  USART1_SendString(Info_data, Com_Len);
}

/***********************************************************
* ��    �ƣ� void Voice_Broadcast(void)
* ��    �ܣ� ������	�������
* ��ڲ����� ��
* ���ڲ�������
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��
* ���÷�����ͨ�������Ѿ�������������������á�
���ڣ�2021.3.30
**********************************************************/
void Voice_Broadcast(void)
{
	u8 redbuf[3]; 			//��ӷ���
	u8 bluebuf[3];			//���ӷ���
	if((Key_flag.Stop_Start == 0) && (Key_flag.Voice_Broadcast == 1)){	//������ͣ������������־λΪ1
		Key_flag.Voice_Broadcast = 0;										//�������������־λ
		sprintf((char*)redbuf, "%3d",score.Red_Score);	//����������ת���ַ���
		sprintf((char*)bluebuf,"%3d",score.Blue_Score);
		strcat((char *)Voice_Broadcast_buf, (char *)bluebuf);	//�����ӵķ������Ƶ�����������
		strcat((char *)Voice_Broadcast_buf, "��");
		strcat((char *)Voice_Broadcast_buf, (char *)redbuf);
		SYN_FrameInfo(0,Voice_Broadcast_buf);				//��������
		strcpy((char *)Voice_Broadcast_buf,"[v10][m1][t3]���ڵıȷ���");		//���¶Բ���������г�ʼ��
	}
}


