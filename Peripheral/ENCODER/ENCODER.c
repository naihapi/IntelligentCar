#include "ENCODER.h"

// 无论正或反转一圈,都产生1400个计数
uint16_t Interrupt_L_ENCODER_CNT = 0; // 左侧中断编码计数
uint16_t Interrupt_R_ENCODER_CNT = 0; // 右侧中断编码计数
uint16_t Real_L_ENCODER_CNT = 0;      // 左侧定时计数
uint16_t Real_R_ENCODER_CNT = 0;      // 右侧定时计数
uint16_t ENCODER_Speed = 0;           // 编码电机速度(cm/s)

void ENCODER_InitPro(void) {}

/**
 * @brief 定时采集编码计数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void ENCODER_Collection_CodeData(void)
{
    // 实际编码计数值
    Real_L_ENCODER_CNT = Interrupt_L_ENCODER_CNT;
    Real_R_ENCODER_CNT = Interrupt_R_ENCODER_CNT;

    // 速度预测
    float average_encoder = (Real_L_ENCODER_CNT + Real_R_ENCODER_CNT) / 2.0;
    ENCODER_Speed = ((average_encoder / ENCODER_Cycle) * ENCODER_Perimeter) * 100.0;

    // 清空编码计数值
    Interrupt_L_ENCODER_CNT = 0;
    Interrupt_R_ENCODER_CNT = 0;

    vTaskDelay(10);
}

void EXTI9_5_IRQHandler(void)
{
    // E1A-PB6
    if (EXTI_GetITStatus(EXTI_Line6) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
        {
            Interrupt_L_ENCODER_CNT++;
            GPIO_LED_TipsLED(1);
        }
        EXTI_ClearITPendingBit(EXTI_Line6);
    }

    // E1B-PB7
    if (EXTI_GetITStatus(EXTI_Line7) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
        {
            Interrupt_L_ENCODER_CNT++;
            GPIO_LED_TipsLED(1);
        }
        EXTI_ClearITPendingBit(EXTI_Line7);
    }

    // E2A-PB8
    if (EXTI_GetITStatus(EXTI_Line8) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
        {
            Interrupt_R_ENCODER_CNT++;
            GPIO_LED_TipsLED(1);
        }
        EXTI_ClearITPendingBit(EXTI_Line8);
    }

    // E2A-PB9
    if (EXTI_GetITStatus(EXTI_Line9) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)
        {
            Interrupt_R_ENCODER_CNT++;
            GPIO_LED_TipsLED(1);
        }
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}
