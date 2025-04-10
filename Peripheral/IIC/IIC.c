#include "IIC.h"

/**
 * @brief IIC起始信号
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void IIC_Start(void)
{
    GPIO_SDA_Config(1);
    GPIO_SCL_Config(1);

    GPIO_SDA_Config(0);
    GPIO_SCL_Config(0);
}

/**
 * @brief IIC结束信号
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void IIC_Stop(void)
{
    GPIO_SDA_Config(0);
    GPIO_SCL_Config(1);

    GPIO_SDA_Config(1);
}

/**
 * @brief IIC发送应答
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 无
 */
void IIC_SendAck(uint8_t ackx)
{
    GPIO_SDA_Config(ackx);
    GPIO_SCL_Config(1);
    GPIO_SCL_Config(0);
}

/**
 * @brief IIC接收应答
 *
 * @param 无
 *
 * @retval 1：无应答
 * @retval 0：有应答
 *
 * @note 无
 */
uint8_t IIC_ReceiveAck(void)
{
    uint8_t ack;

    GPIO_SDA_Config(1);
    GPIO_SCL_Config(1);
    ack = GPIO_SDA_Read();
    GPIO_SCL_Config(0);

    return ack;
}

/**
 * @brief IIC发送1字节
 *
 * @param byte 1字节数据
 *
 * @retval 无
 *
 * @note 无
 */
void IIC_SendByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        GPIO_SDA_Config(byte & 0x80);

        GPIO_SCL_Config(1);
        GPIO_SCL_Config(0);
        byte <<= 1;
    }
}

/**
 * @brief IIC接收1字节
 *
 * @param byte 1字节数据
 * @param ackx 应答信号(0：应答，1：不应答)
 *
 * @retval 无
 *
 * @note 先左移1位，再获取数据到byte
 */
void IIC_ReceiveByte(uint8_t *byte)
{
    *byte = 0x00;       // 初始化数据
    GPIO_SDA_Config(1); // 松开数据线

    for (uint8_t i = 0; i < 8; i++)
    {
        GPIO_SCL_Config(1);

        // 移位后赋值
        *byte <<= 1;

        if (GPIO_SDA_Read())
        {
            *byte |= 0x01;
        }

        GPIO_SCL_Config(0);
    }
}

/**
 * @brief IIC寄存器写
 *
 * @param addr 设备地址
 * @param reg 寄存器地址
 * @param len 数据长度
 * @param buf 数据缓冲区
 *
 * @retval 0 成功
 * @retval -1 失败
 *
 * @note 设备地址addr不包含读写位
 * @note 微秒延时依据逻辑分析仪结果
 */
uint8_t IIC_WriteReg(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    IIC_Start(); // 起始信号

    IIC_SendByte(addr << 1 | 0x00); // 发送设备地址
    IIC_ReceiveAck();               // 应答位

    IIC_SendByte(reg); // 发送设备寄存器地址
    IIC_ReceiveAck();  // 应答位

    for (uint8_t i = 0; i < len - 1; i++)
    {
        IIC_SendByte(*buf); // 发送前7个字节
        IIC_ReceiveAck();   // 应答位
        *buf++;             // 数组自增
    }

    IIC_SendByte(*buf); // 发送最后一个字节
    IIC_ReceiveAck();   // 应答位

    Delay_us(30); // 延时缓冲
    IIC_Stop();   // 停止信号

    return 0; // 完成
}

/**
 * @brief IIC寄存器读
 *
 * @param addr 设备地址
 * @param reg 寄存器地址
 * @param len 数据长度
 * @param buf 数据缓冲区
 *
 * @retval 0 成功
 * @retval -1 失败
 *
 * @note 设备地址addr不包含读写位
 * @note 微秒延时依据逻辑分析仪结果
 */
uint8_t IIC_ReadReg(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    IIC_Start(); // 起始信号

    IIC_SendByte(addr << 1 | 0x00); // 发送设备地址+写
    IIC_ReceiveAck();               // 应答位

    IIC_SendByte(reg); // 发送设备寄存器地址
    IIC_ReceiveAck();  // 应答位

    Delay_us(10); // 延时

    IIC_Start(); // 重复起始信号，由地址写转为地址读

    IIC_SendByte(addr << 1 | 0x01); // 发送设备地址+读
    IIC_ReceiveAck();               // 应答位

    // 接收数据
    for (uint8_t i = 0; i < len - 1; i++)
    {
        IIC_ReceiveByte(buf++); // 接收前起七字节
        IIC_SendAck(0);         // 应答位
    }

    IIC_ReceiveByte(buf); // 接收最后一字节
    IIC_SendAck(1);       // 应答位

    Delay_us(30); // 延时
    IIC_Stop();   // 停止信号

    return 0; // 成功
}

/**
 * @brief IIC初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note PB10=SCL PB11=SDA
 * @note IIC2挂载设备：MPU6050
 */
void IIC_InitPro(void)
{
}
