/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include <stdint.h>

#include "main.h"
#include "app_ath20.h"
#include "bsp_i2c_soft.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 延时回调
 * @detail:
 * @return {*}
 * @author: lkc
 */
static void I2C_Delay(void)
{
    for (unsigned int i = 0; i < 1000; i++) {
        // __NOP();
        volatile int temp = 0;
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
    I2C_InterfaceResister(&gstCommonI2c, I2C_SLAVE_ADDR, I2C_Scl, I2C_Sda, I2C_SdaRead, I2C_Delay);
    I2C_Stop(&gstCommonI2c);
}


uint8_t ATH20_Read_Status(void)//读取AHT10的状态寄存器
{
    uint8_t Byte_first;
    Sensors_I2C_ReadRegister(ATH20_SLAVE_ADDRESS, 0x00, 1, &Byte_first);

	return Byte_first;
}

uint8_t ATH20_Read_Cal_Enable(void)
{
    uint8_t val = 0;//ret = 0,

    val = ATH20_Read_Status();
    if((val & 0x68) == 0x08)  //判断NOR模式和校准输出是否有效
        return 1;
    else
        return 0;
}

void ATH20_Read_CTdata(uint32_t *ct) //读取AHT10的温度和湿度数据
{
    uint32_t RetuData = 0;
	uint16_t cnt = 0;
    uint8_t Data[10];
    uint8_t tmp[10];

    tmp[0] = 0x33;
    tmp[1] = 0x00;
	Sensors_I2C_WriteRegister(ATH20_SLAVE_ADDRESS, StartTest, 2, tmp);  //P0 口中断不使能
	I2C_DELAY_MS(75);//等待75ms

    cnt = 0;
	while(((ATH20_Read_Status()&0x80) == 0x80))//等待忙状态结束
	{
        I2C_DELAY_MS(1);
        if(cnt++ >= 100)
        {
            printf("ATH20_Read_CTdata error\n");
            break;
        }
	}

    Sensors_I2C_ReadRegister(ATH20_SLAVE_ADDRESS, 0x00, 7,Data);

	RetuData = 0;
    RetuData = (RetuData|Data[1]) << 8;
	RetuData = (RetuData|Data[2]) << 8;
	RetuData = (RetuData|Data[3]);
	RetuData = RetuData >> 4;
	ct[0] = RetuData;

    RetuData = 0;
	RetuData = (RetuData|Data[3]) << 8;
	RetuData = (RetuData|Data[4]) << 8;
	RetuData = (RetuData|Data[5]);
	RetuData = RetuData&0xfffff;
	ct[1] = RetuData;
}

uint8_t count;
uint8_t ATH20_Init(void)
{
    uint8_t tmp[10];

    I2C_DELAY_MS(40);

    tmp[0] = 0x08;
    tmp[1] = 0x00;
	Sensors_I2C_WriteRegister(ATH20_SLAVE_ADDRESS,INIT, 2, tmp);  //P0 口中断不使能

    I2C_DELAY_MS(500);
    count = 0;

    while(ATH20_Read_Cal_Enable() == 0)//需要等待状态字status的Bit[3]=1时才去读数据。如果Bit[3]不等于1 ，发软件复位0xBA给AHT10，再重新初始化AHT10，直至Bit[3]=1
    {
        Sensors_I2C_WriteRegister(ATH20_SLAVE_ADDRESS,SoftReset, 0, tmp);
        I2C_DELAY_MS(200);

        Sensors_I2C_WriteRegister(ATH20_SLAVE_ADDRESS,INIT, 2, tmp);

        count++;
        if(count >= 10)
            return 0;
        I2C_DELAY_MS(500);
    }
    return 1;
}
#ifdef __cplusplus
}
#endif
