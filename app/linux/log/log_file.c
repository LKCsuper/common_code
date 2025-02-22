/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>

#if defined LOG_FILE_WIN
#include <io.h>
#include <stdlib.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "Zayk_KeyU_log.h"

char gLOG_LogDir[1024];

/**
 * @description: 检查文件是否存在
 * @detail: 
 * @param {char} *pFileName
 * @return {*}
 * @author: lkc
 */
int LOG_CheckFile(char *pFileName)
{
    if (strlen(pFileName) >= 128) {
        return -1;
    }
#ifdef WIN32
    if ((_access(pFileName, 4)) == -1) {
        return 0;
    }
#else
    if ((access(pFileName, F_OK)) != 0) {
        return 0;
    }
#endif

    return 1;
}

/**
 * @description: 打印hex
 * @detail: 
 * @param {char} *sItemName
 * @param {unsigned char} *pSourceData
 * @param {unsigned int} nDataLength
 * @param {unsigned int} nRowCount
 * @return {*}
 * @author: lkc
 */
int LOG_PrintHex(char *sItemName, unsigned char *pSourceData, unsigned int nDataLength, unsigned int nRowCount)
{
    int i, j;
    int rowCount;

    if ((pSourceData == NULL) || (nDataLength == 0)) {
        return -1;
    }

    if (nRowCount <= 0) {
        rowCount = nRowCount;
    } else {
        rowCount = 16;
    }

    if (sItemName != NULL) {
        printf("%s[%d]:\n", sItemName, nDataLength);
    }

    for (i = 0; i < (int)(nDataLength / rowCount); i++) {
        printf("%08x  ", i * rowCount);
        for (j = 0; j < (int)rowCount; j++) {
            printf("%02x ", *(pSourceData + i * rowCount + j));
        }
        printf("\n");
    }
    if (!(nDataLength % rowCount)) {
        return 0;
    }

    printf("%08x  ", (nDataLength / rowCount) * rowCount);
    for (j = 0; j < (int)(nDataLength % rowCount); j++) {
        printf("%02x ", *(pSourceData + (nDataLength / rowCount) * rowCount + j));
    }
    printf("\n");

    return 0;
}

/**
 * @description: 获取文件路径
 * @detail: 
 * @param {char} *path
 * @return {*}
 * @author: lkc
 */
void LOG_GetDir(char *path)
{
    char *pSystemPan;
    char *pSystemPath;

#if defined LOG_FILE_WIN
    pSystemPan = getenv("SYSTEMROOT");
    if (pSystemPan == NULL) {
        return;
    }

    pSystemPath = getenv("HOMEPATH");
    if (pSystemPath == NULL) {
        return;
    }
    sprintf(path, "%c:\\%s", pSystemPan[0], pSystemPath);
    return;
#else
    // pSystemPath =getenv("HOME");
    // starcpy(path, getenv("HOME"));
    sprintf(path, "%s", "/tmp");
#endif

    return;
}

/**
 * @description: 文件夹是否存在
 * @detail: 
 * @param {char} *pFileName
 * @return {*}
 * @author: lkc
 */
int LOG_CheckDir(char *pFileName)
{

#if defined LOG_FILE_WIN
    int rv = 0;
#else
    struct stat st;
#endif

    if (pFileName == NULL) {
        return -1;
    }

#if defined LOG_FILE_WIN
    rv = GetFileAttributes(pFileName);
    if (rv == INVALID_FILE_ATTRIBUTES) {
        return -1;
    }

    if (rv & FILE_ATTRIBUTE_DIRECTORY) {
        return 0;
    }
#else
    if (stat(pFileName, &st) < 0) {
        return -1;
    }

    if (S_ISDIR(st.st_mode)) {
        return 0;
    }
#endif

    return -1;
}
#if !defined LOG_FILE_WIN
#include <sys/syscall.h>
pid_t gettid(void)
{
    return syscall(SYS_gettid);
}
#endif

