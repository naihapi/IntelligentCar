#ifndef _PID_H
#define _PID_H

// MCU型号
#include "stm32f10x.h"

// 外设
#include "ADC.h"

// 变量
//...

// 外置硬件
#include "MOTOR.h"
#include "MPU6050.h"
#include "ENCODER.h"

// 宏定义
#define PID_ITROFFSIDE_TargetValue (float)290.0 // 对管两侧ADC目标值
#define PID_CAMERA_TargetValue (float)80.0      // 视觉目标值

// 软件
//...

// API
int PID_RetExecutionQuantity_SpeedControl(float TargetValue);
void PID_RetExecutionQuantity_vITRControl(int *Left_ExecutionQuantity, int *Right_ExecutionQuantity);
void PID_RetExecutionQuantity_vCamControl(int *ExecutionQuantity);

#endif
