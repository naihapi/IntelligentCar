#include "ENCODER.h"

uint16_t L_ENCODER_CNT = 0; // 无论正或反转一圈,都产生1400个计数
uint16_t R_ENCODER_CNT = 0; // 无论正或反转一圈,都产生1400个计数

void ENCODER_InitPro(void) {}

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
