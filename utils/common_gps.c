/*
 * @Description: 已验证 吊钩使用 GPS数据解析
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-10-10 14:07:36
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include <stdlib.h>
#include "rtthread.h"

#include "common.h"
#include "common_gps.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#if ENABLE_RMC
/**
 * @description: gps RMC句柄分析
 * @detail:
 * $GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
 * @param {UCHAR} *pcData
 * @return {*}
 * @author: lkc
 */
RMC_S Common_Gps_RmcPrase(CHAR *pcData)
{
    CHAR *pucTmpStr = pcData;
    CHAR ucStrNum = 0;
    CHAR *pucSplictStr = NULL;
    RMC_S stRmp;
    CHAR aucTim[3] = {0};

    /* 分割字符串 */
    pucSplictStr = Common_Strsplit(&pucTmpStr, (CHAR *)",");

    stRmp.ucFlag = 1;

    while (pucSplictStr) {
        switch (ucStrNum) {
            case 1:
                /* 拷贝字符串 utc时间 */
                strcpy(stRmp.cUtcTim, pucSplictStr);
                aucTim[0] = stRmp.cUtcTim[0];
                aucTim[1] = stRmp.cUtcTim[1];
                stRmp.Hour = atoi(aucTim);
                aucTim[0] = stRmp.cUtcTim[2];
                aucTim[1] = stRmp.cUtcTim[3];
                stRmp.Min = atoi(aucTim);
                aucTim[0] = stRmp.cUtcTim[4];
                aucTim[1] = stRmp.cUtcTim[5];
                stRmp.Sec = atoi(aucTim);
                /* 跳过小数点 */
                aucTim[0] = stRmp.cUtcTim[7];
                aucTim[1] = stRmp.cUtcTim[8];
                stRmp.mSec = atoi(aucTim);
                break;
            case 2:
                /* 位置状态,占一个字符,通过该字符可以判断是否定位成功 */
                stRmp.cIsPos = pucSplictStr[0];
                break;
            case 3:
                /* 双浮点数 维度 */
                stRmp.dLat = strtod(pucSplictStr, NULL);
                break;
            case 4:
                /* 方向 */
                stRmp.cLatDir = pucSplictStr[0];
                break;
            case 5:
                /* 双浮点数 经度 */
                stRmp.dLon = strtod(pucSplictStr, NULL);
                break;
            case 6:
                /* 方向 */
                stRmp.cLonDir = pucSplictStr[0];
                break;
            case 7:
                /* 速度,转换浮点数 */
                stRmp.dSpeedKn = strtod(pucSplictStr, NULL);
                break;
            case 8:
                /* 方向角,转换浮点数 */
                stRmp.dTrackTrue = strtod(pucSplictStr, NULL);
                break;
            case 9:
                /* UTC日期,拷贝字???串 */
                strcpy(stRmp.cDate, pucSplictStr);
                aucTim[0] = stRmp.cDate[0];
                aucTim[1] = stRmp.cDate[1];
                stRmp.Day = atoi(aucTim);
                aucTim[0] = stRmp.cDate[2];
                aucTim[1] = stRmp.cDate[3];
                stRmp.Month = atoi(aucTim);
                aucTim[0] = stRmp.cDate[4];
                aucTim[1] = stRmp.cDate[5];
                stRmp.Year = atoi(aucTim);
                break;
            case 10:
                /* 磁偏角,换浮点数 */
                stRmp.dMagVar = strtod(pucSplictStr, NULL);
                break;
            case 11:
                /* 磁偏角 */
                stRmp.cVarDir = pucSplictStr[0];
                break;
            case 14:
                /* 模式 */
                stRmp.cModeInd = pucSplictStr[0];
                break;
            default:
                break;
        }
        pucSplictStr = Common_Strsplit(&pucTmpStr, ",");
        ucStrNum++;
    }

    return stRmp;
}
#endif

#if ENABLE_RMC
/**
 * @description: gps RMC句柄分析
 * @detail:
 * $GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
 * @param {UCHAR} *pcData
 * @return {*}
 * @author: lkc
 */
