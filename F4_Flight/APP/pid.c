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
	/*Pitch PID �ṹ���ʼ��*/
	pitch_pid.KP = 5;	// ����ϵ��
	pitch_pid.KI = 0;		// ����ϵ��
	pitch_pid.KD = 2;		// ΢��ϵ��
	pitch_pid.last_error = 0;	// �ϴ����
	pitch_pid.last_Dout = 0;   	// �ϴ�΢��ֵ
	pitch_pid.D_alpha = 0.4f;	// ����ȫ΢��ϵ��	0 - 1
	pitch_pid.integral = 0;	// ����ֵ
	pitch_pid.output = 0;	// ���ֵ
	pitch_pid.Ilimit = 30;	// ���ַ�Χ
	pitch_pid.I_Range = 500;	// �����޷���
	pitch_pid.Ilimit_flag = 0;		// ���ַ����־
	
	/*Roll PID �ṹ���ʼ��*/
	roll_pid.KP = 5;	// ����ϵ��
	roll_pid.KI = 0;		// ����ϵ��
	roll_pid.KD = 2;		// ΢��ϵ��
	roll_pid.last_error = 0;	// �ϴ����
	roll_pid.last_Dout = 0;   	// �ϴ�΢��ֵ
	roll_pid.D_alpha = 0.4f;	// ����ȫ΢��ϵ��	0 - 1
	roll_pid.integral = 0;	// ����ֵ
	roll_pid.output = 0;	// ���ֵ
	roll_pid.Ilimit = 30;	// ���ַ�Χ
	roll_pid.I_Range = 500;	// �����޷���
	roll_pid.Ilimit_flag = 0;		// ���ַ����־
	
	/*Gx PID �ṹ���ʼ��*/
	gx_pid.KP = 0.73f;	// ����ϵ��
	gx_pid.KI = 0.012f;		// ����ϵ��
	gx_pid.KD = 8;		// ΢��ϵ��
	gx_pid.last_error = 0;	// �ϴ����
	gy_pid.last_Dout = 0;   	// �ϴ�΢��ֵ
	gy_pid.D_alpha = 0.4f;	// ����ȫ΢��ϵ��	0 - 1
	gx_pid.integral = 0;	// ����ֵ
	gx_pid.output = 0;	// ���ֵ
	gx_pid.Ilimit = 300;	// ���ַ�Χ
	gx_pid.I_Range = 500;	// �����޷���
	gx_pid.Ilimit_flag = 0;		// ���ַ����־
	
	/*Gy PID �ṹ���ʼ��*/
	gy_pid.KP = 0.73f;	// ����ϵ��
	gy_pid.KI = 0.012f;		// ����ϵ��
	gy_pid.KD = 8;		// ΢��ϵ��
	gy_pid.last_error = 0;	// �ϴ����
	gy_pid.last_Dout = 0;   	// �ϴ�΢��ֵ
	gy_pid.D_alpha = 0.4f;	// ����ȫ΢��ϵ��	0 - 1
	gy_pid.integral = 0;	// ����ֵ
	gy_pid.output = 0;	// ���ֵ
	gy_pid.Ilimit = 300;	// ���ַ�Χ
	gy_pid.I_Range = 500;	// �����޷���
	gy_pid.Ilimit_flag = 0;		// ���ַ����־
	
	/*Pos PID �ṹ���ʼ��*/	
	pos_pid.KP = 0.025f;	// ����ϵ��
	pos_pid.KI = 0;		// ����ϵ��
	pos_pid.KD = 0;		// ΢��ϵ��
	pos_pid.last_error = 0;	// �ϴ����
	pos_pid.last_Dout = 0;   	// �ϴ�΢��ֵ
	pos_pid.D_alpha = 0.4f;	// ����ȫ΢��ϵ��	0 - 1
	pos_pid.integral = 0;	// ����ֵ
	pos_pid.output = 0;	// ���ֵ
	pos_pid.Ilimit = 300;	// ���ַ�Χ
	pos_pid.I_Range = 2.5;	// �����޷���
	pos_pid.Ilimit_flag = 0;		// ���ַ����־
	
	/*Vec PID �ṹ���ʼ��*/
	vec_pid.KP = 0.028f;	// ����ϵ��
	vec_pid.KI = 0;		// ����ϵ��
	vec_pid.KD = 0;		// ΢��ϵ��
	vec_pid.last_error = 0;	// �ϴ����
	vec_pid.last_Dout = 0;   	// �ϴ�΢��ֵ
	vec_pid.D_alpha = 0.4f;	// ����ȫ΢��ϵ��	0 - 1
	vec_pid.integral = 0;	// ����ֵ
	vec_pid.output = 0;	// ���ֵ
	vec_pid.Ilimit = 50;	// ���ַ�Χ
	vec_pid.I_Range = 10;	// �����޷���
	vec_pid.Ilimit_flag = 0;		// ���ַ����־

}

float Pid_Calculate(pid_param *pid, float target, float measure)
{
	float error, Pout, Iout, Dout;
	error = target - measure;
	
	Pout = pid->KP * error;			// ��������
	
	if(Speed >= 250)
	{
		if(measure > pid->Ilimit || measure < -pid->Ilimit)
		{
			pid->Ilimit_flag = 0;
		}else{
			pid->Ilimit_flag = 1;
			pid->integral += error;
			// �����޷�
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
	Dout = Dout * pid->D_alpha + pid->last_Dout * (1-pid->D_alpha);	// һ�׵�ͨ�˲�
	pid->last_Dout = Dout;		// ΢�ֿ���
	
	pid->output = Pout + Iout + Dout;		// ���ֿ���
	
	pid->last_error = error;
	return pid->output;
}
