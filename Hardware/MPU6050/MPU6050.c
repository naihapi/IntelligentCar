#include "MPU6050.h"

#define DEFAULT_MPU_HZ 100 // 采样率默认值，来自DMP库官方例程

float MPU6050_Pitch = 0;				// 俯仰角
float MPU6050_Roll = 0;					// 横滚角
float MPU6050_Yaw = 0;					// 偏航角
uint16_t MPU6050_YawDeviaction_CNT = 0; // 偏航角零漂误差
int8_t Debug_InitResult = 100;
int16_t MPU6050_AngleRecord = 0;

/*
 * 来自DMP库官方例程
 * The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from thei
 * driver(s).
 * TODO: The following matrices refer to the configuration on an internal test
 * board at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static signed char gyro_orientation[9] = {-1, 0, 0, 0, -1, 0, 0, 0, 1};

/**
 * @brief 横滚方向的速度分量
 *
 * @param row 编码值
 *
 * @retval 返回行向量
 *
 * @note 来自DMP库官方例程
 * @note
 */
static unsigned short inv_row_2_scale(const signed char *row)
{
	unsigned short b;

	if (row[0] > 0)
		b = 0;
	else if (row[0] < 0)
		b = 4;
	else if (row[1] > 0)
		b = 1;
	else if (row[1] < 0)
		b = 5;
	else if (row[2] > 0)
		b = 2;
	else if (row[2] < 0)
		b = 6;
	else
		b = 7; // error
	return b;
}

/**
 * @brief 方向矩阵转换为标量值
 *
 * @param mtx 方向矩阵的数组
 *
 * @retval 无
 *
 * @note 来自DMP库官方例程
 * @note 方向矩阵orientation matrix
 * @note 标量值标量scalar
 * @note 方向矩阵通常用于表示三维空间中的旋转，提取旋转信息进行量化
 */
static unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
	unsigned short scalar;

	/*
	   XYZ  010_001_000 Identity Matrix
	   XZY  001_010_000
	   YXZ  010_000_001
	   YZX  000_010_001
	   ZXY  001_000_010
	   ZYX  000_001_010
	 */

	scalar = inv_row_2_scale(mtx);
	scalar |= inv_row_2_scale(mtx + 3) << 3;
	scalar |= inv_row_2_scale(mtx + 6) << 6;

	return scalar;
}

/**
 * @brief 传感器自检
 *
 * @param 无
 *
 * @retval 0 测试通过
 * @retval -1 测试失败
 *
 * @note 来自DMP库官方例程
 * @note 注释块中，教程提到是官方上报信息使用，暂未用到
 * @note 根据教程指引：加入了返回结果
 */
static int8_t run_self_test(void)
{
	int result;
	// char test_packet[4] = {0};
	long gyro[3], accel[3];

	result = mpu_run_self_test(gyro, accel);
	if (result == 0x7)
	{
		/* Test passed. We can trust the gyro data here, so let's push it down
		 * to the DMP.
		 */
		float sens;
		unsigned short accel_sens;
		mpu_get_gyro_sens(&sens);
		gyro[0] = (long)(gyro[0] * sens);
		gyro[1] = (long)(gyro[1] * sens);
		gyro[2] = (long)(gyro[2] * sens);
		dmp_set_gyro_bias(gyro);
		mpu_get_accel_sens(&accel_sens);
		accel[0] *= accel_sens;
		accel[1] *= accel_sens;
		accel[2] *= accel_sens;
		dmp_set_accel_bias(accel);
	}
	else
	{
		return -1;
	}

	return 0;

	/* Report results. */
	// test_packet[0] = 't';
	// test_packet[1] = result;
	// send_packet(PACKET_TYPE_MISC, test_packet);
}

/**
 * @brief MPU寄存器写
 *
 * @param reg 寄存器地址
 * @param data 写入的数据
 *
 * @retval 无
 *
 * @note 无
 */
void MPU6050_WriteReg(uint8_t reg, uint8_t data)
{
	IIC_WriteReg(MPU6050_ADDRESS, reg, 1, &data);
}

/**
 * @brief MPU寄存器读
 *
 * @param reg 寄存器地址
 * @param buf 数据缓冲区
 *
 * @retval 无
 *
 * @note 无
 */
