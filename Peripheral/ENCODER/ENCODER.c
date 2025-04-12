#include "ENCODER.h"

uint16_t L_ENCODER_CNT = 0; // 无论正或反转一圈,都产生1400个计数
uint16_t R_ENCODER_CNT = 0; // 无论正或反转一圈,都产生1400个计数
uint8_t ENCODER_Speed = 0;  // 编码电机速度(cm/s)

void ENCODER_InitPro(void) {}

/**
 * @brief 编码电机测速
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void ENCODER_Collection_CodeData(void)
{
    float encoder = 0;
    float speed = 0;

    // 取两路编码平均值
    encoder = (L_ENCODER_CNT + R_ENCODER_CNT) / 2;

    // 速度：XXcm/100ms
    speed = (encoder / ENCODER_Cycle) * ENCODER_Perimeter;

    // 预测速度：XXcm/1s
    ENCODER_Speed = (uint8_t)(speed * 10);

    // 清空编码计数值
    L_ENCODER_CNT = 0;
    R_ENCODER_CNT = 0;

    vTaskDelay(100);
}

void EXTI9_5_IRQHandler(void)
{
    // E1A-PB6
    if (EXTI_GetITStatus(EXTI_Line6) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
        {
            L_ENCODER_CNT++;
            GPIO_LED_TipsLED(1);
        }
        EXTI_ClearITPendingBit(EXTI_Line6);
    }

    // E1B-PB7
    if (EXTI_GetITStatus(EXTI_Line7) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
        {
            L_ENCODER_CNT++;
            GPIO_LED_TipsLED(1);
        }
        EXTI_ClearITPendingBit(EXTI_Line7);
    }

    // E2A-PB8
    if (EXTI_GetITStatus(EXTI_Line8) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
        {
            R_ENCODER_CNT++;
            GPIO_LED_TipsLED(1);
        }
        EXTI_ClearITPendingBit(EXTI_Line8);
    }

    // E2A-PB9
    if (EXTI_GetITStatus(EXTI_Line9) == SET)
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)
        {
            R_ENCODER_CNT++;
            GPIO_LED_TipsLED(1);
        }
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
}
