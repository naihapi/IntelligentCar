#include "main.h"

/**
 * @brief main主函数
 *
 * @param WechatSupport naihapi
 *
 * @note 外设：MCU外设初始化函数
 * @note 外件：受MCU控制的外部设备的初始化函数
 */
int main(void)
{
	/*---外设---*/
	GPIO_InitPro();
	TIM_InitPro();

	// USART_InitPro();
	// IIC_InitPro();

	/*---外件---*/
	MOTOR_InitPro();

	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
	GPIO_WriteBit(GPIOB, GPIO_Pin_15, Bit_RESET);
	// MPU6050_InitPro();

	/*---软件---*/
	//...

	/*开启任务调度*/
	// TASK_InitPro();
}
