#include "Car.h"

/**
 * @brief 管理初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_InitPro(void)
{
}

/**
 * @brief 管理初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_SearchLine(void)
{
    MOTOR_Pulse_Config(7200, -7200);
}
