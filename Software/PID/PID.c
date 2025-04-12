#include "PID.h"

float TargetValue_Speed = 0;
float TargetValue_Turn = 0;
uint8_t PID_State = 1;

/*直立环*/
float PID_Vertical_P = 420, PID_Vertical_D = 960;
float PID_vertival_Error = 0;
float PID_vertival_Error_Sum = 0;
float PID_vertival_Error_Last = 0;
float PID_vertival_Error_Difference = 0;

/*速度环*/
float PID_Speed_P = -3500, PID_Speed_I = -30, PID_Speed_D = 0;
float PID_Speed_Error = 0;
float PID_Speed_Error_Sum = 0;
float PID_Speed_Error_Last = 0;
float PID_Speed_Error_Difference = 0;

/*转向环*/
float PID_Turn_P = 45, PID_Turn_I = 0, PID_Turn_D = 0;
float PID_Turn_Error = 0;
float PID_Turn_Error_Sum = 0;
float PID_Turn_Error_Last = 0;
float PID_Turn_Error_Difference = 0;
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
void PID_Limit(int *NowValue, int TargetValue)
{
    int HighTarget = TargetValue;
    int LowTarget = -TargetValue;

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
 * @brief PID直立环节
 *
 * @param NowValue 当前值
 * @param TargetValue 目标值
 *
 * @retval 无
 *
 * @note 陀螺仪前倾负值，车轮向前正值
 */
int PID_Vertical_Function(float NowValue, float TargetValue)
{
    // 误差值（比例P）
    PID_vertival_Error = TargetValue - NowValue;

    // 误差值的差值（微分D）
    PID_vertival_Error_Difference = PID_vertival_Error - PID_vertival_Error_Last;

    // 误差值记录
    PID_vertival_Error_Last = PID_vertival_Error;

    // PID计算并返回
    return PID_Vertical_P * PID_vertival_Error +
           PID_Vertical_D * PID_vertival_Error_Difference;
}

int PID_Speed_Function(float NowValue, float TargetValue)
{
    // 误差值（比例P）
    PID_Speed_Error = TargetValue - NowValue;

    // 误差值滤波
    PID_Speed_Error = PID_Speed_Error * 0.3 + PID_Speed_Error_Last * 0.7;

    // 误差值累加（积分I）
    PID_Speed_Error_Sum += PID_Speed_Error;
    if (PID_Speed_Error_Sum > 1000)
    {
        PID_Speed_Error_Sum = 1000;
    }
    if (PID_Speed_Error_Sum < -1000)
    {
        PID_Speed_Error_Sum = -1000;
    }

    // 误差值的差值（微分D）
    PID_Speed_Error_Difference = PID_Speed_Error - PID_Speed_Error_Last;

    // 误差值记录
    PID_Speed_Error_Last = PID_Speed_Error;

    // PID计算并返回
    return PID_Speed_P * PID_Speed_Error +
           PID_Speed_I * PID_Speed_Error_Sum +
           PID_Speed_D * PID_Speed_Error_Difference;
}

int PID_Return_Function(float NowValue, float TargetValue)
{
    // 误差值（比例P）
    PID_Turn_Error = TargetValue - NowValue;

    // 如果误差值超过180，会导致车体快速翻转，应当限幅避免翻转
    if (PID_Turn_Error > 180)
    {
        PID_Turn_Error = PID_Turn_Error - 360;
    }
    else if (PID_Turn_Error < -180)
    {
        PID_Turn_Error = PID_Turn_Error + 360;
    }

    // 误差值累加（积分I）
    PID_Turn_Error_Sum += PID_Turn_Error;
    if (PID_Turn_Error_Sum > 500)
    {
        PID_Turn_Error_Sum = 500;
    }
    if (PID_Turn_Error_Sum < -500)
    {
        PID_Turn_Error_Sum = -500;
    }

    if (NowValue < 0 && TargetValue < 0 || NowValue >= 0 || TargetValue >= 0)
    {
        return PID_Turn_P * PID_Turn_Error +
               PID_Turn_I * PID_Turn_Error_Sum +
               PID_Turn_D * PID_Turn_Error_Difference;
    }

    if (NowValue < 0 || TargetValue < 0)
    {
        return -PID_Turn_P * PID_Turn_Error +
               -PID_Turn_I * PID_Turn_Error_Sum +
               -PID_Turn_D * PID_Turn_Error_Difference;
    }

    return 0;
}

/**
 * @brief 外部中断线1中断服务程序
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 用于PID控制，PID控制在中断内完成
 */
void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) == SET)
    {
        if (!MPU6050_DMP_Get_Data(&MPU6050_Pitch, &MPU6050_Roll, &MPU6050_Yaw))
        {
        }
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
