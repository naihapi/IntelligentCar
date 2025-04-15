#include "ADC.h"

uint16_t ADC_ITR9909_Value[3];  // 对管数值(ADC_ITRBuffer_xxxValue)
uint8_t ADC_Threshold_Flag = 2; // 对管阈值(ADC_FLAGSTATE_ITR9909_xxx)
uint8_t ADC_Threshold_Dir = 2;  // 对管方向(ADC_FLAGSTATE_DIRECTION_xxx)

/**
 * @brief 模转数初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note ITR9909左侧：PA5
 * @note ITR9909中间：PA4
 * @note ITR9909右侧：PA0
 * @note 采用ADC+DMA模式
 */
void ADC_ITR9909_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5); // ADC通道5在序列1进行转换
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_55Cycles5); // ADC通道4在序列2进行转换
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 3, ADC_SampleTime_55Cycles5); // ADC通道0在序列3进行转换

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // 独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 数据右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 不使用外部触发
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 打开连续模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        // 打开扫描模式
    ADC_InitStructure.ADC_NbrOfChannel = 3;                             // 共计三个通道
    ADC_Init(ADC1, &ADC_InitStructure);                                 // ADC初始化

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;             // 外设地址
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 半节传输
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 关闭外设地址自增
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_ITR9909_Value;         // 内存地址
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 半节传输
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 打开内存地址自增
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // 方向为从外设
    DMA_InitStructure.DMA_BufferSize = 3;                                       // 传输次数
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // 循环模式
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // 关闭内存到内存传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       // 优先级中
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                // DMA初始化

    // 打开ADC+DMA
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);

    // 校准ADC
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET)
    {
    };
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET)
    {
    };

    // 启动转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC_SetFlag(uint8_t flag, uint8_t state)
{
    switch (flag)
    {
    case ADC_FLAG_ITR9909_THRESHOLD:
    {
        ADC_Threshold_Flag = state;
    }
    break;
    case ADC_FLAG_DIRECTION:
    {
        ADC_Threshold_Dir = state;
    }
    break;
    }
}

uint8_t ADC_GetFlag(uint8_t flag)
{
    switch (flag)
    {
    case ADC_FLAG_ITR9909_THRESHOLD:
    {
        return ADC_Threshold_Flag;
    }
    case ADC_FLAG_DIRECTION:
    {
        return ADC_Threshold_Dir;
    }
    }

    return 9;
}

/**
 * @brief 阈值比较
 *
 * @param max 最大阈值
 * @param mini 最小阈值
 *
 * @retval 无
 *
 * @note 2个或3个对管平均值<最低阈值，输出0
 * @note 2个或3个对管平均值>最高阈值，输出1
 * @note 2个或3个对管平均值不超过阈值，输出2
 * @note 靠近黑色，数值上升；远离黑色，数值下降
 */
void ADC_ITR9909_ThresholdCompare(uint16_t max, uint16_t mini)
{
    uint16_t average = 0;
    ADC_SetFlag(ADC_FLAG_ITR9909_THRESHOLD, ADC_FLAGSTATE_ITR9909_NORMAL);

    average = (ADC_ITR9909_Value[ADC_ITRBuffer_LeftValue] +
               ADC_ITR9909_Value[ADC_ITRBuffer_MiddleValue] +
               ADC_ITR9909_Value[ADC_ITRBuffer_RightValue]) /
              3.0;
    if (average < mini)
    {
        // 标志位记录
        ADC_SetFlag(ADC_FLAG_ITR9909_THRESHOLD, ADC_FLAGSTATE_ITR9909_TINY);
    }
    else if (average > max)
    {
        // 标志位记录
        ADC_SetFlag(ADC_FLAG_ITR9909_THRESHOLD, ADC_FLAGSTATE_ITR9909_OVERFLOW);
        ADC_SetFlag(ADC_FLAG_DIRECTION, ADC_FLAGSTATE_DIRECTION_BOTH);
    }

    average = (ADC_ITR9909_Value[ADC_ITRBuffer_LeftValue] +
               ADC_ITR9909_Value[ADC_ITRBuffer_MiddleValue]) /
              2.0;
    if (average < mini)
    {
        // 标志位记录
        ADC_SetFlag(ADC_FLAG_ITR9909_THRESHOLD, ADC_FLAGSTATE_ITR9909_TINY);
    }
    else if (average > max)
    {
        // 标志位记录
        ADC_SetFlag(ADC_FLAG_ITR9909_THRESHOLD, ADC_FLAGSTATE_ITR9909_OVERFLOW);
        ADC_SetFlag(ADC_FLAG_DIRECTION, ADC_FLAGSTATE_DIRECTION_LEFT);

        // 时间记录
        Delay_TimeLog_Record();

        // 仅在车辆不旋转、错误线未检测到时，记录偏航角信息
        if (Car_GetFlag(CAR_FLAG_SPIN) == 0 && Car_GetFlag(CAR_FLAG_ERRLINE) == 0)
        {
            MPU6050_YawAngleLog_Record();
            Debug_Flag1 = 1;
        }
    }

    average = (ADC_ITR9909_Value[ADC_ITRBuffer_RightValue] +
               ADC_ITR9909_Value[ADC_ITRBuffer_MiddleValue]) /
              2.0;
    if (average < mini)
    {
        // 标志位记录
        ADC_SetFlag(ADC_FLAG_ITR9909_THRESHOLD, ADC_FLAGSTATE_ITR9909_TINY);
    }
    else if (average > max)
    {
        // 标志位记录
        ADC_SetFlag(ADC_FLAG_ITR9909_THRESHOLD, ADC_FLAGSTATE_ITR9909_OVERFLOW);
        ADC_SetFlag(ADC_FLAG_DIRECTION, ADC_FLAGSTATE_DIRECTION_RIGHT);

        // 时间记录
        Delay_TimeLog_Record();

        // 仅在车辆不旋转、错误线未检测到时，记录偏航角信息
        if (Car_GetFlag(CAR_FLAG_SPIN) == 0 && Car_GetFlag(CAR_FLAG_ERRLINE) == 0)
        {
            MPU6050_YawAngleLog_Record();
            Debug_Flag1 = 1;
        }
    }
}
uint8_t Debug_Flag1 = 0; // 蜂鸣器

/**
 * @brief 获取比较值
 *
 * @param 无
 *
 * @retval 返回0或1或2
 *
 * @note 2个或3个对管平均值<最低阈值，输出0
 * @note 2个或3个对管平均值>最高阈值，输出1
 * @note 2个或3个对管平均值不超过阈值，输出2
 */
// uint8_t ADC_ITR9909_Compare(void)
// {
//     return ADC_Threshold_Flag;
// }

void ADC_InitPro(void)
{
    ADC_ITR9909_Init();
}
