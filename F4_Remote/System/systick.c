#include "systick.h"
#include "FreeRTOS.h"
#include "task.h"


static uint32_t g_fac_us = 0;       /* us��ʱ������ */
extern void xPortSysTickHandler(void);

/**
 * @brief     systick�жϷ�����,ʹ��OSʱ�õ�
 * @param     ticks : ��ʱ�Ľ�����  
 * @retval    ��
 */  
void SysTick_Handler(void)
{

    /* OS ��ʼ����,��ִ�������ĵ��ȴ��� */
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}

/**
 * @brief     ��ʼ���ӳٺ���
 * @param     sysclk: ϵͳʱ��Ƶ��, ��CPUƵ��(rcc_c_ck), 180MHz
 * @retval    ��
 */  

void SysTick_Init(uint16_t SYSCLK)
{
	/* �����Ҫ֧��OS */
    uint32_t reload;

    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);	/* SYSTICKʹ���ⲿʱ��Դ,Ƶ��ΪHCLK */
    g_fac_us = SYSCLK;                                  /* �����Ƿ�ʹ��OS,g_fac_us����Ҫʹ�� */
  /* �����Ҫ֧��OS. */
    reload = SYSCLK;                                    /* ÿ���ӵļ������� ��λΪM */
    reload *= 1000000 / configTICK_RATE_HZ;            	/* ����delay_ostickspersec�趨���ʱ��,reloadΪ24λ
                                                         * �Ĵ���,���ֵ:16777216,��180M��,Լ��0.0932s����  */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;          /* ����SYSTICK�ж� */
    SysTick->LOAD = reload;                             /* ÿ1/delay_ostickspersec���ж�һ�� */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;           /* ����SYSTICK */
}

void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;        /* LOAD��ֵ */
    ticks = nus * g_fac_us;                 /* ��Ҫ�Ľ����� */
    told = SysTick->VAL;                    /* �ս���ʱ�ļ�����ֵ */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;        /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ����� */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) 
            {
                break;                      /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
            }
        }
    }
        
} 

/**
 * @brief     ��ʱnms
 * @param     nms: Ҫ��ʱ��ms�� (0< nms <= 65535) 
 * @retval    ��
 */
void delay_ms(uint16_t nms)
{
    uint32_t i;
    for (i=0; i<nms; i++)
    {
        delay_us(1000);
    }
}

