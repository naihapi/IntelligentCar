#ifndef _MOTOR_H
#define _MOTOR_H

// MCU型号
#include "stm32f10x.h"

// 外设
#include "GPIO.h"
#include "TIM.h"

// 标准库
#include "stdlib.h"

// API
void MOTOR_InitPro(void);
void MOTOR_Left_Positive(void);
void MOTOR_Left_Negative(void);
void MOTOR_Right_Positive(void);
void MOTOR_Right_Negative(void);
void MOTOR_ENABLE(void);
void MOTOR_DISABLE(void);
void MOTOR_Left_Pulse(uint16_t pulse);
void MOTOR_Right_Pulse(uint16_t pulse);
void MOTOR_Pulse_Config(int LeftPulse, int RightPulse);

#endif
