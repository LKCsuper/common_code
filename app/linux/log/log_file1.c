/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */

#include "log_file1.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include <sys/syscall.h>
#include <unistd.h>

// PTHREAD_MUTEX_INITIALIZER 用于初始化静态互斥锁
pthread_mutex_t g_mutexData = PTHREAD_MUTEX_INITIALIZER;
#define LOG_FILE_PATH "/tmp/"
#define LOG_NAME      "zayk_log"

/**
 * @description: 检测文件是否存在
 * @detail:
 * @param {char} *pFileName
 * @return {*}
 * @author: lkc
 */
static int ZAYK_CheckFile(char *pFileName)
{
    if (strlen(pFileName) >= 128) {
        return -1;
    }

#ifdef WIN32
    if ((_access(pFileName, 4)) == -1) {
        return 0;
    }
#else
    /*Check file exist*/
    if ((access(pFileName, F_OK)) != 0) {
        return 0;
    }

#endif
    return 1;
}

/**
 * @description:  文件加锁，参数为文件名
 * @detail:
 * @param {char} *pKeyName
 * @param {int} nTimeout
 * @return {*}
 * @author: lkc
 */
static int ZAYK_LockFile(char *pKeyName, int nTimeout)
{
#if defined WIN32
    int rv;
    int hSem;

    rv = Create_Semaphore(pKeyName, 1, &hSem);
    rv = Semaphore_P(hSem, 0, nTimeout);
    if (rv) {
        return rv;
    }
#else
    pthread_mutex_lock(&(g_mutexData));
#endif
    return 0;
}

/**
 * @description: 文件解锁，参数为文件名
 * @detail:
 * @param {char} *pKeyName
 * @return {*}
 * @author: lkc
 */
static int ZAYK_UnLockFile(char *pKeyName)
{
#if defined WIN32
    int rv;
    int hSem;

    rv = Create_Semaphore(pKeyName, 1, &hSem);
    rv = Semaphore_V(hSem, 0);
    if (rv) {
        return rv;
    }
#else
    pthread_mutex_unlock(&(g_mutexData));
#endif

    return 0;
}

/**
 * @description: 检测目录是否存在，返回目录位置
 * @detail:
 * @param {char} *path
 * @return {*}
 * @author: lkc
 */
static void ZAYK_GetDir(char *path)
{
#if defined WIN32
    char *pSystemPan;
    char *pSystemPath;

    pSystemPan = getenv("SYSTEMROOT");
    if (pSystemPan == NULL) {
        return NULL;
    }
    pSystemPath = getenv("HOMEPATH");
    if (pSystemPath == NULL) {
        return NULL;
    }
    sprintf(path, "%c:%s", pSystemPan[0], pSystemPath);
    return;

#else
    // sprintf(path, "%s", getenv("HOME"));
    sprintf(path, "%s", LOG_FILE_PATH);
#endif

    return;
}

/**
 * @description: 检测文件夹是否存在
 * @detail:
 * @param {char} *pFileName
 * @return {*}
 * @author: lkc
 */
