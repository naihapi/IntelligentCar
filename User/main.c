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
	USART_InitPro();
	IIC_InitPro();

	/*---外件---*/
	MOTOR_InitPro();
	MPU6050_InitPro();

	/*---软件---*/
	//...

	/*开启任务调度*/
	TASK_InitPro();
}
