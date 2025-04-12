#include "ADC.h"

uint16_t ADC_ITR9909_Value[3];

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

void ADC_InitPro(void)
{
    ADC_ITR9909_Init();
}
