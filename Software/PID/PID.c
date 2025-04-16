#include "PID.h"

float PID_Speed_ExecutionQuantity = 0; // 速度环执行量

/*速度环*/
float PID_Speed_P = 900, PID_Speed_I = 2, PID_Speed_D = 0;
float PID_Speed_Error = 0;
float PID_Speed_Error_Sum = 0;
float PID_Speed_Error_Last = 0;
float PID_Speed_Error_Difference = 0;

/*转向环-对管控制*/
float PID_vITR_P = 300, PID_vITR_I = 5, PID_vITR_D = 100;
float PID_vITR_Error = 0;
float PID_vITR_Error_Sum = 0;
float PID_vITR_Error_Last = 0;
float PID_vITR_Error_Difference = 0;

/**
 * @brief 限幅到目标值
 *
 * @param NowValue 当前值
 * @param TargetValue 目标值
 *
 * @retval 无
 *
 * @note 无
 */
void PID_Limit(float *NowValue, float TargetValue)
{
    float HighTarget = TargetValue;
    float LowTarget = -TargetValue;

    if (*NowValue > HighTarget)
    {
        *NowValue = HighTarget;
    }

    if (*NowValue < LowTarget)
    {
        *NowValue = LowTarget;
    }
}

/**
 * @brief 速度闭环控制
 *
 * @param NowValue 当前值
 * @param TargetValue 目标值
 *
 * @retval 返回执行量
 *
 * @note 无
 */
int PID_Speed_Function(float NowValue, float TargetValue)
{
    // 误差值（比例P）
    PID_Speed_Error = TargetValue - NowValue;

    // 误差值滤波
    PID_Speed_Error = PID_Speed_Error * 0.3 + PID_Speed_Error_Last * 0.7;

    // 误差值累加（积分I）
    PID_Speed_Error_Sum += PID_Speed_Error;

    // 误差值累加限幅(±3000)
    PID_Limit(&PID_Speed_Error_Sum, 3000);

    // 误差值的差值（微分D）
    PID_Speed_Error_Difference = PID_Speed_Error - PID_Speed_Error_Last;

    // 误差值记录
    PID_Speed_Error_Last = PID_Speed_Error;

    // PID计算并返回
    return PID_Speed_P * PID_Speed_Error +
           PID_Speed_I * PID_Speed_Error_Sum +
           PID_Speed_D * PID_Speed_Error_Difference;
}

/**
 * @brief 获取速度执行量
 *
 * @param TargetValue 目标速度
 *
 * @retval 返回速度执行量
 *
 * @note 已指定输入量为电机速度
 */
int PID_RetExecutionQuantity_SpeedControl(float TargetValue)
{
    return PID_Speed_Function(ENCODER_Speed, TargetValue);
}

/**
 * @brief 对管控制的转向闭环控制
 *
 * @param NowValue 当前值
 * @param TargetValue 目标值
 *
 * @retval 返回执行量
 *
 * @note 无
 */
int PID_Vertical_ITR9909_Function(float NowValue, float TargetValue)
{
    // 误差值（比例P）
    PID_vITR_Error = TargetValue - NowValue;

    // 误差值滤波
    PID_vITR_Error = PID_vITR_Error * 0.3 + PID_vITR_Error_Last * 0.7;

    // 误差值累加（积分I）
    PID_vITR_Error_Sum += PID_vITR_Error;

    // 误差值累加限幅(±3000)
    PID_Limit(&PID_vITR_Error_Sum, 3000);

    // 误差值的差值（微分D）
    PID_vITR_Error_Difference = PID_vITR_Error - PID_vITR_Error_Last;

    // 误差值记录
    PID_vITR_Error_Last = PID_vITR_Error;

    // PID计算并返回
    return PID_vITR_P * PID_vITR_Error +
           PID_vITR_I * PID_vITR_Error_Sum +
           PID_vITR_D * PID_vITR_Error_Difference;
}

/**
 * @brief 获取两侧对管执行量
 *
 * @param Left_ExecutionQuantity 左侧执行量
 * @param Right_ExecutionQuantity 右侧执行量
 *
 * @retval 无
 *
 * @note 应当将此执行量反馈到车轮上
 */
void PID_RetExecutionQuantity_vITRControl(int *Left_ExecutionQuantity, int *Right_ExecutionQuantity)
{
    // 获取对管数值
    uint16_t ITR_Left = ADC_ITR9909_Value[ADC_ITRBuffer_LeftValue];
    uint16_t ITR_Right = ADC_ITR9909_Value[ADC_ITRBuffer_RightValue];

    // 返回执行量
    *Left_ExecutionQuantity = PID_Vertical_ITR9909_Function(ITR_Left, PID_ITROFFSIDE_TargetValue);
    *Right_ExecutionQuantity = PID_Vertical_ITR9909_Function(ITR_Right, PID_ITROFFSIDE_TargetValue);
}
