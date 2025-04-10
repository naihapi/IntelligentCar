#ifndef _UltraSound_H
#define _UltraSound_H

// MCU型号
#include "stm32f10x.h"

// 软件
#include "Delay.h"

// 外设
#include "TIM.h"

// RTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// 变量
extern float UltraSound_Distance;

// API
void ULtraSound_InitPro(void);
void UltraSound_Trig(void);
void UltraSound_GetDistance(void);
void UltraSound_Function(void);

#endif
