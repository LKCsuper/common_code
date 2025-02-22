/*
 * @Description:
 * @Version: 2.0
 * @Author: lkc
 * @Date: 2022-11-19 09:57:21
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include "common.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief 大小端切换函数
 * Common_ToEdian
 * @param result 数据本身
 * @param len 数据总长,字节数
 * @param size 每个数据元素的大小，例如short，该值为2
 */
VOID Common_ToEdian(UCHAR *result, ULONG len, UCHAR size)
{
    ULONG i = 0;
    UCHAR temp, j;
    for (i = 0; i < len / size; i++) {
        for (j = 0; j < size / 2; j++) {
            temp = result[i * size + j];
            result[i * size + j] = result[i * size + size - 1 - j];
            result[i * size + size - 1 - j] = temp;
        }
    }
}

/**
 * @description: 交换值
 * @detail:
 * @param {VOID} *dest
 * @param {VOID} *src
 * @param {ULONG} len
 * @return {*}
 * @author: lkc
 */
VOID Common_Swap(VOID *dest, VOID *src, ULONG len)
{
    memcpy(dest, src, len);
    Common_ToEdian((UCHAR *)dest, len, len);

    return;
}


/**
 * @description: 拷贝小端转换大端
 * @detail:
 * @param {VOID} *dest
 * @param {VOID} *src
 * @param {ULONG} len
 * @return {*}
 * @author: lkc
 */
VOID Common_MemEndian(VOID *dest, VOID *src, ULONG len)
{
    memcpy(dest, src, len);
    Common_ToEdian((UCHAR *)dest, len, len);

    return;
}

/**
 * @description: 判断大小端
 * @detail:
 * @return {*}
 * @author: lkc
 */
ULONG Common_JudgeSystem(void)
{
    LONG a = 1;
    UCHAR *p = (UCHAR *)&a;

    // 如果是小端则返回 1，如果是大端则返回 0
    return *p;
}

/**
 * str to hex array
 * "1122334455667788"
 * if big endian & group_nbytes is 4
 *     out: {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}
 * if little endian & grouP-nbytes is 4
 *     out: {0x44, 0x33, 0x22, 0x11, 0x88, 0x77, 0x66, 0x55}
 *
 * example: str_to_hex(pcaIn, src, srclen, 4, BIG_ENDI);
 */
int COMMON_StrToHex(const char *str, unsigned char *buf, unsigned int hex_nbytes, unsigned int group_nbytes, unsigned int endian)
{
    unsigned char t;
    unsigned int i, j, str_nbytes;
    unsigned char tmp[3];
    tmp[2] = '\0';

    assert(str != NULL);

    str_nbytes = hex_nbytes * 2;

    for (i = 0; i < str_nbytes; i += 2 * group_nbytes) {
        for (j = 0; j < group_nbytes; j++) {
            memcpy(tmp, str + i + 2 * j, 2);
            t = 0;
            t = strtol((char *)tmp, NULL, 16);
            if (endian == BIG_ENDI) {
                buf[i / 2 + j] = t;
            } else if (endian == LITTLE_ENDI) {
                buf[i / 2 + group_nbytes - j - 1] = t;
            }
        }
    }

    return 0;
}

int COMMON_HexToStr(char inArray[], unsigned int maxInSize, char outHexString[], unsigned int maxOutSize)
{
    int len = 0;
    unsigned int i = 0, j = 0;
    unsigned char c;
    
    // Check if the output buffer is large enough to hold the hexadecimal string
    if (maxOutSize < maxInSize * 3)
    {
        return -1; // Return -1 if the output buffer is too small
    }
    
    // Convert each character into its hexadecimal representation
    for (i = 0; i < maxInSize; i++)
    {
        // Convert the upper 4 bits of the character into hexadecimal
        c = (inArray[i] >> 4) & 0x0F;
        outHexString[j++] = (c < 10) ? ('0' + c) : ('A' + c - 10);
        
        // Convert the lower 4 bits of the character into hexadecimal
        c = inArray[i] & 0x0F;
        outHexString[j++] = (c < 10) ? ('0' + c) : ('A' + c - 10);
        
        // Add a space between each pair of hexadecimal characters
        outHexString[j++] = ' ';
        
        // Increment the length counter
        len++;
    }
    
    // Null-terminate the output string
    outHexString[j - 1] = 0;
    
    return len; // Return the length of the output hexadecimal string
}

/**
 * @description: 比较数组函数
 * @detail:
 * @return {*}
 * @author: lkc
 */
ULONG comresult_byte(UCHAR *src, volatile UCHAR *dst, ULONG len)
{
    ULONG i;
    for (i = 0; i < len; i++) {
        if (src[i] != dst[i]) {
            PRINT_HEX("src [%d]\n", src, len);
            PRINT_HEX("cmp [%d]\n", dst, len);
            while (1)
                ;
        }
    }
    return 0;
}

// 通用读写接口
/**
 * @description:
 * @detail:
 * @param {UCHAR} isRdWr 读或者写
 * @param {UCHAR} *pucDatabuf 读写buf
 * @param {ULONG} ulLen 读写长度
 * @param {ULONG} ulMaxlen 一次性最大读写长度
 * @return {*}
 * @author: lkc
 */
LONG Common_ReadWrite(UCHAR isRdWr, UCHAR *pucDatabuf, ULONG ulLen, ULONG ulMaxlen)
{
    ULONG totalLen = ulLen;

    while (totalLen >= ulMaxlen) { // 如果是大于一次性buf
        // read write
        if (0 == isRdWr) {
            // read
        } else {
            // write
        }

        totalLen -= ulMaxlen;
    }

    if (0 != totalLen) { // 剩余buf
        // read write
        if (0 == isRdWr) {
            // read
        } else {
            // write
        }
    }

    return SUCCESS;
}

UCHAR COMMON_Bcd2Dec(UCHAR BCD)
{
    return (((BCD & 0xF0) >> 4) * 10) + (BCD & 0xF);
}

UCHAR COMMON_Dec2Bcd(UCHAR DEC)
{
    return ((DEC / 10) << 4) + (DEC % 10);
}
#ifdef __cplusplus
}
#endif
