#ifndef __KEY_H__
#define __KEY_H__

#include "system.h"

#define KEY1_PIN	GPIO_Pin_10
#define	KEY2_PIN	GPIO_Pin_12

#define Key_1		GPIO_ReadInputDataBit(GPIOC, KEY1_PIN)
#define Key_2		GPIO_ReadInputDataBit(GPIOC, KEY2_PIN)

void Key_Init(void);

#endif	// __KEY_H__
