#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "system.h"

void Touch_Init(void);
void Touch_ISR(void);

typedef struct {
    u16 x1; // 左上角 X 坐标
    u16 y1; // 左上角 Y 坐标
    u16 x2; // 右下角 X 坐标
    u16 y2; // 右下角 Y 坐标
    void (*OnClick)(void); // 点击事件回调函数
} Icon;

#endif	// __TOUCH_H__

