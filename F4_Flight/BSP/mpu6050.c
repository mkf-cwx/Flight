#include "mpu6050.h"
#include "i2c.h"
#include "filter.h"
#include "systick.h"
#include "usart1.h"

/*在完成mpu6050初始化后，提取200次陀螺仪数据获取零偏的平均值*/
float gyro_offset[3];

/*通过线性回归模型 y = kx+b，使用最小二乘法求出静止状态下的最佳的 k 和 b */
const float acc_offset_fac[3] = {1.0031,0.9932,0.9784};		// 缩放因子
const float acc_offset_bias[3] = {-0.0749,0.0261,0.1296};	// 零偏

static uint8_t GYRO_OFFSET_FLAG = 0;
static uint8_t ACC_OFFSET_FLAG = 0;
/**
 * @brief       MPU6050初始化
 * @param 
 * @return 		成功返回0，失败返回1
 */
u8 MPU6050_Init(void)
{
	uint8_t res;
	I2C_GPIO_Init();
	delay_ms(100);
	while(!MPU_WriteReg(MPU_PWR_MGMT1_REG, 0x80));		//复位mpu6050
	delay_ms(100);
	while(!MPU_WriteReg(MPU_PWR_MGMT1_REG, 0x00));		//唤醒mpu6050
	while(!MPU_Set_Gyro_Fsr(3));					//陀螺仪量程
	while(!MPU_Set_Accel_Fsr(2));					//加速度计量程
	while(!MPU_WriteReg(MPU_INT_EN_REG, 0x00));		//关闭所有中断
	while(!MPU_WriteReg(MPU_USER_CTRL_REG, 0x00));		//i2c主模式关闭
	while(!MPU_WriteReg(MPU_FIFO_EN_REG, 0x00));		//关闭FIFO
	while(!MPU_WriteReg(MPU_INTBP_CFG_REG, 0x80));		//INT引脚低电平有效

	res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
	
	if(res == MPU_ADDR)
	{
		while(!MPU_WriteReg(MPU_PWR_MGMT1_REG,0X01));	//设置CLKSEL,PLL X轴为参考
		while(!MPU_WriteReg(MPU_PWR_MGMT2_REG,0X00));	//加速度与陀螺仪都工作
		while(!MPU_Set_Rate(50));						//设置采样率为50Hz
	}else{
		return 1;
	}

	return 0;
}


/**
 * @brief       MPU6050设置陀螺仪传感器量程范围
 * @param       frs: 0 --> ±250dps
 *                   1 --> ±500dps
 *                   2 --> ±1000dps
 *                   3 --> ±2000dps
 */
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_WriteReg(MPU_GYRO_CFG_REG,fsr<<3); //设置陀螺仪满量程范围
}


/**
 * @brief       MPU6050设置加速度传感器量程范围
 * @param       frs: 0 --> ±2g
 *                   1 --> ±4g
 *                   2 --> ±8g
 *                   3 --> ±16g
 */
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_WriteReg(MPU_ACCEL_CFG_REG,fsr<<3); //设置加速度传感器满量程范围  
}


/**********************************************
函数名称：MPU_Set_Rate
函数功能：设置MPU6050的采样率(假定Fs=1KHz)
函数参数：rate:4~1000(Hz)  初始化中rate取50
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_WriteReg(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);											//自动设置LPF为采样率的一半
}

/**********************************************
函数名称：MPU_Set_LPF
函数功能：设置MPU6050的数字低通滤波器
函数参数：lpf:数字低通滤波频率(Hz)
函数返回值：0,设置成功  其他,设置失败
**********************************************/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_WriteReg(MPU_CFG_REG,data);//设置数字低通滤波器  
}

u8 MPU_WriteReg(uint8_t reg_addr, uint8_t reg_data)
{
	uint8_t ERROR_FLAG;
	I2C_Start();
	I2C_SendByte(MPU_I2C_ADDR);
	ERROR_FLAG = I2C_WaitAck();
	I2C_SendByte(reg_addr);
	ERROR_FLAG = I2C_WaitAck();
	I2C_SendByte(reg_data);
	ERROR_FLAG = I2C_WaitAck();
	I2C_Stop();
	
	return ERROR_FLAG;
}

u8 MPU_Read_Byte(u8 reg)
{
	uint8_t res;
	
	I2C_Start();
	I2C_SendByte(MPU_I2C_ADDR);
	I2C_WaitAck();

	I2C_SendByte(reg);
	I2C_WaitAck();
	I2C_Start();
	I2C_SendByte(MPU_I2C_ADDR|1);
	I2C_WaitAck();
	res = I2C_ReadByte();
	I2C_NAck();
	I2C_Stop();
	return res;
}

