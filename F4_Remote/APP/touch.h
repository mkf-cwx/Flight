#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "system.h"

void Touch_Init(void);
void Touch_ISR(void);

typedef struct {
    u16 x1; // ���Ͻ� X ����
    u16 y1; // ���Ͻ� Y ����
    u16 x2; // ���½� X ����
    u16 y2; // ���½� Y ����
    void (*OnClick)(void); // ����¼��ص�����
} Icon;

#endif	// __TOUCH_H__

