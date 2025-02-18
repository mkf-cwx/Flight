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
TaskHandle_t ControlTask_Handler;		//任务句柄

void Send_task(void * pvParameters);
TaskHandle_t SendTask_Handler;		//任务句柄


// 任务入口
void Flight_task(void)
{
	while(MPU6050_Init());	// MPU6050 初始化
	NRF24L01_Init();	// NRF24L01 初始化
	while(NRF24L01_Check()); 	// 检测NRF24L01是否正常
	NRF24L01_RxMode();	// NRF24L01 接收模式
	Led_Red_Twinkle(5);		// 红灯闪烁
	MPU6050_GyroOffset();	// 获取陀螺仪零偏
	MPU6050_AccOffset();	// 校准加速度计零偏
	PWM_Init();			// PWM 初始化
	LED2_ON;	// 打开蓝灯
	Pid_Param_Init();	// Pid 参数初始化
	USART6_Init();	// 光流计串口初始化
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	// 配置为中断优先级分组4
	TIM3_Init();	// 定时器初始化
	
	// 创建控制任务
	xTaskCreate(Control_task, "Control_task", 1024, NULL, 2, &ControlTask_Handler);
	
	// 创建发送数据任务	
	xTaskCreate(Send_task, "Send_task", 256, NULL, 1, &SendTask_Handler);	
	
	vTaskStartScheduler();          //开启任务调度
}

// 执行频率为 250 Hz
void Control_task(void * pvParameters)
{
	static u8  outer_counter= 1;		// 角度外环更新  125hz
	static u8 angle_update = 1;		// 角度更新		125hz
	while(1)
	{
		Remote_Receive();	// 更新遥控数据
		
		IMU_Data(angle_update);	// 更新imu数据

		Control(outer_counter);	// 控制逻辑
				
		outer_counter++;
		angle_update++;
		outer_counter = outer_counter > 1 ? 0 : outer_counter;
		angle_update = angle_update > 1 ? 0 : angle_update;

		vTaskDelay(4);		// 延时4ms
	}
}
							
void Send_task(void * pvParameters)
{
	while(1)
	{
		
		Send_Remote();
		vTaskDelay(2);		// 延时2ms
	}
}


