/*
 * @Description: 打印颜色
 * @detail:
 * @Version: 1.0
 * @Date: 2023-06-30 09:07:15
 * @LastEditTime: 2024-09-03 18:23:24
 * @LastEditors: Please set LastEditors
 */
#ifndef _PRINTF_COLOR_H
#define _PRINTF_COLOR_H

#include "stdio.h"
#define PRINTF_COLOR 1 // 颜色日志输出模式（显示终端需要支持）
#if PRINTF_COLOR
/* 字体颜色 */
#define BLACK  30
#define RED    31
#define GREEN  32
#define BROWN  33
#define YELLOW 33
#define BLUE   34
#define PURPLE 35
#define CYAN   36
#define GRAY   37
#define WHITE  37

/* 字体格式 */
#define BOLD      1
#define UNDERLINE 4
#define BLINK     5
#define REVERSE   7
#define HIDE      8
#define CLEAR     2
#define NONE      0

/* 背景颜色 */
#define BLACK_BACKGROUND  40
#define RED_BACKGROUND    41
#define GREEN_BACKGROUND  42
#define YELLOW_BACKGROUND 43
#define BLUE_BACKGROUND   44
#define PURPLE_BACKGROUND 45
#define CYAN_BACKGROUND   46
#define WHITE_BACKGROUND  47

#define HIGH_LEVEL 1

#ifndef M_BOOT
#define PRINTF(fmt, ...) rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define PRINTF(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif
/* dis_mode:鏄剧ず妯″紡 fwd_clor:鍓嶆櫙锟?? bak_clor:鑳屾櫙锟?? */
#define COLR_SET(dis_mode, fwd_clor, bak_clor)                                                                         \
    {                                                                                                                  \
        PRINTF("\033[%d;%d;%dm", dis_mode, fwd_clor, bak_clor);                                                        \
    }
#define DEVICE_LOG(dis_mode, fwd_clor, bak_clor, dev_name, ...)                                                        \
    {                                                                                                                  \
        /*设置颜色*/ COLR_SET(dis_mode, fwd_clor, bak_clor);                                                           \
        /*设置参数*/ PRINTF(dev_name, ##__VA_ARGS__);                                                                  \
        /*设置结束*/ COLR_SET(0, 0, 0);                                                                                \
    }
#define DEVICE_MULTI_LOG(dis_mode0, fwd_clor0, bak_clor0, dis_mode1, fwd_clor1, bak_clor1, dev_name, ...)              \
    {                                                                                                                  \
        /*设置device name的颜色*/ COLR_SET(x0, y0, z0);                                                                \
        /*设置device name*/ PRINTF("%s", #dev_name);                                                                   \
        /*设置参数的颜色*/ COLR_SET(x1, y1, z1);                                                                       \
        /*设置参数*/ PRINTF(__VA_ARGS__);                                                                              \
        /*设置结束*/ COLR_SET(0, 0, 0);                                                                                \
    }

/* 不同颜色的打印 */
#define PRINTF_YELLOW(fmt, ...) DEVICE_LOG(BOLD, YELLOW, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)
#define PRINTF_RED(fmt, ...)    DEVICE_LOG(BOLD, RED, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)
#define PRINTF_BLACK(fmt, ...)  DEVICE_LOG(BOLD, BLACK, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)
#define PRINTF_GREEN(fmt, ...)  DEVICE_LOG(BOLD, GREEN, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)
#define PRINTF_BROWN(fmt, ...)  DEVICE_LOG(NONE, BROWN, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)
#define PRINTF_BLUE(fmt, ...)   DEVICE_LOG(BOLD, BLUE, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)
#define PRINTF_PURPLE(fmt, ...) DEVICE_LOG(BOLD, PURPLE, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)
#define PRINTF_CYAN(fmt, ...)   DEVICE_LOG(BOLD, CYAN, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)
#define PRINTF_GRAY(fmt, ...)   DEVICE_LOG(NONE, GRAY, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)
#define PRINTF_WHITE(fmt, ...)  DEVICE_LOG(BOLD, WHITE, BLACK_BACKGROUND, fmt, ##__VA_ARGS__)


/* 通知 */
#define INFO(fmt, ...)               PRINTF_BLUE("INFO:" fmt, ##__VA_ARGS__)
/* 警告 */
#define WARNING(fmt, ...)            PRINTF_YELLOW("WARNING:[%s %s line%d]" fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
/* 错误 */
#define ERR(fmt, ...)                PRINTF_RED("ERROR:[%s %s line%d]" fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
/* 打印日期 */
#define PRINTF_Date()                rt_kprintf("Firmware compile time:%s %s\r\n", __DATE__, __TIME__)
#define NOTICE(fmt, ...)             PRINTF_GREEN("NOTICE:" fmt, ##__VA_ARGS__)
#define DEBUG(fmt, ...)              printf(fmt, ##__VA_ARGS__)
#define PRINT_HEX(title, in_data, a) print_hex(title, 0, in_data, a)
#define PRINT_BUF(title, in_data, a) print_buff(title, in_data, a)

/* 通知 */
#define INFO_LEVEL(level, fmt, ...)                                                                                    \
    if (level >= HIGH_LEVEL) {                                                                                         \
        PRINTF_BLUE("INFO:" fmt, ##__VA_ARGS__);                                                                       \
    }

/* 警告 */
#define WARNING_LEVEL(level, fmt, ...)                                                                                 \
    if (level >= HIGH_LEVEL) {                                                                                         \
        PRINTF_YELLOW("WARNING:[%s %s line%d]" fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);                  \
    }

/* 错误 */
#define ERR_LEVEL(level, fmt, ...)                                                                                     \
    if (level >= HIGH_LEVEL) {                                                                                         \
        PRINTF_RED("ERROR:[%s %s line%d]" fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);                       \
    }

/* 打印日期 */
#define PRINTF_Date_LEVEL(level)                                                                                       \
    if (level >= HIGH_LEVEL) {                                                                                         \
        printf("Firmware compile time:%s %s\r\n", __DATE__, __TIME__);                                                 \
    }

#define NOTICE_LEVEL(level, fmt, ...)                                                                                  \
    if (level >= HIGH_LEVEL) {                                                                                         \
        PRINTF_GREEN("NOTICE:" fmt, ##__VA_ARGS__);                                                                    \
    }


#define DEBUG_LEVEL(level, fmt, ...)                                                                                   \
    if (level >= HIGH_LEVEL) {                                                                                         \
        printf(fmt, ##__VA_ARGS__);                                                                                    \
    }


#define PRINT_HEX_LEVEL(level, title, in_data, a)                                                                      \
    if (level >= HIGH_LEVEL) {                                                                                         \
        print_hex(title, 0, in_data, a);                                                                               \
    }

#define PRINT_BUF_LEVEL(level, title, in_data, a)                                                                      \
    if (level >= HIGH_LEVEL) {                                                                                         \
        print_buff(title, in_data, a);                                                                                 \
    }
#endif
#endif
