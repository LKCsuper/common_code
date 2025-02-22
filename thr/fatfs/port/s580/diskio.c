/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "diskio.h" /* FatFs lower layer API */
#include "ff_port.h"

extern fs_disk_t fs_disk;
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE_X pdrv /* Physical drive nmuber to identify the drive */
)
{
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE_X pdrv /* Physical drive nmuber to identify the drive */
)
{
    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE_X pdrv,    /* Physical drive nmuber to identify the drive */
                  BYTE_X *buff,   /* Data buffer to store read data */
                  DWORD_X sector, /* Start sector in LBA */
                  UINT_X count    /* Number of sectors to read */
)
{
#if CONFIG_FILE_SFC_FLASH
    if (HAL_OK != cap_store_flash_read(fs_disk.sector_start_addr + sector * fs_disk.sector_size, (void *)buff,
                                       count * fs_disk.sector_size)) {
        printf("read error addr %x size %d\n", fs_disk.sector_start_addr + sector * fs_disk.sector_size,
               count * fs_disk.sector_size);
        return RES_ERROR;
    }
#else
    if (HAL_OK != rsp_hal_spi_read_flash(&spi_master_cap_store,
                                         fs_disk.sector_start_addr + fs_disk.sector_size * sector,
                                         (uint8_t *)buff, count * fs_disk.sector_size)) {
        printf("read error addr %x size %d\n", fs_disk.sector_start_addr + fs_disk.sector_size * sector,
               count * fs_disk.sector_size);
        return RES_ERROR;
    }
#endif

    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(BYTE_X pdrv,        /* Physical drive nmuber to identify the drive */
                   const BYTE_X *buff, /* Data to be written */
                   DWORD_X sector,     /* Start sector in LBA */
                   UINT_X count        /* Number of sectors to write */
)
{
#if CONFIG_FILE_SFC_FLASH
    if (HAL_OK != cap_store_flash_write(fs_disk.sector_start_addr + sector * fs_disk.sector_size, (void *)buff,
                                        count * fs_disk.sector_size)) {
        printf("write error sector%d startaddr %x sector size %daddr %x size %d\n", sector, fs_disk.sector_start_addr,
               fs_disk.sector_size, fs_disk.sector_start_addr + sector * fs_disk.sector_size,
               count * fs_disk.sector_size);
        return RES_ERROR;
    }
#else
    if (HAL_OK != rsp_hal_spi_write_flash(&spi_master_cap_store,
                                          fs_disk.sector_start_addr + fs_disk.sector_size * sector,
                                          (uint8_t *)buff, count * fs_disk.sector_size)) {
        printf("write error addr %x size %d\n", fs_disk.sector_start_addr + fs_disk.sector_size * sector,
               count * fs_disk.sector_size);
        return RES_ERROR;
    }
#endif

    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE_X pdrv, /* Physical drive nmuber (0..) */
                   BYTE_X cmd,  /* Control code */
                   void *buff   /* Buffer to send/receive control data */
)
{
    /* nor flash */
    DRESULT ret = RES_OK;
    switch (cmd) {
        case CTRL_SYNC:
            break;
        case GET_SECTOR_COUNT:
            *(uint32_t *)buff = fs_disk.sector_count;
            break;
        case GET_SECTOR_SIZE:
            *(uint32_t *)buff = fs_disk.sector_size;
            ;
            break;
        case GET_BLOCK_SIZE:
            *(uint32_t *)buff = 1;
            break;
        default:
            ret = RES_PARERR;
            break;
    }
    return ret;
}

DWORD_X get_fattime(void)
{

    return 0;
}