void MPU6050_ReadReg(uint8_t reg, uint8_t *buf)
{
	IIC_ReadReg(MPU6050_ADDRESS, reg, 1, buf);
}

/**
 * @brief MPU读加速度、角速度
 *
 * @param AccX x轴加速度
 * @param AccY y轴加速度
 * @param AccZ z轴加速度
 * @param GyroX x轴角速度
 * @param GyroY y轴角速度
 * @param GyroZ z轴角速度
 *
 * @retval 无
 *
 * @note 无
 */
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;

	// x轴加速度
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_H, 1, &DataH);
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT_L, 1, &DataL);
	*AccX = (DataH << 8) | DataL;

	// y轴加速度
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_ACCEL_YOUT_H, 1, &DataH);
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_ACCEL_YOUT_L, 1, &DataL);
	*AccY = (DataH << 8) | DataL;

	// z轴加速度
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_ACCEL_ZOUT_H, 1, &DataH);
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_ACCEL_ZOUT_L, 1, &DataL);
	*AccZ = (DataH << 8) | DataL;

	// x轴角速度
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_GYRO_XOUT_H, 1, &DataH);
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_GYRO_XOUT_L, 1, &DataL);
	*GyroX = (DataH << 8) | DataL;

	// y轴角速度
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_GYRO_YOUT_H, 1, &DataH);
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_GYRO_YOUT_L, 1, &DataL);
	*GyroY = (DataH << 8) | DataL;

	// z轴角速度
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_GYRO_ZOUT_H, 1, &DataH);
	IIC_ReadReg(MPU6050_ADDRESS, MPU6050_GYRO_ZOUT_L, 1, &DataL);
	*GyroZ = (DataH << 8) | DataL;
}

/**
 * @brief DMP库初始化
 *
 * @param 无
 *
 * @retval 0 成功
 * @retval -1~-10 失败
 *
 * @note 无
 */
int8_t MPU6050_DMP_Init(void)
{
	struct int_param_s int_param;
	int result;

	// 初始化硬件
	result = mpu_init(&int_param);
	if (result != 0)
	{
		return -1;
	}

	// 设置硬件
	result = mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	if (result != 0)
	{
		return -2;
	}

	// 设置FIFO缓冲
	result = mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);
	if (result != 0)
	{
		return -3;
	}

	// 设置采样率
	result = mpu_set_sample_rate(DEFAULT_MPU_HZ);
	if (result != 0)
	{
		return -4;
	}

	result = dmp_load_motion_driver_firmware();
	if (result != 0)
	{
		return -5;
	}

	result = dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));
	if (result != 0)
	{
		return -6;
	}

	result = dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT |
								DMP_FEATURE_TAP |
								DMP_FEATURE_ANDROID_ORIENT |
								DMP_FEATURE_SEND_RAW_ACCEL |
								DMP_FEATURE_SEND_CAL_GYRO |
								DMP_FEATURE_GYRO_CAL);
	if (result != 0)
	{
		return -7;
	}

	result = dmp_set_fifo_rate(DEFAULT_MPU_HZ);
	if (result != 0)
	{
		return -8;
	}

	result = run_self_test();
	if (result != 0)
	{
		return -9;
	}

	result = mpu_set_dmp_state(1);
	if (result != 0)
	{
		return -10;
	}

	return 0;
}

/**
 * @brief DMP库获取欧拉角
 *
 * @param pitch 俯仰角
 * @param roll 横滚角
 * @param yaw 偏航角
 *
 * @retval 0 成功
 * @retval -1 失败
 *
 * @note 无
 */
float q0 = 0.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
short gyro[3];	// 角速度
short accel[3]; // 加速度
int8_t MPU6050_DMP_Get_Data(float *pitch, float *roll, float *yaw)
{
	long quat[4]; // 四元数
	unsigned long timestamp;
	short sensors;
	unsigned char more;

	if (dmp_read_fifo(gyro, accel, quat, &timestamp, &sensors, &more))
	{
		return -1;
	}
	if (sensors & INV_WXYZ_QUAT)
	{
		// Q30四元数转float浮点
		q0 = quat[0] / MPU6050_Q30;
		q1 = quat[1] / MPU6050_Q30;
		q2 = quat[2] / MPU6050_Q30;
		q3 = quat[3] / MPU6050_Q30;

		// 四元数转欧拉角
		*pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;
		*roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;
		*yaw = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;
	}

	return 0;
}

