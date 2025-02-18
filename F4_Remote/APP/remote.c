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
	ADC_Pitch = Get_ADC_Average(ADC_Channel_0, 10);	// ��ȡ Pitch - ͨ��0 ADC ֵ
	ADC_Roll = Get_ADC_Average(ADC_Channel_1, 10);	// ��ȡ Roll - ͨ��1 ADC ֵ
	ADC_Yaw = Get_ADC_Average(ADC_Channel_2, 10);	// ��ȡ Yaw - ͨ��2 ADC ֵ
	ADC_Speed = Get_ADC_Average(ADC_Channel_3, 10);	// ��ȡ Speed - ͨ��3 ADC ֵ
	
	
	// ����ӳ��Ϊ�ֶ�����ģʽ - ������ �� ����Ƕ�
	Pitch = (float)ADC_Pitch / 4096 * 100;		// ���������� ��Χ�� 0 - 100��, ��Ҫ�ڷɿض˽���: -50 ������50���� 
	Roll = (float)ADC_Roll / 4096 * 100;		// ���������� ��Χ�� 0 - 100��, ��Ҫ�ڷɿض˽���: -50 ������50���� 
	Yaw = (float)ADC_Yaw / 4096 * 100;			// ���������� ��Χ�� 0 - 100��, ��Ҫ�ڷɿض˽���: -50 ������50���� 
	Speed = (float)ADC_Speed / 4096 * 800;	// ���������� ��Χ�� 0 - 1000
	
	nrf_Send_buf[0] = Pitch;
	nrf_Send_buf[1] = Roll;
	nrf_Send_buf[2] = Yaw;
	nrf_Send_buf[3] = Speed>>8;
	nrf_Send_buf[4] = Speed&0xff;
	nrf_Send_buf[5] = Mode;
	
	NRF24L01_TxPacket(nrf_Send_buf);     // ��������	
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


