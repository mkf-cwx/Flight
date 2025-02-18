#include "adc.h"

/********************************************
* @brief: Battery_ADC_Init ��ص��ź�adc������ų�ʼ��
* @param:	
* @return:
 ********************************************
*/
void Battery_ADC_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//ʹ�� GPIOB ʱ��

	// ��ʼ�� ADC1 ͨ��5 IO��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  // PB1 ͨ��9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  // ģʽΪģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // ����������
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}
/********************************************
* @brief: Remote_ADC_Init ң�˵�λ�����ź�adc������ų�ʼ��
* @param:	
* @return:
 ********************************************
*/
void Remote_ADC_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);// ʹ�� GPIOA ʱ��

	// ��ʼ�� ADC1 ͨ��5 IO��
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;  // PA0, PA1, PA2, PA3 ͨ��0, ͨ��1, ͨ��2, ͨ��3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;  // ģʽΪģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // ����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}
/********************************************
* @brief: ADC_Config ����ADC1����ʼ��ADC1����ز���
* @param:	
* @return:
 ********************************************
*/
void ADC_Config(void)
{
	Battery_ADC_Init();
	Remote_ADC_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);// ʹ��ADC1ʱ��
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);   // ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);    // ��λ����
	
	// ��ʼ�� CCR ͨ�ÿ��ƼĴ�������
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; // DMAʧ��
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  // ����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;  // Ԥ��Ƶֵ����	��֤ADC1��ʱ��Ƶ�ʲ�����36MHz
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;// ���������׶�֮���ӳ�5��ʱ��
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	// ��ʼ�� ADC1 ��ز���
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // �ر�����ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // �Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // ��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_NbrOfConversion = 1;  // 1��ת���ڹ���������
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  // 12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; // ��ɨ��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_Init(ADC1,&ADC_InitStructure); // ADC��ʼ��
	
	ADC_Cmd(ADC1,ENABLE);   // ����ADת����
}
/********************************************
* @brief: Get_ADC_Value ��ȡADCֵ
* @param: 	u8 channel��ͨ��ֵ 0~16	
* @return: 	ת�����
 ********************************************
*/
u16 Get_ADC_Value(u8 channel)
{
	ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_480Cycles);//����ADC������ͨ����һ�����У�����ʱ��480
	ADC_SoftwareStartConv(ADC1);//ʹ��ָ����ADC1�����ת����������
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//��ȡ״̬�Ĵ�����״̬λEOC���ȴ�ת������
	return ADC_GetConversionValue(ADC1);//�������һ�ε�ADC1�������ת�����
}

/******************************************************************
* @brief: 	Get_ADC_Average ��ȡͨ��ch��ת��ֵ��ȡtimes�Σ�Ȼ��ƽ��
* @param: 	u8 channel ͨ����� 
			u8 times ��ȡ����
* @return: 	ͨ��ch��times��ת�����ƽ��ֵ
 ******************************************************************
*/
u16 Get_ADC_Average(u8 channel, u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val = temp_val + Get_ADC_Value(channel); //ȡtimes��ͨ��ֵ�������
		delay_ms(5);
	}
	return temp_val/times; //����ƽ��ֵ
}
