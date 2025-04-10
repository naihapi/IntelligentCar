#ifndef _USART_H_
#define _USART_H_

// 标准库
#include "string.h"
#include "stdio.h"
#include "stdarg.h"

// MCU型号
#include "stm32f10x.h"

// 变量
extern uint8_t USART2_RecFlag;
extern uint8_t USART2_RecBuffer[1024];

// 宏定义
//...

// API
void USART_InitPro(void);
void USART2_Clear_RecBuffer(void);
void USART2_SendByte(uint16_t Byte);
void USART2_SendString(uint8_t *String);
void USART2_SendNumber(int32_t Number);
void USART2_RecState(void);

#endif
