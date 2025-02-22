/*
 * @Description: flash 操作
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2024-05-10 09:27:31
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "drv.h"
/* Private define ------------------------------------------------------------*/
#define FLASH_TOTAL      0x50000 // 共320K
#define BLOCK_START_ADDR 0x2F800 // 前190K为COS预留空间
#define PAGE_SIZE        512     // ! 注意这里并不是页大小，而是可以擦除的最小单位
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 读取flash
 * @detail:
 * @param {UCHAR} *pucBuf 读取buf
 * @param {ULONG} ulSrcAddr 地址
 * @param {ULONG} ulBytelen 字节长度
 * @return {*}
 * @author: lkc
 */
VOID DRV_FlashRead(UCHAR *pucBuf, ULONG ulSrcAddr, ULONG ulBytelen)
{
    if ((NULL == pucBuf) || (0 == ulBytelen)) {
        return;
    }

    if ((ulSrcAddr + ulBytelen) > FLASH_TOTAL) {
        return;
    }

    // norflash_read(ulSrcAddr, pucBuf, ulBytelen);
    return;
}

/**
 * @description: flash写
 * @detail:
 * @param {UCHAR} *pucBuf 写buf
 * @param {UINT32} ulDstAddr 目的地址
 * @param {ULONG} ulBytelen 字节长度
 * @return {*}
 * @author: lkc
 */
VOID DRV_FlashWrite(UCHAR *pucBuf, ULONG ulDstAddr, ULONG ulBytelen)
{
    if ((NULL == pucBuf) || (0 == ulBytelen)) {
        return;
    }

    if ((ulDstAddr + ulBytelen) > FLASH_TOTAL) {
        return;
    }
    // norflash_write(ulDstAddr, pucBuf, ulBytelen);
    return;
}

/**
 * @description: flash单页擦除
 * @detail:
 * @param {UCHAR} *pucBuf 写buf
 * @param {UINT32} ulDstAddr 目的地址
 * @param {ULONG} ulBytelen 字节长度
 * @return {*}
 * @author: lkc
 */
VOID DRV_FlashErasePage(ULONG ulAddress)
{
    // norflash_sector_erase(ulAddress);
    return;
}

/**
 * @description: flash擦除多扇区
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID DRV_FLASHEraseN(ULONG ulAddr, ULONG ulPageN)
{
    unsigned int i = 0;
    for (i = 0; i < ulPageN; i++) {
        DRV_FlashErasePage(ulAddr + i * PAGE_SIZE);
    }
}

/**
 * @description: 先擦除再写
 * @detail:
 * @return {*}
 * @author: lkc
 */
ULONG DRV_FLASHWriteWithErase(UCHAR *pucSrc, ULONG ulDes, ULONG ulBytelen)
{
    USHORT unalignment_bytes1 = 0, unalignment_bytes2 = 0, totle_len = 0;
    UCHAR buf1[PAGE_SIZE], buf2[PAGE_SIZE], i;
    if (ulBytelen <= 0 || ulDes < BLOCK_START_ADDR || (ulDes + ulBytelen) >= FLASH_TOTAL) {
        return FALSE;
    }

    /*判断是否地址PAGESIZEbyte对齐*/
    if ((ulDes % PAGE_SIZE) != 0) {
        unalignment_bytes1 = ulDes % PAGE_SIZE; // X1的数据长度，即需要保护的数据
        DRV_FlashRead(buf1, ulDes - unalignment_bytes1, unalignment_bytes1); // 读取X1的数据到buf1中
    }

    /*判断写入长度PAGESIZE字节对齐，*/
    if (((ulDes + ulBytelen) % PAGE_SIZE) != 0) {
        unalignment_bytes2 = PAGE_SIZE - (ulDes + ulBytelen) % PAGE_SIZE; // X2的数据长度
        DRV_FlashRead(buf2, ulDes + ulBytelen, unalignment_bytes2);       // 读取X2的数据到buf2中
    }

    /*数据拼接*/
    totle_len = ulBytelen + unalignment_bytes1 + unalignment_bytes2;

    /*擦除从地址des-unalignment_bytes1开始的n页，n = totle_len/PAGE_SIZE*/
    DRV_FLASHEraseN(ulDes - unalignment_bytes1, totle_len / PAGE_SIZE);

    /*当只有一页*/
    if (totle_len / PAGE_SIZE == 1) {
        memcpy(buf1 + unalignment_bytes1, pucSrc, ulBytelen);
        memcpy(buf1 + unalignment_bytes1 + ulBytelen, buf2, unalignment_bytes2);
        DRV_FlashWrite(buf1, ulDes - unalignment_bytes1, PAGE_SIZE);

    } else {
        for (i = 0; i < totle_len / PAGE_SIZE; i++) {
            if (i == 0) {
                memcpy(buf1 + unalignment_bytes1, pucSrc, PAGE_SIZE - unalignment_bytes1);
            } else if (i == (totle_len / PAGE_SIZE - 1)) {
                memcpy(buf1, pucSrc + ulBytelen - (PAGE_SIZE - unalignment_bytes2), PAGE_SIZE - unalignment_bytes2);
                memcpy(buf1 + PAGE_SIZE - unalignment_bytes2, buf2, unalignment_bytes2);
            } else {
                memcpy(buf1, pucSrc + PAGE_SIZE - unalignment_bytes1 + (i - 1) * PAGE_SIZE, PAGE_SIZE);
            }
            DRV_FlashWrite(buf1, ulDes - unalignment_bytes1 + i * PAGE_SIZE, PAGE_SIZE);
        } 
    }

    return SUCCESS;
}

