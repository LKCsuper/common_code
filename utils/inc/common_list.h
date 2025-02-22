#ifndef _COMMON_LIST_H
#define _COMMON_LIST_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct tagList {
    VOID *pData;
    ULONG key;         /* 键值 */
    struct List *next; /* 下一个列表 */
    struct List *prev; /* 前一个列表 */
    struct List *tail; /* 尾部链表 */
    struct List *head; /* 头部链表 */
} LIST_S;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
BOOL List_Remove(LIST_S *pstListHead, LIST_S *pstItem);
BOOL List_RemoveKey(LIST_S *pstListHead, ULONG key);
LIST_S *List_Find(LIST_S *pstListHead, ULONG key);
VOID List_InsertEnd(LIST_S *pstList, LIST_S *pstItem);
VOID List_Init(LIST_S *pstListHead);
#ifdef __cplusplus
}
#endif
#endif