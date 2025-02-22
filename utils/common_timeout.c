/*
 * @Description: 超时机制
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-09-10 15:38:33
 * @LastEditors: lkc
 * @LastEditTime: 2023-09-10 20:23:38
 * @detail:
 * 超时任务判断需要哪些参数：当前时间 超时时间 超时回调 完成回调
 *
 * 参数配置：
 * 当前时间 超时时间 判断是否成功是必须的
 *
 */
#ifndef _INCLUDES_H
#define _INCLUDES_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "common_includes.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct tagCommonTimeout {
    ULONG ulCurTim;          /* 当前时间 */
    ULONG ulTimeout;         /* 超时时间 */
    VOID (*timeoutHandle)(); /* 超时回调 */
    VOID (*successHandle)(); /* 成功回调 */
    VOID (*failHandle)();    /* 成功回调 */
    ULONG retryNum;          /* 尝试次数 */
    VOID (*retryHandle)();   /* 重新尝试回调 */
    VOID (*errHandle)();     /* 错误回调 */
    int (*judgeHandle)();    /* 判断是否成功 */
    BOOL bIsDone;            /* 是否已经完成 */
} COMMON_TIMEOUT_S;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
LIST_S gstTimeoutList; /* 超时链表 */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 获取当前时间
 * @detail:
 * @return {*}
 * @author: lkc
 */
ULONG Common_Timeout_GetTim(VOID)
{

    return;
}

/**
 * @description: 插入一个对象
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Common_Timeout_InsertObj(LIST_S *pstInsertList, COMMON_TIMEOUT_S *pstTimoutObj)
{
    pstInsertList->pData = (VOID *)pstTimoutObj;
    List_InsertEnd(gstTimeoutList, pstInsertList);
    return;
}

/**
 * @description: 超时任务
 * @detail:
 * @return {*}
 * @author: lkc
 */
VOID Common_Timeout_Task(VOID)
{
    ULONG ulCurTim = 0; /* 当前时间 */
    COMMON_TIMEOUT_S *pstTimoutTmp = NULL;
    LIST_S *pstCurList = NULL;
    int ret = 0;

    while (1) {
        /* 如果只剩下头部链表，已经无数据 */
        if (NULL == gstTimeoutList.next) {
            continue;
        }
        pstCurList = gstTimeoutList.next;
        while (pstCurList) {
            pstTimoutTmp = (COMMON_TIMEOUT_S *)pstCurList->pData;
            /* 获取当前时间 */
            ulCurTim = Common_Timeout_GetTim();

            /* 判断是否超时 */
            if (ulCurTim >= (pstTimoutTmp->ulCurTim + pstTimoutTmp->ulTimeout)) {
                ((NULL != pstTimoutTmp->timeoutHandle) && (pstTimoutTmp->timeoutHandle()), 1);

                /* 是否尝试 */
                if (pstTimoutTmp->retryNum) {
                    pstTimoutTmp->retryHandle();
                    pstTimoutTmp->retryNum--;
                } else {
                    /* 如果不尝试直接进入失败 */
                    pstTimoutTmp->errHandle();
                    /* 删除该队列 */
                    List_Remove(gstTimeoutList, pstCurList);
                }

                continue;
            }

            ret = pstTimoutTmp->judgeHandle();

            /* 判断是否成功 */
            if (0 == ret) {
                ((NULL != pstTimoutTmp->successHandle) && (pstTimoutTmp->successHandle()), 1);
            } else if (ret < 0) {
                ((NULL != pstTimoutTmp->failHandle) && (pstTimoutTmp->failHandle()), 1);
            }
            pstCurList = pstCurList->next;
        }
    }

    return;
}

#ifdef __cplusplus
}
#endif
#endif
