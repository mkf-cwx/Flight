#ifndef __OPTICAL_H__
#define __OPTICAL_H__

#include "system.h"

/*������ݽ��պ궨��*/
#define BEGIN_BYTE			0xFE	//��ʼ״̬��
#define DATA_BYTE			0x0A	//���ݰ��ֽ���
#define END_BYTE			0x55	//����״̬��
#define LIGHT_ERR			0x00	//�������ݲ�����
#define LIGHT_RIGHT			0xF5	//�������ݿ���

/*ͨѶ���ݽṹ*/
typedef struct
{
    uint8_t startByte1;
    uint8_t startByte2;
	uint16_t data[10];
    uint8_t checkXOR;
	uint8_t endByte;
} OpticFlow_pack;

/*�������ݽṹ��*/
typedef struct
{
	int16_t flow_x;			//X ���ص��ۼ�ʱ���ڵ��ۼ�λ��
	int16_t flow_y;			//Y ���ص��ۼ�ʱ���ڵ��ۼ�λ��
	uint16_t height;			//���������(mm)��
	uint16_t integration_timespan;		//��һ�η��͹������ݵ����η��͹������ݵ��ۼ�ʱ��us
	uint8_t valid;			//״ֵ̬��0(0x00)Ϊ�������ݲ����ã�245(0xF5)Ϊ�������ݿ���
	uint8_t laser_belief;		//���������Ŷȣ����� 0x64 ��ʾ���������Ŷ�Ϊ100%
	float to_s;			// ���ݰ�ʱ���� (��)
	float x_rad;		// x_radΪ����x����ٶ� ��λΪ rad/s
	float y_rad;		// y_radΪ����y����ٶ� ��λΪ rad/s
}OpticFlow_Data;



u8 OpticFlow_Process(u8 res);
void unpack(OpticFlow_pack *packet);
u8 OpticFlow_Update(void);

extern OpticFlow_Data _opticflow_Data;
extern float x_speed;
extern float y_speed;
extern float x_offset;
extern float y_offset;
#endif	//__OPTICAL_H__