static int ZAYK_CheckDir(char *pFileName)
{

#if defined WIN32
    int rv = 0;
#else
    struct stat st;
#endif

    if (pFileName == NULL) {
        return -1;
    }

#if defined WIN32
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

typedef enum {
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WRAN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_PRINT,
} ZAYK_LOG_LEVEL_E;

static const char *level_str[] = {"ERROR", "WARN ", "INFO ", "DEBUG"};

void __Zayk_Debug(int level, char *file, int line, char *func, unsigned char *msg, int msglen, char *fmt, ...)
{
    FILE *fp;
    int rv, i, j;
    struct tm *newtime;
    time_t aclock;
    struct timeval nowTime;
    char sAvailableLogFile[256], sTmpFileName[1024], tmpLogPath[256];
    va_list args;
    int rowCount = 16;
    char logPath[256] = {0};
    char timeBuf[64];
    char buf[64];

    // 获取日志目录位置
    ZAYK_GetDir(logPath);

#if defined WIN32
    sprintf(tmpLogPath, "%s\\%s", logPath, LOG_NAME);
#else
    sprintf(tmpLogPath, "%s%s", logPath, LOG_NAME);
#endif
    // printf("\n %s ",tmpLogPath);
    // 判断日志位置是否存在，不存在则返回
    rv = ZAYK_CheckDir(tmpLogPath);
    if (rv != 0) {
        return;
    }
    time(&aclock);
    newtime = localtime(&aclock);

#if defined _ZAYK_WINDOWS_
    sprintf(sAvailableLogFile, "%s\\" CARD_MODEL_NAME "_%4d%02d%02d.log", tmpLogPath, newtime->tm_year + 1900,
            newtime->tm_mon + 1, newtime->tm_mday);
#else
    sprintf(sAvailableLogFile, "%s/" CARD_MODEL_NAME "_%4d%02d%02d.log", tmpLogPath, newtime->tm_year + 1900,
            newtime->tm_mon + 1, newtime->tm_mday);
#endif

    rv = ZAYK_LockFile(LOG_LOCK_KEY, 1);
    if (rv != 0) {
        return;
    }
    fp = fopen(sAvailableLogFile, "a+");
    if (NULL == fp) {
        ZAYK_UnLockFile(LOG_LOCK_KEY);
        return;
    }

    switch (level) {
        case LOG_LEVEL_ERROR:
        case LOG_LEVEL_WRAN:
        case LOG_LEVEL_INFO:
        case LOG_LEVEL_DEBUG:
            gettimeofday(&nowTime, NULL);
            newtime = localtime(&nowTime.tv_sec);
            strftime(buf, sizeof buf, "%H:%M:%S", newtime);
            snprintf(timeBuf, sizeof timeBuf, "%s.%03d", buf, nowTime.tv_usec / 1000);
            fprintf(fp, "%s %s [%d:%lu] [%s:%d] %s: ", level_str[level], timeBuf, getpid(), (int)syscall(SYS_gettid),
                    file, line, func);
            break;
        case LOG_LEVEL_PRINT:
            break;
        default:
            break;
    }

    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
    fprintf(fp, "\n");

    // 日志大于 10M则进行备份
    if (ftell(fp) > (10 * 1024 * 1024)) {
        fclose(fp);
        for (i = 1;; i++) {
            memset(sTmpFileName, 0x00, sizeof(sTmpFileName));
            sprintf(sTmpFileName, "%s.bak%d", sAvailableLogFile, i);
            if (!ZAYK_CheckFile(sTmpFileName)) {
#ifdef WIN32
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

    ZAYK_UnLockFile(LOG_LOCK_KEY);

    return;
}

char *Zayk_GetErrCodeInfo(int errCode)
{
    if (0xA001 == errCode) {
        return "未知错误";
    } else if (0xA002 == errCode) {
        return "打开Key失败";
    } else if (0xA003 == errCode) {
        return "没有发现Key";
    } else if (0xA004 == errCode) {
        return "Malloc出错";
    } else if (0xA005 == errCode) {
        return "通信写出错";
    } else if (0xA006 == errCode) {
        return "通信读出错";
    } else if (0xA007 == errCode) {
        return "通信数据头错误";
    } else if (0x1 == (errCode & 0xFFFF)) {
        return "未知错误";
    } else if (0x2 == (errCode & 0xFFFF)) {
        return "不支持";
    } else if (0x3 == (errCode & 0xFFFF)) {
        return "通信错误";
    } else if (0x4 == (errCode & 0xFFFF)) {
        return "硬件错误";
    } else if (0x5 == (errCode & 0xFFFF)) {
        return "打开设备错误";
    } else if (0x6 == (errCode & 0xFFFF)) {
        return "打开会话句柄错误";
    } else if (0x7 == (errCode & 0xFFFF)) {
        return "权限不满足";
    } else if (0x8 == (errCode & 0xFFFF)) {
        return "密钥不存在";
    } else if (0x9 == (errCode & 0xFFFF)) {
        return "不支持的算法";
    } else if (0xA == (errCode & 0xFFFF)) {
        return "不支持的算法模式";
    } else if (0xB == (errCode & 0xFFFF)) {
        return "公钥运算错误";
    } else if (0xC == (errCode & 0xFFFF)) {
        return "私钥运算错误";
    } else if (0xD == (errCode & 0xFFFF)) {
        return "签名错误";
    } else if (0xE == (errCode & 0xFFFF)) {
        return "验证错误";
    } else if (0xF == (errCode & 0xFFFF)) {
        return "对称运算错误";
    } else if (0x10 == (errCode & 0xFFFF)) {
        return "步骤错误";
    } else if (0x11 == (errCode & 0xFFFF)) {
        return "文件大小错误或输入数据长度非法";
    } else if (0x12 == (errCode & 0xFFFF)) {
        return "文件不存在";
    } else if (0x13 == (errCode & 0xFFFF)) {
        return "文件操作偏移量错误";
    } else if (0x14 == (errCode & 0xFFFF)) {
        return "密钥类型错误";
    } else if (0x15 == (errCode & 0xFFFF)) {
        return "密钥错误";
    } else if (0x16 == (errCode & 0xFFFF)) {
        return "加密数据错误";
    } else if (0x17 == (errCode & 0xFFFF)) {
        return "随机数产生失败";
    } else if (0x18 == (errCode & 0xFFFF)) {
        return "私钥使用权限获取失败";
    } else if (0x19 == (errCode & 0xFFFF)) {
        return " MAC运算失败";
    } else if (0x1A == (errCode & 0xFFFF)) {
        return "指定文件已存在";
    } else if (0x1B == (errCode & 0xFFFF)) {
        return "文件写入失败";
    } else if (0x1C == (errCode & 0xFFFF)) {
        return "存储空间不足";
    } else if (0x1D == (errCode & 0xFFFF)) {
        return "输入参数错误";
    } else if (0x1E == (errCode & 0xFFFF)) {
        return "输出参数错误";
    } else if (0x1F == (errCode & 0xFFFF)) {
        return "管理权限不满足";
    } else if (0x20 == (errCode & 0xFFFF)) {
        return "操作权限不满足";
    } else if (0x21 == (errCode & 0xFFFF)) {
        return "内存分配错误";
    } else if (0x22 == (errCode & 0xFFFF)) {
        return "句柄为空";
    } else if (0x23 == (errCode & 0xFFFF)) {
        return "参数错误";
    } else if (0x24 == (errCode & 0xFFFF)) {
        return "调用设备函数错误";
    } else if (0x25 == (errCode & 0xFFFF)) {
        return "创建文件失败";
    } else if (0x26 == (errCode & 0xFFFF)) {
        return "私钥权限码错误";
    } else if (0x27 == (errCode & 0xFFFF)) {
        return "传入的长度错误";
    } else if (0x28 == (errCode & 0xFFFF)) {
        return "密钥索引错误";
    } else if (0x29 == (errCode & 0xFFFF)) {
        return "密钥长度错误";
    } else if (0x2A == (errCode & 0xFFFF)) {
        return "备份时管理员数目不符合要求";
    } else if (0x2B == (errCode & 0xFFFF)) {
        return "门限拆分错误";
    } else if (0x2C == (errCode & 0xFFFF)) {
        return "门限合成错误";
    } else if (0x2D == (errCode & 0xFFFF)) {
        return "恢复时校验值错误";
    } else if (0x2E == (errCode & 0xFFFF)) {
        return "未初始化或已触发密钥销毁，需要重新初始化才能使用";
    } else if (0x30 == (errCode & 0xFFFF)) {
        return "管理员数目已达上限";
    } else if (0x31 == (errCode & 0xFFFF)) {
        return "操作员数目已达上限";
    } else if (0x32 == (errCode & 0xFFFF)) {
        return "管理员已经存在";
    } else if (0x33 == (errCode & 0xFFFF)) {
        return "操作员已经存在";
    } else if (0x34 == (errCode & 0xFFFF)) {
        return "管理员不存在";
    } else if (0x35 == (errCode & 0xFFFF)) {
        return "操作员不存在";
    } else if (0x36 == (errCode & 0xFFFF)) {
        return "管理员登录失败";
    } else if (0x37 == (errCode & 0xFFFF)) {
        return "操作员登录失败";
    } else if (0x38 == (errCode & 0xFFFF)) {
        return "没有管理员权限";
    } else if (0x39 == (errCode & 0xFFFF)) {
        return "没有操作员权限";
    } else if (0x3A == (errCode & 0xFFFF)) {
        return "只剩一个管理员，无法删除";
    } else if (0x3B == (errCode & 0xFFFF)) {
        return "同步信息错误";
    } else if (0x50 == (errCode & 0xFFFF)) {
        return "曲线运算错误";
    } else if (0x2F == (errCode & 0xFFFF)) {
        return "无效的命令";
    } else if (0x100 == (errCode & 0xFFFF)) {
        return "授权错误";
    } else if (0x101 == (errCode & 0xFFFF)) {
        return "授权超时";
    } else {
        return "错误码没有描述信息，请资讯开发人员";
    }

    return "未知错误";
}
