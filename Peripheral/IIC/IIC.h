#ifndef _IIC_H
#define _IIC_H

// MCU型号
#include "stm32f10x.h"

// 外设
#include "GPIO.h"

// 软件
#include "Delay.h"

// API
void IIC_InitPro(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_SendByte(uint8_t byte);
void IIC_ReceiveByte(uint8_t *byte);
void IIC_SendAck(uint8_t ackx);
uint8_t IIC_ReceiveAck(void);
uint8_t IIC_ReadReg(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
uint8_t IIC_WriteReg(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

#endif
