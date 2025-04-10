#ifndef _PID_H
#define _PID_H

// MCU型号
#include "stm32f10x.h"

// 变量
extern float TargetValue_Speed;
extern float TargetValue_Turn;
extern uint8_t PID_State;
extern float PID_Speed_Error_Sum;
extern float PID_Speed_P, PID_Speed_I;
// 外置硬件
#include "MOTOR.h"
#include "MPU6050.h"

// 软件
#include "Manual.h"

int PID_Vertical_Function(float NowValue, float TargetValue);

#endif
