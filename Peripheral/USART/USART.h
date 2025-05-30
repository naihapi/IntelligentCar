#ifndef _USART_H
#define _USART_H

// 标准库
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"

// MCU型号
#include "stm32f10x.h"

// 变量
extern uint8_t USART2_RecFlag;
extern uint8_t USART2_RecBuffer[1024];
extern uint8_t USART3_RecFlag;
extern uint8_t USART3_RecBuffer[1024];
extern uint16_t USART3_CamData;

// 宏定义
//...

// API
void USART_InitPro(void);
void USART2_Clear_RecBuffer(void);
void USART3_Clear_RecBuffer(void);
uint8_t *USART2_Get_RecBuffer(void);
uint8_t *USART3_Get_RecBuffer(void);
void USART2_SendByte(uint16_t Byte);
void USART3_SendByte(uint16_t Byte);
void USART2_SendString(uint8_t *String);
void USART3_SendString(uint8_t *String);
void USART2_SendNumber(int32_t Number);
void USART3_SendNumber(int32_t Number);
void USART2_RecState(void);
void USART3_RecState(void);
void USART_CollectData(void);

#endif
