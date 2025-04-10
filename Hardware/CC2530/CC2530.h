#ifndef __CC2530_H
#define __CC2530_H

// MCU型号
#include "stm32f10x.h"

// 外设
#include "USART.h"

// API
void CC2350_InitPro(void);
uint8_t CC2530_RecState(void);
void CC2530_Clear_RecState(void);
uint8_t *CC2530_GetBuffer(void);
void CC2530_SendNumber(int32_t number);
void CC2530_SendString(uint8_t *string);

#endif
