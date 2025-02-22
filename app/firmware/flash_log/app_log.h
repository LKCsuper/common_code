#ifndef __APP_LOG_H__
#define __APP_LOG_H__
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef struct tagLogParam {
    ULONG LogNum;           // 日志数
    UCHAR aucRes[127];
} LOG_PARAM_S;

typedef struct tagLog {
    ULONG ulTim; // 时间戳

    // 异常日志
    ULONG ulAbLog1;

    // 导致关机异常日志
    ULONG ulPwrLog1;
} LOG_S;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID LOG_StoreAB(VOID);
VOID LOG_ReInit(VOID);
VOID LOG_Show(VOID);
VOID LOG_Pro(VOID);
#ifdef __cplusplus
}
#endif
#endif
