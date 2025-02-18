#ifndef __PID_H__
#define __PID_H__

#include "system.h"


typedef struct{
	float KP;
	float KI;
	float KD;
	float last_error;
	float last_Dout;
	float D_alpha;
	float integral;
	float Ilimit;
	float I_Range;
	float output;
	uint8_t Ilimit_flag;
}pid_param;

void Pid_Param_Init(void);
float Pid_Calculate(pid_param *pid, float target, float measure);

extern pid_param pitch_pid;
extern pid_param roll_pid;
extern pid_param gx_pid;
extern pid_param gy_pid;
extern pid_param pos_pid;
extern pid_param vec_pid;

#endif	//__PID_H__
