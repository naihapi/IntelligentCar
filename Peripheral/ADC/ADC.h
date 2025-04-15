#ifndef __ADC_H
#define __ADC_H

// MCU型号
#include "stm32f10x.h"

// 软件
#include "Delay.h"
#include "Car.h"

// 外设
#include "GPIO.h"

// 宏定义
#define ADC_FLAG_ITR9909_THRESHOLD (uint8_t)1       // 对管阈值标志位
#define ADC_FLAG_DIRECTION (uint8_t)2               // 对管方向标志位
#define ADC_ITRBuffer_LeftValue (uint8_t)0          // ADC数组-左对管值
#define ADC_ITRBuffer_MiddleValue (uint8_t)1        // ADC数组-中对管值
#define ADC_ITRBuffer_RightValue (uint8_t)2         // ADC数组-右对管值
#define ADC_ITR9909_CompareValue_MAX (uint16_t)2000 // ADC比较值-最高阈值
#define ADC_ITR9909_CompareValue_MINI (uint16_t)300 // ADC比较值-最低阈值
#define ADC_FLAGSTATE_DIRECTION_LEFT (uint8_t)0     // 对管左方向
#define ADC_FLAGSTATE_DIRECTION_RIGHT (uint8_t)1    // 对管右方向
#define ADC_FLAGSTATE_DIRECTION_BOTH (uint8_t)2     // 对管双方向
#define ADC_FLAGSTATE_ITR9909_TINY (uint8_t)0       // ADC输出过低
#define ADC_FLAGSTATE_ITR9909_OVERFLOW (uint8_t)1   // ADC输出过高
#define ADC_FLAGSTATE_ITR9909_NORMAL (uint8_t)2     // ADC输出常规

// 变量
extern uint16_t ADC_ITR9909_Value[3];
extern uint8_t ADC_Threshold_Flag;
extern uint8_t Debug_Flag1;

// API
void ADC_InitPro(void);
void ADC_ITR9909_ThresholdCompare(uint16_t max, uint16_t mini);
void ADC_SetFlag(uint8_t flag, uint8_t state);
uint8_t ADC_GetFlag(uint8_t flag);
void ADC_TIR9909Log_Record(void);
uint16_t ADC_TIR9909Log_Get(uint8_t select);

#endif