u8 MPU_ReadData(uint8_t reg, uint8_t len, uint8_t *buf)
{
	uint8_t i, ERROR_FLAG;
	
	I2C_Start();
	I2C_SendByte(MPU_I2C_ADDR);
	ERROR_FLAG = I2C_WaitAck();
	I2C_SendByte(reg);
	ERROR_FLAG = I2C_WaitAck();
	
	I2C_Start();
	I2C_SendByte(MPU_I2C_ADDR|1);
	ERROR_FLAG = I2C_WaitAck();
	for(i=0; i<len; i++)
	{
		buf[i] = I2C_ReadByte();
		if(i != len-1)
		{
			I2C_Ack();
		}else
		{
			I2C_NAck();
		}
	}
	I2C_Stop();
	
	return ERROR_FLAG;
}


void MPU6050_GyroOffset(void)
{
	uint8_t i;
	MPU6050_RAW_Data offset;
	float sum_x=0, sum_y=0, sum_z=0;
	offset.x=0;
	offset.y=0;
	offset.z=0;
	for(i=0; i<200; i++)
	{
		MPU_Get_GyroData(&offset);
		sum_x += offset.x;
		sum_y += offset.y;
		sum_z += offset.z;
	}
	gyro_offset[0] = sum_x/200;
	gyro_offset[1] = sum_y/200;
	gyro_offset[2] = sum_z/200;
	GYRO_OFFSET_FLAG = 1;
}

void MPU6050_AccOffset(void)
{
	ACC_OFFSET_FLAG = 1;
}


void MPU_Get_GyroData(MPU6050_RAW_Data *gyro_data)
{
	uint8_t buf[6], ret;
	int16_t recv_gyro_data[3];
	static int16_t last_gyro[3] = {0, 0, 0};
	ret = MPU_ReadData(MPU_GYRO_XOUTH_REG, 6, buf);
	if(ret == 1)
	{
		recv_gyro_data[0]=((int16_t)buf[0]<<8) | buf[1];
		recv_gyro_data[1]=((int16_t)buf[2]<<8) | buf[3];
		recv_gyro_data[2]=((int16_t)buf[4]<<8) | buf[5];
	}
	if(GYRO_OFFSET_FLAG)
	{
		recv_gyro_data[0] = Lpf_Filter(last_gyro[0], recv_gyro_data[0], 250, 30);
		recv_gyro_data[1] = Lpf_Filter(last_gyro[1], recv_gyro_data[1], 250, 30);
		recv_gyro_data[2] = Lpf_Filter(last_gyro[2], recv_gyro_data[2], 250, 30);
		
		last_gyro[0] = recv_gyro_data[0];
		last_gyro[1] = recv_gyro_data[1];
		last_gyro[2] = recv_gyro_data[2];
	}
	gyro_data->x = (float)recv_gyro_data[0] * Gyro_Range_fac;
	gyro_data->y = (float)recv_gyro_data[1] * Gyro_Range_fac;
	gyro_data->z = (float)recv_gyro_data[2] * Gyro_Range_fac;
	
	if(GYRO_OFFSET_FLAG)
	{
		gyro_data->x -= gyro_offset[0];
		gyro_data->y -= gyro_offset[1];
		gyro_data->z -= gyro_offset[2];
	}
}

void MPU_Get_AccelData(MPU6050_RAW_Data *acc_data)
{
	uint8_t buf[6], ret;
	int16_t recv_acc_data[3];
	ret = MPU_ReadData(MPU_ACCEL_XOUTH_REG, 6, buf);
	if(ret == 1)
	{
		recv_acc_data[0]=((int16_t)buf[0]<<8) | buf[1];
		recv_acc_data[1]=((int16_t)buf[2]<<8) | buf[3];
		recv_acc_data[2]=((int16_t)buf[4]<<8) | buf[5];
	}
	
	acc_data->x = (float)recv_acc_data[0] * Acc_Range_fac;
	acc_data->y = (float)recv_acc_data[1] * Acc_Range_fac;
	acc_data->z = (float)recv_acc_data[2] * Acc_Range_fac;
	
	if(ACC_OFFSET_FLAG)
	{
		// 满足线性关系 Kx+b
		acc_data->x = acc_offset_fac[0] * acc_data->x + acc_offset_bias[0];
		acc_data->y = acc_offset_fac[1] * acc_data->y + acc_offset_bias[1];
		acc_data->z = acc_offset_fac[2] * acc_data->z + acc_offset_bias[2];
	}
	
}





