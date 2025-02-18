#include "system.h"
#include "systick.h"
#include "usart1.h"
#include "led.h"
#include "usart6.h"
#include "adc.h"
#include "flight.h"

int main()
{
	SysTick_Init(100);
	delay_ms(2000);
	USART1_Init(115200);
	Led_Init();
	Battery_ADC_Init();
	ADC_Config();
	delay_ms(500);
	Flight_task();
}


