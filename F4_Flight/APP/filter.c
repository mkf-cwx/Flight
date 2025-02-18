#include "filter.h"

/**
 *�������˲���
 *@param 	Kalman *kfp �������ṹ�����
 *   		float input ��Ҫ�˲��Ĳ����Ĳ���ֵ�����������Ĳɼ�ֵ��
 *@return �˲���Ĳ���������ֵ��
 */
float KalmanFilter(Kalman *kfp,float input)
{
   //Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
   kfp->Now_P = kfp->Last_P + kfp->Q;
   //���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
   kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
   //��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
   kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
   //����Э�����: ���ε�ϵͳЭ����� kfp->LastP Ϊ��һ������׼����
   kfp->Last_P = (1-kfp->Kg) * kfp->Now_P;
   return kfp->out;
}

/**
 *һ�׵�ͨ�˲���
 *@param 	int16_t old_value ��һʱ�̵�ֵ
 *   		int16_t cur_value ��ǰʱ�̵�ֵ
 *          uint8_t	exe_Freq	����Ƶ��
 *			uint8_t	cutFreq		��ֹƵ��
 *@return �˲���Ĳ���
 */
int16_t Lpf_Filter(int16_t old_value, int16_t cur_value, uint8_t sample_Freq, uint8_t cutFreq)
{
	float lpf_factor;
	// �˲�ϵ��
	lpf_factor = 2 * 3.1415926 * (float)cutFreq / (float)sample_Freq;
	lpf_factor = lpf_factor > 1 ? 1 : (lpf_factor < 0 ? 0 : lpf_factor);
	return (int16_t)(old_value * (1 - lpf_factor) + cur_value * lpf_factor);
}
