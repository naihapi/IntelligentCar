#ifndef _MPU6050_H
#define _MPU6050_H

// MCU型号
#include "stm32f10x.h"

// 标准库
#include "math.h"

// 解算库
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

// 宏定义
#define MPU6050_Q30 1073741824.0f

// 外设
#include "IIC.h"
#include "GPIO.h"
#include "TIM.h"
#include "ENCODER.h"
#include "USART.h"

// 软件
#include "PID.h"

// 变量
extern float MPU6050_Pitch;
extern float MPU6050_Roll;
extern float MPU6050_Yaw;

// 外置硬件
#include "MPU6050_Reg.h"

// API
void MPU6050_InitPro(void);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
int8_t MPU6050_DMP_Init(void);
void MPU6050_ReadReg(uint8_t reg, uint8_t *buf);
int8_t MPU6050_DMP_Get_Data(float *pitch, float *roll, float *yaw);
void MPU6050_Remove_YawDeviaction(void);
float MPU6050_AngleConvert(float Angle);

#endif
