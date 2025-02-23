#ifndef __APP_MPU6050_H__
#define __APP_MPU6050_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/**************************************************************************************/
#define IIC2_SCL_GPIO_PORT SOFT_I2C2_SCL_GPIO_Port
#define IIC2_SCL_GPIO_PIN  SOFT_I2C2_SCL_Pin
#define IIC2_SDA_GPIO_PORT SOFT_I2C2_SDA_GPIO_Port
#define IIC2_SDA_GPIO_PIN  SOFT_I2C2_SDA_Pin

#define I2C2_SCL_SET  LL_GPIO_SetOutputPin(IIC2_SCL_GPIO_PORT, IIC2_SCL_GPIO_PIN)
#define I2C2_SCL_CLR  LL_GPIO_ResetOutputPin(IIC2_SCL_GPIO_PORT, IIC2_SCL_GPIO_PIN)
#define I2C2_SDA_SET  LL_GPIO_SetOutputPin(IIC2_SDA_GPIO_PORT, IIC2_SDA_GPIO_PIN)
#define I2C2_SDA_CLR  LL_GPIO_ResetOutputPin(IIC2_SDA_GPIO_PORT, IIC2_SDA_GPIO_PIN)
#define I2C2_SDA_READ LL_GPIO_IsInputPinSet(IIC2_SDA_GPIO_PORT, IIC2_SDA_GPIO_PIN)
/**************************************************************************************/

//****************************************
// 定义MPU6050内部地址
//****************************************
#define SMPLRT_DIV   0x19 // 陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG       0x1A // 低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG  0x1B // 陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG 0x1C // 加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48
#define PWR_MGMT_1   0x6B // 电源管理，典型值：0x00(正常启用)
#define WHO_AM_I     0x75 // IIC地址寄存器(默认数值0x68，只读)
// #define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取
#define SlaveAddress 0x68
//****************************************
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
float MPU6050_GetTemp(void);
int GetData(unsigned char REG_Address);
void InitMPU6050(void);
#ifdef __cplusplus
}
#endif
#endif
