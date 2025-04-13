#ifndef _Delay_H
#define _Delay_H

// MCU型号
#include "stm32f10x.h"

// 标准库
#include "stdio.h"

// 外设
#include "GPIO.h"

// 变量
extern volatile uint32_t Delay_CNT;

// API
void Delay_us(uint32_t xus);
void Delay_ms(uint32_t xms);
void Delay_s(uint32_t xs);
uint32_t Delay_Getxms(unsigned long *count);
void Delay_TimeLog_Record(void);
uint32_t Delay_TimeLog_Get(void);

#endif