void log_message(int nLevel, char *sSourceFile, int nCodeLine, char *sMessageFormat, ...)
{
    FILE *fp;
    int rv, i;
    struct tm *newtime;
    time_t aclock;
    char sAvailableLogFile[256] = {0}, sTmpFileName[256] = {0}, tmpLogPath[256] = {0};
    va_list args;
    char logPath[256] = {0};

#ifdef __ANDROID__
    fp = fopen("skf.log", "w");
    if (fp == NULL) {
        return;
    }
#else
    // 获取日志目录位置
    LOG_GetDir(logPath);

#if defined LOG_FILE_WIN
    sprintf(tmpLogPath, "%s\\zayk_log", logPath);
#else
    sprintf(tmpLogPath, "%s/zayk_log", logPath);
#endif
    // 判断日志位置是否存在，不存在则返回
    rv = LOG_CheckDir(tmpLogPath);
    if (rv != 0) {
        return;
    }
    time(&aclock);
    newtime = localtime(&aclock);

    // 检测用户指定的日志目录是否存在
    sprintf(sAvailableLogFile, "%s/zayk_keyu_%4d%02d%02d.log", tmpLogPath, newtime->tm_year + 1900, newtime->tm_mon + 1,
            newtime->tm_mday);

    fp = fopen(sAvailableLogFile, "a+");
    if (NULL == fp) {
        return;
    }
#endif

    time(&aclock);
    newtime = localtime(&aclock);
#ifdef LOG_FILE_WIN
    SYSTEMTIME t1;
    GetSystemTime(&t1);
#else
    struct timeval stv;
    gettimeofday(&stv, NULL);
#endif
    char time_str[32];
    sprintf(time_str, "%02d:%02d:%02d", newtime->tm_hour, newtime->tm_min, newtime->tm_sec);

    // LOG_SEM_P(g_logSemHandle, 0, 60 * 1000);
    switch (nLevel) {
        case 1: // Error
#ifdef LOG_FILE_WIN
            fprintf(fp, "ERRO [%s.%03d] [%lu %lu] [%s:%d] ", time_str, t1.wMilliseconds, GetCurrentProcessId(),
                    GetCurrentThreadId(), sSourceFile, nCodeLine);
#else
            fprintf(fp, "ERRO [%s.%03d] [%d %d] [%s:%d] ", time_str, stv.tv_usec / 1000, getpid(), gettid(),
                    sSourceFile, nCodeLine);
#endif
            va_start(args, sMessageFormat);
            vfprintf(fp, sMessageFormat, args);
            va_end(args);

            fprintf(fp, "\n");
            break;
        case 2: // Warning
#ifdef LOG_FILE_WIN
            fprintf(fp, "INFO [%s.%03d] [%lu %lu] [%s:%d] ", time_str, t1.wMilliseconds, GetCurrentProcessId(),
                    GetCurrentThreadId(), sSourceFile, nCodeLine);
#else
            fprintf(fp, "INFO [%s.%03d] [%d %d] [%s:%d] ", time_str, stv.tv_usec / 1000, getpid(), gettid(),
                    sSourceFile, nCodeLine);
#endif
            va_start(args, sMessageFormat);
            vfprintf(fp, sMessageFormat, args);
            va_end(args);

            fprintf(fp, "\n");
            break;

        default:
            break;
    }
    // LOG_SEM_V(g_logSemHandle, 0);

    if (ftell(fp) > 0xa00000) // over 10M  bytes
    {
        fclose(fp);
        for (i = 1;; i++) {
            sprintf(sTmpFileName, "%s.bak%d", sAvailableLogFile, i);
            if (!LOG_CheckFile(sTmpFileName)) {
#ifdef LOG_FILE_WIN
                sprintf(sTmpFileName, "move %s %s.bak%d", sAvailableLogFile, sAvailableLogFile, i);
#else
                sprintf(sTmpFileName, "mv %s %s.bak%d", sAvailableLogFile, sAvailableLogFile, i);
#endif
                system(sTmpFileName);
                break;
            } else {
                continue;
            }
        }
    } else {
        fclose(fp);
    }

    return;
}

