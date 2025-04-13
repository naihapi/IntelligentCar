#include "Car.h"

/**
 * @brief 车体初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Car_InitPro(void)
{
}

/**
 * @brief 车体旋转
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 360°向左旋转
 */
void Car_Spin360(void)
{
    MOTOR_Pulse_Config(7200, -7200);
}

/**
 * @brief 车体停止旋转
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Car_SpinStop(void)
{
    MOTOR_Pulse_Config(0, 0);
}

/**
 * @brief 车体寻线
 *
 * @param angle 在此角度内不巡线
 *
 * @retval 无
 *
 * @note 从空白地面旋转处巡线
 * @note 若参数为0°，则0°±30范围内失能巡线
 */
void Car_SearchLine(void)
{
    Car_Spin360();

    while (1)
    {
        if (ADC_ITR9909_Value[0] > 1400 || ADC_ITR9909_Value[1] > 1400 || ADC_ITR9909_Value[2] > 1400)
        {
            Debug_Flag1 = 1;
            Car_SpinStop();

            break;
        }
    }
}

/**
 * @brief 车体脱离线
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 从带线地面寻找新线
 */

void Car_BreakLine(void)
{
    Car_Spin360();

    while (1)
    {
        if (ADC_ITR9909_Value[0] < 300 && ADC_ITR9909_Value[1] < 300 & ADC_ITR9909_Value[2] < 300)
        {
            Debug_Flag1 = 1;
            Car_SpinStop();

            break;
        }
    }
}
