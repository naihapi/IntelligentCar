#include "UltraSound.h"

uint8_t UltraSound_MeasureFinish = 0; // 测量完成标志位(0：未完成，1：已完成)
float UltraSound_Distance = 0;        // 超声波测出距离

/**
 * @brief 超声波触发1次测距
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void UltraSound_Trig(void)
{
    GPIO_UltraSound_Trig(0);

    GPIO_UltraSound_Trig(1);
    for (volatile int i = 0; i < 300; i++)
        __NOP(); // 约20us延时

    GPIO_UltraSound_Trig(0);
}

/**
 * @brief 超声波测距
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 距离长度存储在全局变量：UltraSound_Distance
 */

void UltraSound_GetDistance(void)
{
    if (UltraSound_MeasureFinish == 1)
    {
        UltraSound_Distance = TIM_Count_GetCNT() * 0.017;

        TIM_Count_SetCNT(0);
        UltraSound_MeasureFinish = 0;
    }
}

/**
 * @brief 超声波测距1次
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */

void UltraSound_Function(void)
{
    taskENTER_CRITICAL();
    UltraSound_Trig();
    taskEXIT_CRITICAL();

    UltraSound_GetDistance();
}

/**
 * @brief 超声波初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */

void ULtraSound_InitPro(void)
{
}

/**
 * @brief 外部中断线0
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET)
    {
        // 超声波信号返回开始
        if (GPIO_UltraSound_Echo() == 1)
        {
            TIM_Count_ON();
        }

        // 超声波信号返回结束
        if (GPIO_UltraSound_Echo() == 0)
        {
            TIM_Count_OFF();

            UltraSound_MeasureFinish = 1;
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}
