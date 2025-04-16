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
 * @brief 车体向左旋转
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 360°旋转
 */
void Car_Spin360_Left(void)
{
    MOTOR_Pulse_Config(-7200, 7200);
    Car_SetFlag(CAR_FLAG_SPIN, 1);
}

/**
 * @brief 车体向右旋转
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 360°旋转
 */
void Car_Spin360_Right(void)
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
 * @param dir 方向(Car_xxxSpin)
 *
 * @retval 无
 *
 * @note 无
 */
void Car_SearchLine_Spin(uint8_t dir)
{
    if (dir == Car_LeftSpin)
    {
        Car_Spin360_Left();
    }
    else if (dir == Car_RightSpin)
    {
        Car_Spin360_Right();
    }

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
    Car_Spin360_Right();
    int16_t yaw = 0;

    while (1)
    {
        yaw = (int16_t)MPU6050_Yaw;

        if (ADC_ITR9909_Value[ADC_ITRBuffer_LeftValue] > 1400 ||
            ADC_ITR9909_Value[ADC_ITRBuffer_MiddleValue] > 1400 ||
            ADC_ITR9909_Value[ADC_ITRBuffer_RightValue] > 1400)
        {
            if (MPU6050_ThresholdCompare(MPU6050_Yaw, MPU6050_OppositeAngle(angle), 45) == 0 && MPU6050_ThresholdCompare(yaw, angle, 45) == 0)
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
    Car_Spin360_Right();

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
    int SpeedEXE = 0;
    int LeftEXE = 0, RightEXE = 0;

    Car_StraightBack(); // 车辆后退
    vTaskDelay(100);    // 延时

    Car_SearchLine_Spin(Car_RightSpin); // 旋转巡线

    while (1)
    {
        // 巡线回退到十字路口
        Car_SpeedExecutionQuantity_ENCODER(&SpeedEXE, 20);
        Car_TurnExecutionQuantity_ITR9909(&SpeedEXE, &LeftEXE, &RightEXE);
        MOTOR_Pulse_Config(LeftEXE, RightEXE);

        // 回到十字路口时进入
        if (ADC_GetFlag(ADC_FLAG_ITR9909_THRESHOLD) == ADC_FLAGSTATE_ITR9909_OVERFLOW)
        {
            // 再前进一点点
            Car_StraightFront();
            vTaskDelay(150);
            Car_Stop();

            break; // 退出循环
        }
    }

    Car_SearchLine_Spin_Range(MPU6050_YawAngleLog_Get()); // 旋转区域巡线(不原路返回)
}

void Car_ErrorLine_Handler2(void)
{
    uint16_t left = ADC_TIR9909Log_Get(ADC_ITRBuffer_LeftValue);
    uint16_t middle = ADC_TIR9909Log_Get(ADC_ITRBuffer_MiddleValue);
    uint16_t right = ADC_TIR9909Log_Get(ADC_ITRBuffer_RightValue);
    uint8_t dir = 0;

    if (left > middle && middle > right)
    {
        Car_SearchLine_Spin(Car_LeftSpin);
    }
    else if (right > middle && middle > left)
    {
        Car_SearchLine_Spin(Car_RightSpin);
    }
    else
    {
        // 处理'T'字路口
        Car_SearchLine_Spin(Car_LeftSpin);
    }
}

void Car_SearchLine_NormalMode(int *SpeedEXE, int *LeftEXE, int *RightEXE)
{

    Car_SpeedExecutionQuantity_ENCODER(SpeedEXE, 20);

    if (ADC_GetFlag(ADC_FLAG_ITR9909_THRESHOLD) == ADC_FLAGSTATE_ITR9909_NORMAL)
    {
        Car_TurnExecutionQuantity_ITR9909(SpeedEXE, LeftEXE, RightEXE);
    }

    // 输出到电机
    MOTOR_Pulse_Config(*LeftEXE, *RightEXE);
}

void Car_SearchLine_ErrorLineMode(void)
{
}
