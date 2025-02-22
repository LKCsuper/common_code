#ifndef _ZAYK_DEBUG_H_
#define _ZAYK_DEBUG_H_

#include <stdarg.h>
#include <stdio.h>

#define LOG_LOCK_KEY "7289" // 0x1c0c
#define DEBUG_LEVEL  101

typedef enum {
    ZAYK_LOG_LEVEL_ERROR,
    ZAYK_LOG_LEVEL_WRAN,
    ZAYK_LOG_LEVEL_INFO,
    ZAYK_LOG_LEVEL_DEBUG
} ZAYK_LOG_LEVEL_E;

void __Zayk_Debug(int nLevel, char *sSourceFile, int nCodeLine, char *func, unsigned char *sMsgBin, int nMsgBinLen,
                  char *sMessageFormat, ...);

#define ZAYK_DEBUG(...) __Zayk_Debug(ZAYK_LOG_LEVEL_DEBUG, __FILE__, __LINE__, (char *)__func__, NULL, 0, ##__VA_ARGS__)
#define ZAYK_ERROR(...) __Zayk_Debug(ZAYK_LOG_LEVEL_ERROR, __FILE__, __LINE__, (char *)__func__, NULL, 0, ##__VA_ARGS__)
#define ZAYK_WARN(...)  __Zayk_Debug(ZAYK_LOG_LEVEL_WRAN, __FILE__, __LINE, (char *)__func__, NULL, 0, ##__VA_ARGS__)
#define ZAYK_INFO(...)  __Zayk_Debug(ZAYK_LOG_LEVEL_INFO, __FILE__, __LINE, (char *)__func__, NULL, 0, ##__VA_ARGS__)

char *Zayk_GetErrCodeInfo(int errCode);
#endif
