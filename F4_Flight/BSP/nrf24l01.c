#include "nrf24l01.h"

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x1F,0x2E,0x3D,0x4C,0x5B}; 	
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x1F,0x2E,0x3D,0x4C,0x5B};

void NRF24L01_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// CSN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// CE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// IRQ
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_4);

    SPI_Config_Init();
	NRF_CE_L; 			
	SPI_CSN_H;
}

u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
    u8 i;
    SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��        
    SPI_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.        
    SPI_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
    for(i=0;i<5;i++){
		if(buf[i]!=0XA5){
			break;
		}
	}
    if(i!=5){
		return 1;//���24L01����    
    }
	return 0;         //��⵽24L01
}	
//��Ĵ���д��ֵ
u8 SPI_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	SPI_CSN_L;        
  	status = SPI1_SwapByte(reg);
  	SPI1_SwapByte(value);  
  	SPI_CSN_H;   
    
  	return(status);       			
}

//��ȡ�Ĵ���ֵ
u8 SPI_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	SPI_CSN_L;  	
  	SPI1_SwapByte(reg);   
  	reg_val = SPI1_SwapByte(0XFF);	// 0xff �ղ���
  	SPI_CSN_H;   
    
  	return(reg_val);        
}	

//�����Ĵ���������len���ֽڳ��ȵ�ֵ
u8 SPI_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,i;	    
  	SPI_CSN_L;         	
  	status = SPI1_SwapByte(reg);	   
 	for(i=0;i<len;i++){
		pBuf[i]=SPI1_SwapByte(0XFF);
	}
        
  	SPI_CSN_H;
    
  	return status;        
}

//��Ĵ���д������len���ֽڵ�ֵ
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,i;
    
 	SPI_CSN_L;
  	status = SPI1_SwapByte(reg);
  	for(i=0; i<len; i++){
		SPI1_SwapByte(*pBuf++);
	}
  	SPI_CSN_H;   
    
  	return status;         
}

u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
	NRF_CE_L;   
	SPI_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
 	NRF_CE_H;		//��������
	while(NRF_IRQ != 0);		//�ȴ��������
	sta=SPI_Read_Reg(STATUS);		//��ȡ״̬�Ĵ�����ֵ	   
	SPI_Write_Reg(NRF_WRITE_REG+STATUS,sta);	//���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX){		//�ﵽ����ط�����	
		SPI_Write_Reg(FLUSH_TX,0xff);		//���TX FIFO�Ĵ��� 		
		return FLUSH_TX;
	}
	if(sta&TX_OK){		//�������
		return TX_OK;
	}
	return 0xff;		//����ԭ����ʧ��
}

u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
	sta = SPI_Read_Reg(STATUS); 	 					    //״̬��־λ
	SPI_Write_Reg(NRF_WRITE_REG+STATUS,sta); 		//����жϱ�־
	if(sta&RX_OK)											//���ճɹ�
	{
		SPI_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);		
		SPI_Write_Reg(FLUSH_RX,0xff);
		return 0; 
	}	   
	return 1;
}

void NRF24L01_RxMode(void)
{
	NRF_CE_L;
	
    SPI_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
      
    SPI_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
    SPI_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ       
    SPI_Write_Reg(NRF_WRITE_REG+RF_CH,40);         //����RFͨ��Ƶ��  
	// ��ƵRF���㹫ʽ��2400+RF_CH(Mhz) ����40��Ҳ����2440Mhz	
    SPI_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ��         
    SPI_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
	SPI_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ   
	NRF_CE_H; //CEΪ��,�������ģʽ 
}

void NRF24L01_TxMode(void)
{
	NRF_CE_L;
	SPI_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
    SPI_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK      
 
    SPI_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
    SPI_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
    SPI_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
    SPI_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
    SPI_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
    SPI_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�    
	NRF_CE_H;  //CEΪ��,10us����������
}

