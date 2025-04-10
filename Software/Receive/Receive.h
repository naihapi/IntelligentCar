#ifndef __Receive_H
#define __Receive_H

// MCU型号
#include "stm32f10x.h"

// 宏定义
#define ItemType_Switch 1 // 菜单项类型-开关型
#define ItemType_Slider 2 // 菜单项类型-滑块型
#define ItemType_View 3   // 菜单项类型-视图型
#define ItemType_Custom 4 // 菜单项类型-自定义型

typedef struct MenuItem
{
    uint8_t *Name;         // 接收名称
    uint8_t ItemType;      // 菜单项类型(ItemType_xxx)
    void *Function;        // 指向一个函数
    void *ItemPointer;     // 具体组件数据(Menu_xxxModule)
    struct MenuItem *Last; // 指向上一个菜单项
    struct MenuItem *Next; // 指向下一个菜单项
} MenuItem;

// 标准库
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
// 外置硬件
#include "CC2530.h"

// 软件
#include "Manual.h"

// 外设
#include "USART.h"

// API
void Receive_Function(void);
uint8_t *Receive_FindChar(uint8_t *str, char check, uint8_t number);
uint8_t Receive_Parse_CheckModuleType(uint8_t *check);
int32_t Receive_FindNumber(uint8_t *start, uint8_t *end);
uint8_t Receive_Parse(uint8_t *data, uint8_t **datapack);

#endif
