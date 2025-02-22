/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#ifndef __BSP_I2C_SOFTWARE_H__
#define __BSP_I2C_SOFTWARE_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_SLAVE_ADDR 0XB0
#define I2C_READ_ADDR  0XB1
#define I2C_WRITE_ADDR 0XB0

#define IIC_SCL_GPIO_PORT I2C_CLOCK_GPIO_Port
#define IIC_SCL_GPIO_PIN  I2C_CLOCK_Pin
#define IIC_SDA_GPIO_PORT I2C_DATA_GPIO_Port
#define IIC_SDA_GPIO_PIN  I2C_DATA_Pin

#define I2C_SOFTWARE 1
#define I2C_TYPE     I2C_SOFT_S
/* Private typedef -----------------------------------------------------------*/
typedef char (*i2c_handler)(unsigned char);
typedef char (*i2c_read_handler)(void);
typedef char (*i2c_delay_handler)(void);

typedef struct tagI2cSoft {
    unsigned char ucDevAddr;  // 从设备地址
    i2c_handler sda;          // SDA控制
    i2c_handler scl;          // SCL控制
    i2c_read_handler sdaRead; // 读SDA
    i2c_delay_handler delay;  // 延时
} I2C_SOFT_S;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern I2C_SOFT_S gstPmbusI2c;
extern I2C_SOFT_S gstRtcI2c;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void I2C_SendByte(I2C_SOFT_S *i2c, uint8_t data);
uint8_t I2C_ReadByte(I2C_SOFT_S *i2c, uint8_t ack);
unsigned char I2C_CheckDevice(I2C_SOFT_S *i2c, unsigned char _Address);
void I2C_DeInit(I2C_SOFT_S *i2c);
void I2C_InterfaceResister(I2C_SOFT_S *i2c, unsigned char ucDevAddr, i2c_handler I2C_Scl, i2c_handler I2C_Sda,
                           i2c_read_handler I2C_SdaRead, i2c_delay_handler I2C_Delay);
bool SOFT_I2C_Mem_Write(void *i2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData,
                        uint16_t Size, uint64_t timeout);
bool SOFT_I2C_Mem_Read(void *i2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData,
                       uint16_t Size, uint64_t timeout);
void WriteBitToControlRegister(void *i2c, uint8_t ControlRegister, uint8_t BitNumber, uint8_t Value);
#ifdef __cplusplus
}
#endif
#endif
