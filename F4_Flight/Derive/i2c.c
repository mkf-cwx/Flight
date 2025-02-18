#include "i2c.h"

void I2C_GPIO_Init(void)
{
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStructure;
	// SCL
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// SDA
	GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	I2C_SCL_1;
	I2C_SDA_1;
}

/**
  * @brief SDA������ģʽ����
  * @param None
  * @retval None
  */
void SDA_Input_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
  * @brief SDA�����ģʽ����
  * @param None
  * @retval None
  */
void SDA_Output_Mode(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

static void I2C_Delay(void)
{
	uint8_t i;

	for (i=0; i<10; i++);
}

void I2C_Start()
{
	SDA_Output_Mode();
	I2C_SDA_1;
	I2C_SCL_1;
	I2C_Delay();
	I2C_SDA_0;
	I2C_Delay();
	I2C_SCL_0;
	I2C_Delay();
}

void I2C_Stop()
{
	SDA_Output_Mode();
	I2C_SCL_0;
	I2C_SDA_0;
	I2C_Delay();
	I2C_SCL_1;
	I2C_SDA_1;
	I2C_Delay();
	
}

void I2C_Ack(void)
{
	SDA_Output_Mode();
	I2C_SCL_0;
	I2C_SDA_0;	/*CPU����SDA=0*/
	I2C_Delay();
	I2C_SCL_1;	/*CPU����1��ʱ��*/
	I2C_Delay();
	I2C_SCL_0;
}

void I2C_NAck(void)
{
	SDA_Output_Mode();
	I2C_SCL_0;
	I2C_SDA_1;	/*CPU����SDA = 1*/
	I2C_Delay();
	I2C_SCL_1;	/*CPU����1��ʱ��*/
	I2C_Delay();
	I2C_SCL_0;
}

uint8_t I2C_WaitAck(void)
{
	uint8_t wait = 0;
	
	I2C_SDA_1;	/*CPU�ͷ�SDA����*/
	SDA_Input_Mode();
	I2C_Delay();
	I2C_SCL_1;	/*CPU����SCL=1,��ʱ�����᷵��ACKӦ��*/
	I2C_Delay();
	while(I2C_SDA_READ)	/*CPU��ȡSDA����״̬*/
	{
		wait++;
		if (wait > 250)
        {
            I2C_Stop();
			SDA_Output_Mode();
            return 0;
        }
	}
	I2C_SCL_0;
	SDA_Output_Mode();
	return 1;
}

void I2C_SendByte(uint8_t byte)
{
	uint8_t i;
	SDA_Output_Mode();
	/* �ȷ����ֽڵĸ�λbit7 */
	for(i = 0; i < 8; i++)
	{
		I2C_SCL_0;
		if(byte & 0x80)
		{
			I2C_SDA_1;
		}else{
			I2C_SDA_0;
		}
		byte <<= 1;	/*����һ��bit*/
		I2C_Delay();
		I2C_SCL_1;
		I2C_Delay();	

	}
	I2C_SCL_0;	
	I2C_Delay();
}

uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	SDA_Input_Mode();
	/*������1��bitΪ���ݵ�bit7*/
	value = 0;
	for (i = 0; i < 8; i++)
	{
		I2C_SCL_0;
		I2C_Delay();
		I2C_SCL_1;
		value <<= 1;
		if (I2C_SDA_READ)
		{
			value |= 0x01;
		}
		I2C_Delay();
	}
	I2C_SCL_0;
	SDA_Output_Mode();
	return value;
}
