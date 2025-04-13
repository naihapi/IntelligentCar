#ifndef _PID_H
#define _PID_H

// MCU型号
#include "stm32f10x.h"

// 变量
//...

// 外置硬件
#include "MOTOR.h"
#include "MPU6050.h"
#include "ENCODER.h"

// 软件
#include "Manual.h"

// API
int PID_RetExecutionQuantity_SpeedControl(float TargetValue);

#endif
