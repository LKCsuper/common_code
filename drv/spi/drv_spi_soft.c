/*
 * @Description: 软件模拟spi
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "drv.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 初始化SPI 引脚
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID SPI_IoInit(VOID)
{
    // rsp_hal_gpio_set_afio(2, 0, 0);
    // rsp_hal_gpio_set_afio(2, 1, 0);
    // rsp_hal_gpio_set_afio(2, 2, 0);
    // rsp_hal_gpio_set_afio(2, 3, 0);
    // rsp_hal_gpio_set_dir(2, 0, RSP_GPIO_OUTPUT);
    // rsp_hal_gpio_set_dir(2, 1, RSP_GPIO_OUTPUT);
    // rsp_hal_gpio_set_dir(2, 2, RSP_GPIO_OUTPUT);
    // rsp_hal_gpio_set_dir(2, 3, RSP_GPIO_INPUT);

    return;
}

/**
 * @description: 读取MISO
 * @detail: 
 * @return {*}
 * @author: lkc
 */
bool SPI_READ_MISO(VOID)
{
    bool val;
    rsp_hal_gpio_get_pin(2, 3, &val);
    return val;
}

/**
 * @description: 读写协议 
 * @detail: 
 * @param {UCHAR} ucData
 * @return {*}
 * @author: lkc
 */
UCHAR SPI_ReadWriteByte(UCHAR ucData)
{
    UCHAR i;
    UCHAR rxData = 0;
    for (i = 0; i < 8; i++) {
        SPI_SCK_0;
        rsp_hal_udelay(1);
        // 数据发送
        if (ucData & 0x80) {
            SPI_MOSI_1;
        } else {
            SPI_MOSI_0;
        }
        ucData <<= 1;
        rsp_hal_udelay(1);
        SPI_SCK_1;
        rsp_hal_udelay(1);
        // 数据接收
        rxData <<= 1;
        if (SPI_READ_MISO()) {
            rxData |= 0x01;
        }
        rsp_hal_udelay(1);
    }
    SPI_SCK_0;
    return rxData;
}

/**
 * @description: 读字节
 * @detail:
 * @return {*}
 * @author: lkc
 */
UCHAR SPI_ReadByte(VOID)
{
    return SPI_ReadWriteByte(Dummy_Byte);
}


/**
 * @description: 写字节
 * @detail:
 * @param {UCHAR} ucData
 * @return {*}
 * @author: lkc
 */
VOID SPI_WriteByte(UCHAR ucData)
{
    (VOID) SPI_ReadWriteByte(ucData);
}
#ifdef __cplusplus
}
#endif