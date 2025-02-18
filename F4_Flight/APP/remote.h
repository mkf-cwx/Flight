#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "system.h"

extern float nrf_Roll;
extern float nrf_Pitch;
extern float nrf_Yaw;
extern u16 Mode;
extern u16 Speed;

void Remote_Receive(void);
void Send_Remote(void);

#endif	//__REMOTE_H__
