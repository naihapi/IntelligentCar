#include "USART.h"

uint8_t USART2_RecBuffer[1024];       // 串口2接收缓冲区-电脑
uint16_t USART2_Intrrupt_CNT = 0;      // 串口2接收1帧计数（中断函数内使用）
uint16_t USART2_Intrrupt_LastCNT = 0; // 串口2接收1帧计数（记录上一次的值，当前与上次进行对比，相等即接收完成）
uint8_t USART2_RecFlag = 0;           // 串口2接收标志（0=没有数据 2=正在接收数据 1=接收完成）

/**
 * @brief 串口2清空缓冲区
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void USART2_Clear_RecBuffer(void)
{
    memset(USART2_RecBuffer, 0, sizeof(USART2_RecBuffer));
}

/**
 * @brief 串口2初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无线串口初始化
 */
void USART2_Init(void)
{
    // 清空接收缓冲区
    USART2_Clear_RecBuffer();

    // 开启时钟和结构体定义
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // USART初始化
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStructure);

    // 串口2使能字节中断位
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    // 使能串口2中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE);
}

/**
 * @brief 串口2发送字符
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void USART2_SendByte(uint16_t Byte)
{
    USART_SendData(USART2, Byte);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {
    }
}

/**
 * @brief 串口2发送字符串
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void USART2_SendString(uint8_t *String)
{
    for (uint16_t i = 0; String[i] != '\0'; i++)
    {
        USART2_SendByte((uint8_t)String[i]);
    }
}

/**
 * @brief 串口2发送数字
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void USART2_SendNumber(int32_t Number)
{
    uint8_t Buffer[16];

    snprintf((char *)Buffer, sizeof(Buffer), "%d", Number);
    USART2_SendString(Buffer);
}

/**
 * @brief 使用串口2接收状态判断
 *
 * @param 无
 *
 * @retval 标志位置位
 * @retval 0 没有收到数据
 * @retval 1 数据接收完成
 * @retval 2 数据接收中
 *
 * @note 不能快速执行函数
 * @note 如果判断太快，会导致<接收完成标志位>提前置位
 * @note 建议500ms判断一次
 */
void USART2_RecState(void)
{
    if (USART2_Intrrupt_CNT == 0)
    {
        // 没有收到数据
        USART2_RecFlag = 0;
    }
    else if (USART2_Intrrupt_CNT != USART2_Intrrupt_LastCNT)
    {
        // 正在接收数据
        USART2_Intrrupt_LastCNT = USART2_Intrrupt_CNT;
        USART2_RecFlag = 2;
    }
    else if (USART2_Intrrupt_CNT == USART2_Intrrupt_LastCNT)
    {
        // 数据接收完成，数据缓冲区从头接收
        USART2_RecBuffer[USART2_Intrrupt_CNT] = '\0';
        USART2_Intrrupt_CNT = 0;
        USART2_RecFlag = 1;
    }
}

void USART_InitPro(void)
{
    USART2_Init();

    USART2_SendString("A\nWireless: Car By Naihapi\n");
}

/**
 * @brief 串口2中断服务函数
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 使用字节中断进行数据接收
 */
void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        // 接收缓冲区溢出时，从头开始覆盖原有数据
        if (USART2_Intrrupt_CNT >= sizeof(USART2_RecBuffer))
        {
            USART2_Intrrupt_CNT = 0;
        }

        // 获取1个字节，然后自增
        USART2_RecBuffer[USART2_Intrrupt_CNT] = USART2->DR;
        USART2_Intrrupt_CNT++;

        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}
