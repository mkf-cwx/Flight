#include "spi.h"


void NRF_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// ʹ�� GPIOA ʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;// ���ù��� 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// ������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;// 100MHz 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// ���� 
	GPIO_Init(GPIOA, &GPIO_InitStructure);// ��ʼ�� 
	// �������Ÿ���ӳ�� 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1); // PB5����Ϊ SPI1 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1); // PB6����Ϊ SPI1 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1); // PB7����Ϊ SPI1
}

void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);// ʹ�� GPIOB ʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;// ���ù��� 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;// ������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;// 100MHz 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// ���� 
	GPIO_Init(GPIOB, &GPIO_InitStructure);// ��ʼ�� 
	// �������Ÿ���ӳ�� 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); // PB13����Ϊ SPI2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); // PB15����Ϊ SPI2
}

void SPI1_Config_Init(void)
{
	NRF_GPIO_Init();
	SPI_Cmd(SPI1, DISABLE); //ʧ��SPI1
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);// ʹ�� SPI1 ʱ�� 
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  // ����SPIȫ˫�� 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  // ����SPI����ģʽ:��SPI 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // ����SPI�����ݴ�С: 8λ֡�ṹ 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// ����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; // ���ݲ����ڵ�һ��ʱ���� 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   // NSS�ź���������� 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // Ԥ��Ƶ256 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // ���ݴ����MSBλ��ʼ 
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRCֵ����Ķ���ʽ 
	SPI_Init(SPI1, &SPI_InitStructure); // ����ָ���Ĳ�����ʼ������SPIx�Ĵ��� 
	  
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI1  
}

void SPI2_Config_Init(void)
{
	LCD_GPIO_Init();
	SPI_Cmd(SPI2, DISABLE); //ʧ��SPI2
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);// ʹ�� SPI2 ʱ�� 
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  // ����SPIֻ���� 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  // ����SPI����ģʽ:��SPI 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // ����SPI�����ݴ�С: 8λ֡�ṹ 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;// ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; // ���ݲ����ڵڶ���ʱ���� 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   // NSS�ź���������� 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // Ԥ��Ƶ2 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; // ���ݴ����MSBλ��ʼ 
	SPI_InitStructure.SPI_CRCPolynomial = 7; // CRCֵ����Ķ���ʽ 
	SPI_Init(SPI2, &SPI_InitStructure); // ����ָ���Ĳ�����ʼ������SPIx�Ĵ��� 
	  
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI2
}


// ����ֵ:��ȡ�����ֽ� 
u8 SPI1_SwapByte(u8 TxData) 
{      
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������   
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ���� 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ������� 
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ����� 
} 

// ����SPI1�ٶ�
//---------------------------------------------------
// SPI_DivideFrequency_2    2��Ƶ�� 50MHz
// SPI_DivideFrequency_4    4��Ƶ�� 25MHz
// SPI_DivideFrequency_8    8��Ƶ�� 12.5MHz
// SPI_DivideFrequency_16    16��Ƶ��6.25MHz
// SPI_DivideFrequency_32    32��Ƶ��3.125MHz

void SPI1_SetSpeed(u8 SPI_DivideFrequency)
{
    SPI1->CR1&=0XFFC7;
    SPI1->CR1|=SPI_DivideFrequency;    //����SPI1�ٶ�  
}

void SPI_Delay(int t)
{
	while(t--);
}


void SPI_SendByte(u8 TxData) 
{      
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������   
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ��byte  ���� 
	SPI_Delay(20);
} 

