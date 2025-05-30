#ifndef _MAIN_H
#define _MAIN_H

// 标准库
#include "stdio.h"
#include "stdbool.h"

// MCU型号
#include "stm32f10x.h"

// RTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// 外置硬件
#include "MOTOR.h"
#include "MPU6050.h"

// 软件
#include "Data.h"
#include "MyTask.h"
#include "Delay.h"
#include "Car.h"

// 外设
#include "USART.h"
#include "TIM.h"
#include "GPIO.h"
#include "IIC.h"
#include "ADC.h"

#endif
