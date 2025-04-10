#ifndef __Reply_H
#define __Reply_H

// MCU型号
#include "stm32f10x.h"

// RTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// 外置硬件
#include "CC2530.h"

// API
void Reply_InitPro(void);
void Reply_SendSuccess(void);
void Reply_SendFail(void);

#endif
