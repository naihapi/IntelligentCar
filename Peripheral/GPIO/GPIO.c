#include "GPIO.h"

/**
 * @brief 电机转动控制
 *
 * @param State Bit_SET
 * @param State Bit_RESET
 *
 * @retval 无
 *
 * @note 电机开关总控制
 */
void GPIO_MOTOR_STBY(uint8_t State)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_12, (BitAction)State);
}

/**
 * @brief 电机AIN1转动控制
 *
 * @param State Bit_SET
 * @param State Bit_RESET
 *
 * @retval 无
 *
 * @note 通过AIN1和AIN2控制PWMA的正反转
 */
void GPIO_MOTOR_AIN1(uint8_t State)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_11, (BitAction)State);
}

/**
 * @brief 电机AIN2转动控制
 *
 * @param State Bit_SET
 * @param State Bit_RESET
 *
 * @retval 无
 *
 * @note 通过AIN1和AIN2控制PWMA的正反转
 */
void GPIO_MOTOR_AIN2(uint8_t State)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_10, (BitAction)State);
}

/**
 * @brief 电机BIN1转动控制
 *
 * @param State Bit_SET
 * @param State Bit_RESET
 *
 * @retval 无
 *
 * @note 通过BIN1和BIN2控制PWMB的正反转
 */
void GPIO_MOTOR_BIN1(uint8_t State)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_15, (BitAction)State);
}

/**
 * @brief 电机BIN2转动控制
 *
 * @param State Bit_SET
 * @param State Bit_RESET
 *
 * @retval 无
 *
 * @note 通过BIN1和BIN2控制PWMB的正反转
 */
void GPIO_MOTOR_BIN2(uint8_t State)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)State);
}

/**
 * @brief 时钟线SCL引脚设置
 *
 * @param signal 电平（1：高电平，0：低电平）
 *
 * @retval 无
 *
 * @note 无
 */
void GPIO_SCL_Config(uint8_t signal)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)signal);
    Delay_us(1);
}

/**
 * @brief 数据线SDA引脚设置
 *
 * @param signal 电平（1：高电平，0：低电平）
 *
 * @retval 无
 *
 * @note 无
 */
void GPIO_SDA_Config(uint8_t signal)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)signal);
    Delay_us(1);
}

/**
 * @brief 数据线SDA引脚读取
 *
 * @param 无
 *
 * @retval 1 高电平
 * @retval 0 低电平
 *
 * @note 无
 */
uint8_t GPIO_SDA_Read(void)
{
    return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
}

/**
 * @brief 后车灯控制
 *
 * @param State (1：打开，0：关闭)
 *
 * @retval 无
 *
 * @note 无
 */
void GPIO_LED_RearLED(uint8_t State)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)State);
}

/**
 * @brief 前车灯控制
 *
 * @param State (1：打开，0：关闭)
 *
 * @retval 无
 *
 * @note 无
 */
void GPIO_LED_FrontLED(uint8_t State)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)State);
}

/**
 * @brief 提示灯控制
 *
 * @param State (1：打开，0：关闭)
 *
 * @retval 无
 *
 * @note 无
 */
void GPIO_LED_TipsLED(uint8_t State)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)State);
}

/**
 * @brief 蜂鸣器状态
 *
 * @param State (1：打开，0：关闭)
 *
 * @retval 无
 *
 * @note 无
 */
void GPIO_Buzzer_Config(uint8_t State)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)State);
}

/**
 * @brief 蜂鸣器IO初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void GPIO_Buzzer_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief 超声波触发控制
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void GPIO_UltraSound_Trig(uint8_t State)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)State);
}

/**
 * @brief 超声波接收读取
 *
 * @param 无
 *
 * @retval 1 高电平
 * @retval 0 低电平
 *
 * @note 无
 */
uint8_t GPIO_UltraSound_Echo(void)
{
    return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}

