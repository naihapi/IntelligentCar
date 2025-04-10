/*-----------------------------------------------------------
 * 应用程序特定的定义。
 *
 * 这些定义应根据您的特定硬件和应用需求进行调整。
 *
 * 这些参数在 FreeRTOS 官网上的 FreeRTOS API 文档的“配置”部分中有详细描述。
 *
 * 请参见 http://www.freertos.org/a00110.html
 *-----------------------------------------------------------*/
#ifndef _FREERTOS_CONFIG_H
#define _FREERTOS_CONFIG_H

#include "stm32f10x.h"

#define xPortPendSVHandler PendSV_Handler   // 将PendSV_Handler重命名为xPortPendSVHandler
#define vPortSVCHandler SVC_Handler         // 将SVC_Handler重命名为vPortSVCHandler
#define xPortSysTickHandler SysTick_Handler // 将SysTick_Handler重命名为xPortSysTickHandler
#define INCLUDE_xTaskGetCurrentTaskHandle 1 // 包含获取当前任务句柄的函数

// #define configSUPPORT_STATIC_ALLOCATION 1 // 打开静态分配内存（注释掉表示不使用）
#define configSUPPORT_DYNAMIC_ALLOCATION 1 // 打开动态分配内存

#define configUSE_PREEMPTION 1                         // 使用抢占式调度器
#define configUSE_IDLE_HOOK 0                          // 不使用空闲任务钩子函数
#define configUSE_TICK_HOOK 0                          // 不使用滴答定时器钩子函数
#define configCPU_CLOCK_HZ ((unsigned long)72000000)   // CPU时钟频率设置为72MHz
#define configTICK_RATE_HZ ((TickType_t)1000)          // 滴答定时器频率设置为1000Hz（即1ms一次）
#define configMAX_PRIORITIES (5)                       // 设置任务的最大优先级数量
#define configMINIMAL_STACK_SIZE ((unsigned short)128) // 设置任务的最小堆栈大小
#define configTOTAL_HEAP_SIZE ((size_t)(13 * 1024))    // 设置总堆大小
#define configMAX_TASK_NAME_LEN (16)                   // 设置任务名的最大长度
#define configUSE_TRACE_FACILITY 0                     // 不使用跟踪功能
#define configUSE_16_BIT_TICKS 0                       // 不使用16位滴答计数器
#define configIDLE_SHOULD_YIELD 1                      // 空闲任务应该让出CPU时间
#define INCLUDE_uxTaskGetStackHighWaterMark 1          // 启用任务栈空间查询功能

/*将以下定义设置为1以包含API函数，或设置为0以排除API函数。*/
#define INCLUDE_vTaskPrioritySet 1      // 包含设置任务优先级的函数
#define INCLUDE_uxTaskPriorityGet 1     // 包含获取任务优先级的函数
#define INCLUDE_vTaskDelete 1           // 包含删除任务的函数
#define INCLUDE_vTaskCleanUpResources 0 // 不包含清理任务资源的函数
#define INCLUDE_vTaskSuspend 1          // 包含挂起任务的函数
#define INCLUDE_vTaskDelayUntil 1       // 包含延迟到指定时间的函数
#define INCLUDE_vTaskDelay 1            // 包含延迟一段时间的函数

/* 中断嵌套行为配置 */
#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS __NVIC_PRIO_BITS // 如果定义了__NVIC_PRIO_BITS，则使用它作为优先级位数
#else
#define configPRIO_BITS 4 // 否则，默认优先级位数为4
#endif
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15                                                                   // 中断最低优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5                                                               // FreeRTOS可管理的最高中断优先级
#define configKERNEL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))           // 内核中断优先级，根据优先级位数调整
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS)) // 系统调用最大中断优先级，根据优先级位数调整
#define configMAX_API_CALL_INTERRUPT_PRIORITY configMAX_SYSCALL_INTERRUPT_PRIORITY                                   // API调用最大中断优先级与系统调用最大中断优先级相同

#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY 15 // 根据ST库设置的内核中断优先级值

#endif /* FREERTOS_CONFIG_H */
