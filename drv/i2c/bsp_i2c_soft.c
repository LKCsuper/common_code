/*
 * @Description: 软件模拟i2c
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "stdio.h"
#include "main.h"
#include "bsp_i2c_soft.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
extern uint32_t SystemCoreClock;
#define SYSCLK_MHZ (SystemCoreClock / 1000000) // 系统时钟频率
I2C_SOFT_S gstRtcI2c;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static char I2C_Scl(unsigned char ucData);
static char I2C_Sda(unsigned char ucData);
static char I2C_SdaRead(void);
static void I2C_Delay(void);
void I2C_Stop(I2C_SOFT_S *i2c);
extern void Soft_I2C_Delay(uint32_t dly);
/**
 * @brief       初始化IIC
 * @param       无
 * @retval      无
 */
void I2C_Init(void)
{
    // GPIO_InitTypeDef gpio_init_struct;

    // // sda scl cs 时钟使能
    // __HAL_RCC_GPIOB_CLK_ENABLE();

    // gpio_init_struct.Pin = I2C_CLOCK_Pin;
    // gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    // gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    // gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* 快速 */
    // HAL_GPIO_Init(I2C2_CLOCK_GPIO_Port, &gpio_init_struct); /* SCL */

    // gpio_init_struct.Pin = I2C2_DATA_Pin;
    // gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;           /* 开漏输出 */
    // HAL_GPIO_Init(I2C2_DATA_GPIO_Port, &gpio_init_struct); /* SDA */
    /* SDA引脚模式设置,开漏输出,上拉, 这样就不用再设置IO方向了, 开漏输出的时候(=1), 也可以读取外部信号的高低电平 */
    // 地址添加
    I2C_InterfaceResister(&gstRtcI2c, I2C_SLAVE_ADDR, I2C_Scl, I2C_Sda, I2C_SdaRead, I2C_Delay);
    I2C_Stop(&gstRtcI2c);
}



/**
 * @description: 延时微秒
 * @detail: fix us 之前没有加进这个函数
 * @param {unsigned int} 
 * @return {*}
 * @author: lkc
 */
void delay_us(unsigned int us)
{
    for (unsigned int i = 0; i < I2C_DELAY_US_NUM * us; i++) {
        // __NOP();
        volatile int temp = 0;
    }
}

/**
 * @description: 延时毫秒
 * @detail:
 * @param {unsigned int} ms
 * @return {*}
 * @author: lkc
 */
void delay_ms(unsigned int ms)
{
    while (ms-- != 0) {
        delay_us(1000);
    }
}
/**
 * @description: 延时秒
 * @detail:
 * @param {unsigned int} s
 * @return {*}
 * @author: lkc
 */
void delay_s(unsigned int s)
{
    while (s-- != 0) {
        delay_ms(1000);
    }
}

static char I2C_Scl(unsigned char ucData)
{
    if (ucData) {
        I2C_SCL_SET;
    } else {
        I2C_SCL_CLR;
    }
    return 0;
}

static char I2C_Sda(unsigned char ucData)
{
    if (ucData) {
        I2C_SDA_SET;
    } else {
        I2C_SDA_CLR;
    }
    return 0;
}

static char I2C_SdaRead(void)
{
    return I2C_SDA_READ;
}

/**
 * @description: 延时回调
 * @detail:
 * @return {*}
 * @author: lkc
 */
static void I2C_Delay(void)
{
    // delay_us(1);
    delay_us(10);
}

/**
 * @description: 注册接口
 * @detail:
 * @return {*}
 * @author: lkc
 */
void I2C_InterfaceResister(I2C_SOFT_S *i2c, unsigned char ucDevAddr, i2c_handler I2C_Scl, i2c_handler I2C_Sda,
                           i2c_read_handler I2C_SdaRead, i2c_delay_handler I2C_Delay)
{
    i2c->scl = I2C_Scl;
    i2c->sda = I2C_Sda;
    i2c->sdaRead = I2C_SdaRead;
    i2c->delay = I2C_Delay;
    i2c->ucDevAddr = ucDevAddr;

    return;
}

/**
 * @brief       初始化IIC
 * @param       无
 * @retval      无
 */
void I2C_DeInit(I2C_SOFT_S *i2c)
{
    I2C_Stop(i2c); /* 停止总线上所有设备 */
    return;
}

/**
 * @brief       产生IIC起始信号
 * @param       无
 * @retval      无
 */
void I2C_Start(I2C_SOFT_S *i2c)
{
    i2c->sda(1);
    i2c->delay();
    i2c->scl(1);
    i2c->delay();
    if (!i2c->sdaRead()) {
        printf("I2C_Start: bus busy\n");
        return;
    }

    i2c->sda(0); /* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
    i2c->delay();
    i2c->scl(0); /* 钳住I2C总线，准备发送或接收数据 */
    i2c->delay();
    if (i2c->sdaRead()) {
        printf("I2C_Start: bus error\n");
        return;
    }

    return;
}

