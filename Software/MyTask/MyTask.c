#include "MyTASK.h"

TaskHandle_t TASK1_Handler;
TaskHandle_t TASK2_Handler;
TaskHandle_t TASK3_Handler;
TaskHandle_t TASK4_Handler;
TaskHandle_t TASK5_Handler;
TaskHandle_t TASK6_Handler;
TaskHandle_t TASK7_Handler;
TaskHandle_t TASK8_Handler;
TaskHandle_t TASK9_Handler;
TaskHandle_t TASK10_Handler;
TaskHandle_t TASK11_Handler;
TaskHandle_t TASK12_Handler;
TaskHandle_t TASK13_Handler;
TaskHandle_t TASK14_Handler;
TaskHandle_t TASK15_Handler;
TaskHandle_t TASK_START_Handler;

/**
 * @brief 任务1
 *
 * @note 无
 */
void TASK1(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(1);
    }
}

/**
 * @brief 任务2
 *
 * @note 无
 */
void TASK2(void *pvParameters)
{
    while (1)
    {
        USART2_RecState();
        vTaskDelay(100);
    }
}

/**
 * @brief 任务3
 *
 * @note 无
 */
void TASK3(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(1);
    }
}

/**
 * @brief 任务4
 *
 * @note 无
 */
void TASK4(void *pvParameters)
{
    while (1)
    {
    }
}

/**
 * @brief 任务5
 *
 * @note 无
 */
void TASK5(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(1);
    }
}

/**
 * @brief 任务6
 *
 * @note 无
 */
void TASK6(void *pvParameters)
{
    while (1)
    {
        if (Debug_Flag1 == 1)
        {
            // USART2_SendString("Yaw Record:");
            // USART2_SendNumber(MPU6050_YawAngleLog_Get());
            // USART2_SendString("\r\n");

            GPIO_Buzzer_Config(1);
            vTaskDelay(200);
            GPIO_Buzzer_Config(0);
            Debug_Flag1 = 0;
        }
    }
}

/**
 * @brief 任务7
 *
 * @note 阈值比较
 */
void TASK7(void *pvParameters)
{
    while (1)
    {
        ADC_ITR9909_ThresholdCompare(ADC_ITR9909_CompareValue_MAX, ADC_ITR9909_CompareValue_MINI);
    }
}

/**
 * @brief 任务8
 *
 * @note 十字错路口、直角路口巡线
 */
void TASK8(void *pvParameters)
{
    int SpeedEXE = 0;
    int LeftEXE = 0, RightEXE = 0;
    while (1)
    {
        Car_SpeedExecutionQuantity_ENCODER(&SpeedEXE, 20);

        if (ADC_GetFlag(ADC_FLAG_ITR9909_THRESHOLD) == ADC_FLAGSTATE_ITR9909_NORMAL)
        {
            Car_TurnExecutionQuantity_ITR9909(&SpeedEXE, &LeftEXE, &RightEXE);
        }

        // 输出到电机
        MOTOR_Pulse_Config(LeftEXE, RightEXE);
    }
}

/**
 * @brief 任务9
 *
 * @note 无
 */
void TASK9(void *pvParameters)
{
    while (1)
    {
        ENCODER_Collection_CodeData();
    }
}

/**
 * @brief 任务10
 *
 * @note 无
 */
void TASK10(void *pvParameters)
{

    while (1)
    {
        if (ADC_GetFlag(ADC_FLAG_ITR9909_THRESHOLD) == ADC_FLAGSTATE_ITR9909_TINY)
        {
            vTaskDelay(200); // 消抖
            if (ADC_GetFlag(ADC_FLAG_ITR9909_THRESHOLD) == ADC_FLAGSTATE_ITR9909_TINY)
            {
                Car_SetFlag(CAR_FLAG_ERRLINE, 1); // 错线置位
                vTaskSuspend(TASK8_Handler);      // 挂起普通循迹任务

                Car_ErrorLine_Handler1();

                Car_SetFlag(CAR_FLAG_ERRLINE, 0);
                vTaskResume(TASK8_Handler);
            }
        }
    }
}

/**
 * @brief 任务11
 *
 * @note 消除偏航角零度漂移
 */
void TASK11(void *pvParameters)
{
    while (1)
    {
        MPU6050_Remove_YawDeviaction();
    }
}

/**
 * @brief 任务12
 *
 * @note 无
 */
void TASK12(void *pvParameters)
{
    while (1)
    {
        // Car_ErrorLine_Collection();
    }
}

/**
 * @brief 任务13
 *
 * @note 无
 */
void TASK13(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(1);
    }
}

/**
 * @brief 任务14
 *
 * @note 无
 */
void TASK14(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(1);
    }
}

/**
 * @brief 任务15
 *
 * @note 无
 */
void TASK15(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(1);
    }
}