/**
 * @brief MPU6050消除稳态误差
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 在Python中，收集了842个数据，间隔时间1s，拟合一次函数(y=kx)：y=0.004101667*x
 */
void MPU6050_Remove_YawDeviaction(void)
{
	double k = 0.004101667; // 斜率

	MPU6050_Yaw = MPU6050_Yaw - k * (double)MPU6050_YawDeviaction_CNT; // 去除误差

	// 每次等待10s后
	vTaskDelay(10000);
	MPU6050_YawDeviaction_CNT += 10;
}

/**
 * @brief 角度转换
 *
 * @param Angle 角度(0°~180° 0°~-180°)
 *
 * @retval 无
 *
 * @note -180°~180°转换为0~360°
 */
float MPU6050_AngleConvert(float Angle)
{
	if (Angle < 0)
	{
		Angle = Angle + 360;
	}

	return Angle;
}

/**
 * @brief 计算对角度
 *
 * @param angle 原角度
 *
 * @retval 返回对角度
 *
 * @note 获取对角度，例如0°的对角度是180°
 */
int16_t MPU6050_OppositeAngle(int16_t angle)
{
	angle += 180;
	if (angle > 180)
		angle -= 360;

	return angle;
}

/**
 * @brief 阈值比较
 *
 * @param mpu_yaw 当前角度
 * @param compare_value 比较角度
 * @param vlaue ±区间角度
 *
 * @retval 1 区间之内
 * @retval 0 区间之外
 *
 * @note 若区间=10，比较值=30°，则比较范围=30°±10
 * @note 若当前偏航角在区间内返回1，否则返回0
 * @note 由于角度一定是0~360，因此：mini <= mpu_yaw && mpu_yaw <= 360 || 0 <= mpu_yaw && mpu_yaw <= max
 * @note 等价于：mini <= mpu_yaw || mpu_yaw <= max
 */
uint8_t MPU6050_ThresholdCompare(int16_t mpu_yaw, int16_t compare_value, int16_t vlaue)
{
	int16_t max = 0;
	int16_t mini = 0;

	// // 获取偏航角数值
	// mpu_yaw = (int16_t)MPU6050_Yaw;

	// ±区间
	max = compare_value + vlaue;
	mini = compare_value - vlaue;

	// 转换为360°
	mpu_yaw = MPU6050_AngleConvert(mpu_yaw);
	max = MPU6050_AngleConvert(max);
	mini = MPU6050_AngleConvert(mini);

	// 常规非跨界区域
	if (max >= mini)
	{
		return (mini <= mpu_yaw && mpu_yaw <= max) ? 1 : 0;
	}

	// 处理360°跨界区域，跨界时，max和mini数值倒转
	// 数轴两端合并一个圆，360°往后为0°，即350°~5°~10°
	if (max < mini)
	{
		return (mini <= mpu_yaw || mpu_yaw <= max) ? 1 : 0;
	}

	// 在区间之外
	return 0;
}

void MPU6050_YawAngleLog_Record(void)
{
	MPU6050_AngleRecord = MPU6050_Yaw;
}

int16_t MPU6050_YawAngleLog_Get(void)
{
	return MPU6050_AngleRecord;
}

/**
 * @brief MPU6050初始化
 *
 * @param 无
 *
 * @retval 无
 *
 * @note 参考江科大MPU6050初始化
 */
void MPU6050_InitPro(void)
{
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);

	while (Debug_InitResult != 0)
	{
		Debug_InitResult = MPU6050_DMP_Init();
		Delay_ms(300);
	}
}

/**
 * @brief 外部中断线1中断服务程序
 *
 * @param 无
 *
 * @retval 无
 *
 * @note MPU6050中断获取数据
 */
void EXTI1_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		if (!MPU6050_DMP_Get_Data(&MPU6050_Pitch, &MPU6050_Roll, &MPU6050_Yaw))
		{
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
