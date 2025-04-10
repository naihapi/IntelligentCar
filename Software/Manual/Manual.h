#ifndef __Manual_H
#define __Manual_H

// MCU型号
#include "stm32f10x.h"

// RTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// 变量
extern uint8_t Manual_State;
extern uint8_t Manual_Left, Manual_Right, Manual_Front, Manual_Rear;

// 外置硬件
#include "UltraSound.h"

// 软件
#include "Receive.h"
#include "Reply.h"
#include "PID.h"

// API
void Manual_InitPro(void);
void Manual_CustomFunction(uint8_t *DataPack);
void MPU6050_FallDown_Handler(void);

#endif
