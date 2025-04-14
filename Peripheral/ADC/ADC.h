#ifndef __ADC_H
#define __ADC_H

// MCU型号
#include "stm32f10x.h"

// 软件
#include "Delay.h"

// 外设
#include "GPIO.h"

// 宏定义
#define ADC_ITRBuffer_LeftValue (uint8_t)0
#define ADC_ITRBuffer_MiddleValue (uint8_t)1
#define ADC_ITRBuffer_RightValue (uint8_t)2
#define ADC_ITR9909_CompareValue (uint16_t)2000
#define ADC_ITR9909_AwayLine (uint8_t)0
#define ADC_ITR9909_CloseLine (uint8_t)1
#define ADC_ITR9909_OnLine (uint8_t)2

// 变量
extern uint16_t ADC_ITR9909_Value[3];
extern uint8_t ADC_Threshold_Flag;
extern uint8_t Debug_Flag1;

// API
void ADC_InitPro(void);
void ADC_ITR9909_ThresholdCompare(uint16_t max, uint16_t mini);
uint8_t ADC_ITR9909_Compare(void);

#endif
