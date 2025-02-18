#include "dma.h"

void SPI2_DMA_Init(uint32_t Madr,uint32_t DMA_MemoryInc,uint32_t DataSize)
{
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Stream4);
	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE){}//�ȴ� DMA ������ 
	/* ���� DMA */
		
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;	
	//DMA�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI2_DR_Addr;
	//DMA �洢����ַ, Ҫ���������
	DMA_InitStructure.DMA_Memory0BaseAddr  = Madr;
	//�洢��������ģʽ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//���ݴ����� 
	DMA_InitStructure.DMA_BufferSize =0;

	//���������ģʽ
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//�洢������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc;
	if(DataSize==DMA_PeripheralDataSize_Byte||DataSize==DMA_MemoryDataSize_Byte)
	{
		//�������ݳ���
		DMA_InitStructure.DMA_PeripheralDataSize = DataSize;
		//�洢�����ݳ���
		DMA_InitStructure.DMA_MemoryDataSize = DataSize;
	}
	else if(DataSize==DMA_PeripheralDataSize_HalfWord||DataSize==DMA_MemoryDataSize_HalfWord)
	{
		//�������ݳ���
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		//�洢�����ݳ���
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	}
	else 
	{
		//�������ݳ���
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
		//�洢�����ݳ���
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	}
	// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//�е����ȼ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//FIFOģʽ��ֹ          
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//FIFO��ֵ 
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //�洢��ͻ�����δ��� 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //����ͻ�����δ��� 
	//��ʼ��DMA 
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);
	
}

void SPI2_DMAEnable(uint16_t Len)
{
	DMA_Cmd(DMA1_Stream4, DISABLE);                      //�ر�DMA���� 
	DMA_SetCurrDataCounter(DMA1_Stream4,Len);          //���ݴ����� 
	SPI_Cmd(SPI2, ENABLE);//ʹ��SPI	
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);			//ʹ��spi��dma����
	DMA_Cmd(DMA1_Stream4, ENABLE);                      //����DMA���� 
}

