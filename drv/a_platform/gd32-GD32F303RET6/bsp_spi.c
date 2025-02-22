/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-07-18 10:36:47
 * @LastEditors: lkc
 * @LastEditTime: 2023-07-18 13:41:56
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: spi1初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Spi1_Init(VOID)
{
    spi_parameter_struct spi_init_struct;

    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_SPI1);

    /* SPI0_SCK(PA5), SPI0_MISO(PA6) and SPI0_MOSI(PA7) GPIO pin configuration */
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, SPI1_SCK | SPI1_MOSI);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, SPI1_MISO);
    /* SPI0_CS(PE3) GPIO pin configuration */
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, SPI1_NSS);

    /* chip select invalid*/
    SPI1_FLASH_CS_HIGH();

    /* SPI1 parameter configuration */
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.frame_size = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.prescale = SPI_PSC_8;
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI_FLASH, &spi_init_struct);

    /* enable SPI1 */
    spi_enable(SPI_FLASH);

    PRINTF("Bsp Spi1 Flash Init\r\n");

    return;
}

/*!
    \brief      send a byte through the SPI interface and return the byte received from the SPI bus
    \param[in]  byte: byte to send
    \param[out] none
    \retval     the value of the received byte
*/
uint8_t Bsp_Spi1_SendByte(uint8_t byte)
{
    /* loop while data register in not empty */
    while (RESET == spi_i2s_flag_get(SPI_FLASH, SPI_FLAG_TBE))
        ;

    /* send byte through the SPI0 peripheral */
    spi_i2s_data_transmit(SPI_FLASH, byte);

    /* wait to receive a byte */
    while (RESET == spi_i2s_flag_get(SPI_FLASH, SPI_FLAG_RBNE))
        ;

    /* return the byte read from the SPI bus */
    return (spi_i2s_data_receive(SPI_FLASH));
}

/*!
    \brief      send a half word through the SPI interface and return the half word received from the SPI bus
    \param[in]  half_word: half word to send
    \param[out] none
    \retval     the value of the received byte
*/
uint16_t Bsp_Spi1_SendHalfword(uint16_t half_word)
{
    /* loop while data register in not emplty */
    while (RESET == spi_i2s_flag_get(SPI_FLASH, SPI_FLAG_TBE))
        ;

    /* send half word through the SPI0 peripheral */
    spi_i2s_data_transmit(SPI_FLASH, half_word);

    /* wait to receive a half word */
    while (RESET == spi_i2s_flag_get(SPI_FLASH, SPI_FLAG_RBNE))
        ;

    /* return the half word read from the SPI bus */
    return spi_i2s_data_receive(SPI_FLASH);
}

/**
 * @description: spi初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
int Bsp_Spi_Init(VOID)
{
    Bsp_Spi1_Init();

    return 0;
}

INIT_BOARD_EXPORT(Bsp_Spi_Init);
#ifdef __cplusplus
}
#endif
