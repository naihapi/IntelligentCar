#include "Car.h"

volatile uint8_t Car_Spin_Flag = 0;      // 车辆旋转标志位(车辆调用旋转函数时，标志位置1；调用停止函数时，标志位置0)
volatile uint8_t Car_ErrorLine_FLAG = 0; // 车辆错误线处理标志位(车辆进入错路处理时，标志位置1；退出错误路处理时，标志位置0)

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

void Car_SetFlag(uint8_t flag, uint8_t state)
{
    switch (flag)
    {
    case CAR_FLAG_SPIN:
    {
        Car_Spin_Flag = state;
    }
    break;
    case CAR_FLAG_ERRLINE:
    {
        Car_ErrorLine_FLAG = state;
    }
    break;
    }
}

uint8_t Car_GetFlag(uint8_t flag)
{
    switch (flag)
    {
    case CAR_FLAG_SPIN:
    {
        return Car_Spin_Flag;
    }
    case CAR_FLAG_ERRLINE:
    {
        return Car_ErrorLine_FLAG;
    }
    }

    return 9;
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
    Car_SetFlag(CAR_FLAG_SPIN, 1);
}

void Car_StraightFront(void)
{
    MOTOR_Pulse_Config(7200, 7200);
}

void Car_StraightBack(void)
{
    MOTOR_Pulse_Config(-7200, -7200);
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
void Car_Stop(void)
{
    MOTOR_Pulse_Config(0, 0);
    Car_SetFlag(CAR_FLAG_SPIN, 0);
}

/**
 * @brief 车体旋转寻线
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Car_SearchLine_Spin(void)
{
    Car_Spin360();

    while (1)
    {
        if (ADC_ITR9909_Value[0] > 1400 || ADC_ITR9909_Value[1] > 1400 || ADC_ITR9909_Value[2] > 1400)
        {
            Car_Stop();

            break;
        }
    }
}

/**
 * @brief 车体旋转寻线范围版
 *
 * @param angle 忽略的角度
 *
 * @retval 无
 *
 * @note 从空白地面旋转处巡线
 * @note 若参数为0°，则0°±30范围内失能巡线
 */
void Car_SearchLine_Spin_Range(int16_t angle)
{
    Car_Spin360();

    while (1)
    {
        if (ADC_ITR9909_Value[0] > 1400 || ADC_ITR9909_Value[1] > 1400 || ADC_ITR9909_Value[2] > 1400)
        {
            if (MPU6050_ThresholdCompare(MPU6050_Yaw, angle, 30) == 0)
            {
                Car_Stop();

                break;
            }
        }
    }
}

/**
 * @brief 车体旋转脱离线
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 从带线地面寻找新线
 */
void Car_BreakLine_Spin(void)
{
    Car_Spin360();

    while (1)
    {
        if (ADC_ITR9909_Value[0] < 300 && ADC_ITR9909_Value[1] < 300 & ADC_ITR9909_Value[2] < 300)
        {
            Car_Stop();

            break;
        }
    }
}

/**
 * @brief 获取速度执行量
 *
 * @param Speed 速度执行量
 * @param TargetSpeed 目标速度
 *
 * @retval 无
 *
 * @note TargetSpeed取值范围(cm/s)：0~22
 */
void Car_SpeedExecutionQuantity_ENCODER(int *Speed, uint8_t TargetSpeed)
{
    int SpeedExecution = 0; // 速度执行量

    // 获取速度闭环执行量
    SpeedExecution = PID_RetExecutionQuantity_SpeedControl(TargetSpeed);

    *Speed = SpeedExecution;
}

/**
 * @brief 获取转向执行量(对管)
 *
 * @param Speed 速度执行量
 * @param Left 左轮执行量
 * @param Right 右轮执行量
 *
 * @retval 无
 *
 * @note 根据Speed速度闭环执行量，获取转向执行量
 */
void Car_TurnExecutionQuantity_ITR9909(int *Speed, int *Left, int *Right)
{
    int L_ItrExecution = 0, R_ItrExecution = 0; // 两侧对管执行量
    int RealOut_Left = 0, RealOut_Right = 0;    // 最终电机输出数值

    // 默认情况下，最终输出量=速度执行量
    RealOut_Left = RealOut_Right = *Speed;

    // 获取对管执行量
    PID_RetExecutionQuantity_vITRControl(&L_ItrExecution, &R_ItrExecution);

    // 右侧对管输出执行量
    RealOut_Left -= R_ItrExecution;
    RealOut_Right += R_ItrExecution;

    // 左侧对管输出执行量
    RealOut_Left += L_ItrExecution;
    RealOut_Right -= L_ItrExecution;

    // 限幅
    if (RealOut_Left < 0)
        RealOut_Left = 0;
    if (RealOut_Right < 0)
        RealOut_Right = 0;

    *Left = RealOut_Left;
    *Right = RealOut_Right;
}

void Car_ErrorLine_Handler1(void)
{
}
