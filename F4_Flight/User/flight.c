#include "flight.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart1.h"
#include "led.h"
#include "imu.h"
#include "optical.h"
#include "adc.h"
#include "remote.h"
#include "control.h"
#include "pid.h"
#include "usart6.h"
#include "timer.h"
#include "mpu6050.h"
#include "nrf24l01.h"
#include "pwm.h"

void Control_task(void * pvParameters);
TaskHandle_t ControlTask_Handler;		//������

void Send_task(void * pvParameters);
TaskHandle_t SendTask_Handler;		//������


// �������
void Flight_task(void)
{
	while(MPU6050_Init());	// MPU6050 ��ʼ��
	NRF24L01_Init();	// NRF24L01 ��ʼ��
	while(NRF24L01_Check()); 	// ���NRF24L01�Ƿ�����
	NRF24L01_RxMode();	// NRF24L01 ����ģʽ
	Led_Red_Twinkle(5);		// �����˸
	MPU6050_GyroOffset();	// ��ȡ��������ƫ
	MPU6050_AccOffset();	// У׼���ٶȼ���ƫ
	PWM_Init();			// PWM ��ʼ��
	LED2_ON;	// ������
	Pid_Param_Init();	// Pid ������ʼ��
	USART6_Init();	// �����ƴ��ڳ�ʼ��
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	// ����Ϊ�ж����ȼ�����4
	TIM3_Init();	// ��ʱ����ʼ��
	
	// ������������
	xTaskCreate(Control_task, "Control_task", 1024, NULL, 2, &ControlTask_Handler);
	
	// ����������������	
	xTaskCreate(Send_task, "Send_task", 256, NULL, 1, &SendTask_Handler);	
	
	vTaskStartScheduler();          //�����������
}

// ִ��Ƶ��Ϊ 250 Hz
void Control_task(void * pvParameters)
{
	static u8  outer_counter= 1;		// �Ƕ��⻷����  125hz
	static u8 angle_update = 1;		// �Ƕȸ���		125hz
	while(1)
	{
		Remote_Receive();	// ����ң������
		
		IMU_Data(angle_update);	// ����imu����

		Control(outer_counter);	// �����߼�
				
		outer_counter++;
		angle_update++;
		outer_counter = outer_counter > 1 ? 0 : outer_counter;
		angle_update = angle_update > 1 ? 0 : angle_update;

		vTaskDelay(4);		// ��ʱ4ms
	}
}
							
void Send_task(void * pvParameters)
{
	while(1)
	{
		
		Send_Remote();
		vTaskDelay(2);		// ��ʱ2ms
	}
}


