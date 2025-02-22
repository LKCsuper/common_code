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
/*!
    \brief      erase the specified flash sector
    \param[in]  sector_addr: address of the sector to erase
    \param[out] none
    \retval     none
*/
void Bsp_SpiFlash_SectorErase(uint32_t sector_addr)
{
    /* send write enable instruction */
    Bsp_SpiFlash_WriteEnable();

    /* sector erase */
    /* select the flash: chip select low */
    SPI1_FLASH_CS_LOW();
    /* send sector erase instruction */
    Bsp_Spi1_SendByte(SE);
    /* send sector_addr high nibble address byte */
    Bsp_Spi1_SendByte((sector_addr & 0xFF0000) >> 16);
    /* send sector_addr medium nibble address byte */
    Bsp_Spi1_SendByte((sector_addr & 0xFF00) >> 8);
    /* send sector_addr low nibble address byte */
    Bsp_Spi1_SendByte(sector_addr & 0xFF);
    /* deselect the flash: chip select high */
    SPI1_FLASH_CS_HIGH();

    /* wait the end of flash writing */
    Bsp_SpiFlash_WaitForWriteEnd();
}

/*!
    \brief      erase the entire flash
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Bsp_SpiFlash_BulkErase(void)
{
    /* send write enable instruction */
    Bsp_SpiFlash_WriteEnable();

    /* bulk erase */
    /* select the flash: chip select low */
    SPI1_FLASH_CS_LOW();
    /* send bulk erase instruction */
    Bsp_Spi1_SendByte(BE);
    /* deselect the flash: chip select high */
    SPI1_FLASH_CS_HIGH();

    /* wait the end of flash writing */
    Bsp_SpiFlash_WaitForWriteEnd();
}

/*!
    \brief      write more than one byte to the flash
    \param[in]  pbuffer: pointer to the buffer
    \param[in]  write_addr: flash's internal address to write
    \param[in]  num_byte_to_write: number of bytes to write to the flash
    \param[out] none
    \retval     none
*/
void Bsp_SpiFlash_PageWrite(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    /* enable the write access to the flash */
    Bsp_SpiFlash_WriteEnable();

    /* select the flash: chip select low */
    SPI1_FLASH_CS_LOW();

    /* send "write to memory" instruction */
    Bsp_Spi1_SendByte(WRITE);
    /* send write_addr high nibble address byte to write to */
    Bsp_Spi1_SendByte((write_addr & 0xFF0000) >> 16);
    /* send write_addr medium nibble address byte to write to */
    Bsp_Spi1_SendByte((write_addr & 0xFF00) >> 8);
    /* send write_addr low nibble address byte to write to */
    Bsp_Spi1_SendByte(write_addr & 0xFF);

    /* while there is data to be written on the flash */
    while (num_byte_to_write--) {
        /* send the current byte */
        Bsp_Spi1_SendByte(*pbuffer);
        /* point on the next byte to be written */
        pbuffer++;
    }

    /* deselect the flash: chip select high */
    SPI1_FLASH_CS_HIGH();

    /* wait the end of flash writing */
    Bsp_SpiFlash_WaitForWriteEnd();
}

/*!
    \brief      write block of data to the flash
    \param[in]  pbuffer: pointer to the buffer
    \param[in]  write_addr: flash's internal address to write
    \param[in]  num_byte_to_write: number of bytes to write to the flash
    \param[out] none
    \retval     none
*/
void Bsp_SpiFlash_BufferWrite(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    uint8_t num_of_page = 0, num_of_single = 0, addr = 0, count = 0, temp = 0;

    addr = write_addr % SPI_FLASH_PAGE_SIZE;
    count = SPI_FLASH_PAGE_SIZE - addr;
    num_of_page = num_byte_to_write / SPI_FLASH_PAGE_SIZE;
    num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;

    /* write_addr is SPI_FLASH_PAGE_SIZE aligned */
    if (0 == addr) {
        /* num_byte_to_write < SPI_FLASH_PAGE_SIZE */
        if (0 == num_of_page) {
            Bsp_SpiFlash_PageWrite(pbuffer, write_addr, num_byte_to_write);
        }
        /* num_byte_to_write > SPI_FLASH_PAGE_SIZE */
        else {
            while (num_of_page--) {
                Bsp_SpiFlash_PageWrite(pbuffer, write_addr, SPI_FLASH_PAGE_SIZE);
                write_addr += SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }
            Bsp_SpiFlash_PageWrite(pbuffer, write_addr, num_of_single);
        }
    } else {
        /* write_addr is not SPI_FLASH_PAGE_SIZE aligned */
        if (0 == num_of_page) {
            /* (num_byte_to_write + write_addr) > SPI_FLASH_PAGE_SIZE */
            if (num_of_single > count) {
                temp = num_of_single - count;
                Bsp_SpiFlash_PageWrite(pbuffer, write_addr, count);
                write_addr += count;
                pbuffer += count;
                Bsp_SpiFlash_PageWrite(pbuffer, write_addr, temp);
            } else {
                Bsp_SpiFlash_PageWrite(pbuffer, write_addr, num_byte_to_write);
            }
        } else {
            /* num_byte_to_write > SPI_FLASH_PAGE_SIZE */
            num_byte_to_write -= count;
            num_of_page = num_byte_to_write / SPI_FLASH_PAGE_SIZE;
            num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;

            Bsp_SpiFlash_PageWrite(pbuffer, write_addr, count);
            write_addr += count;
            pbuffer += count;

            while (num_of_page--) {
                Bsp_SpiFlash_PageWrite(pbuffer, write_addr, SPI_FLASH_PAGE_SIZE);
                write_addr += SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }

            if (0 != num_of_single) {
                Bsp_SpiFlash_PageWrite(pbuffer, write_addr, num_of_single);
            }
        }
    }
}

