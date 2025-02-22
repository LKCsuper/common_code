/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */

#ifndef __SPI_SIM_H__
#define __SPI_SIM_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_CS_1            rsp_hal_gpio_set_pin(2, 0, 1)
#define SPI_CS_0            rsp_hal_gpio_set_pin(2, 0, 0)
#define SPI_SCK_1           rsp_hal_gpio_set_pin(2, 1, 1)             /* SCK = 1 */
#define SPI_SCK_0           rsp_hal_gpio_set_pin(2, 1, 0)             /* SCK = 0 */
#define SPI_MOSI_1          rsp_hal_gpio_set_pin(2, 2, 1)             /* MOSI = 1 */
#define SPI_MOSI_0          rsp_hal_gpio_set_pin(2, 2, 0)             /* MOSI = 0 */
#define Dummy_Byte    0xFF    //读取时MISO发送的数据，可以为任意数据
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID SPI_IoInit(VOID);
UCHAR SPI_ReadWriteByte(UCHAR txData);
UCHAR SPI_ReadByte(VOID);
VOID SPI_WriteByte(UCHAR txData);
#ifdef __cplusplus
}
#endif
