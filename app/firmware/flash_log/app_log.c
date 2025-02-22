/*
 * @Description: 环境失效保护模组已经实验验证
 * detail
 *  LOG_ADDR_START  日志存储区域
 *  LOG_SIZE 日志存储区域大小
 *  LOG_PARAM_SIZE 日志配置参数大小  
 *  LOG_PARAM_ADDR_START 日志配置参数存储区域
 * @Version: 1.0
 * @Author: lkc
 * @detail:
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "app_log.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
LOG_PARAM_S gstLogParm;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 存储异常
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID LOG_StoreAB(VOID)
{
    LOG_S stLog;
    ULONG ulTotalLogSize = LOG_SIZE / sizeof(LOG_S);
    ULONG ulWriteAddr = 0;
    rtc_calendar_t stTim;

    memset(&stLog, 0x00, sizeof(LOG_S));
    /* 读取日志配置 */
    BSP_FlashRead(LOG_PARAM_ADDR_START, (UCHAR *)&gstLogParm, LOG_PARAM_SIZE);

    ulWriteAddr = (gstLogParm.LogNum % ulTotalLogSize) * sizeof(LOG_S) + LOG_ADDR_START;
    
/*日志参数赋值区域**************************************************************************/
    PCF8563_GetDateTime(&stTim);
    gpstStatusParam->ulTimStamp = Common_ConvertTimstamp(stTim, 1);
    stLog.ulTim = gpstStatusParam->ulTimStamp;
    stLog.ulAbLog1 = gpstStatusParam->ulAbLog;
    stLog.ulPwrLog1 = gpstStatusParam->ulPwrLog;
/***************************************************************************/

    BSP_FLASHWriteWithErase((UCHAR *)&stLog, ulWriteAddr, sizeof(LOG_S));
    gstLogParm.LogNum++; // 日志数增加

    BSP_FlashErase(LOG_PARAM_ADDR_START, LOG_PARAM_SIZE);
    BSP_FlashWrite(LOG_PARAM_ADDR_START, (UCHAR *)&gstLogParm, LOG_PARAM_SIZE);
    return;
}

/**
 * @description: 日志重初始化
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID LOG_ReInit(VOID)
{
    LOG_S stLog;
    memset(&stLog, 0x00, sizeof(LOG_S));

    BSP_FlashErase(LOG_PARAM_ADDR_START, LOG_PARAM_SIZE + LOG_SIZE);

    /* 读取日志配置 */
    BSP_FlashRead(LOG_PARAM_ADDR_START, (UCHAR *)&gstLogParm, LOG_PARAM_SIZE);
    gstLogParm.LogNum = 0;
    BSP_FlashErase(LOG_PARAM_ADDR_START, LOG_PARAM_SIZE);
    BSP_FlashWrite(LOG_PARAM_ADDR_START, (UCHAR *)&gstLogParm, LOG_PARAM_SIZE);

    return;
}

/**
 * @description: 日志显示
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID LOG_Show(VOID)
{
    /* 时间戳转换函数 */
    ULONG i = 0;
    ULONG ulTotalLogSize = LOG_SIZE / sizeof(LOG_S);
    ULONG ulReadLogNum = 0;
    LOG_S stLog;
    memset(&stLog, 0x00, sizeof(LOG_S));

    /* 读取日志配置 */
    BSP_FlashRead(LOG_PARAM_ADDR_START, (UCHAR *)&gstLogParm, LOG_PARAM_SIZE);

    ulReadLogNum = gstLogParm.LogNum;
    if (gstLogParm.LogNum >= ulTotalLogSize) {
        ulReadLogNum = ulTotalLogSize;
    }

    PRINTF("LOG TOTAL %d HAVE %d CUR %d\n", ulTotalLogSize, ulReadLogNum, gstLogParm.LogNum % ulTotalLogSize);
    for (i = 0; i < ulReadLogNum; i++) {
        PRINTF("");
        BSP_FlashRead(LOG_ADDR_START + i * sizeof(LOG_S), (UCHAR *)&stLog, sizeof(LOG_S));
/**********************************************************************************************************/
        rtc_calendar_t stTim;
        stTim = Common_ConvertCalendar(stLog.ulTim, 1);
        PRINTF("%03d %d:%02d:%02d:%02d:%02d:%02d %08x:%08x\n", i, stTim.year, stTim.month, stTim.day, stTim.hour, stTim.minute, stTim.second,
               stLog.ulAbLog1, stLog.ulPwrLog1);
/**********************************************************************************************************/
    }

    return;
}

/**
 * @description: 日志处理
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID LOG_Pro(VOID)
{
    STATIC ULONG ulLastAb = 0;

    if (ulLastAb != gpstStatusParam->aucAbnormalTotal) {
        INFO("STORE LOG\n");
        LOG_StoreAB();
        ulLastAb = gpstStatusParam->aucAbnormalTotal;
    }

    return;
}
#ifdef __cplusplus
}
#endif
