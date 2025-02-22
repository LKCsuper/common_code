/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @detail: 
 */
#ifndef __COMMON_ALG_H__
#define __COMMON_ALG_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct tagAveRangeFilter {
    FLOAT *pStoreArr; // 存储数组
    ULONG ulNum;      // 已存储次数
    FLOAT fRange;     // 范围
    ULONG ulTotalNum; // 总次数
} AVE_RANGE_FILTER_S;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
#endif