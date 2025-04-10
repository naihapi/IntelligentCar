#include "Manual.h"

uint8_t Manual_State = 0;                                                     // 手动控制状态(1：打开，0：关闭)
uint8_t Manual_Left = 0, Manual_Right = 0, Manual_Front = 0, Manual_Rear = 0; // 按键状态(1：按下，0：松开)

/**
 * @brief 检查字符串关键字
 *
 * @param str 需检查的字符串
 * @param check 关键字
 *
 * @retval 无
 *
 * @note 无
 */
uint8_t Manual_CheckKeyWord(uint8_t *str, uint8_t *check)
{
    if (strstr((char *)str, (char *)check) != NULL)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief 获取按键状态
 *
 * @param Str 需获取的字符串
 * @param Left 左转状态
 * @param Right 右转状态
 * @param Front 前进状态
 * @param Rear 后退状态
 *
 * @retval 无
 *
 * @note 格式：#left#right#front#rear
 */
void Manual_Get_KEYState(uint8_t *Str, uint8_t *Left, uint8_t *Right, uint8_t *Front, uint8_t *Rear)
{
    uint8_t *Start = NULL, *End = NULL;

    // 获取左键状态
    Start = Receive_FindChar(Str, '#', 1);
    if (Start == NULL)
        return;
    End = Receive_FindChar(Str, '#', 2);
    if (End == NULL)
        return;
    *Left = Receive_FindNumber(Start, End);

    // 获取右键状态
    Start = Receive_FindChar(Str, '#', 2);
    if (Start == NULL)
        return;
    End = Receive_FindChar(Str, '#', 3);
    if (End == NULL)
        return;
    *Right = Receive_FindNumber(Start, End);

    // 获取前进键状态
    Start = Receive_FindChar(Str, '#', 3);
    if (Start == NULL)
        return;
    End = Receive_FindChar(Str, '#', 4);
    if (End == NULL)
        return;
    *Front = Receive_FindNumber(Start, End);

    // 获取后退键状态
    Start = Receive_FindChar(Str, '#', 4);
    if (Start == NULL)
        return;
    End = Receive_FindChar(Str, '\0', 1);
    if (End == NULL)
        return;
    *Rear = Receive_FindNumber(Start, End);
}

/**
 * @brief 清除按键状态
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_Clear_KEYState(void)
{
    Manual_Left = 0;
    Manual_Right = 0;
    Manual_Front = 0;
    Manual_Rear = 0;
}

/**
 * @brief 获取手动控制状态
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
uint8_t Manual_GetState(void)
{
    return Manual_State;
}

/**
 * @brief 设置手动控制状态
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_SetState(uint8_t State)
{
    Manual_State = State;
}

/**
 * @brief 无用的按键事件处理
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_KEYevent_KEYControl_Useless(void)
{
    if (Manual_Front == 1 && Manual_Rear == 1 && Manual_Left == 1 && Manual_Right == 1)
    {
        // 无用的按键事件-全部按下
        TargetValue_Speed = 0;
    }

    if (Manual_Front == 1 && Manual_Rear == 1)
    {
        // 无用的按键事件-前进后退同时按下
        TargetValue_Speed = 0;
    }

    if (Manual_Left == 1 && Manual_Right == 1)
    {
        // 无用的按键事件-左转右转同时按下
        TargetValue_Turn = TargetValue_Turn;
    }
}

/**
 * @brief 左转的按键事件处理
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_KEYevent_KEYControl_Left(void)
{
    if (Manual_Left == 1)
    {
        // 左转
        TargetValue_Turn += 15;

        if (TargetValue_Turn > 180)
        {
            TargetValue_Turn -= 360;
        }
    }
}

/**
 * @brief 右转的按键事件处理
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_KEYevent_KEYControl_Right(void)
{
    if (Manual_Right == 1)
    {
        // 右转
        TargetValue_Turn -= 15;

        if (TargetValue_Turn < -180)
        {
            TargetValue_Turn += 360;
        }
    }
}

/**
 * @brief 前进的按键事件处理
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 采用线性加速，否则会出现初始速度过快
 * @note 前进时，打开前车灯
 */
uint8_t i = 0;
void Manual_KEYevent_KEYControl_Front(void)
{
    if (Manual_Front == 1)
    {
        if (UltraSound_Distance >= 30)
        {
            // 线性前进
            TargetValue_Speed += 0.1;
            if (TargetValue_Speed > 1)
            {
                TargetValue_Speed = 1;
            }

            i = 0;
        }
        else
        {
            if (i == 0)
            {
                TargetValue_Speed = -0.5;
                i = 1;
            }
            // 线性刹车
            TargetValue_Speed += 0.1;
            if (TargetValue_Speed > 0)
            {
                TargetValue_Speed = 0;
            }
        }

        // 打开前车灯
        GPIO_LED_RearLED(0);
        GPIO_LED_FrontLED(1);
    }
}

/**
 * @brief 后退的按键事件处理
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 采用线性减速，否则会出现前后震荡
 * @note 后退时，打开后车灯
 */
void Manual_KEYevent_KEYControl_Rear(void)
{
    if (Manual_Rear == 1)
    {
        // 线性后退
        TargetValue_Speed -= 0.1;
        if (TargetValue_Speed < -1)
        {
            TargetValue_Speed = -1;
        }

        // 打开后车灯
        GPIO_LED_RearLED(1);
        GPIO_LED_FrontLED(0);
    }
}

/**
 * @brief 刹车的按键事件处理
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 采用线性刹车，否则会出现震荡效果
 * @note 刹车时，关闭前后车灯
 */
void Manual_KEYevent_KEYControl_Stop(void)
{
    if (Manual_Front == 0 && Manual_Rear == 0)
    {
        // 线性刹车
        if (TargetValue_Speed > 0)
        {
            TargetValue_Speed -= 0.08;
            if (TargetValue_Speed < 0)
            {
                TargetValue_Speed = 0;
            }
        }
        else if (TargetValue_Speed < 0)
        {
            TargetValue_Speed += 0.08;
            if (TargetValue_Speed > 0)
            {
                TargetValue_Speed = 0;
            }
        }
    }
}

/**
 * @brief 按键控制汇总
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_KEYevent_KEYControl(void)
{
    Manual_KEYevent_KEYControl_Useless(); // 无用事件

    Manual_KEYevent_KEYControl_Front(); // 前进事件
    Manual_KEYevent_KEYControl_Rear();  // 后退事件
    Manual_KEYevent_KEYControl_Left();  // 左转事件
    Manual_KEYevent_KEYControl_Right(); // 右转事件

    Manual_KEYevent_KEYControl_Stop(); // 停止事件
}

/**
 * @brief 按键状态处理
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_KEYevent(uint8_t *Data)
{
    if (Manual_GetState() == 1)
    {
        Manual_Get_KEYState(Data, &Manual_Left, &Manual_Right, &Manual_Front, &Manual_Rear);
    }
    Manual_KEYevent_KEYControl();
}

/**
 * @brief 按键状态处理
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_DataHandler(uint8_t *Data)
{
    if (Manual_CheckKeyWord(Data, "ON") == 1)
    {
        // POST#Manual#4#ON
        Manual_SetState(1);

        Reply_SendSuccess();
    }
    else if (Manual_CheckKeyWord(Data, "OFF") == 1)
    {
        // POST#Manual#4#OFF
        Manual_SetState(0);

        GPIO_LED_RearLED(0);
        GPIO_LED_FrontLED(0);
        Manual_Clear_KEYState();

        Reply_SendSuccess();
    }
}

/**
 * @brief 手动控制的钩子函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 此函数在创建自定义组件时，包含在内
 */
void Manual_CustomFunction(uint8_t *DataPack)
{
    // 数据处理
    Manual_DataHandler(DataPack);

    // 按键事件
    Manual_KEYevent(DataPack);
}

/**
 * @brief 手动控制初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void Manual_InitPro(void) {}
