#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include "system.h"

/*环形缓冲区数据结构*/
typedef struct
{
    u8 *buffer; // 缓冲区
	u16 size;
    volatile u16 head;           // 写指针
    volatile u16 tail;           // 读指针
	volatile u16 len;
} RingBuffer;

void RingBuffer_Init(RingBuffer *rb, u8 *buffer, u16 size);
u8 RingBuffer_Write(RingBuffer *rb, u8 data);
u8 RingBuffer_Read(RingBuffer *rb, u8 *data);
u16 RingBuffer_ReadMult_Byte(RingBuffer *rb, u8 *data, u16 len);


#endif	//__RINGBUFFER_H__