GGA_S Common_Gps_GgaPrase(CHAR *pcData)
{
    CHAR *pucTmpStr = pcData;
    CHAR ucStrNum = 0;
    CHAR *pucSplictStr = NULL;
    GGA_S stRmp;
    CHAR aucTim[3] = {0};

    /* 分割字???串 */
    pucSplictStr = Common_Strsplit(&pucTmpStr, (CHAR *)",");

    stRmp.ucFlag = 1;

    while (pucSplictStr) {
        switch (ucStrNum) {
            case 1:
                /* 拷贝字???串 utc时间 */
                strcpy(stRmp.cUtcTim, pucSplictStr);
                aucTim[0] = stRmp.cUtcTim[0];
                aucTim[1] = stRmp.cUtcTim[1];
                stRmp.Hour = atoi(aucTim);
                aucTim[0] = stRmp.cUtcTim[2];
                aucTim[1] = stRmp.cUtcTim[3];
                stRmp.Min = atoi(aucTim);
                aucTim[0] = stRmp.cUtcTim[4];
                aucTim[1] = stRmp.cUtcTim[5];
                stRmp.Sec = atoi(aucTim);
                aucTim[0] = stRmp.cUtcTim[7];
                aucTim[1] = stRmp.cUtcTim[8];
                stRmp.mSec = atoi(aucTim);
                break;
            case 2: /* 双浮点数 维度 */
                stRmp.dLat = strtod(pucSplictStr, NULL);
                break;
            case 3: /* 维度方向 */
                stRmp.cLatDir = pucSplictStr[0];
                break;
            case 4: /* 精度 */
                stRmp.dLon = strtod(pucSplictStr, NULL);
                break;
            case 5: /* 经度方向 */
                stRmp.cLonDir = pucSplictStr[0];
                break;
            case 6: /* 质量 */
                stRmp.quality = strtol(pucSplictStr, NULL, 10);
                break;
            case 7: /* 卫星数 */
                stRmp.staNum = strtol(pucSplictStr, NULL, 10);
                break;
            case 8: /* 水平精度因子 */
                stRmp.hdop = strtof(pucSplictStr, NULL);
                break;
            case 9: /* 天线海拔 */
                stRmp.alt = strtof(pucSplictStr, NULL);
                break;
            case 10: /* 天线高度单位 */
                stRmp.aunits = pucSplictStr[0];
                break;
            case 11: /* 大地水准面差距 */
                stRmp.undulation = strtof(pucSplictStr, NULL);
                break;
            case 12: /* 大地水准面差距 */
                stRmp.uunits = pucSplictStr[0];
                break;
            case 13: /* 差分数据龄期 */
                stRmp.age = strtol(pucSplictStr, NULL, 10);
                break;
            case 14: /* 差分数据龄期 */
                stRmp.id = strtol(pucSplictStr, NULL, 10);
                break;
            default:
                break;
        }
        pucSplictStr = Common_Strsplit(&pucTmpStr, ",");
        ucStrNum++;
    }

    return stRmp;
}
#endif

#if ENABLE_HEAD
/**
 * @description: gps HEAD 定向句柄分析
 * @detail:
 * #HEADINGA,COM1,0,74.0,FINE,2076,193804.000,132881,7,18;SOL_COMPUTED,NARROW_INT,
 * 0.0039,135.4320,50.9203,0.0000,204.8335,136.4409,"999",8,7,7,7,3,01,3,0*d473c5e9
 * @param {UCHAR} *pcData
 * @return {*}
 * @author: lkc
 */
HEAD_S Common_Head_GgaPrase(CHAR *pcData)
{
    CHAR *pucTmpStr = pcData;
    CHAR ucStrNum = 0;
    CHAR *pucSplictStr = NULL;
    HEAD_S stRmp;

    /* 分割字???串 */
    pucSplictStr = Common_Strsplit(&pucTmpStr, (CHAR *)",");

    stRmp.ucFlag = 1;

    while (pucSplictStr) {
        switch (ucStrNum) {
            case 10: {
                MEMSET(stRmp.posType, 0, sizeof(stRmp.posType));
                /* 位置类型 */
                char *pcTmp = pucSplictStr;
                int i = 0;
                while (',' != pcTmp[i]) {
                    stRmp.posType[i] = pcTmp[i];
                    i++;
                }
                break;
            }
            case 11: {
                stRmp.length = strtof(pucSplictStr, NULL);
                break;
            }
            case 12: {
                stRmp.heading = strtof(pucSplictStr, NULL);
                break;
            }
            default:
                break;
        }
        pucSplictStr = Common_Strsplit(&pucTmpStr, ",");
        ucStrNum++;
    }

    return stRmp;
}
#endif

