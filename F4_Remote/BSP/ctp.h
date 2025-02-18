#ifndef __CTP_H__
#define __CTP_H__

#include "system.h"

//����ݴ��������ӵ�оƬ����(δ����I2C����) 
//IO��������	 

// FT6336��λ����
#define FT_RST_PORT   			GPIOA
#define FT_RST_PIN				GPIO_Pin_9

#define FT_RST_H   				GPIO_SetBits(FT_RST_PORT, FT_RST_PIN);	
#define FT_RST_L				GPIO_ResetBits(FT_RST_PORT, FT_RST_PIN);

// FT6336�ж�����
#define FT_INT_PORT   			GPIOA
#define FT_INT_PIN				GPIO_Pin_10


//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������
  
//FT6336 ���ּĴ������� 
#define FT_REG_NUM_FINGER       0x02		//����״̬�Ĵ���	 Number of touch points[3:0] 1-5 is valid

/*  X_H
	7:6 - Event Flag:  
	00b: Put Down
	01b: Put Up
	10b: Contact
	11b: Reserved
	
	5:4 - Reserved 
	
	3:0 - Touch X Position 	MSB of Touch X Position in pixels 
	[11:8] 
*/

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		//���������������ݵ�ַ
#define FT_TP4_REG 				0X15		//���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		//��������������ݵ�ַ  

void FT6336_Init(void);

u8 FT6336_GetCoordinates(u16 *x, u16 *y);

#endif	// __CTP_H__

