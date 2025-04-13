#ifndef __ENCODER_H
#define __ENCODER_H

// MCU型号
#include "stm32f10x.h"

// 外设
#include "TIM.h"
#include "GPIO.h"

// RTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// 宏定义
#define ENCODER_Perimeter (float)13.5 // N20电机车轮周长
#define ENCODER_Cycle (float)1400.0   // N20电机一圈计数值

// 变量
extern uint16_t ENCODER_Speed;
extern uint16_t Interrupt_L_ENCODER_CNT;
extern uint16_t Interrupt_R_ENCODER_CNT;
extern uint16_t Real_L_ENCODER_CNT;
extern uint16_t Real_R_ENCODER_CNT;

// API
void ENCODER_InitPro(void);
void ENCODER_Collection_CodeData(void);
int PID_RetExecutionQuantity_SpeedControl(float TargetValue);

#endif
