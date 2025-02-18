#include "optical.h"
#include "usart6.h"
#include "usart1.h"
#include "control.h"

#define PACKET_SIZE 14 // ���ݰ���С
u8 packet[PACKET_SIZE];	// ��������ݰ�
OpticFlow_pack Rx_Pack;	// ���������ݽṹ��
OpticFlow_Data _opticflow_Data;	// �������ݽṹ��
float x_speed = 0.0f;
float y_speed = 0.0f;
float x_offset = 0.0f;
float y_offset = 0.0f;


static enum
{
	waitForFirstByte,		// �ȴ���һ���ֽ�״̬
	waitForSecondByte,		// �ȴ��ڶ����ֽ�״̬
	waitForData,			// �ȴ����ݽ���״̬
	waitForCheckXOR,		// �ȴ����У��ֵ״̬
	waitForEndByte,			// �ȴ������ֽ�״̬
}RxState = waitForFirstByte;	// ��ʼ���ȴ���һ���ֽ�״̬

u8 OpticFlow_Process_Batch(const u8 *data, u16 length)
{
    static u8 checkXOR = 0;
    static u8 dataIndex = 0;

    for (u16 i = 0; i < length; i++)
    {
        u8 res = data[i];

        switch (RxState)
        {
            case waitForFirstByte:
                if (res == BEGIN_BYTE)
                {
                    RxState = waitForSecondByte;
                    Rx_Pack.startByte1 = res;
                }
                break;

            case waitForSecondByte:
                if (res == DATA_BYTE)
                {
                    RxState = waitForData;
                    Rx_Pack.startByte2 = res;
                    dataIndex = 0;
                    checkXOR = 0; // ����У��ֵ
                }
                else
                {
                    RxState = waitForFirstByte;
                }
                break;

            case waitForData:
                Rx_Pack.data[dataIndex] = res;
                dataIndex++;
                checkXOR ^= res; // ����У��ֵ
                if (dataIndex == 10)
                {
                    RxState = waitForCheckXOR;
                }
                break;

            case waitForCheckXOR:
                if (checkXOR == res)
                {
                    Rx_Pack.checkXOR = res;
                    RxState = waitForEndByte;
                }
                else
                {
                    RxState = waitForFirstByte;
                }
                break;

            case waitForEndByte:
                if (res == END_BYTE)
                {
                    Rx_Pack.endByte = res;
                    RxState = waitForFirstByte;
                    checkXOR = 0;
                    return 1; // ���ݰ������ɹ�
                }
                else
                {
                    RxState = waitForFirstByte;
                }
                break;

            default:
                RxState = waitForFirstByte;
                break;
        }
    }

    return 0; // ���ݰ�δ�������
}

u8 OpticFlow_Update(void)
{
	if(RingBuffer_ReadMult_Byte(&USART6_RingBuffer, packet, PACKET_SIZE)){
		if(OpticFlow_Process_Batch(packet, PACKET_SIZE)){
			unpack(&Rx_Pack);
			return 1;
		}
		return 0;
	}else{
		return 0;
	}
	
}

void unpack(OpticFlow_pack *packet)
{
	 // ��������
    _opticflow_Data.flow_x = packet->data[1] << 8 | packet->data[0];
    _opticflow_Data.flow_y = packet->data[3] << 8 | packet->data[2];
    _opticflow_Data.integration_timespan = packet->data[5] << 8 | packet->data[4];
    _opticflow_Data.height = packet->data[7] << 8 | packet->data[6];
    _opticflow_Data.valid = packet->data[8];
    _opticflow_Data.laser_belief = packet->data[9];

    // ����ʱ��(��)
    _opticflow_Data.to_s = (float)_opticflow_Data.integration_timespan * 0.000001f;

    // ��������Ƕ�(����)
    _opticflow_Data.x_rad = (float)_opticflow_Data.flow_x / 10000.0f;
    _opticflow_Data.y_rad = (float)_opticflow_Data.flow_y / 10000.0f;

    // �����ٶ�
    x_speed = _opticflow_Data.x_rad * (float)_opticflow_Data.height / _opticflow_Data.to_s;
    y_speed = _opticflow_Data.y_rad * (float)_opticflow_Data.height / _opticflow_Data.to_s;

    // ����ƫ����
    x_offset += x_speed * _opticflow_Data.to_s;
    y_offset += y_speed * _opticflow_Data.to_s;

    // ����ƫ������Χ
    x_offset = x_offset > 500 ? 0 : (x_offset < -500 ? 0 : x_offset);
    y_offset = y_offset > 500 ? 0 : (y_offset < -500 ? 0 : y_offset);
}                                  

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // �������жϱ�־
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // ����жϱ�־
		
		if(OpticFlow_Update())
		{
			if(_opticflow_Data.valid == 245){
	
			}		
		}
    }
}

