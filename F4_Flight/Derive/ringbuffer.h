#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include "system.h"

/*���λ��������ݽṹ*/
typedef struct
{
    u8 *buffer; // ������
	u16 size;
    volatile u16 head;           // дָ��
    volatile u16 tail;           // ��ָ��
	volatile u16 len;
} RingBuffer;

void RingBuffer_Init(RingBuffer *rb, u8 *buffer, u16 size);
u8 RingBuffer_Write(RingBuffer *rb, u8 data);
u8 RingBuffer_Read(RingBuffer *rb, u8 *data);
u16 RingBuffer_ReadMult_Byte(RingBuffer *rb, u8 *data, u16 len);


#endif	//__RINGBUFFER_H__