/**
 * @brief 打开任务
 *
 * @param pvParameters 系统传递数据
 *
 * @retval 无
 *
 * @note 打开其它的所有任务
 */
void Task_Start(void *pvParameters)
{
    taskENTER_CRITICAL(); // 进入临界区(中断关闭)

    xTaskCreate((TaskFunction_t)TASK1,
                (char *)"TASK1",
                (configSTACK_DEPTH_TYPE)TASK1_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK1_PRIORITY,
                (TaskHandle_t *)&TASK1_Handler);

    xTaskCreate((TaskFunction_t)TASK2,
                (char *)"TASK2",
                (configSTACK_DEPTH_TYPE)TASK2_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK2_PRIORITY,
                (TaskHandle_t *)&TASK2_Handler);

    xTaskCreate((TaskFunction_t)TASK3,
                (char *)"TASK3",
                (configSTACK_DEPTH_TYPE)TASK3_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK3_PRIORITY,
                (TaskHandle_t *)&TASK3_Handler);

    xTaskCreate((TaskFunction_t)TASK4,
                (char *)"TASK4",
                (configSTACK_DEPTH_TYPE)TASK4_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK4_PRIORITY,
                (TaskHandle_t *)&TASK4_Handler);

    xTaskCreate((TaskFunction_t)TASK5,
                (char *)"TASK5",
                (configSTACK_DEPTH_TYPE)TASK5_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK5_PRIORITY,
                (TaskHandle_t *)&TASK5_Handler);

    xTaskCreate((TaskFunction_t)TASK6,
                (char *)"TASK6",
                (configSTACK_DEPTH_TYPE)TASK6_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK6_PRIORITY,
                (TaskHandle_t *)&TASK6_Handler);

    xTaskCreate((TaskFunction_t)TASK7,
                (char *)"TASK7",
                (configSTACK_DEPTH_TYPE)TASK7_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK7_PRIORITY,
                (TaskHandle_t *)&TASK7_Handler);

    xTaskCreate((TaskFunction_t)TASK8,
                (char *)"TASK8",
                (configSTACK_DEPTH_TYPE)TASK8_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK8_PRIORITY,
                (TaskHandle_t *)&TASK8_Handler);

    xTaskCreate((TaskFunction_t)TASK9,
                (char *)"TASK9",
                (configSTACK_DEPTH_TYPE)TASK9_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK9_PRIORITY,
                (TaskHandle_t *)&TASK9_Handler);

    xTaskCreate((TaskFunction_t)TASK10,
                (char *)"TASK10",
                (configSTACK_DEPTH_TYPE)TASK10_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK10_PRIORITY,
                (TaskHandle_t *)&TASK10_Handler);

    xTaskCreate((TaskFunction_t)TASK11,
                (char *)"TASK11",
                (configSTACK_DEPTH_TYPE)TASK11_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK11_PRIORITY,
                (TaskHandle_t *)&TASK11_Handler);

    xTaskCreate((TaskFunction_t)TASK12,
                (char *)"TASK12",
                (configSTACK_DEPTH_TYPE)TASK12_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK12_PRIORITY,
                (TaskHandle_t *)&TASK12_Handler);

    xTaskCreate((TaskFunction_t)TASK13,
                (char *)"TASK13",
                (configSTACK_DEPTH_TYPE)TASK13_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK13_PRIORITY,
                (TaskHandle_t *)&TASK13_Handler);

    xTaskCreate((TaskFunction_t)TASK14,
                (char *)"TASK14",
                (configSTACK_DEPTH_TYPE)TASK14_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK14_PRIORITY,
                (TaskHandle_t *)&TASK14_Handler);

    xTaskCreate((TaskFunction_t)TASK15,
                (char *)"TASK15",
                (configSTACK_DEPTH_TYPE)TASK15_STACK_SIZE,
                (void *)NULL,
                (UBaseType_t)TASK15_PRIORITY,
                (TaskHandle_t *)&TASK15_Handler);

    vTaskDelete(NULL);   // 删除任务
    taskEXIT_CRITICAL(); // 退出临界区(中断打开)
}

/**
 * @brief 任务初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 调用后，将开始RTOS任务调度
 */
void TASK_InitPro(void)
{
    /*中断优先级分组(全抢占优先级模式)*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    xTaskCreate((TaskFunction_t)Task_Start,                    // 函数名称
                (char *)"Task_Start",                          // 任务名称
                (configSTACK_DEPTH_TYPE)TASK_START_STACK_SIZE, // 栈空间
                (void *)NULL,                                  // 传给任务的参数
                (UBaseType_t)TASK_START_PRIORITY,              // 优先级
                (TaskHandle_t *)&TASK_START_Handler);          // 任务句柄

    vTaskStartScheduler(); // 启动任务调度器
}
