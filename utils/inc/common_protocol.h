/*
 * @Description:
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-07-21 15:20:01
 * @LastEditors: lkc
 * @LastEditTime: 2023-09-10 15:46:55
 * @detail:
 */
#ifndef _COMMON_PROTOCOL_H
#define _COMMON_PROTOCOL_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct tagCommonProtocolRecv {
    ULONG ulTimeout;        /* 超时时间*/
    VOID (*timoutHandle)(); /* 超时回调 */
    ULONG ulRetry;          /* 尝试次数 */
    ULONG ulRetrySum;       /* 已经尝试了几次 */
    VOID (*retryHandle)();  /* 重试回调 */
    VOID (*errHandle)();    /* 错误回调 */
    ULONG ulBufMaxLen;      /* buf */
    ULONG ulBufRealLen;     /* buf */
    UCHAR *pucBuf;          /* buf */
    ULONG ulBufLen;         /* buf长度 */
    UCHAR *pucHead;         /* 头部字符 */
    BOOL bIsHead;
    ULONG ulHeadLen;                                    /* 头部字符长度 */
    UCHAR *pucEnd;                                      /* 尾部字符 */
    ULONG ulEndLen;                                     /* 尾部字符长度 */
    BOOL (*successHandle)(UCHAR *pucPack, ULONG ulLen); /* 成功回调 */
} COMMON_PROTOCOL_RECV;

typedef struct tagCommonProtocolSend {
    ULONG ulTimeout;         /* 超时时间*/
    ULONG ulCurTim;          /* 当前时间 */
    VOID (*timoutHandle)();  /* 超时回调 */
    ULONG ulRetry;           /* 尝试次数 */
    ULONG ulRetrySum;        /* 已经尝试了几次 */
    VOID (*retryHandle)();   /* 重试回调 */
    VOID (*errHandle)();     /* 错误回调 */
    BOOL (*successHandle)(); /* 成功回调 */
    BOOL bIsOk;              /* 是否收到正确回复 */
} COMMON_PROTOCOL_SEND;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
VOID Common_Pro_RecvHead(COMMON_PROTOCOL_RECV *pstCommonProRecv, COMMON_PROTOCOL_RECV *pstCommonProSend, CHAR *pucBuf,
                         ULONG ulBufLen);
VOID Common_Pro_RecvEnd(COMMON_PROTOCOL_RECV *pstCommonProRecv, COMMON_PROTOCOL_RECV *pstCommonProSend, CHAR *pucBuf,
                        ULONG ulBufLen);
#ifdef __cplusplus
}
#endif
#endif
