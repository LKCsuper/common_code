/*
 * @Description: 
 * @Version: 2.0
 * @Author: lkc
 * @Date: 2022-11-28 19:28:49
 * @LastEditors: lkc
 * @LastEditTime: 2023-10-13 15:39:32
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* base address of the flash sectors */
#define ADDR_FLASH_SECTOR_0      ((uint32_t)0x08000000) /* Base address of Sector 0, 16 K bytes   */
#define ADDR_FLASH_SECTOR_1      ((uint32_t)0x08004000) /* Base address of Sector 1, 16 K bytes   */
#define ADDR_FLASH_SECTOR_2      ((uint32_t)0x08008000) /* Base address of Sector 2, 16 K bytes   */
#define ADDR_FLASH_SECTOR_3      ((uint32_t)0x0800C000) /* Base address of Sector 3, 16 K bytes   */
#define ADDR_FLASH_SECTOR_4      ((uint32_t)0x08010000) /* Base address of Sector 4, 64 K bytes   */
#define ADDR_FLASH_SECTOR_5      ((uint32_t)0x08020000) /* Base address of Sector 5, 128 K bytes  */
#define ADDR_FLASH_SECTOR_6      ((uint32_t)0x08040000) /* Base address of Sector 6, 128 K bytes  */
#define ADDR_FLASH_SECTOR_7      ((uint32_t)0x08060000) /* Base address of Sector 7, 128 K bytes  */
#define ADDR_FLASH_SECTOR_8      ((uint32_t)0x08080000) /* Base address of Sector 8, 128 K bytes  */
#define ADDR_FLASH_SECTOR_9      ((uint32_t)0x080A0000) /* Base address of Sector 9, 128 K bytes  */
#define ADDR_FLASH_SECTOR_10     ((uint32_t)0x080C0000) /* Base address of Sector 10, 128 K bytes */
#define ADDR_FLASH_SECTOR_11     ((uint32_t)0x080E0000) /* Base address of Sector 11, 128 K bytes */
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * Get the sector of a given address
 * 16 16 16 16 64 128 128  128 128 128 128
 * @param address flash address
 *
 * @return The sector of a given address
 */