/*!
    \brief      read a block of data from the flash
    \param[in]  pbuffer: pointer to the buffer that receives the data read from the flash
    \param[in]  read_addr: flash's internal address to read from
    \param[in]  num_byte_to_read: number of bytes to read from the flash
    \param[out] none
    \retval     none
*/
void Bsp_SpiFlash_BufferRead(uint8_t *pbuffer, uint32_t read_addr, uint16_t num_byte_to_read)
{
    /* select the flash: chip slect low */
    SPI1_FLASH_CS_LOW();

    /* send "read from memory " instruction */
    Bsp_Spi1_SendByte(READ);

    /* send read_addr high nibble address byte to read from */
    Bsp_Spi1_SendByte((read_addr & 0xFF0000) >> 16);
    /* send read_addr medium nibble address byte to read from */
    Bsp_Spi1_SendByte((read_addr & 0xFF00) >> 8);
    /* send read_addr low nibble address byte to read from */
    Bsp_Spi1_SendByte(read_addr & 0xFF);

    /* while there is data to be read */
    while (num_byte_to_read--) {
        /* read a byte from the flash */
        *pbuffer = Bsp_Spi1_SendByte(DUMMY_BYTE);
        /* point to the next location where the byte read will be saved */
        pbuffer++;
    }

    /* deselect the flash: chip select high */
    SPI1_FLASH_CS_HIGH();
}

/*!
    \brief      read flash identification
    \param[in]  none
    \param[out] none
    \retval     flash identification
*/
uint32_t Bsp_SpiFlash_ReadId(void)
{
    uint32_t temp = 0, temp0 = 0, temp1 = 0, temp2 = 0;

    /* select the flash: chip select low */
    SPI1_FLASH_CS_LOW();

    /* send "RDID " instruction */
    Bsp_Spi1_SendByte(RDID);

    /* read a byte from the flash */
    temp0 = Bsp_Spi1_SendByte(DUMMY_BYTE);

    /* read a byte from the flash */
    temp1 = Bsp_Spi1_SendByte(DUMMY_BYTE);

    /* read a byte from the flash */
    temp2 = Bsp_Spi1_SendByte(DUMMY_BYTE);

    /* deselect the flash: chip select high */
    SPI1_FLASH_CS_HIGH();

    temp = (temp0 << 16) | (temp1 << 8) | temp2;

    return temp;
}

/*!
    \brief      initiate a read data byte (read) sequence from the flash
    \param[in]  read_addr: flash's internal address to read from
    \param[out] none
    \retval     none
*/
void Bsp_SpiFlash_StartReadSequence(uint32_t read_addr)
{
    /* select the flash: chip select low */
    SPI1_FLASH_CS_LOW();

    /* send "read from memory " instruction */
    Bsp_Spi1_SendByte(READ);

    /* send the 24-bit address of the address to read from */
    /* send read_addr high nibble address byte */
    Bsp_Spi1_SendByte((read_addr & 0xFF0000) >> 16);
    /* send read_addr medium nibble address byte */
    Bsp_Spi1_SendByte((read_addr & 0xFF00) >> 8);
    /* send read_addr low nibble address byte */
    Bsp_Spi1_SendByte(read_addr & 0xFF);
}

