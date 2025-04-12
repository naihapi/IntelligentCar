#ifndef __Data_H
#define __Data_H

//  MCU型号
#include "stm32f10x.h"

// 标准库
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

// 宏定义
#define SwitchType 1
#define ViewType 2
#define SliderType 3

// 结构体
typedef struct DataBase_t
{
    uint8_t name[16];        // 名称
    void *module;            // 指向具体组件(xxxModule_t)
    uint8_t type;            // 组件类型(xxxType)
    struct DataBase_t *last; // 链表上一个
    struct DataBase_t *next; // 链表下一个
} DataBase_t;
typedef struct SwitchModule_t
{
    uint8_t state; // 状态
} SwitchModule_t;
typedef struct ViewModule_t
{
    int value; // 数值
} ViewModule_t;
typedef struct SliderModule_t
{
    int nowvalue;  // 当前值
    int maxvalue;  // 最大值
    int minivalue; // 最小值
} SliderModule_t;

// 变量
extern DataBase_t *Head_DataBase;
extern SwitchModule_t Data_Buzzer;
extern SwitchModule_t Data_TipsLED;

// API
void Data_InitPro(void);
void Data_ConfigModule_Switch(SwitchModule_t *module, uint8_t state);
void Data_ConfigModule_View(ViewModule_t *module, int value);
void Data_ConfigModule_Slider(SliderModule_t *module, int now, int max, int mini);
DataBase_t *Data_DataBase_GetEndItem(DataBase_t *head);
DataBase_t *Data_DataBase_CreateHead(void);
DataBase_t *Data_DataBase_CreateItem(uint8_t *name, void *module, uint8_t type);
void *Data_DataBase_GetItem(DataBase_t *head, uint8_t *name);
void Data_DataBase_AddItem(DataBase_t *head, DataBase_t *item);

#endif
