#include "system.h"
#include "systick.h"
#include "led.h"
#include "adc.h"
#include "lcd.h"
#include "adc.h"
#include "rc.h"

int main(void)
{
	SysTick_Init(100);
	//Led_Init();
	//Remote_ADC_Init();
	ADC_Config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	RC_Task_Start();
}




