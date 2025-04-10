#include "Reply.h"

uint8_t Reply_Success[8] = {"OK"};
uint8_t Reply_Fail[8] = {"Fail"};

/**
 * @brief 回复成功消息
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Reply_SendSuccess(void)
{
    CC2530_SendString(Reply_Success);
}

/**
 * @brief 回复失败消息
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Reply_SendFail(void)
{
    CC2530_SendString(Reply_Fail);
}

/**
 * @brief 回复消息初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Reply_InitPro(void)
{
}
