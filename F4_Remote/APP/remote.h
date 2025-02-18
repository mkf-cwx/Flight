#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "system.h"

extern u16 Speed;
extern u8 Pitch;
extern u8 Roll;
extern u8 Yaw;
extern u8 Mode;
extern u8 battery_level;
extern u8 f_Pitch;
extern u8 f_Roll;
extern u8 f_Yaw;

void Remote_SendData(void);
void Remote_ReceiveData(void);

#endif	// __REMOTE_H__
