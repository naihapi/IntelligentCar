#ifndef __ENCODER_H
#define __ENCODER_H

// 标准库
#include "stm32f10x.h"

// 外设
#include "TIM.h"

// RTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// 宏定义
#define ENCODER_OneRound_CNT (float)1456.0 // 编码器1圈计数(线数*减速比*边沿模式=13*28*4=1456)
#define ENCODER_WheelLengt (float)6.5

// 变量
extern int16_t ENCODER_CNT;

// API
void ENCODER_InitPro(void);
void ENCODER_Collection_CodeData(void);
void ENCODER_GetSpeed(float *speed);
#endif
