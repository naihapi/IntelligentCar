#ifndef __ENCODER_H
#define __ENCODER_H

// 标准库
#include "stm32f10x.h"

// 外设
#include "TIM.h"
#include "GPIO.h"

// RTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// 宏定义
//...

// 变量
//...

// API
void ENCODER_InitPro(void);
#endif
