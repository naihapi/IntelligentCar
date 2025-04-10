#include "TIM.h"

/**
 * @brief 定时器1初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 此函数用于左侧车轮编码器计数
 * @note TIM_EncoderInterfaceConfig函数的参数三、四为设置正计数或倒计数模式，与触发无关
 */
void TIM_Encoder_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // 结构体定义
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    // 定时器1配置
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;  // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    // 定时器1输入捕获
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);

    // 默认计数值
    TIM_SetCounter(TIM1, 0);

	// TIM1编码模式 TI1和TI2都开启计数，边沿信号不反转
    TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    // 使能定时器
    TIM_Cmd(TIM1, ENABLE);
}

/**
 * @brief 定时器2初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 此函数用于中断延时，详见Delay.c，TIM2的中断服务函数也在那里
 */
void TIM_Delay_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 999;                // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 71;              // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = DISABLE; // 溢出x次后才触发更新中断
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    TIM_SetCounter(TIM2, 0);
    TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief 定时器3初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 输出双路PWM
 * @note 1个周期为7200下计数
 */
void TIM_MOTOR_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 7201 - 1;  // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = DISABLE;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 左侧CCR
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 右侧CCR
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief 定时器4初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 1us计数1次
 * @note 此函数用于超声波计数
 */
void TIM_Count_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 65535;              // ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 71;              // PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = DISABLE; // 溢出x次后才触发更新中断
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    TIM_SetCounter(TIM4, 0);
    TIM_Cmd(TIM4, DISABLE);
}

/**
 * @brief 定时器3通道1脉冲控制
 *
 * @param Pulse 0~7200
 *
 * @retval 无
 *
 * @note 左侧车轮CCR
 * @note 脉冲：1个周期内，输出信号保持在高电平（或有效电平）的时间段
 */
void TIM_MOTOR_CH1_Pulse(uint16_t Pulse)
{
    TIM_SetCompare1(TIM3, Pulse);
}

/**
 * @brief 定时器3通道2脉冲控制
 *
 * @param Pulse 0~7200
 *
 * @retval 无
 *
 * @note 右侧车轮CCR
 * @note 脉冲：1个周期内，输出信号保持在高电平（或有效电平）的时间段
 */
void TIM_MOTOR_CH2_Pulse(uint16_t Pulse)
{
    TIM_SetCompare2(TIM3, Pulse);
}

/**
 * @brief 定时计数器配置
 *
 * @param cnt 计数值
 *
 * @retval 无
 *
 * @note 无
 */
void TIM_Count_SetCNT(uint16_t cnt)
{
    TIM_SetCounter(TIM4, cnt);
}

/**
 * @brief 定时计数器获取计数值
 *
 * @param 无
 *
 * @retval 返回计数值
 *
 * @note 无
 */
uint16_t TIM_Count_GetCNT(void)
{
    return TIM_GetCounter(TIM4);
}

/**
 * @brief 定时计数器打开
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void TIM_Count_ON(void)
{
    TIM_Cmd(TIM4, ENABLE);
}

/**
 * @brief 定时计数器关闭
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void TIM_Count_OFF(void)
{
    TIM_Cmd(TIM4, DISABLE);
}

/**
 * @brief 获取编码器计数值
 *
 * @param count 计数值
 *
 * @retval 无
 *
 * @note 无
 */
void TIM_Encoder_GetCount(int16_t *count)
{
    *count = (int16_t)TIM_GetCounter(TIM1);
}

/**
 * @brief 设置编码器计数值
 *
 * @param count 计数值
 *
 * @retval 无
 *
 * @note 无
 */
void TIM_Encoder_SetCount(int16_t count)
{
    TIM_SetCounter(TIM1, count);
}

/**
 * @brief 定时器初始化汇总
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void TIM_InitPro(void)
{
    TIM_Encoder_Init();
    TIM_Delay_Init();
    TIM_MOTOR_Init();
    TIM_Count_Init();
}