/*!
    \brief      read a byte from the SPI flash
    \param[in]  none
    \param[out] none
    \retval     byte read from the SPI flash
*/
uint8_t Bsp_SpiFlash_ReadByte(void)
{
    return (Bsp_Spi1_SendByte(DUMMY_BYTE));
}

/*!
    \brief      enable the write access to the flash
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Bsp_SpiFlash_WriteEnable(void)
{
    /* select the flash: chip select low */
    SPI1_FLASH_CS_LOW();

    /* send "write enable" instruction */
    Bsp_Spi1_SendByte(WREN);

    /* deselect the flash: chip select high */
    SPI1_FLASH_CS_HIGH();
}

/*!
    \brief      poll the status of the write in progress(wip) flag in the flash's status register
    \param[in]  none
    \param[out] none
    \retval     none
*/
void Bsp_SpiFlash_WaitForWriteEnd(void)
{
    uint8_t flash_status = 0;

    /* select the flash: chip select low */
    SPI1_FLASH_CS_LOW();

    /* send "read status register" instruction */
    Bsp_Spi1_SendByte(RDSR);

    /* loop as long as the memory is busy with a write cycle */
    do {
        /* send a dummy byte to generate the clock needed by the flash
        and put the value of the status register in flash_status variable */
        flash_status = Bsp_Spi1_SendByte(DUMMY_BYTE);
    } while ((flash_status & WIP_FLAG) == SET);

    /* deselect the flash: chip select high */
    SPI1_FLASH_CS_HIGH();
}

/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint16_t length)
{
    while (length--) {
        if (*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

#define BUFFER_SIZE    256
#define TX_BUFFER_SIZE BUFFER_SIZE
#define RX_BUFFER_SIZE BUFFER_SIZE

#define VERSION_ID          "1.0.0"
#define SFLASH_ID           0xEF4018
#define FLASH_WRITE_ADDRESS 0x000000
#define FLASH_READ_ADDRESS  FLASH_WRITE_ADDRESS

uint32_t int_device_serial[3];
uint8_t count;
uint8_t tx_buffer[TX_BUFFER_SIZE];
uint8_t rx_buffer[TX_BUFFER_SIZE];
uint32_t flash_id = 0;
uint16_t i = 0;
uint8_t is_successful = 0;

int Bsp_SpiFlash_Init(VOID)
{
    ULONG flash_id = Bsp_SpiFlash_ReadId();

    PRINTF("\n\rThe Flash_ID:0x%X\n\r", flash_id);

    /* flash id is correct */
    if (SFLASH_ID == flash_id) {
        PRINTF("\n\r\n\rWrite to tx_buffer:\n\r\n\r");

        /* printf tx_buffer value */
        for (i = 0; i < BUFFER_SIZE; i++) {
            tx_buffer[i] = i;
            PRINTF("0x%02X ", tx_buffer[i]);

            if (15 == i % 16) {
                PRINTF("\n\r");
            }
        }

        PRINTF("\n\r\n\rRead from rx_buffer:\n\r\n\r");

        /* erases the specified flash sector */
        Bsp_SpiFlash_SectorErase(FLASH_WRITE_ADDRESS);
        /* write tx_buffer data to the flash */
        Bsp_SpiFlash_BufferWrite(tx_buffer, FLASH_WRITE_ADDRESS, TX_BUFFER_SIZE);

        rt_thread_delay(10);

        /* read a block of data from the flash to rx_buffer */
        Bsp_SpiFlash_BufferRead(rx_buffer, FLASH_READ_ADDRESS, RX_BUFFER_SIZE);
        /* printf rx_buffer value */
        for (i = 0; i <= 255; i++) {
            PRINTF("0x%02X ", rx_buffer[i]);
            if (15 == i % 16) {
                PRINTF("\n\r");
            }
        }

        if (ERROR == memory_compare(tx_buffer, rx_buffer, 256)) {
            PRINTF("Err:Data Read and Write aren't Matching.\n\r");
            is_successful = 1;
        }

        /* spi flash test passed */
        if (0 == is_successful) {
            PRINTF("\n\rSPI-GD25Q16 Test Passed!\n\r");
        } else {
            /* spi flash read id fail */
            PRINTF("\n\rSPI Flash: Read ID Fail!\n\r");
        }
    }
    return 0;
}

// INIT_DEVICE_EXPORT(Bsp_SpiFlash_Init);
#ifdef __cplusplus
}
#endif
