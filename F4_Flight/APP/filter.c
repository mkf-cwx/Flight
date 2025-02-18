#include "filter.h"

/**
 *卡尔曼滤波器
 *@param 	Kalman *kfp 卡尔曼结构体参数
 *   		float input 需要滤波的参数的测量值（即传感器的采集值）
 *@return 滤波后的参数（最优值）
 */
float KalmanFilter(Kalman *kfp,float input)
{
   //预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
   kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
   //更新协方差方程: 本次的系统协方差付给 kfp->LastP 为下一次运算准备。
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;
   return kfp->out;
}

/**
 *一阶低通滤波器
 *@param 	int16_t old_value 上一时刻的值
 *   		int16_t cur_value 当前时刻的值
 *          uint8_t	exe_Freq	采样频率
 *			uint8_t	cutFreq		截止频率
 *@return 滤波后的参数
 */
int16_t Lpf_Filter(int16_t old_value, int16_t cur_value, uint8_t sample_Freq, uint8_t cutFreq)
{
	float lpf_factor;
	// 滤波系数
	lpf_factor = 2 * 3.1415926 * (float)cutFreq / (float)sample_Freq;
	lpf_factor = lpf_factor > 1 ? 1 : (lpf_factor < 0 ? 0 : lpf_factor);
	return (int16_t)(old_value * (1 - lpf_factor) + cur_value * lpf_factor);
}
