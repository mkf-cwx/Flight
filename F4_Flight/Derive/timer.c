#include "timer.h"

void TIM3_Init(void)
{
    // 使能TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 配置TIM3
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 10000-1; // 预分频值
    TIM_TimeBaseStructure.TIM_Period = 199; // 自动重装载值	  50hz

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 使能TIM3更新中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // 使能TIM3
    TIM_Cmd(TIM3, ENABLE);

    // 配置NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // TIM3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断
    NVIC_Init(&NVIC_InitStructure);
}
// 开启定时器
void TIM3_Start(void)
{
    TIM_Cmd(TIM3, ENABLE); // 使能定时器
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // 使能更新中断
}

// 关闭定时器
void TIM3_Stop(void)
{
    TIM_Cmd(TIM3, DISABLE); // 禁用定时器
    TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE); // 禁用更新中断
}
