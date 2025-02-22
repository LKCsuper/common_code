/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#include "common.h"
#include "printf_color.h"
#include "app_version.h"
#include "build_info.h"
#define PRINT_HEX_PRINTF(...) printf(__VA_ARGS__)

/**
 * @description: 打印hex数组
 * @detail: 
// 0000: 7E 25 02 00 00 7D 02 02  00 00 2B 48 54 00 47 56  |~%...}.. ..+HT.GV|
// 0010: 35 55 5C 00 00 00 00 00  00 00 00 00 00 00 00 00  |5U\..... ........|
// 0020: 00 00 00 00 00 00 00 00  00 20 01 14 16 31 00 E1  |........ . ...1..|
// 0030: 0D 02 00 0F 0A 00 F0 58  53 15 0C DA BD 05 61 7D  |.......X S.....a}|
// 0040: 02 E8 5A 7F                                       |..Z.             |
// DEC:00068 HEX:0044
 * @param {char} *title
 * @param {int} offset
 * @param {void} *in_data
 * @param {int} dataLen
 * @return {*}
 * @author: lkc
 */
void print_hex(char *title, int offset, void *in_data, int dataLen, ...)
{
    int i = 0;
    int lineLen;
    char tmp_buff[20];
    char *buf;
    int len = dataLen;
    char title_buff[64];
    unsigned char *data = in_data;

    if (title != NULL) {
        va_list arp;

        va_start(arp, dataLen);
        vsnprintf(title_buff, sizeof(title_buff), title, arp);
        va_end(arp);
        PRINT_HEX_PRINTF("%s", title_buff);
    }

    while (1) {
        buf = tmp_buff;

        lineLen = (dataLen > 16 ? 16 : dataLen);
        PRINT_HEX_PRINTF("%06X:", offset);

        for (i = 0; i < 16; i++) {
            if (i < lineLen) {
                PRINT_HEX_PRINTF(" %02X", data[i]);
                if (data[i] < 0x20 || data[i] > 0x7e) {
                    *buf++ = '.';
                } else {
                    *buf++ = data[i];
                }
            } else {
                PRINT_HEX_PRINTF("   ");
                *buf++ = ' ';
            }

            if (i == 7) {
                PRINT_HEX_PRINTF(" ");
                *buf++ = ' ';
            }
        }

        *buf++ = '\0';
        PRINT_HEX_PRINTF("  |%s|\r\n", tmp_buff);

        offset += lineLen;
        dataLen -= lineLen;
        data += lineLen;

        if (lineLen < 16 || dataLen == 0) {
            PRINT_HEX_PRINTF("DEC:%05d HEX:%04X\r\n\r\n", len, len);
            return;
        }
    }
}

/**
 * @description: 打印buf数组
 * @detail: 7EC00100007D02800100054854004758
 * @param {char} *title
 * @param {void} *buf
 * @param {int} dataLen
 * @return {*}
 * @author: lkc
 */
void print_buff(char *title, void *buf, int dataLen, ...)
{
    int i;
    char title_buff[64];
    unsigned char *data = buf;

    if (title != NULL) {
        va_list arp;

        va_start(arp, dataLen);
        vsnprintf(title_buff, sizeof(title_buff), title, arp);
        va_end(arp);

        PRINT_HEX_PRINTF("%s", title_buff);
    }

    for (i = 0; i < dataLen; i++) {
        PRINT_HEX_PRINTF("%02X", data[i]);
    }

    PRINT_HEX_PRINTF("\r\n\r\n");
}

/**
 * @description: 打印版本
 * @detail: 
 * @return {*}
 * @author: lkc
 */
VOID COMMON_Show(VOID)
{
    PRINTF_GREEN("/**************************************************/\n");
    PRINTF_GREEN("Branch :  %s  , Commit : %s \r\n", GIT_BRANCH, GIT_COMMIT_HASH);
    PRINTF_Date();
    PRINTF_GREEN("Version(H) : " HARDWAVE_VERSION " , Version(S) : " SOFTWAVE_VERSION);
    PRINTF_GREEN("\r\n");
    PRINTF_GREEN("/**************************************************/\n");
    return;
}
