#ifndef __FILTER_H__
#define __FILTER_H__

#include "system.h"


typedef struct 
{
    float Last_P;//�ϴι���Э���� ������Ϊ0 ! ! ! ! ! 
    float Now_P;//��ǰ����Э����
    float out;//�������˲������
    float Kg;//����������
    float Q;//��������Э����
    float R;//�۲�����Э����
}Kalman;
float KalmanFilter(Kalman *kfp,float input);
int16_t Lpf_Filter(int16_t old_value, int16_t cur_value, uint8_t sample_Freq, uint8_t cutFreq);

#endif	//__FILTER_H__