/**
 * @brief       产生IIC停止信号
 * @param       无
 * @retval      无
 */
void I2C_Stop(I2C_SOFT_S *i2c)
{
    i2c->sda(0); /* STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号 */
    i2c->delay();
    i2c->scl(1);
    i2c->delay();
    i2c->sda(1); /* 发送I2C总线结束信号 */
    i2c->delay();

    return;
}

/**
 * @brief       等待应答信号到来
 * @param       无
 * @retval      1，接收应答失败
 *              0，接收应答成功
 */
unsigned char I2C_WaitAck(I2C_SOFT_S *i2c)
{
    unsigned char waittime = 0;
    unsigned char rack = SUCCESS;

    i2c->sda(1); /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
    i2c->delay();
    i2c->scl(1); /* SCL=1, 此时从机可以返回ACK */
    i2c->delay();

    while (i2c->sdaRead()) /* 等待应答 */
    {
        waittime++;

        if (waittime > 250) {
            I2C_Stop(i2c);
            rack = ERROR;
            // printf("I2C_WaitAck: wait ack timeout\n");
            break;
        }
    }

    i2c->scl(0); /* SCL=0, 结束ACK检查 */
    i2c->delay();
    return rack;
}

/**
 * @brief       产生ACK应答
 * @param       无
 * @retval      无
 */
void I2C_Ack(I2C_SOFT_S *i2c)
{
    i2c->sda(0); /* SCL 0 -> 1 时 SDA = 0,表示应答 */
    i2c->delay();
    i2c->scl(1); /* 产生一个时钟 */
    i2c->delay();
    i2c->scl(0);
    i2c->delay();
    i2c->sda(1); /* 主机释放SDA线 */
    i2c->delay();
}

/**
 * @brief       不产生ACK应答
 * @param       无
 * @retval      无
 */
void I2C_Nack(I2C_SOFT_S *i2c)
{
    i2c->sda(1); /* SCL 0 -> 1  时 SDA = 1,表示不应答 */
    i2c->delay();
    i2c->scl(1); /* 产生一个时钟 */
    i2c->delay();
    i2c->scl(0);
    i2c->delay();

    return;
}

/**
 * @brief       IIC发送一个字节
 * @param       data: 要发送的数据
 * @retval      无
 */
void I2C_SendByte(I2C_SOFT_S *i2c, unsigned char data)
{
    unsigned char t;

    i2c->scl(0); /* 先将SCL拉低 */
    for (t = 0; t < 8; t++) {
        i2c->sda((data & 0x80) >> 7); /* 高位先发送 */
        data <<= 1; /* 左移1位,用于下一次发送 */
        i2c->delay();
        i2c->scl(1);
        i2c->delay();
        i2c->scl(0);
        i2c->delay();
    }
    i2c->sda(1); /* 发送完成, 主机释放SDA线 */
    I2C_WaitAck(i2c); // 等待应答
    return;
}

/**
 * @brief       IIC读取一个字节
 * @param       ack:  ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
unsigned char I2C_ReadByte(I2C_SOFT_S *i2c, unsigned char ack)
{
    unsigned char i, receive = 0;

    i2c->sda(1); /* 释放SDA线 */
    i2c->scl(0);
    receive = 0; 

    for (i = 0; i < 8; i++) /* 接收1个字节数据 */
    {
        i2c->scl(1);
        i2c->delay();
        receive <<= 1; /* 高位先输出,所以先收到的数据位要左移 */

        if (i2c->sdaRead()) {
            receive |= 0x01;
        }

        i2c->scl(0);
        i2c->delay();
    }

    if (!ack) { 
        I2C_Nack(i2c); /* 发送nACK */
    } else {
        I2C_Ack(i2c); /* 发送ACK */
    }

    return receive;
}

/**
 * @description: 检测I2C是否存在应答
 * @detail:
 * @param {unsigned char} _Address
 * @return {*}
 * @author: lkc
 */
unsigned char I2C_CheckDevice(I2C_SOFT_S *i2c, unsigned char _Address)
{
    unsigned char ucAck;
    I2C_Start(i2c); /* 发送启动信号 */
    /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
    I2C_SendByte(i2c, _Address | 0);
    ucAck = I2C_WaitAck(i2c); /* 检测设备的ACK应答 */
    I2C_Stop(i2c);            /* 发送停止信号 */

    return ucAck;
}

/**
 * @description:
 * @detail:
 * @return {*}
 * @author: lkc
 */
