#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "common_includes.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: pmbus滤波
 * @detail:
 * @param {FLOAT} fCurVoltage
 * @param {FLOAT} *pfFilterVoltage
 * @return {*}
 * @author: lkc
 */
BOOL COMMON_AveRangeFilter(AVE_RANGE_FILTER *pstFilter, FLOAT fCurVoltage, FLOAT *pfFilterVoltage)
{
    /* 采集一次值 */
    FLOAT *faVoltage = pstFilter->pStoreArr;
    ULONG *ulVoltageNum = &pstFilter->ulNum;
    FLOAT fVoltageSum = 0.0;

    /* 第一次进入 */
    if (0 == *ulVoltageNum) {
        faVoltage[0] = fCurVoltage;
        (*ulVoltageNum)++;
        return FALSE;
    }

    if (ABS(faVoltage[0] - fCurVoltage) > pstFilter->fRange) {
        *ulVoltageNum = 0;
        faVoltage[*ulVoltageNum] = fCurVoltage;
        (*ulVoltageNum)++;
        return FALSE;
    } else {
        faVoltage[*ulVoltageNum] = fCurVoltage;
        (*ulVoltageNum)++;
    }

    /* 滤波完成 */
    if (pstFilter->ulTotalNum == *ulVoltageNum) {
        *ulVoltageNum = 1;
        faVoltage[*ulVoltageNum] = fCurVoltage;

        for (size_t i = 0; i < pstFilter->ulTotalNum; i++) {
            fVoltageSum += faVoltage[i];
        }
        *pfFilterVoltage = fVoltageSum / pstFilter->ulTotalNum;

        return TRUE;
    } else /* 没有滤波完成 */
    {
        return FALSE;
    }
}
#ifdef __cplusplus
}
#endif
