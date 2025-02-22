/*
 * @Description: 协议
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-07-21 15:20:41
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2024-09-06 13:53:51
 * @detail:
 * 想要 处理协议收发的问题：例如重发次数 超时时间
 * 协议头尾，判断包长
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMMON_PROTOCOL_RECV gstCommonProRecv;
COMMON_PROTOCOL_SEND gstCommonProSend;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// TODO 判断头部长度
VOID Common_Pro_RecvEnd(COMMON_PROTOCOL_RECV *pstCommonProRecv, COMMON_PROTOCOL_RECV *pstCommonProSend, CHAR *pucBuf,
                        ULONG ulBufLen)
{
    ASSERT_EQL(ulBufLen, 0)

    ULONG i = 0;
    STATIC ULONG ulCmpNum = 0; /* 已经匹配的个数 */
    // PRINTF("ulBufLen[%d] \r\n", ulBufLen);
    for (i = 0; i < ulBufLen; i++) {
        pstCommonProRecv->pucBuf[pstCommonProRecv->ulBufLen] = pucBuf[i];
        pstCommonProRecv->ulBufLen++;
        /* 如果已经是最大长度,或者达到实际长度 */
        if ((pstCommonProRecv->ulBufMaxLen <= pstCommonProRecv->ulBufLen) &&
            (pstCommonProRecv->ulBufRealLen <= pstCommonProRecv->ulBufLen)) {
            /* 恢复默认参数 */
        }

        if (pstCommonProRecv->pucEnd[ulCmpNum] == pucBuf[i]) {
            ulCmpNum++;
        } else {
            ulCmpNum = 0;
        }

        /* 匹配成功 */
        if (pstCommonProRecv->ulEndLen == ulCmpNum) {
            PRINTF("End 匹配成功 ulBufLen[%d] [%d]\r\n", ulBufLen, i + 1);
            ulCmpNum = 0;
            pstCommonProRecv->bIsHead = FALSE;

            if (NULL != pstCommonProRecv->successHandle) {
                PRINTF("BUF:[%02x] [%02x]\r\n", pstCommonProRecv->pucBuf[0], pstCommonProRecv->pucBuf[1]);
                gstCommonProSend.bIsOk =
                    pstCommonProRecv->successHandle(pstCommonProRecv->pucBuf, pstCommonProRecv->ulBufLen);
            }
            MEMSET(pstCommonProRecv->pucBuf, 0, pstCommonProRecv->ulBufLen);
            pstCommonProRecv->ulBufLen = 0;

            /* 如果是最后一位匹配 */
            if (ulBufLen == i + 1) {

            } else {
                i++; /* 移动到下一位 */
                // PRINTF("再次匹配Head\r\n");
                Common_Pro_RecvHead(pstCommonProRecv, pstCommonProSend, &pucBuf[i], (ulBufLen - i));
            }
            return;
        }
    }

    return;
}

VOID Common_Pro_RecvHead(COMMON_PROTOCOL_RECV *pstCommonProRecv, COMMON_PROTOCOL_RECV *pstCommonProSend, CHAR *pucBuf,
                         ULONG ulBufLen)
{
    ULONG i = 0;
    STATIC ULONG ulCmpNum = 0; /* 已经匹配的个数 */
    // PRINTF("BUFlen[%d] %02x %02x\r\n", ulBufLen, pucBuf[0], pucBuf[1]);
    /* 判断是否匹配到头部 */
    if (TRUE == pstCommonProRecv->bIsHead) {
        /* 继续匹配尾部 */
        Common_Pro_RecvEnd(pstCommonProRecv, pstCommonProSend, pucBuf, ulBufLen);
    } else {
        for (i = 0; i < ulBufLen; i++) {
            if (pstCommonProRecv->pucHead[ulCmpNum] == pucBuf[i]) {
                ulCmpNum++;
            } else {
                ulCmpNum = 0;
            }

            /* 匹配成功 */
            if (pstCommonProRecv->ulHeadLen == ulCmpNum) {

                i++;
                ulCmpNum = 0;
                pstCommonProRecv->bIsHead = TRUE;
                MEMCPY(pstCommonProRecv->pucBuf, pstCommonProRecv->pucHead, pstCommonProRecv->ulHeadLen);
                pstCommonProRecv->ulBufLen = pstCommonProRecv->ulHeadLen;
                PRINTF("Head 匹配成功 Buflen[%d] [%d] [%02x] [%02x]\r\n", ulBufLen - i, pstCommonProRecv->ulBufLen,
                       pstCommonProRecv->pucBuf[0], pstCommonProRecv->pucBuf[1]);
                Common_Pro_RecvEnd(pstCommonProRecv, pstCommonProSend, &pucBuf[i], (ulBufLen - i));
                return;
            }
        }
    }

    return;
}

VOID Common_Pro_Send(COMMON_PROTOCOL_SEND *pstCommonProSend)
{
    while (1) {
        /* 判断是否收到回复 */
        if (TRUE == pstCommonProSend->bIsOk) {
            if (NULL != pstCommonProSend->successHandle) {
                pstCommonProSend->successHandle();
            }
        }

        /* 判断是否到达最高重复次数 */
        if (pstCommonProSend->ulRetry == pstCommonProSend->ulRetrySum) {
            if (NULL != pstCommonProSend->errHandle) {
                pstCommonProSend->errHandle();
            }
            return;
        }

        /* 判断是否超时 */
        if (IS_TIME_OUT(pstCommonProSend->ulCurTim, pstCommonProSend->ulTimeout)) {
            /* 重发 */
            pstCommonProSend->ulRetrySum++;
            if (NULL != pstCommonProSend->retryHandle) {
                pstCommonProSend->retryHandle();
            }
        }
    }
}

/* 例程 */
#if 0
/**
 * @description: 接收任务
 * @detail: 
 * @param {VOID} *parameter
 * @return {*}
 * @author: lkc
 */
VOID L716_Recv_Task(VOID *parameter)
{
    ULONG ulL716BufSize = 0;
    SOCKET_S stSocket;
    ULONG ulRet = 0;
    ULONG i = 0;
    COMMON_PROTOCOL_RECV gstL716ProRecv;
    COMMON_PROTOCOL_SEND gstL716ProSend;
    USHORT usHead = 0x5AA5;
    UCHAR ucEnd = 0x96;
    UCHAR ucPackBuf[512] = {0};

    MEMSET(&gstL716ProRecv, 0, sizeof(COMMON_PROTOCOL_RECV));
    MEMSET(&gstL716ProSend, 0, sizeof(COMMON_PROTOCOL_SEND));
    gstL716ProRecv.pucHead = &usHead;
    gstL716ProRecv.ulHeadLen = sizeof(usHead);
    gstL716ProRecv.pucEnd = &ucEnd;
    gstL716ProRecv.ulEndLen = sizeof(ucEnd);
    gstL716ProRecv.pucBuf = ucPackBuf;
    gstL716ProRecv.ulBufLen = 0;
    gstL716ProRecv.successHandle = Pro_Server_ProcessPack;
    PRINTF("L716 Recv Task Start \r\n");
    /* 实时分析4g接收数据 */
    while (1)
    {
        /* 队列 */
        rt_mq_recv(&gstL716RecvQueue, &ulL716BufSize, sizeof(ulL716BufSize), RT_WAITING_FOREVER);
        /* 首先应该拼成一个包 */
        Common_Pro_RecvHead(&gstL716ProRecv, &gstL716ProSend, gaucL716RecvBuf, ulL716BufSize);
    }
}
#endif
#ifdef __cplusplus
}
#endif
