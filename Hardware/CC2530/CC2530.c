#include "CC2530.h"

/**
 * @brief 读取接收状态
 *
 * @param 无
 *
 * @retval 0 无接收数据
 * @retval 1 接收完成
 * @retval 2 正在接收
 *
 * @note 无
 */
uint8_t CC2530_RecState(void)
{
    return USART2_RecFlag;
}

/**
 * @brief 清除接收状态
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void CC2530_Clear_RecState(void)
{
    USART2_RecFlag = 0;
}

/**
 * @brief 发送有符号数字
 *
 * @param number 数字
 *
 * @retval 无
 *
 * @note 无
 */
void CC2530_SendNumber(int32_t number)
{
    USART2_SendNumber(number);
}

uint8_t *CC2530_GetBuffer(void)
{
    return USART2_RecBuffer;
}

/**
 * @brief 发送字符串
 *
 * @param string 字符串
 *
 * @retval 无
 *
 * @note 无
 */
void CC2530_SendString(uint8_t *string)
{
    USART2_SendString(string);
}

/**
 * @brief CC2530初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void CC2350_InitPro(void) {}