static uint32_t stm32_get_sector(uint32_t address)
{
    uint32_t sector = 0;

    if ((address < ADDR_FLASH_SECTOR_1) && (address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_Sector_0;
    }
    else if ((address < ADDR_FLASH_SECTOR_2) && (address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_Sector_1;
    }
    else if ((address < ADDR_FLASH_SECTOR_3) && (address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_Sector_2;
    }
    else if ((address < ADDR_FLASH_SECTOR_4) && (address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_Sector_3;
    }
    else if ((address < ADDR_FLASH_SECTOR_5) && (address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_Sector_4;
    }
    else if ((address < ADDR_FLASH_SECTOR_6) && (address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_Sector_5;
    }
    else if ((address < ADDR_FLASH_SECTOR_7) && (address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_Sector_6;
    }
    else if ((address < ADDR_FLASH_SECTOR_8) && (address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_Sector_7;
    }
    else if ((address < ADDR_FLASH_SECTOR_9) && (address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_Sector_8;
    }
    else if ((address < ADDR_FLASH_SECTOR_10) && (address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_Sector_9;
    }
    else if ((address < ADDR_FLASH_SECTOR_11) && (address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_Sector_10;
    }
    else
    {
        sector = FLASH_Sector_11;
    }

    return sector;
}

/**
 * Get the sector size
 *
 * @param sector sector
 *
 * @return sector size
 */
static uint32_t stm32_get_sector_size(uint32_t sector) {
    //assert(IS_FLASH_SECTOR(sector));

    switch (sector) {
    case FLASH_Sector_0: return 16 * 1024;
    case FLASH_Sector_1: return 16 * 1024;
    case FLASH_Sector_2: return 16 * 1024;
    case FLASH_Sector_3: return 16 * 1024;
    case FLASH_Sector_4: return 64 * 1024;
    case FLASH_Sector_5: return 128 * 1024;
    case FLASH_Sector_6: return 128 * 1024;
    case FLASH_Sector_7: return 128 * 1024;
    case FLASH_Sector_8: return 128 * 1024;
    case FLASH_Sector_9: return 128 * 1024;
    case FLASH_Sector_10: return 128 * 1024;
    case FLASH_Sector_11: return 128 * 1024;
    default : return 128 * 1024;
    }
}

/**
 * @description: 擦除flash
 * @detail: 
 * @param {long} addr
 * @param {size_t} size
 * @return {*}
 * @author: lkc
 */
ULONG Bsp_Flash_Erase(long addr, size_t size)
{
    FLASH_Status flash_status;
    size_t erased_size = 0;
    uint32_t cur_erase_sector = 0;
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR| FLASH_FLAG_PGSERR);

    /* 喂狗 */
    Bsp_Misc_IWdgtReload();
    while (erased_size < size)
    {
        cur_erase_sector = stm32_get_sector(addr + erased_size);
        flash_status = FLASH_EraseSector(cur_erase_sector, VoltageRange_3);
        if (flash_status != FLASH_COMPLETE)
        {
            return -1;
        }
        erased_size += stm32_get_sector_size(cur_erase_sector);
        /* 喂狗 */
        Bsp_Misc_IWdgtReload();
    }

    FLASH_Lock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR| FLASH_FLAG_PGSERR);

    return size;
}

/**
 * @description: 编程
 * @detail: 
 * @param {IN ULONG} ulAddr
 * @param {IN UCHAR*} pucData
 * @param {IN ULONG} pucSize
 * @return {*}
 * @author: lkc
 */
UCHAR Bsp_Flash_Program(IN ULONG ulAddr, IN UCHAR* pucData, IN ULONG pucSize)
{
    LONG lRet = YMODEM_SUCCESS;
    ULONG i = 0;
    ULONG tmpAddr = ulAddr;
    UCHAR *pucTmp = pucData;
    UCHAR read_data;

    // !NOTICE
    //FLASH_Unlock();
    FLASH_ClearFlag(
            FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR
                    | FLASH_FLAG_PGSERR);
    for (i = 0; i < pucSize; i++, pucTmp++, tmpAddr++)
    {
        /* write data */
        FLASH_ProgramByte(tmpAddr, *pucTmp);
        read_data = *(uint8_t *) tmpAddr;
        /* check data */
        if (read_data != *pucTmp)
        {
            return -1;
        }
    }
    //FLASH_Lock();

    return lRet;
}

/**
 * @description: 
 * @detail: 
 * @param {IN ULONG} ulAddr
 * @param {IN ULONG} pucData
 * @return {*}
 * @author: lkc
 */
UCHAR Bsp_Flash_Program32(IN ULONG ulAddr, IN ULONG pucData)
{
    // !NOTICE
    //FLASH_Unlock();
    FLASH_ProgramWord(ulAddr, pucData);
    //FLASH_Lock();
    return YMODEM_SUCCESS;
}

/**
 * @brief  关闭flash写保护
 * @param  None
 * @retval 1: Write Protection successfully disabled
 *         2: Error: Flash write unprotection failed
 */
uint32_t FLASH_DisableWriteProtection(void)
{
    /* Unlock the Option Bytes */
    FLASH_OB_Unlock();

    /* 这里把所有区域都取消保护 */
    FLASH_OB_WRPConfig(OB_WRP_Sector_All, DISABLE);

    /* Start the Option Bytes programming process. */
    if (FLASH_OB_Launch() != FLASH_COMPLETE)
    {
        FLASH_OB_Lock();
        /* Error: Flash write unprotection failed */
        return ERROR;
    }

    FLASH_OB_Lock();
    /* Write Protection successfully disabled */
    return SUCCESS;
}

/**
 * @brief  关闭flash写保护
 * @param  None
 * @retval 1: Write Protection successfully disabled
 *         2: Error: Flash write unprotection failed
 */
uint32_t FLASH_EnableWriteProtection(void)
{
    /* Unlock the Option Bytes */
    FLASH_OB_Unlock();

    /* 所有区域实行保护 */
    FLASH_OB_WRPConfig(OB_WRP_Sector_All, ENABLE);

    /* 这里还是对config区进行解锁，以便??来写配置参数 */
    FLASH_OB_WRPConfig(FLASH_Sector_3, ENABLE);
    FLASH_OB_WRPConfig(FLASH_Sector_4, ENABLE);

    /* Start the Option Bytes programming process. */
    if (FLASH_OB_Launch() != FLASH_COMPLETE)
    {
        FLASH_OB_Lock();
        /* Error: Flash write unprotection failed */
        return ERROR;
    }

    FLASH_OB_Lock();
    /* Write Protection successfully disabled */
    return SUCCESS;
}

/**
 * @brief  flash初始化
 * @param  None
 * @retval None
 */
void Bsp_FLASH_Init(void)
{
    /* 需要解除写保护 */
    FLASH_Unlock();

    /* 清除所有的flash标志 */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

    return;
}
#ifdef __cplusplus
};
#endif