/**
 * @description: gps数据处理
 * @detail:
 * @param {UCHAR} *pcData
 * @return {*}
 * @author: lkc
 */
// GPS_ALL_INFO_S Common_Gps_Prase(CHAR *pcData)
// {
//     GPS_ALL_INFO_S stGpsInfo;
//     CHAR *pucTmpStr = strdup(pcData);

//     /* 清空gps信息 */
//     memset(&stGpsInfo, 0, sizeof(stGpsInfo));

// #if ENABLE_RMC
//     /* RMC数据解析 strdup复制并拷贝字符 */
//     pucTmpStr = STRDUP(pcData);
//     stGpsInfo.stRmc = strstr(pucTmpStr, PRE_RMC) ? Common_Gps_RmcPrase(strtok(strstr(pucTmpStr, PRE_RMC), "\r\n")) :
//     stGpsInfo.stRmc; FREE(pucTmpStr);
// #endif

// #if ENABLE_GGA
//     pucTmpStr = STRDUP(pcData);
//     stGpsInfo.stGga = strstr(pucTmpStr, PRE_GGA) ? Common_Gps_GgaPrase(strtok(strstr(pucTmpStr, PRE_GGA), "\r\n")) :
//     stGpsInfo.stGga; FREE(pucTmpStr);
// #endif

// #if ENABLE_HEAD
//     pucTmpStr = STRDUP(pcData);
//     stGpsInfo.stHead = strstr(pucTmpStr, PRE_HEAD) ? Common_Head_GgaPrase(strtok(strstr(pucTmpStr, PRE_HEAD),
//     "\r\n")) : stGpsInfo.stHead; FREE(pucTmpStr);
// #endif
//     return stGpsInfo;
// }

/**
 * @description:
 * @detail: 上边这个函数太耗费内存了
 * @param {CHAR} *pcData
 * @param {GPS_ALL_INFO_S} *pstGpsInfo
 * @return {*}
 * @author: lkc
 */
VOID Common_Gps_Prase(CHAR *pcData, GPS_ALL_INFO_S *pstGpsInfo)
{
    // GPS_ALL_INFO_S stGpsInfo;
    CHAR *pucTmpStr = strdup(pcData);

    /* 清空gps信息 */
    memset(pstGpsInfo, 0, sizeof(GPS_ALL_INFO_S));

#if ENABLE_RMC
    /* RMC数据解析 strdup复制并拷贝字符 */
    pucTmpStr = STRDUP(pcData);
    pstGpsInfo->stRmc = strstr(pucTmpStr, PRE_RMC) ? Common_Gps_RmcPrase(strtok(strstr(pucTmpStr, PRE_RMC), "\r\n"))
                                                   : pstGpsInfo->stRmc;
    FREE(pucTmpStr);
#endif

#if ENABLE_GGA
    pucTmpStr = STRDUP(pcData);
    pstGpsInfo->stGga = strstr(pucTmpStr, PRE_GGA) ? Common_Gps_GgaPrase(strtok(strstr(pucTmpStr, PRE_GGA), "\r\n"))
                                                   : pstGpsInfo->stGga;
    FREE(pucTmpStr);
#endif

#if ENABLE_HEAD
    pucTmpStr = STRDUP(pcData);
    pstGpsInfo->stHead = strstr(pucTmpStr, PRE_HEAD) ? Common_Head_GgaPrase(strtok(strstr(pucTmpStr, PRE_HEAD), "\r\n"))
                                                     : pstGpsInfo->stHead;
    FREE(pucTmpStr);
#endif
    return;
}
#ifdef __cplusplus
}
#endif
