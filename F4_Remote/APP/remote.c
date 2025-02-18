#include "remote.h"
#include "nrf24l01.h"
#include "adc.h"

u8 nrf_Send_buf[33];
u8 nrf_Recev_buf[33];
u8 nrf_rx;

u16 ADC_Speed = 0;
u16 ADC_Pitch = 0;
u16 ADC_Roll = 0;
u16 ADC_Yaw = 0;

u16 Speed = 0;
u8 Pitch = 0;
u8 Roll = 0;
u8 Yaw = 0;
u8 Mode = 0;

u8 battery_level;
u8 f_Pitch;
u8 f_Roll;
u8 f_Yaw;

void Remote_SendData(void)
{
	NRF24L01_TxMode();
	ADC_Pitch = Get_ADC_Average(ADC_Channel_0, 10);	// 获取 Pitch - 通道0 ADC 值
	ADC_Roll = Get_ADC_Average(ADC_Channel_1, 10);	// 获取 Roll - 通道1 ADC 值
	ADC_Yaw = Get_ADC_Average(ADC_Channel_2, 10);	// 获取 Yaw - 通道2 ADC 值
	ADC_Speed = Get_ADC_Average(ADC_Channel_3, 10);	// 获取 Speed - 通道3 ADC 值
	
	
	// 数据映射为手动控制模式 - 油门量 和 三轴角度
	Pitch = (float)ADC_Pitch / 4096 * 100;		// 待发送数据 范围在 0 - 100度, 需要在飞控端解析: -50 在正负50度内 
	Roll = (float)ADC_Roll / 4096 * 100;		// 待发送数据 范围在 0 - 100度, 需要在飞控端解析: -50 在正负50度内 
	Yaw = (float)ADC_Yaw / 4096 * 100;			// 待发送数据 范围在 0 - 100度, 需要在飞控端解析: -50 在正负50度内 
	Speed = (float)ADC_Speed / 4096 * 800;	// 待发送数据 范围在 0 - 1000
	
	nrf_Send_buf[0] = Pitch;
	nrf_Send_buf[1] = Roll;
	nrf_Send_buf[2] = Yaw;
	nrf_Send_buf[3] = Speed>>8;
	nrf_Send_buf[4] = Speed&0xff;
	nrf_Send_buf[5] = Mode;
	
	NRF24L01_TxPacket(nrf_Send_buf);     // 发送数据	
}

void Remote_ReceiveData(void)
{
	NRF24L01_RxMode();
	
	nrf_rx = NRF24L01_RxPacket(nrf_Recev_buf);
	if(nrf_rx == 0)
	{
		battery_level = nrf_Recev_buf[0];
		f_Pitch = nrf_Recev_buf[1];
		f_Roll = nrf_Recev_buf[2];
		f_Yaw = nrf_Recev_buf[3];
	}
	
}


