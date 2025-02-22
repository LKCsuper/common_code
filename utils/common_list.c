/*
 * @Description: 
 * @Version: 1.0
 * @Author: lkc
 * @Date: 2023-09-10 16:19:04
 * @LastEditors: lkc
 * @LastEditTime: 2024-07-25 11:48:50
 * @detail: 
 */
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "common_includes.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @description: 链表初始化
 * @detail: 
 * @param {LIST_S} *pstListHead
 * @return {*}
 * @author: lkc
 */
VOID List_Init(LIST_S *pstListHead)
{
    ASSERT_NULL(pstListHead);
    pstListHead->next = NULL;
    pstListHead->prev = NULL;
    pstListHead->key = 0;

    pstListHead->head = pstListHead;
    pstListHead->tail = pstListHead;

    return;
}

/**
 * @description: 插入队尾巴
 * @detail: 
 * @param {LIST_S} *pstList
 * @param {LIST_S} *pstItem
 * @return {*}
 * @author: lkc
 */
VOID List_InsertEnd(LIST_S *pstList, LIST_S *pstItem)
{
    ASSERT_NULL(pstItem);
    ASSERT_NULL(pstList);
    LIST_S *pstListTmp = pstListHead;
    LIST_S *pstListTail = pstListTmp->tail;

    /* 尾部插入 */
    pstListTail->next = pstItem;
    pstItem->prev = pstListTail;
    pstItem->next = NULL;
    pstListTmp->tail = pstItem;

    return;
}

/**
 * @description: 根据键值查找队列
 * @detail: 
 * @param {LIST_S} *pstList
 * @param {ULONG} key
 * @return {*}
 * @author: lkc
 */
LIST_S *List_Find(LIST_S *pstListHead, ULONG key)
{
    ASSERT_NULL(pstListHead);
    LIST_S *pstListTmp = pstListHead;

    pstListTmp = pstListTmp->next;
	while(pstListTmp)
    {
        if (key == pstListTmp.key)
        {
            return pstListTmp;
        }
		pstListTmp = pstListTmp->next;
	}

    return NULL;
}

/**
 * @description: 根据键值删除
 * @detail: 
 * @param {LIST_S} *pstListHead
 * @param {ULONG} key
 * @return {*}
 * @author: lkc
 */
BOOL List_RemoveKey(LIST_S *pstListHead, ULONG key)
{
    ASSERT_NULL(pstListHead);
    LIST_S *pstListTmp = pstListHead;
    LIST_S *pstListPre = NULL;
    LIST_S *pstListNext = NULL;

    pstListTmp = pstListTmp->next;
	while(pstListTmp)
    {
        if (key == pstListTmp.key)
        {
            pstListPre = pstListTmp->prev;
            pstListNext = pstListTmp->next;

            pstListPre->next = pstListNext;
            /* 如果删除的是尾部 */
            if (pstListHead->tail == pstListTmp)
            {
                pstListHead->tail = pstListPre;
            }
            else
            {
                pstListNext->prev = pstListPre;
            }
            
            return TRUE;
        }
		pstListTmp = pstListTmp->next;
	}

    return FALSE;
}

/**
 * @description: 删除
 * @detail: 
 * @param {LIST_S} *pstListHead
 * @param {LIST_S} *pstItem
 * @return {*}
 * @author: lkc
 */
BOOL List_Remove(LIST_S *pstListHead, LIST_S *pstItem)
{
    ASSERT_NULL(pstItem);
    ASSERT_NULL(pstListHead);

    LIST_S *pstListPre = NULL;
    LIST_S *pstListNext = NULL;

    pstListTmp = pstListTmp->next;
	while(pstListTmp)
    {
        /* 队列地址一致 */
        if (pstListTmp == pstItem)
        {
            pstListPre = pstListTmp->prev;
            pstListNext = pstListTmp->next;

            pstListPre->next = pstListNext;
            /* 如果删除的是尾部 */
            if (pstListHead->tail == pstListTmp)
            {
                pstListHead->tail = pstListPre;
            }
            else
            {
                pstListNext->prev = pstListPre;
            }
            
            return TRUE;
        }
		pstListTmp = pstListTmp->next;
	}
    
    return FALSE;
}
#ifdef __cplusplus
}
#endif