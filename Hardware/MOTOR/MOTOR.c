#include "MOTOR.h"

/**
 * @brief  电机限幅
 *
 * @param LeftPulse 当前值
 * @param TargetValue 目标值
 *
 * @retval 限幅后的值
 *
 * @note 正数限幅后为正数
 * @note 负数限幅后为负数
 */
int MOTOR_Limit(int NowValue, int TargetValue)
{
    int HighTarget = TargetValue;
    int LowTarget = -TargetValue;

    if (NowValue > HighTarget)
    {
        NowValue = HighTarget;
    }

    if (NowValue < LowTarget)
    {
        NowValue = LowTarget;
    }

    return NowValue;
}

/**
 * @brief  电机左轮正转
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void MOTOR_Left_Positive(void)
{
    GPIO_MOTOR_AIN1(1);
    GPIO_MOTOR_AIN2(0);
}

/**
 * @brief  电机左轮反转
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void MOTOR_Left_Negative(void)
{
    GPIO_MOTOR_AIN1(0);
    GPIO_MOTOR_AIN2(1);
}

/**
 * @brief  电机右轮正转
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void MOTOR_Right_Positive(void)
{
    GPIO_MOTOR_BIN1(1);
    GPIO_MOTOR_BIN2(0);
}

/**
 * @brief  电机右轮反转
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void MOTOR_Right_Negative(void)
{
    GPIO_MOTOR_BIN1(0);
    GPIO_MOTOR_BIN2(1);
}

/**
 * @brief  打开两侧电机
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void MOTOR_ENABLE(void)
{
    GPIO_MOTOR_STBY(1);
}

/**
 * @brief  关闭两侧电机
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void MOTOR_DISABLE(void)
{
    GPIO_MOTOR_STBY(0);
}

/**
 * @brief  左侧车轮速度
 *
 * @param pulse 0~7200脉冲
 *
 * @retval 无
 *
 * @note 左车轮脉冲
 */
void MOTOR_Left_Pulse(uint16_t pulse)
{
    TIM_MOTOR_CH1_Pulse(pulse);
}

/**
 * @brief  右侧车轮速度
 *
 * @param pulse 0~7200脉冲
 *
 * @retval 无
 *
 * @note 右车轮脉冲
 */
void MOTOR_Right_Pulse(uint16_t pulse)
{
    TIM_MOTOR_CH2_Pulse(pulse);
}

/**
 * @brief  两侧车轮脉冲控制
 *
 * @param LeftPulse -7200~7200脉冲
 * @param RightPulse -7200~7200脉冲
 *
 * @retval 无
 *
 * @note 无
 */
void MOTOR_Pulse_Config(int LeftPulse, int RightPulse)
{
    if (LeftPulse >= 0)
    {
        // 左侧车轮正转
        MOTOR_Left_Positive();

        LeftPulse = MOTOR_Limit(LeftPulse, 7200);
        MOTOR_Left_Pulse(LeftPulse);
    }
    else
    {
        // 左侧车轮反转
        MOTOR_Left_Negative();

        LeftPulse = MOTOR_Limit(LeftPulse, 7200);
        MOTOR_Left_Pulse(abs(LeftPulse));
    }

    if (RightPulse >= 0)
    {
        // 右侧车轮正转
        MOTOR_Right_Positive();

        RightPulse = MOTOR_Limit(RightPulse, 7200);
        MOTOR_Right_Pulse(RightPulse);
    }
    else
    {
        // 右侧车轮反转
        MOTOR_Right_Negative();

        RightPulse = MOTOR_Limit(RightPulse, 7200);
        MOTOR_Right_Pulse(abs(RightPulse));
    }
}

void MOTOR_InitPro(void)
{
    MOTOR_ENABLE();
    MOTOR_Pulse_Config(0, 0);
}
