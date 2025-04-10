#ifndef _TIM_H
#define _TIM_H

// MCU型号
#include "stm32f10x.h"

// API
void TIM_MOTOR_CH1_Pulse(uint16_t Pulse);
void TIM_MOTOR_CH2_Pulse(uint16_t Pulse);
void TIM_Count_SetCNT(uint16_t cnt);
uint16_t TIM_Count_GetCNT(void);
void TIM_Count_ON(void);
void TIM_Count_OFF(void);
void TIM_Encoder_GetCount(int16_t *count);
void TIM_Encoder_SetCount(int16_t count);
void TIM_InitPro(void);

#endif
