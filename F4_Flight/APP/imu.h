#ifndef __IMU_H__
#define __IMU_H__

#include "system.h"
#include "mpu6050.h"
#include "math.h"
#include "usart1.h"

#define G			9.80665f		      	// m/s^2	
#define RadtoDeg    57.324841f				//���ȵ��Ƕ� (���� * 180/3.1415)
#define DegtoRad    0.0174533f				//�Ƕȵ����� (�Ƕ� * 3.1415/180)
 
extern float Roll, Pitch, Yaw;
extern float gx, gy, gz;

void IMU_Data(uint8_t count);
void Posture_Update(float ax, float ay, float az, float gx, float gy, float gz);

#endif	// __IMU_H__
