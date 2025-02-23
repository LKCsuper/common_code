#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_i2c_soft.h"
#include "app_mpu6050.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_SOFT_S gstMpuI2c;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static char I2C2_Scl(unsigned char ucData)
{
    if (ucData) {
        I2C2_SCL_SET;
    } else {
        I2C2_SCL_CLR;
    }
    return 0;
}

static char I2C2_Sda(unsigned char ucData)
{
    if (ucData) {
        I2C2_SDA_SET;
    } else {
        I2C2_SDA_CLR;
    }
    return 0;
}

static char I2C2_SdaRead(void)
{
    return I2C2_SDA_READ;
}

/**
 * @description: 延时回调
 * @detail:
 * @return {*}
 * @author: lkc
 */
static void I2C2_Delay(void)
{
    for (unsigned int i = 0; i < 100; i++) {
        // __NOP();
        volatile int temp = 0;
    }
}


void InitMPU6050(void)
{
    I2C_InterfaceResister(&gstMpuI2c, SlaveAddress, I2C2_Scl, I2C2_Sda, I2C2_SdaRead, I2C2_Delay);
    I2C_Stop(&gstMpuI2c);

    unsigned char ucData = 0;
    ucData = 0x00;
    SOFT_I2C_Mem_Write(&gstMpuI2c, SlaveAddress, PWR_MGMT_1, 1, &ucData, 1, 1000); // 解除休眠状态
    ucData = 0x07;
    SOFT_I2C_Mem_Write(&gstMpuI2c, SlaveAddress, SMPLRT_DIV, 1, &ucData, 1, 1000);
    ucData = 0x06;
    SOFT_I2C_Mem_Write(&gstMpuI2c, SlaveAddress, CONFIG, 1, &ucData, 1, 1000);
    ucData = 0x18;
    SOFT_I2C_Mem_Write(&gstMpuI2c, SlaveAddress, GYRO_CONFIG, 1, &ucData, 1, 1000);
    ucData = 0x01;
    SOFT_I2C_Mem_Write(&gstMpuI2c, SlaveAddress, ACCEL_CONFIG, 1, &ucData, 1, 1000);
    return;
}


//**************************************
// 合成数据
//**************************************
int GetData(unsigned char REG_Address)
{
    unsigned char H, L;

    SOFT_I2C_Mem_Read(&gstMpuI2c, SlaveAddress, REG_Address, 1, &H, 1, 1000);
    SOFT_I2C_Mem_Read(&gstMpuI2c, SlaveAddress, REG_Address + 1, 1, &L, 1, 1000);
    return (H << 8) + L; // 合成数据
}

//**************************************
// 显示温度
//**************************************
float MPU6050_GetTemp(void)
{
    unsigned char buf[2]; // 用于存放温度
    short raw;
    float temp;

    SOFT_I2C_Mem_Read(&gstMpuI2c, SlaveAddress, TEMP_OUT_H, 1, &buf[0], 1, 1000);
    SOFT_I2C_Mem_Read(&gstMpuI2c, SlaveAddress, TEMP_OUT_L, 1, &buf[1], 1, 1000);

    raw = ((unsigned short)buf[0] << 8) | buf[1];  // 将8位数据整合为16位
    temp = 36.53 + ((double)raw) / 340; // 得到温度值

    return temp;
}
#ifdef __cplusplus
}
#endif