void log_hexdump(char *sSourceFile, int nCodeLine, unsigned char *array, int datalen, char *fmt, ...)
{
    FILE *fp;
    int rv, i;
    struct tm *newtime;
    time_t aclock;
    char sAvailableLogFile[256] = {0}, sTmpFileName[256] = {0}, tmpLogPath[256] = {0};
    va_list args;
    char logPath[256] = {0};

#ifdef __ANDROID__
    fp = fopen("skf.log", "w");
    if (fp == NULL) {
        return;
    }
#else
    // 获取日志目录位置
    LOG_GetDir(logPath);

#if defined LOG_FILE_WIN
    sprintf(tmpLogPath, "%s\\zayk_log", logPath);
#else
    sprintf(tmpLogPath, "%s/zayk_log", logPath);
#endif
    // 判断日志位置是否存在，不存在则返回
    rv = LOG_CheckDir(tmpLogPath);
    if (rv != 0) {
        return;
    }

    time(&aclock);
    newtime = localtime(&aclock);

    // 检测用户指定的日志目录是否存在
    sprintf(sAvailableLogFile, "%s/zayk_keyu_%4d%02d%02d.log", tmpLogPath, newtime->tm_year + 1900, newtime->tm_mon + 1,
            newtime->tm_mday);

    fp = fopen(sAvailableLogFile, "a+");
    if (NULL == fp) {
        return;
    }
#endif

    // LOG_SEM_P(g_logSemHandle, 0, 60 * 1000);
    time(&aclock);
    newtime = localtime(&aclock);
#ifdef LOG_FILE_WIN
    SYSTEMTIME t1;
    GetSystemTime(&t1);
#else
    struct timeval stv;
    gettimeofday(&stv, NULL);
#endif
    char time_str[32];
    sprintf(time_str, "%02d:%02d:%02d", newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
#ifdef LOG_FILE_WIN
    fprintf(fp, "VERB [%s.%03d] [%lu %lu] [%s:%d] ", time_str, t1.wMilliseconds, GetCurrentProcessId(),
            GetCurrentThreadId(), sSourceFile, nCodeLine);
#else
    fprintf(fp, "VERB [%s.%03d] [%d %d] [%s:%d] ", time_str, stv.tv_usec / 1000, getpid(), gettid(), sSourceFile,
            nCodeLine);
#endif

    int lineLen;
    char tmpBuf[20];
    char *buf;
    int len = datalen;
    unsigned char *data = array;
    int offset = 0;

    if (fmt != NULL) {
        va_start(args, fmt);
        vfprintf(fp, fmt, args);
        va_end(args);
    }

    while (1) {
        buf = tmpBuf;

        lineLen = (datalen > 16 ? 16 : datalen);
        fprintf(fp, "%06X:", offset);

        for (i = 0; i < 16; i++) {
            if (i < lineLen) {
                fprintf(fp, " %02X", data[i]);
                if (data[i] < 0x20 || data[i] > 0x7e) {
                    *buf++ = '.';
                } else {
                    *buf++ = data[i];
                }
            } else {
                fprintf(fp, "   ");
                *buf++ = ' ';
            }

            if (i == 7) {
                fprintf(fp, " ");
                *buf++ = ' ';
            }
        }

        *buf++ = '\0';
        fprintf(fp, "  |%s|\n", tmpBuf);

        offset += lineLen;
        datalen -= lineLen;
        data += lineLen;

        if (lineLen < 16 || datalen == 0) {
            fprintf(fp, "DEC:%05d HEX:%04X\n\n", len, len);
            break;
        }
    }

    if (ftell(fp) > 0xa00000) // over 10M  bytes
    {
        fclose(fp);
        for (i = 1;; i++) {
            sprintf(sTmpFileName, "%s.bak%d", sAvailableLogFile, i);
            if (!LOG_CheckFile(sTmpFileName)) {
#ifdef LOG_FILE_WIN
                sprintf(sTmpFileName, "move %s %s.bak%d", sAvailableLogFile, sAvailableLogFile, i);
#else
                sprintf(sTmpFileName, "mv %s %s.bak%d", sAvailableLogFile, sAvailableLogFile, i);
#endif
                system(sTmpFileName);
                break;
            } else {
                continue;
            }
        }
    } else {
        fclose(fp);
    }
    // LOG_SEM_V(g_logSemHandle, 0);

    return;
}