char SOFT_I2C_Mem_Write(void *i2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, unsigned char *pData,
                        uint16_t Size, uint64_t timeout)
{
    DevAddress = DevAddress << 1;

    // 1. 产生起始信号
    I2C_Start(i2c);

    // 2. 发送设备地址，写控制位 (0)
    I2C_SendByte(i2c, DevAddress | 0);

    // 3. 发送内存地址
    if (MemAddSize == 1) { // 单字节地址
        I2C_SendByte(i2c, (unsigned char)(MemAddress & 0xFF));
    } else if (MemAddSize == 2) {                                     // 双字节地址
        I2C_SendByte(i2c, (unsigned char)((MemAddress >> 8) & 0xFF)); // 发送高字节
        I2C_SendByte(i2c, (unsigned char)(MemAddress & 0xFF)); // 发送低字节
    }

    // 4. 发送数据
    for (uint16_t i = 0; i < Size; i++) {
        I2C_SendByte(i2c, pData[i]);
    }

    // 5. 产生停止信号
    I2C_Stop(i2c);
    return true; // 写入成功
    // return HAL_I2C_Mem_Write((I2C_HandleTypeDef *)i2c, DevAddress, MemAddress, MemAddSize, pData, Size, timeout);
}

/**
 * @description: SOFT_I2C_Mem_Read
 * @detail:
 * @return {*}
 * @author: lkc
 */
char SOFT_I2C_Mem_Read(void *i2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, unsigned char *pData,
                       uint16_t Size, uint64_t timeout)
{
    unsigned char ack;

    DevAddress = DevAddress << 1;

    // 1. 产生起始信号
    I2C_Start(i2c);

    // 2. 发送设备地址，写控制位 (0)
    I2C_SendByte(i2c, DevAddress | 0);

    // 3. 发送内存地址
    if (MemAddSize == 1) {
        I2C_SendByte(i2c, (unsigned char)(MemAddress & 0xFF));
    } else if (MemAddSize == 2) {
        I2C_SendByte(i2c, (unsigned char)((MemAddress >> 8) & 0xFF)); // 发送高字节
        I2C_SendByte(i2c, (unsigned char)(MemAddress & 0xFF)); // 发送低字节
    }

    // 4. 产生重复起始信号
    I2C_Start(i2c);

    // 5. 发送设备地址，读控制位 (1)
    I2C_SendByte(i2c, DevAddress| 1);

    // 6. 读取数据
    for (uint16_t i = 0; i < Size; i++) {
        if (i == (Size - 1)) {
            // 最后一个字节发送 NACK
            pData[i] = I2C_ReadByte(i2c, 0);
        } else {
            // 发送 ACK
            pData[i] = I2C_ReadByte(i2c, 1);
        }
    }

    // 7. 产生停止信号
    I2C_Stop(i2c);

    return true; // 读取成功
    // return HAL_I2C_Mem_Read((I2C_HandleTypeDef *)i2c, DevAddress, MemAddress, MemAddSize, pData, Size, timeout);
}

/**
 * @description: WriteBitToControlRegister
 * @detail: 写某一位到控制寄存器
 * @param {void} *i2c
 * @param {unsigned char} ControlRegister
 * @param {unsigned char} BitNumber
 * @param {unsigned char} Value
 * @return {*}
 * @author: lkc
 */
// void WriteBitToControlRegister(void *i2c, unsigned char ControlRegister, unsigned char BitNumber, unsigned char Value,
//                                unsigned int ulTimeOut)
// {
//     unsigned char tmp;

//     if (Value > 1) {
//         Value = 1;
//     }
//     if (ControlRegister > 1) {
//         Value = 1;
//     }
//     if (BitNumber > 7) {
//         Value = 7;
//     }

//     SOFT_I2C_Mem_Read(i2c, PCF8563_ADDRESS, ControlRegister ? PCF8563_REG_CONTROL_STATUS1 : PCF8563_REG_CONTROL_STATUS2,
//                       1, &tmp, 1, ulTimeOut);

//     tmp &= ~(1 << BitNumber);
//     tmp |= (Value << BitNumber);
//     //    // ControlRegister?(Value &= 0b10101000):(Value &= 0b00011111); // Put zeros where zero is needed

//     if (ControlRegister) {
//         Value &= 0xA8; // Put zeros where zero is needed
//     } else {
//         Value &= 0x1F; // Put zeros where zero is needed
//     }

//     SOFT_I2C_Mem_Write(i2c, PCF8563_ADDRESS,
//                        ControlRegister ? PCF8563_REG_CONTROL_STATUS1 : PCF8563_REG_CONTROL_STATUS2, 1, &Value, 1,
//                        ulTimeOut);

//     return;
// }
#ifdef __cplusplus
}
#endif
