#ifndef _MyTASK_H
#define _MyTASK_H

// 模板取样处
// ⬇️

// 宏定义
//...

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
#include "UltraSound.h"
#include "MOTOR.h"
#include "MPU6050.h"
#include "ENCODER.h"
#include "CC2530.h"

// 软件
#include "MyTask.h"
#include "Delay.h"
#include "Receive.h"
#include "Manual.h"

// 外设
#include "USART.h"
#include "TIM.h"
#include "GPIO.h"
#include "IIC.h"

// 变量
//...

// RTOS任务函数
void TASK1(void *pvParameters);
void TASK2(void *pvParameters);
void TASK3(void *pvParameters);
void TASK4(void *pvParameters);
void TASK5(void *pvParameters);
void TASK6(void *pvParameters);
void TASK7(void *pvParameters);
void TASK8(void *pvParameters);
void TASK9(void *pvParameters);
void TASK10(void *pvParameters);
void TASK11(void *pvParameters);
void TASK12(void *pvParameters);
void TASK13(void *pvParameters);
void TASK14(void *pvParameters);
void TASK15(void *pvParameters);
void Task_Start(void *pvParameters);

// RTOS可压栈空间(局部变量、调用函数信息、上下文信息)
#define TASK1_STACK_SIZE 128
#define TASK2_STACK_SIZE 128
#define TASK3_STACK_SIZE 128
#define TASK4_STACK_SIZE 128
#define TASK5_STACK_SIZE 128
#define TASK6_STACK_SIZE 128
#define TASK7_STACK_SIZE 128
#define TASK8_STACK_SIZE 128
#define TASK9_STACK_SIZE 128
#define TASK10_STACK_SIZE 128
#define TASK11_STACK_SIZE 128
#define TASK12_STACK_SIZE 128
#define TASK13_STACK_SIZE 128
#define TASK14_STACK_SIZE 128
#define TASK15_STACK_SIZE 128
#define TASK_START_STACK_SIZE 128

// RTOS优先级
#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 1
#define TASK3_PRIORITY 1
#define TASK4_PRIORITY 1
#define TASK5_PRIORITY 1
#define TASK6_PRIORITY 1
#define TASK7_PRIORITY 1
#define TASK8_PRIORITY 1
#define TASK9_PRIORITY 1
#define TASK10_PRIORITY 1
#define TASK11_PRIORITY 1
#define TASK12_PRIORITY 1
#define TASK13_PRIORITY 1
#define TASK14_PRIORITY 1
#define TASK15_PRIORITY 1
#define TASK_START_PRIORITY 1

// RTOS任务句柄
extern TaskHandle_t TASK1_Handler;
extern TaskHandle_t TASK2_Handler;
extern TaskHandle_t TASK3_Handler;
extern TaskHandle_t TASK4_Handler;
extern TaskHandle_t TASK5_Handler;
extern TaskHandle_t TASK6_Handler;
extern TaskHandle_t TASK7_Handler;
extern TaskHandle_t TASK8_Handler;
extern TaskHandle_t TASK9_Handler;
extern TaskHandle_t TASK10_Handler;
extern TaskHandle_t TASK11_Handler;
extern TaskHandle_t TASK12_Handler;
extern TaskHandle_t TASK13_Handler;
extern TaskHandle_t TASK14_Handler;
extern TaskHandle_t TASK15_Handler;
extern TaskHandle_t TASK_START_Handler;

// API
void TASK_InitPro(void);

#endif
