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
    // ��黺�����Ƿ�����
    if (rb->len >= rb->size){
        return 0;	// ����������
    }
	rb->buffer[rb->tail] = data;
	rb->tail = (rb->tail + 1) % rb->size;
	rb->len++;
	
	return 1;	// д��ɹ�
}

u8 RingBuffer_Read(RingBuffer *rb, u8 *data)
{
    // ��黺�����Ƿ�Ϊ��
    if (rb->len == 0){
        return 0; 	// ������Ϊ��
    }

    *data = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % rb->size;
	rb->len--;

    return 1; // ��ȡ�ɹ�
}

u16 RingBuffer_ReadMult_Byte(RingBuffer *rb, u8 *data, u16 len)
{
	if(rb->len < len){
		return 0;	// �������е����ݲ���
	}
	// ��ȡ����
	for (u16 i = 0; i < len; i++) {
		data[i] = rb->buffer[rb->head];
		rb->head = (rb->head + 1) % rb->size;
	}

	// ���»���������
	rb->len -= len;
	return len;		
}

