#include "remote.h"
#include "nrf24l01.h"
#include "adc.h"
#include "usart1.h"
#include "imu.h"

u8 nrf_Recev_buf[33];
u8 nrf_Send_buf[33];
u8 nrf_rx;
float nrf_Roll;
float nrf_Pitch;
float nrf_Yaw;
u16 Speed;
u16 Mode;

float battery;
u8 battery_level;


void Remote_Receive(void)
{
	NRF24L01_RxMode();
	nrf_rx = NRF24L01_RxPacket(nrf_Recev_buf);
	if(nrf_rx == 0)
	{
		
		nrf_Pitch = (float)nrf_Recev_buf[0] - 50;
		nrf_Roll = (float)nrf_Recev_buf[1] - 50;
		nrf_Yaw = (float)nrf_Recev_buf[2] - 50;
		Speed = ((u16)nrf_Recev_buf[3]<<8)|nrf_Recev_buf[4];
		
		Mode = nrf_Recev_buf[5];
			
	}
}

void Send_Remote(void)
{
	NRF24L01_TxMode();
	
	battery = (float)Get_ADC_Value(ADC_Channel_9);
	battery = battery/4096 * 3.3f * 126 /75;
	if(battery > 3.6f)
	{
		battery_level = (battery-3.6f)*100 / (4.2f - 3.6f);
	}else{
		battery_level = 0;
	}
	
	nrf_Send_buf[0] = battery_level;
	nrf_Send_buf[1] = (u8)Pitch;
	nrf_Send_buf[2] = (u8)Roll;
	nrf_Send_buf[3] = (u8)Yaw;
	
	delay_us(10);
	
	NRF24L01_TxPacket(nrf_Send_buf);     // ·¢ËÍÊý¾Ý	
}



