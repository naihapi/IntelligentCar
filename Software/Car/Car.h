#ifndef __Car_H
#define __Car_H

// MCU型号
#include "stm32f10x.h"

// RTOS
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

// 变量
//...

// 宏定义
#define CAR_FLAG_SPIN 1         // 车辆旋转标志位
#define CAR_FLAG_ERRLINE 2      // 车辆错线标志位
#define CAR_FALG_MPUYAWRECORD 3 // 车辆记录MPU偏航角标志位
#define Car_LeftSpin 1          // 车辆左旋转
#define Car_RightSpin 2         // 车辆右旋转

// 外置硬件
//...

// 外设
#include "MOTOR.h"
#include "ADC.h"

// 软件
#include "PID.h"
#include "MyTask.h"

// API
void Manual_InitPro(void);
void Car_Stop(void);
void Car_SearchLine_Spin(uint8_t dir);
void Car_BreakLine_Spin(void);
void Car_SearchLine_Spin_Range(int16_t angle);
void Car_SpeedExecutionQuantity_ENCODER(int *Speed, uint8_t TargetSpeed);
void Car_TurnExecutionQuantity_ITR9909(int *Speed, int *Left, int *Right);
void Car_StraightBack(void);
void Car_StraightFront(void);
void Car_SetFlag(uint8_t flag, uint8_t state);
uint8_t Car_GetFlag(uint8_t flag);
void Car_ErrorLine_Handler1(void);
void Car_ErrorLine_Handler2(void);
void Car_SearchLine_NormalMode(int *SpeedEXE, int *LeftEXE, int *RightEXE);
void Car_SearchLine_ErrorLineMode(void);

#endif
