#include "control.h"
#include "optical.h"
#include "imu.h"
#include "remote.h"
#include "pid.h"
#include "pwm.h"
#include "led.h"
#include "usart1.h"
/* 
************************************************************************************************
		 y
									 M4				 M1				  +PITCH
		 |							  	 \		   /		 	 		 |
		 |								  \	  |	  /			 		     |
		 |									  |					 		 |
	-----|------ x						------|-------		  -ROLL ----------- +ROLL
		 |							  	  /	  |	   \			 		 |	
		 |							  	 /	  |		\					 |
		 |							 M3				 M2					 |
	mpu6050放置方向						四旋翼电机朝向			  	  -PITCH		向上倾斜角度正负号
	
***********************************************************************************************
*/							


u8 Att_Flag;
u8 Fly_Flag;
u8 Remote_Flag;
u8 Hover_Time;


int16_t M1, M2, M3, M4;

float target_Gx;
float target_Gy;
float target_Vecx;
float target_Vecy;
float target_Pitch;
float target_Roll;
float pwm_x;
float pwm_y;
int16_t motor = 0;

void Fly_Error(void)
{
	LED1_ON;
	M1-=3;
	M2-=3;
	M3-=3;
	M4-=3;
	Motor_PWM_Out(M1, M2, M3, M4);
	
}

void Flight_Check(void)
{
	if(Speed > 350){	//	大于起飞油门
		Fly_Flag = 1;
	}else{
		Fly_Flag = 0;
	}
	if(Pitch > -60 && Pitch < 60 && Roll > -60 && Roll < 60){	
		Att_Flag = 1;
	}else{
		Att_Flag = 0;
		Fly_Error();
	}
}


void Fly_Att_mode(u8 outer)
{
	// 角度环
	if(outer){
		target_Gx = Pid_Calculate(&pitch_pid, nrf_Pitch, Pitch);
		target_Gy = Pid_Calculate(&roll_pid, nrf_Roll, Roll);
	}
	// 角速度环
	pwm_x = Pid_Calculate(&gx_pid, target_Gx, gx);
	pwm_y = Pid_Calculate(&gy_pid, target_Gy, gy);

}

void Fly_Hover_Mode(u8 outer)
{
	// 位置环
	if(outer){
		target_Vecx = Pid_Calculate(&pos_pid, 0, x_offset);
		target_Vecy = Pid_Calculate(&pos_pid, 0, y_offset);
	}
	// 速度环
	target_Pitch = Pid_Calculate(&vec_pid, target_Vecx, x_speed);
	target_Roll = Pid_Calculate(&vec_pid, target_Vecy, y_speed);
	// 角度环
	if(outer){
		target_Gx = Pid_Calculate(&pitch_pid, target_Pitch, Pitch);
		target_Gy = Pid_Calculate(&roll_pid, target_Roll, Roll);
	}
	// 角速度环
	pwm_x = Pid_Calculate(&gx_pid, target_Gx, gx);
	pwm_y = Pid_Calculate(&gy_pid, target_Gy, gy);
}

void Control(u8 outer)
{
	Flight_Check();
	if(Att_Flag){
		if(Mode == 0){
			Fly_Att_mode(outer);	// 姿态控制模式
		}
		else if(Mode==1){
			Fly_Hover_Mode(outer);	// 悬停模式
		}
		M1 = (1000 + Speed) + pwm_x - pwm_y;
		M2 = (1000 + Speed) - pwm_x - pwm_y;
		M3 = (1000 + Speed) - pwm_x + pwm_y;
		M4 = (1000 + Speed) + pwm_x + pwm_y;
		Motor_PWM_Out(M1, M2, M3, M4);
	}

}



