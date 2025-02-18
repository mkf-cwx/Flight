#include "dma.h"

void SPI2_DMA_Init(uint32_t Madr,uint32_t DMA_MemoryInc,uint32_t DataSize)
{
	DMA_InitTypeDef  DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Stream4);
	while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE){}//等待 DMA 可配置 
	/* 配置 DMA */
		
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;	
	//DMA外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI2_DR_Addr;
	//DMA 存储器地址, 要传输的数据
	DMA_InitStructure.DMA_Memory0BaseAddr  = Madr;
	//存储器到外设模式
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//数据传输量 
	DMA_InitStructure.DMA_BufferSize =0;

	//外设非增量模式
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//存储器增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc;
	if(DataSize==DMA_PeripheralDataSize_Byte||DataSize==DMA_MemoryDataSize_Byte)
	{
		//外设数据长度
		DMA_InitStructure.DMA_PeripheralDataSize = DataSize;
		//存储器数据长度
		DMA_InitStructure.DMA_MemoryDataSize = DataSize;
	}
	else if(DataSize==DMA_PeripheralDataSize_HalfWord||DataSize==DMA_MemoryDataSize_HalfWord)
	{
		//外设数据长度
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		//存储器数据长度
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	}
	else 
	{
		//外设数据长度
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
		//存储器数据长度
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	}
	// 使用普通模式 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//中等优先级
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//FIFO模式禁止          
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//FIFO阈值 
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //存储器突发单次传输 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输 
	//初始化DMA 
	DMA_Init(DMA1_Stream4, &DMA_InitStructure);
	
}

void SPI2_DMAEnable(uint16_t Len)
{
	DMA_Cmd(DMA1_Stream4, DISABLE);                      //关闭DMA传输 
	DMA_SetCurrDataCounter(DMA1_Stream4,Len);          //数据传输量 
	SPI_Cmd(SPI2, ENABLE);//使能SPI	
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);			//使能spi的dma功能
	DMA_Cmd(DMA1_Stream4, ENABLE);                      //开启DMA传输 
}

