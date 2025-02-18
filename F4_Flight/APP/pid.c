#include "pid.h"
#include "remote.h"

pid_param pitch_pid;
pid_param roll_pid;
pid_param gx_pid;
pid_param gy_pid;
pid_param pos_pid;
pid_param vec_pid;

void Pid_Param_Init(void)
{
	/*Pitch PID 结构体初始化*/
	pitch_pid.KP = 5;	// 比例系数
	pitch_pid.KI = 0;		// 积分系数
	pitch_pid.KD = 2;		// 微分系数
	pitch_pid.last_error = 0;	// 上次误差
	pitch_pid.last_Dout = 0;   	// 上次微分值
	pitch_pid.D_alpha = 0.4f;	// 不完全微分系数	0 - 1
	pitch_pid.integral = 0;	// 积分值
	pitch_pid.output = 0;	// 输出值
	pitch_pid.Ilimit = 30;	// 积分范围
	pitch_pid.I_Range = 500;	// 积分限幅度
	pitch_pid.Ilimit_flag = 0;		// 积分分离标志
	
	/*Roll PID 结构体初始化*/
	roll_pid.KP = 5;	// 比例系数
	roll_pid.KI = 0;		// 积分系数
	roll_pid.KD = 2;		// 微分系数
	roll_pid.last_error = 0;	// 上次误差
	roll_pid.last_Dout = 0;   	// 上次微分值
	roll_pid.D_alpha = 0.4f;	// 不完全微分系数	0 - 1
	roll_pid.integral = 0;	// 积分值
	roll_pid.output = 0;	// 输出值
	roll_pid.Ilimit = 30;	// 积分范围
	roll_pid.I_Range = 500;	// 积分限幅度
	roll_pid.Ilimit_flag = 0;		// 积分分离标志
	
	/*Gx PID 结构体初始化*/
	gx_pid.KP = 0.73f;	// 比例系数
	gx_pid.KI = 0.012f;		// 积分系数
	gx_pid.KD = 8;		// 微分系数
	gx_pid.last_error = 0;	// 上次误差
	gy_pid.last_Dout = 0;   	// 上次微分值
	gy_pid.D_alpha = 0.4f;	// 不完全微分系数	0 - 1
	gx_pid.integral = 0;	// 积分值
	gx_pid.output = 0;	// 输出值
	gx_pid.Ilimit = 300;	// 积分范围
	gx_pid.I_Range = 500;	// 积分限幅度
	gx_pid.Ilimit_flag = 0;		// 积分分离标志
	
	/*Gy PID 结构体初始化*/
	gy_pid.KP = 0.73f;	// 比例系数
	gy_pid.KI = 0.012f;		// 积分系数
	gy_pid.KD = 8;		// 微分系数
	gy_pid.last_error = 0;	// 上次误差
	gy_pid.last_Dout = 0;   	// 上次微分值
	gy_pid.D_alpha = 0.4f;	// 不完全微分系数	0 - 1
	gy_pid.integral = 0;	// 积分值
	gy_pid.output = 0;	// 输出值
	gy_pid.Ilimit = 300;	// 积分范围
	gy_pid.I_Range = 500;	// 积分限幅度
	gy_pid.Ilimit_flag = 0;		// 积分分离标志
	
	/*Pos PID 结构体初始化*/	
	pos_pid.KP = 0.025f;	// 比例系数
	pos_pid.KI = 0;		// 积分系数
	pos_pid.KD = 0;		// 微分系数
	pos_pid.last_error = 0;	// 上次误差
	pos_pid.last_Dout = 0;   	// 上次微分值
	pos_pid.D_alpha = 0.4f;	// 不完全微分系数	0 - 1
	pos_pid.integral = 0;	// 积分值
	pos_pid.output = 0;	// 输出值
	pos_pid.Ilimit = 300;	// 积分范围
	pos_pid.I_Range = 2.5;	// 积分限幅度
	pos_pid.Ilimit_flag = 0;		// 积分分离标志
	
	/*Vec PID 结构体初始化*/
	vec_pid.KP = 0.028f;	// 比例系数
	vec_pid.KI = 0;		// 积分系数
	vec_pid.KD = 0;		// 微分系数
	vec_pid.last_error = 0;	// 上次误差
	vec_pid.last_Dout = 0;   	// 上次微分值
	vec_pid.D_alpha = 0.4f;	// 不完全微分系数	0 - 1
	vec_pid.integral = 0;	// 积分值
	vec_pid.output = 0;	// 输出值
	vec_pid.Ilimit = 50;	// 积分范围
	vec_pid.I_Range = 10;	// 积分限幅度
	vec_pid.Ilimit_flag = 0;		// 积分分离标志

}

float Pid_Calculate(pid_param *pid, float target, float measure)
{
	float error, Pout, Iout, Dout;
	error = target - measure;
	
	Pout = pid->KP * error;			// 比例控制
	
	if(Speed >= 250)
	{
		if(measure > pid->Ilimit || measure < -pid->Ilimit)
		{
			pid->Ilimit_flag = 0;
		}else{
			pid->Ilimit_flag = 1;
			pid->integral += error;
			// 积分限幅
			if(pid->integral > pid->I_Range){
				pid->integral = pid->I_Range;
			}
			if(pid->integral < -pid->I_Range){
				pid->integral = -pid->I_Range;
			}
		}

	}else{
		pid->integral = 0;
	}
	Iout = pid->KI * pid->Ilimit_flag * pid->integral;
	
	Dout = pid->KD * (error - pid->last_error);
	Dout = Dout * pid->D_alpha + pid->last_Dout * (1-pid->D_alpha);	// 一阶低通滤波
	pid->last_Dout = Dout;		// 微分控制
	
	pid->output = Pout + Iout + Dout;		// 积分控制
	
	pid->last_error = error;
	return pid->output;
}