/**
 * @description: flash测试程序
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID DRC_FlashTest(VOID)
{
    unsigned int buff_r[128];
    unsigned int buff_w[128] = {
        0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef, 0x10111213, 0x14151617, 0x18191a1b, 0x1c1d1e1f, 0x20212223,
        0x24252627, 0x28292a2b, 0x2c2d2e2f, 0x30313233, 0x34353637, 0x38393a3b, 0x3c3d3e3f, 0x40414243, 0x44454647,
        0x48494a4b, 0x4c4d4e4f, 0x50515253, 0x54555657, 0x58595a5b, 0x5c5d5e5f, 0x60616263, 0x64656667, 0x68696a6b,
        0x6c6d6e6f, 0x70717273, 0x74757677, 0x78797a7b, 0x7c7d7e7f, 0x80818283, 0x84858687, 0x88898a8b, 0x8c8d8e8f,
        0x90919293, 0x94959697, 0x98999a9b, 0x9c9d9e9f, 0xa0a1a2a3, 0xa4a5a6a7, 0xa8a9aaab, 0xacadaeaf, 0xb0b1b2b3,
        0xb4b5b6b7, 0xb8b9babb, 0xbcbdbebf, 0xc0c1c2c3, 0xc4c5c6c7, 0xc8c9cacb, 0xcccdcecf, 0xd0d1d2d3, 0xd4d5d6d7,
        0xd8d9dadb, 0xdcdddedf, 0xe0e1e2e3, 0xe4e5e6e7, 0xe8e9eaeb, 0xecedeeef, 0x11111111, 0x22222222, 0x33333333,
        0x44444444, 0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef, 0x10111213, 0x14151617, 0x18191a1b, 0x1c1d1e1f,
        0x20212223, 0x24252627, 0x28292a2b, 0x2c2d2e2f, 0x30313233, 0x34353637, 0x38393a3b, 0x3c3d3e3f, 0x40414243,
        0x44454647, 0x48494a4b, 0x4c4d4e4f, 0x50515253, 0x54555657, 0x58595a5b, 0x5c5d5e5f, 0x60616263, 0x64656667,
        0x68696a6b, 0x6c6d6e6f, 0x70717273, 0x74757677, 0x78797a7b, 0x7c7d7e7f, 0x80818283, 0x84858687, 0x88898a8b,
        0x8c8d8e8f, 0x90919293, 0x94959697, 0x98999a9b, 0x9c9d9e9f, 0xa0a1a2a3, 0xa4a5a6a7, 0xa8a9aaab, 0xacadaeaf,
        0xb0b1b2b3, 0xb4b5b6b7, 0xb8b9babb, 0xbcbdbebf, 0xc0c1c2c3, 0xc4c5c6c7, 0xc8c9cacb, 0xcccdcecf, 0xd0d1d2d3,
        0xd4d5d6d7, 0xd8d9dadb, 0xdcdddedf, 0xe0e1e2e3, 0xe4e5e6e7, 0xe8e9eaeb, 0xecedeeef, 0x55555555, 0x66666666,
        0x77777777, 0x88888888,
    };

    memset(buff_r, 0, 512);
    DRV_FLASHWriteWithErase((unsigned char *)buff_w, 0x40000, 512);
    DRV_FlashRead((unsigned char *)buff_r, 0x40000, 512);
    comresult_byte(buff_w, buff_r, 512);

    memset(buff_r, 0, 512);
    DRV_FLASHWriteWithErase((unsigned char *)buff_w, 0x400FF, 512);
    DRV_FlashRead((unsigned char *)buff_r, 0x400FF, 512);
    comresult_byte(buff_w, buff_r, 512);
    INFO("FLASH Test Ok\n");
}
#ifdef __cplusplus
}
#endif
