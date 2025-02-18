#include "mpu6050.h"
#include "i2c.h"
#include "filter.h"
#include "systick.h"
#include "usart1.h"

/*�����mpu6050��ʼ������ȡ200�����������ݻ�ȡ��ƫ��ƽ��ֵ*/
float gyro_offset[3];

/*ͨ�����Իع�ģ�� y = kx+b��ʹ����С���˷������ֹ״̬�µ���ѵ� k �� b */
const float acc_offset_fac[3] = {1.0031,0.9932,0.9784};		// ��������
const float acc_offset_bias[3] = {-0.0749,0.0261,0.1296};	// ��ƫ

static uint8_t GYRO_OFFSET_FLAG = 0;
static uint8_t ACC_OFFSET_FLAG = 0;
/**
 * @brief       MPU6050��ʼ��
 * @param 
 * @return 		�ɹ�����0��ʧ�ܷ���1
 */
u8 MPU6050_Init(void)
{
	uint8_t res;
	I2C_GPIO_Init();
	delay_ms(100);
	while(!MPU_WriteReg(MPU_PWR_MGMT1_REG, 0x80));		//��λmpu6050
	delay_ms(100);
	while(!MPU_WriteReg(MPU_PWR_MGMT1_REG, 0x00));		//����mpu6050
	while(!MPU_Set_Gyro_Fsr(3));					//����������
	while(!MPU_Set_Accel_Fsr(2));					//���ٶȼ�����
	while(!MPU_WriteReg(MPU_INT_EN_REG, 0x00));		//�ر������ж�
	while(!MPU_WriteReg(MPU_USER_CTRL_REG, 0x00));		//i2c��ģʽ�ر�
	while(!MPU_WriteReg(MPU_FIFO_EN_REG, 0x00));		//�ر�FIFO
	while(!MPU_WriteReg(MPU_INTBP_CFG_REG, 0x80));		//INT���ŵ͵�ƽ��Ч

	res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
	
	if(res == MPU_ADDR)
	{
		while(!MPU_WriteReg(MPU_PWR_MGMT1_REG,0X01));	//����CLKSEL,PLL X��Ϊ�ο�
		while(!MPU_WriteReg(MPU_PWR_MGMT2_REG,0X00));	//���ٶ��������Ƕ�����
		while(!MPU_Set_Rate(50));						//���ò�����Ϊ50Hz
	}else{
		return 1;
	}

	return 0;
}


/**
 * @brief       MPU6050���������Ǵ��������̷�Χ
 * @param       frs: 0 --> ��250dps
 *                   1 --> ��500dps
 *                   2 --> ��1000dps
 *                   3 --> ��2000dps
 */
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_WriteReg(MPU_GYRO_CFG_REG,fsr<<3); //���������������̷�Χ
}


/**
 * @brief       MPU6050���ü��ٶȴ��������̷�Χ
 * @param       frs: 0 --> ��2g
 *                   1 --> ��4g
 *                   2 --> ��8g
 *                   3 --> ��16g
 */
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_WriteReg(MPU_ACCEL_CFG_REG,fsr<<3); //���ü��ٶȴ����������̷�Χ  
}


/**********************************************
�������ƣ�MPU_Set_Rate
�������ܣ�����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
����������rate:4~1000(Hz)  ��ʼ����rateȡ50
��������ֵ��0,���óɹ�  ����,����ʧ��
**********************************************/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_WriteReg(MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);											//�Զ�����LPFΪ�����ʵ�һ��
}

/**********************************************
�������ƣ�MPU_Set_LPF
�������ܣ�����MPU6050�����ֵ�ͨ�˲���
����������lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
��������ֵ��0,���óɹ�  ����,����ʧ��
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
	return MPU_WriteReg(MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
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
		// �������Թ�ϵ Kx+b
		acc_data->x = acc_offset_fac[0] * acc_data->x + acc_offset_bias[0];
		acc_data->y = acc_offset_fac[1] * acc_data->y + acc_offset_bias[1];
		acc_data->z = acc_offset_fac[2] * acc_data->z + acc_offset_bias[2];
	}
	
}





