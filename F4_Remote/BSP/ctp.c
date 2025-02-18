#include "ctp.h"
#include "i2c.h"
#include "lcd.h"
#include "touch.h"
#include "systick.h"

/********************************************
* @brief: FT6336_Init FT6336���ݴ���оƬ���ų�ʼ��
* @param:	
* @return:
 ********************************************
*/
void FT6336_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); //ʹ��SYSCFGʱ��
	// RST
	GPIO_InitStructure.GPIO_Pin = FT_RST_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(FT_RST_PORT, &GPIO_InitStructure);
	
	// INT
	GPIO_InitStructure.GPIO_Pin = FT_INT_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(FT_INT_PORT, &GPIO_InitStructure);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);	// ѡ��GPIO�ܽ������ⲿ�ж���·
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;// EXTI10�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;// ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		// �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	// ����ָ���Ĳ�����ʼ��VIC�Ĵ���	
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line10;// ѡ��EXTIͨ��
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;// �����жϷ���ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;// �����жϲ�����ʽ
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;// ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_ClearITPendingBit(EXTI_Line10);
	
	I2C_GPIO_Init();
	
	FT_RST_L;				//��λ
	delay_ms(20);
 	FT_RST_H;				//�ͷŸ�λ		    
	delay_ms(300);  	
	
}

void FT6336_WR_Reg(u16 addr,u8 data)
{
	I2C_Start();
	I2C_SendByte(FT_CMD_WR);
	I2C_WaitAck();
	I2C_SendByte(addr);
	I2C_WaitAck();
	I2C_SendByte(data);
	I2C_WaitAck();
	I2C_Stop();
}

void FT6336_RD_Reg(u16 addr,u8 *buf, u8 len)
{
	u8 i;
	I2C_Start();
	I2C_SendByte(FT_CMD_WR);
	I2C_WaitAck();
	I2C_SendByte(addr);
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(FT_CMD_RD);
	I2C_WaitAck();
	for(i=0;i<len;i++){
		buf[i]=I2C_ReadByte(i==(len-1)?0:1);
	}
	I2C_Stop();
}

/********************************************
* @brief: FT6336_GetCoordinates ��ȡ�������꺯��
* @param:	u16 *x x������ָ��
			u16 *y y������ָ��	
* @return:	0:��ȡʧ��		1:��ȡ�ɹ�
 ********************************************
 */
u8 FT6336_GetCoordinates(u16 *x, u16 *y)
{
	u8 buf[4];
	u8 state;
	u16 temp_x=0, temp_y=0;
	*x = 0;
	*y = 0;
	
	FT6336_RD_Reg(FT_REG_NUM_FINGER,&state,1);
	FT6336_RD_Reg(FT_TP1_REG, buf, 4);
	// ֻ����һ�������㣬��ָ������Ч

	if(state == 1){
		if(lcd.scan_dir == VERTICAL){
			temp_x = (u16)((buf[0]&0x0f)<<8)|buf[1];
			temp_y = (u16)((buf[2]&0x0f)<<8)|buf[3];
		}else{
			temp_y = (u16)((buf[0]&0x0f)<<8)|buf[1];
			temp_x = (u16)((buf[2]&0x0f)<<8)|buf[3];
		}

//		printf("Sta:%d,X:%d,Y:%d\t\n",state,temp_x,temp_y);
		// ֻ�д��� Contact �¼�����Ч
		if((buf[0]&0xf0) != 0x80){
			temp_x = 0;
			temp_y = 0;
		}
	}else{
		return 0;	// ��ȡʧ��
	}
	
	// ��������Ƿ�����Ļ��Χ��
	if(temp_x<lcd.width && temp_y<lcd.height){
		*x = temp_x;
		*y = temp_y;
	}
	return 1;	 // ��ȡ�ɹ�
}

void EXTI15_10_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line10)==1)
	{
		
		Touch_ISR();
	}
	EXTI_ClearITPendingBit(EXTI_Line10);
}
