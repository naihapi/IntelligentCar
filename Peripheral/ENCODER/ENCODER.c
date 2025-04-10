#include "ENCODER.h"

int16_t ENCODER_CNT = 0;   // 编码器计数值(-65534~65535)
float ENCODER_Speed = 0.0; // 编码器速度(CM/ms)

void ENCODER_InitPro(void) {}

/**
 * @brief 获取编码数据
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 在任务中，每100ms获取一次编码计数值
 * @note 编码数值存储到全局变量ENCODER_Speed中
 * @note 后续版本，更改为消息队列处理
 */
void ENCODER_Collection_CodeData(void)
{
    TIM_Encoder_GetCount(&ENCODER_CNT);
    ENCODER_Speed = (float)ENCODER_CNT / ENCODER_OneRound_CNT * ENCODER_WheelLengt;
    TIM_Encoder_SetCount(0);
}

// CM/ms
void ENCODER_GetSpeed(float *speed)
{
    *speed = ENCODER_Speed;
}
