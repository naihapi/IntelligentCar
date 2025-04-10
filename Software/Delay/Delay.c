#include "Delay.h"

volatile uint32_t Delay_CNT = 0;

/**
 * @brief NOP指令微秒级延时
 *
 * @param 微秒数值
 *
 * @retval 无
 *
 * @note 使用NOP延时
 * @note 准确性较低，适合粗略延时
 */
void Delay_us(uint32_t xus)
{
    __NOP();
    __NOP();
}

/**
 * @brief 定时器2毫秒级延时
 *
 * @param 毫秒数值
 *
 * @retval 无
 *
 * @note 使用TIM2计数延时
 */
void Delay_ms(uint32_t xms)
{
    uint32_t cnt = Delay_CNT;
    cnt += xms;

    while (Delay_CNT < cnt)
    {
    }
}

/**
 * @brief 定时器2秒级延时
 *
 * @param 秒数值
 *
 * @retval 无
 *
 * @note 使用TIM2计数延时
 */
void Delay_s(uint32_t xs)
{
    while (xs--)
    {
        Delay_ms(1000);
    }
}

/**
 * @brief 获取当前延时数值
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 用于对接DMP解算库
 * @note 由于编译警告，将uint32_t改为unsigned long
 */
void Delay_Getxms(unsigned long *count)
{
    *count = Delay_CNT;
}

/**
 * @brief 定时器2中断服务函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 1ms中断1次
 */
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        Delay_CNT++;
    }

    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
