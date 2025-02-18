#include "task_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart1.h"
#include "led.h"
#include "posture.h"
#include "optical.h"
#include "adc.h"
#include "remote.h"
#include "control.h"
#include "pid.h"

void Control_task(void * pvParameters);
TaskHandle_t ControlTask_Handler;		//任务句柄

void LightFlow_task(void * pvParameters);
TaskHandle_t LightFlowTask_Handler;		//任务句柄

void Send_task(void * pvParameters);
TaskHandle_t SendTask_Handler;		//任务句柄

float battery;
float battery_level;

// 任务入口
void Flight_task(void)
{
	LED2_ON;
	Pid_Param_Init();
	// 创建控制函数
	xTaskCreate((TaskFunction_t ) Control_task,
			(char*			) "Control_task",
			(uint16_t		) 256,
			(void * 		) NULL,
			(UBaseType_t	) 3,
			(TaskHandle_t*	) &ControlTask_Handler);
			
	// 创建光流任务	
	xTaskCreate((TaskFunction_t ) LightFlow_task,
			(char*			) "LightFlow_task",
			(uint16_t		) 256,
			(void * 		) NULL,
			(UBaseType_t	) 2,
			(TaskHandle_t*	) &LightFlowTask_Handler);
			
	// 创建发送数据任务	
	xTaskCreate((TaskFunction_t ) Send_task,
			(char*			) "Send_task",
			(uint16_t		) 256,
			(void * 		) NULL,
			(UBaseType_t	) 1,
			(TaskHandle_t*	) &SendTask_Handler);	
	vTaskStartScheduler();          //开启任务调度
}

// 执行频率为 250 Hz
void Control_task(void * pvParameters)
{
	static uint8_t outerEN = 1;
	static uint8_t angle_update = 1;
	while(1)
	{
		Remote_Receive();
		
		IMU_Data(angle_update);
		
//		if(OpticFlow_Update())
//		{
//			printf("height: %d\r\n", _opticflow_Data.height);
//			if(_opticflow_Data.valid == 245){
//				printf("x_speed: %f\r\n", x_speed);
//				printf("y_speed: %f\r\n", y_speed);
//				printf("x_offset: %f\r\n", x_offset);
//				printf("y_offset: %f\r\n", y_offset);
////				printf("e_x: %f\r\n", e_vec_x);
////				printf("e_y: %f\r\n", e_vec_y);
////				printf("e_p: %f\r\n", e_angle_p);
////				printf("e_r: %f\r\n", e_angle_r);
//			}
			
//		}		
		Control(outerEN);
		
		angle_update++;
		outerEN++;
		if(angle_update>1){
			angle_update=0;
		}
		if(outerEN > 1){
			outerEN = 0;
		}	
		vTaskDelay(1);
	}
}

void LightFlow_task(void * pvParameters)
{
	
	while(1)
	{


		//vTaskDelay(2);
	}
}


void Send_task(void * pvParameters)
{
	
	while(1)
	{	
//		battery = (float)Get_ADC_Value(ADC_Channel_9);
//		battery = battery/4096 * 3.3f * 126 /75;
//		printf("battary: %f\r\n", battery);
//		if(battery > 3.6f)
//		{
//			battery_level = (battery-3.6)*100 / (4.2f - 3.6f);
//		}else{
//			battery_level = 0;
//		}
//		printf("battary: %f\r\n", battery_level);
	}
}


