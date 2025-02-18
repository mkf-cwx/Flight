#include "optical.h"
#include "usart6.h"
#include "usart1.h"
#include "control.h"

#define PACKET_SIZE 14 // 数据包大小
u8 packet[PACKET_SIZE];	// 待检测数据包
OpticFlow_pack Rx_Pack;	// 检测完成数据结构体
OpticFlow_Data _opticflow_Data;	// 解析数据结构体
float x_speed = 0.0f;
float y_speed = 0.0f;
float x_offset = 0.0f;
float y_offset = 0.0f;


static enum
{
	waitForFirstByte,		// 等待第一个字节状态
	waitForSecondByte,		// 等待第二个字节状态
	waitForData,			// 等待数据接收状态
	waitForCheckXOR,		// 等待检查校验值状态
	waitForEndByte,			// 等待结束字节状态
}RxState = waitForFirstByte;	// 初始化等待第一个字节状态

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
                    checkXOR = 0; // 重置校验值
                }
                else
                {
                    RxState = waitForFirstByte;
                }
                break;

            case waitForData:
                Rx_Pack.data[dataIndex] = res;
                dataIndex++;
                checkXOR ^= res; // 计算校验值
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
                    return 1; // 数据包解析成功
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

    return 0; // 数据包未解析完成
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
	 // 解析数据
    _opticflow_Data.flow_x = packet->data[1] << 8 | packet->data[0];
    _opticflow_Data.flow_y = packet->data[3] << 8 | packet->data[2];
    _opticflow_Data.integration_timespan = packet->data[5] << 8 | packet->data[4];
    _opticflow_Data.height = packet->data[7] << 8 | packet->data[6];
    _opticflow_Data.valid = packet->data[8];
    _opticflow_Data.laser_belief = packet->data[9];

    // 计算时间(秒)
    _opticflow_Data.to_s = (float)_opticflow_Data.integration_timespan * 0.000001f;

    // 计算光流角度(弧度)
    _opticflow_Data.x_rad = (float)_opticflow_Data.flow_x / 10000.0f;
    _opticflow_Data.y_rad = (float)_opticflow_Data.flow_y / 10000.0f;

    // 计算速度
    x_speed = _opticflow_Data.x_rad * (float)_opticflow_Data.height / _opticflow_Data.to_s;
    y_speed = _opticflow_Data.y_rad * (float)_opticflow_Data.height / _opticflow_Data.to_s;

    // 更新偏移量
    x_offset += x_speed * _opticflow_Data.to_s;
    y_offset += y_speed * _opticflow_Data.to_s;

    // 限制偏移量范围
    x_offset = x_offset > 500 ? 0 : (x_offset < -500 ? 0 : x_offset);
    y_offset = y_offset > 500 ? 0 : (y_offset < -500 ? 0 : y_offset);
}                                  

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) // 检查更新中断标志
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志
		
		if(OpticFlow_Update())
		{
			if(_opticflow_Data.valid == 245){
	
			}		
		}
    }
}

