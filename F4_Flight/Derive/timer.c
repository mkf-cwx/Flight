#include "timer.h"

void TIM3_Init(void)
{
    // ʹ��TIM3ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // ����TIM3
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 10000-1; // Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_Period = 199; // �Զ���װ��ֵ	  50hz

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // ʱ�ӷ�Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // ʹ��TIM3�����ж�
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // ʹ��TIM3
    TIM_Cmd(TIM3, ENABLE);

    // ����NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // TIM3�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // ʹ���ж�
    NVIC_Init(&NVIC_InitStructure);
}
// ������ʱ��
void TIM3_Start(void)
{
    TIM_Cmd(TIM3, ENABLE); // ʹ�ܶ�ʱ��
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // ʹ�ܸ����ж�
}

// �رն�ʱ��
void TIM3_Stop(void)
{
    TIM_Cmd(TIM3, DISABLE); // ���ö�ʱ��
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE); // ���ø����ж�
}
