#ifndef __Car_H
#define __Car_H

// MCU型号
#include "stm32f10x.h"

// RTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// 变量
//...

// 外置硬件
//...

// 外设
#include "MOTOR.h"
#include "ADC.h"

// 软件
#include "PID.h"

// API
void Manual_InitPro(void);
void Car_SpinStop(void);
void Car_SearchLine(void);
void Car_BreakLine(void);

#endif
