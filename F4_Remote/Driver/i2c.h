#ifndef __I2C_H__
#define __I2C_H__

#include "system.h"

#define I2C_SCL_PORT		GPIOA								/*连接到SCL时钟线的GPIO端口*/
#define I2C_SDA_PORT		GPIOC								/*连接到SDA数据线的GPIO端口*/
#define I2C_SCL_PIN		GPIO_Pin_8								/*连接到SCL时钟线的GPIO引脚*/
#define I2C_SDA_PIN		GPIO_Pin_9								/*连接到SDA数据线的GPIO引脚*/

// SCL = 1
#define I2C_SCL_1  GPIO_SetBits(I2C_SCL_PORT, I2C_SCL_PIN)		
// SCL = 0	
#define I2C_SCL_0  GPIO_ResetBits(I2C_SCL_PORT, I2C_SCL_PIN)		
// SDA = 1
#define I2C_SDA_1  GPIO_SetBits(I2C_SDA_PORT, I2C_SDA_PIN)		
// SDA = 0
#define I2C_SDA_0  GPIO_ResetBits(I2C_SDA_PORT, I2C_SDA_PIN)		
// 读SDA口线状态 
#define I2C_SDA_READ  GPIO_ReadInputDataBit(I2C_SDA_PORT, I2C_SDA_PIN)	

void I2C_GPIO_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_NAck(void);
uint8_t I2C_WaitAck(void);
void I2C_SendByte(uint8_t byte);
uint8_t I2C_ReadByte(uint8_t ack);


#endif	// __I2C_H__

