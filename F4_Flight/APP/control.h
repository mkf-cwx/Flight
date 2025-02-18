#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "system.h"

void Control(uint8_t outerEN);

extern float target_Vecx;
extern float target_Vecy;
extern float target_Pitch;
extern float target_Roll;


#endif	//__CONTROL_H__
