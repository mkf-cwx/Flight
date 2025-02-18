#include "ringbuffer.h"


void RingBuffer_Init(RingBuffer *rb, u8 *buffer, u16 size)
{
	rb->buffer = buffer;
	rb->size = size;
    rb->head = 0;
    rb->tail = 0;
	rb->len = 0;
}

u8 RingBuffer_Write(RingBuffer *rb, u8 data)
{
    // 检查缓冲区是否已满
    if (rb->len >= rb->size){
        return 0;	// 缓冲区已满
    }
	rb->buffer[rb->tail] = data;
	rb->tail = (rb->tail + 1) % rb->size;
	rb->len++;
	
	return 1;	// 写入成功
}

u8 RingBuffer_Read(RingBuffer *rb, u8 *data)
{
    // 检查缓冲区是否为空
    if (rb->len == 0){
        return 0; 	// 缓冲区为空
    }

    *data = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % rb->size;
	rb->len--;

    return 1; // 读取成功
}

u16 RingBuffer_ReadMult_Byte(RingBuffer *rb, u8 *data, u16 len)
{
	if(rb->len < len){
		return 0;	// 缓冲区中的数据不足
	}
	// 读取数据
	for (u16 i = 0; i < len; i++) {
		data[i] = rb->buffer[rb->head];
		rb->head = (rb->head + 1) % rb->size;
	}

	// 更新缓冲区长度
	rb->len -= len;
	return len;		
}

