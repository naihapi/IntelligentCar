#ifndef _GPIO_H
#define _GPIO_H

// MCU型号
#include "stm32f10x.h"

// 软件
#include "Delay.h"

// API
void GPIO_IIC_Init(void);
void GPIO_ENCODER_Init(void);
void GPIO_MPU6050_Init(void);
void GPIO_MOTOR_Init(void);
void GPIO_MOTOR_STBY(uint8_t State);
void GPIO_MOTOR_AIN1(uint8_t State);
void GPIO_MOTOR_AIN2(uint8_t State);
void GPIO_MOTOR_BIN1(uint8_t State);
void GPIO_MOTOR_BIN2(uint8_t State);
void GPIO_LED_RearLED(uint8_t State);
void GPIO_LED_FrontLED(uint8_t State);
void GPIO_LED_TipsLED(uint8_t State);
void GPIO_Buzzer_Config(uint8_t State);
void GPIO_UltraSound_Trig(uint8_t State);
uint8_t GPIO_UltraSound_Echo(void);
void GPIO_SCL_Config(uint8_t signal);
void GPIO_SDA_Config(uint8_t signal);
uint8_t GPIO_SDA_Read(void);
void GPIO_InitPro(void);

#endif
