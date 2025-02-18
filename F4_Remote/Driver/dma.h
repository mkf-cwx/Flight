#ifndef __DMA_H__
#define __DMA_H__

#include "system.h"

#define SPI2_DR_Addr					(uint32_t)(&(SPI2->DR))
void SPI2_DMA_Init(uint32_t Madr,uint32_t DMA_MemoryInc,uint32_t DataSize);
void SPI2_DMAEnable(uint16_t Len);

#endif	// __DMA_H__
