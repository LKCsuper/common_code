#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FMC_PAGE_SIZE        (0x400U)
#define FMC_WRITE_START_ADDR (0x08000000U)
#define FMC_WRITE_SIZE       (0x80000U)
#define FMC_WRITE_END_ADDR   (FMC_WRITE_START_ADDR + FMC_WRITE_SIZE)
#define FMC_CLEAR()                                                                                                    \
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR | FMC_FLAG_BANK0_WPERR | FMC_FLAG_BANK0_END | FMC_FLAG_BANK1_PGERR |           \
                   FMC_FLAG_BANK1_WPERR | FMC_FLAG_BANK1_END)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 擦除页
 * @detail:
 * @param {ULONG} ulPageSize
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Flash_Erase(ULONG ulPageAddr)
{
    /* 判断是不是页对齐 */
    if (0 != ulPageAddr & FMC_PAGE_SIZE) {
        ERR("Erase Addr Set Err\n");
    }

    /* unlock the flash program/erase controller */
    fmc_unlock();
    /* clear all pending flags */
    FMC_CLEAR();
    /* erase the flash pages */
    fmc_page_erase(ulPageAddr);
    FMC_CLEAR();
    /* lock the main FMC after the erase operation */
    fmc_lock();

    return;
}

/**
 * @description: 写buf
 * @detail:
 * @param {ULONG} *ulBuf
 * @param {ULONG} ulBufLen
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Flash_WriteBuf(ULONG *ulBuf, ULONG ulBufLen, ULONG ulAdr)
{
    /* unlock the flash program/erase controller */
    fmc_unlock();

    uint32_t address = ulAdr;
    /* program flash */
    while (address <= FMC_WRITE_END_ADDR) {
        if (ulBufLen <= 0) {
            break;
        }
        fmc_word_program(address, *ulBuf);

        /* 写完然后读对比 */
        if (REG32(address) != *ulBuf) {
            ERR("Write Err Len[%x] Addr[%x] [%x] [%x]\n", ulBufLen, ulAdr, *ulBuf, REG32(address));
        }

        address += 4U;
        ulBuf++;
        FMC_CLEAR();
        ulBufLen -= 4;
    }
    /* lock the main FMC after the program operation */
    fmc_lock();

    return;
}

/**
 * @description: 读32
 * @detail:
 * @param {ULONG} ulAddr
 * @return {*}
 * @author: lkc
 */
ULONG Bsp_Flash_Read32(ULONG ulAddr)
{
    return REG32(ulAddr);
}

/**
 * @description: flash 初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Bsp_Flash_Init(VOID)
{
    fmc_unlock();
    FMC_CLEAR();
    fmc_lock();

    return;
}
#ifdef __cplusplus
}
#endif
