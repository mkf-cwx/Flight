#include "rc.h"
#include "adc.h"
#include "nrf24l01.h"
#include "led.h"
#include "key.h"
#include "screen.h"
#include "touch.h"
#include "remote.h"
#include "FreeRTOS.h"
#include "task.h"

void Remote_Controller(void * pvParameters);
TaskHandle_t ControllerTask_Handler;		

void Receive_task(void * pvParameters);
TaskHandle_t RecviceTask_Handler;		


void RC_Task_Start(void)
{
	NRF24L01_Init();	// NRF24L01 初始化
	while(NRF24L01_Check()); 	// 检测NRF24L01是否正常
	NRF24L01_TxMode();	// NRF24L01 发送模式
	Screen_Init();
	Touch_Init();
	Key_Init();
	//LED1_ON;
	xTaskCreate(Remote_Controller, "Remote_Controller" ,1024, NULL, 2, &ControllerTask_Handler);
	// 创建接收遥控信号任务	
	xTaskCreate(Receive_task, "Receive_task", 256, NULL, 1, &RecviceTask_Handler);
	
	vTaskStartScheduler();          //开启任务调度
}

void Remote_Controller(void * pvParameters)
{
	while(1)
	{
		Remote_SendData();
		Screen_Show();
		vTaskDelay(10);
	}
}


void Receive_task(void * pvParameters)
{
	
	while(1)
	{

		//Remote_ReceiveData();
		vTaskDelay(100);
		
	}
}

