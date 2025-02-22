/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-07-18 10:36:55
 * @LastEditors: lkc
 * @LastEditTime: 2023-07-19 17:51:31
 * @detail: 
 */
#ifndef _BSP_SPI_H
#define _BSP_SPI_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH                                                   SPI1
#define SPI1_PORT                                                   GPIOB
#define SPI1_NSS                                                    GPIO_PIN_12
#define SPI1_SCK                                                    GPIO_PIN_13
#define SPI1_MISO                                                   GPIO_PIN_14
#define SPI1_MOSI                                                   GPIO_PIN_15
#define SPI1_FLASH_CS_HIGH()                                        gpio_bit_set(SPI1_PORT, SPI1_NSS)
#define SPI1_FLASH_CS_LOW()                                         gpio_bit_reset(SPI1_PORT, SPI1_NSS)

#define SPI_FLASH_PAGE_SIZE                                         (0x100U)
#define WRITE            0x02     /* write to memory instruction */
#define WRSR             0x01     /* write status register instruction */
#define WREN             0x06     /* write enable instruction */

#define READ             0x03     /* read from memory instruction */
#define RDSR             0x05     /* read status register instruction  */
#define RDID             0x9F     /* read identification */
#define SE               0x20     /* sector erase instruction */
#define BE               0xC7     /* bulk erase instruction */

#define WIP_FLAG         0x01     /* write in progress(wip)flag */
#define DUMMY_BYTE       0xA5
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* 驱动接口 */
uint8_t Bsp_Spi1_SendByte(uint8_t byte);
uint16_t Bsp_Spi1_SendHalfword(uint16_t half_word);

/* spi flash 接口 */
void Bsp_SpiFlash_SectorErase(uint32_t sector_addr);
void Bsp_SpiFlash_BulkErase(void);
void Bsp_SpiFlash_PageWrite(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write);
void Bsp_SpiFlash_BufferWrite(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write);
void Bsp_SpiFlash_BufferRead(uint8_t *pbuffer, uint32_t read_addr, uint16_t num_byte_to_read);
uint32_t Bsp_SpiFlash_ReadId(void);
void Bsp_SpiFlash_StartReadSequence(uint32_t read_addr);
uint8_t Bsp_SpiFlash_ReadByte(void);
void Bsp_SpiFlash_WriteEnable(void);
void Bsp_SpiFlash_WaitForWriteEnd(void);
#ifdef __cplusplus
}
#endif
#endif