/**
 * @brief  电机引脚配置
 *
 * @param  无
 *
 * @retval 无
 *
 * @note PA12-STBY
 * @note PA11-AIN1
 * @note PA10-AIN2
 * @note PA15-BIN1
 * @note PB3-BIN2
 * @note PA6-PWMA
 * @note PA7-PWMB
 * @note 编码器引脚在ENCODER_InitPro()中配置
 */
void GPIO_MOTOR_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 结构体定义
    GPIO_InitTypeDef GPIO_INITSTRUCTURE;

    // 解除复用JTAG
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    // 左轮PWMA TIM3_CH1
    GPIO_INITSTRUCTURE.GPIO_Pin = GPIO_Pin_6;
    GPIO_INITSTRUCTURE.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_INITSTRUCTURE.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_INITSTRUCTURE);

    // 右轮PWMB TIM3_CH2
    GPIO_INITSTRUCTURE.GPIO_Pin = GPIO_Pin_7;
    GPIO_INITSTRUCTURE.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_INITSTRUCTURE.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_INITSTRUCTURE);

    // 引脚配置 STBY|AINx|BINx
    GPIO_INITSTRUCTURE.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_INITSTRUCTURE.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_INITSTRUCTURE.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_INITSTRUCTURE);

    // 引脚配置 STBY|AINx|BINx
    GPIO_INITSTRUCTURE.GPIO_Pin = GPIO_Pin_3;
    GPIO_INITSTRUCTURE.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_INITSTRUCTURE.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_INITSTRUCTURE);

    // 引脚默认配置
    GPIO_MOTOR_STBY(Bit_RESET);
    GPIO_MOTOR_AIN1(Bit_RESET);
    GPIO_MOTOR_AIN2(Bit_RESET);
    GPIO_MOTOR_BIN1(Bit_RESET);
    GPIO_MOTOR_BIN2(Bit_RESET);
}

/**
 * @brief 编码器引脚初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 左侧车轮 PB6=E2A=TIM4_CH1|PB7=E2B=TIM4_CH2
 * @note 右侧车轮 PA8=E1B=TIM1_CH1|PA9=E1A=TIM1_CH2
 */
void GPIO_ENCODER_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;

    // 左侧车轮 PB6=E2A=TIM4_CH1|PB7=E2B=TIM4_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 右侧车轮 PA8=E1B=TIM1_CH1|PA9=E1A=TIM1_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief IIC对应的引脚初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note PB10=SCL PB11=SDA
 * @note IIC2挂载设备：MPU6050
 */
void GPIO_IIC_Init(void)
{
    // 开启时钟和结构体定义
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, Bit_SET);
}

/**
 * @brief MPU6050中断引脚配置
 *
 * @param 无
 *
 * @retval 无
 *
 * @note MPU6050的INT引脚对STM32进行外部中断
 */
void GPIO_MPU6050_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 结构体定义
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // GPIO配置INT
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 外部中断线配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);

    // 外部中断配置
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_Init(&EXTI_InitStructure);

    // 中断向量配置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief LED灯初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note PA4-提示灯
 * @note PC13-后车灯
 * @note PB13-前车灯
 */
void GPIO_LED_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;

    // 后车灯
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 前车灯
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 指示灯
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief 超声波IO初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 依靠外部中断测量距离
 */
void GPIO_UltraSound_Init(void)
{
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 结构体定义
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // TRIG
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ECHO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 外部中断线配置
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    // 外部中断配置
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_Init(&EXTI_InitStructure);

    // 中断向量配置
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    // 关闭TRIG
    GPIO_UltraSound_Trig(0);
}

void GPIO_USART_Init(void)
{
    // 开启时钟和结构体定义
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    // 引脚TX配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 引脚RX配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief IO引脚初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void GPIO_InitPro(void)
{
    GPIO_IIC_Init();
    GPIO_ENCODER_Init();
    GPIO_MPU6050_Init();
    GPIO_MOTOR_Init();

    GPIO_USART_Init();
    GPIO_LED_Init();
    GPIO_Buzzer_Init();
    GPIO_UltraSound_Init();
}
