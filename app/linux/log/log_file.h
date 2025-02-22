#ifndef _ZAYK_LOG_H_
#define _ZAYK_LOG_H_

#include <stdarg.h>
#include <stdio.h>

#if defined ZAYK_KEYU_WIN
    #define DEFAULT_LOG_FILE "c:/zayk_log"
#else
    #define DEFAULT_LOG_FILE "/tmp/zayk_log"
#endif

#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__)

void zayk_log_message(int nLevel, char *sSourceFile, int nCodeLine, char *sMessageFormat, ...);

#define LOGE(fmt, ...)                                                                                                 \
    do {                                                                                                               \
        zayk_log_message(1, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__);                                               \
    } while (0);

#define LOGI(fmt, ...)                                                                                                 \
    do {                                                                                                               \
        zayk_log_message(2, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__);                                               \
    } while (0);

#define LOG_HEXDUMP(fmt, offset, xarray, xlen, ...)                                                                    \
    do {                                                                                                               \
        zayk_log_hexdump(__FILENAME__, __LINE__, xarray, xlen, fmt, ##__VA_ARGS__);                                    \
    } while (0)

#endif
