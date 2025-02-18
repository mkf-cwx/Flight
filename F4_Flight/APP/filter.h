#ifndef __FILTER_H__
#define __FILTER_H__

#include "system.h"


typedef struct 
{
    float Last_P;//上次估算协方差 不可以为0 ! ! ! ! ! 
    float Now_P;//当前估算协方差
    float out;//卡尔曼滤波器输出
    float Kg;//卡尔曼增益
    float Q;//过程噪声协方差
    float R;//观测噪声协方差
}Kalman;
float KalmanFilter(Kalman *kfp,float input);
int16_t Lpf_Filter(int16_t old_value, int16_t cur_value, uint8_t sample_Freq, uint8_t cutFreq);

#endif	//__FILTER_H__
