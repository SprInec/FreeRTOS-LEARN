/**
 * @file list.h
 * @author SprInec (julycubsprine@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef LIST_H
#define LIST_H
#ifdef __cplusplus
extern "C" {
#endif

#include "portmacro.h"

/* 链表节点数据结构定义 */ 
struct xLIST_ITEM
{
    TickType_t xItemValue;              /* 辅助值,用于帮助节点做顺序排序 */
    struct xLIST_ITEM *pxNext;         /* 指向下一个节点 */
    struct xLIST_ITEM *pxPrevious;     /* 指向上一个节点 */
    void *pvOwner;                     /* 指向拥有该节点的内核对象,通常是TCB */
    void *pvContainer;                 /* 指向该节点所在的链表 */
};
typedef struct xLIST_ITEM ListItem_t;

/* 链表精简节点结构体定义 */
struct xMINI_LIST_ITEM
{
    TickType_t xItemValue;              /* 辅助值,用于帮助节点做升序排序 */
    struct xLIST_ITEM *pxNext;         /* 指向下一个节点 */
    struct xLIST_ITEM *pxPrevious;     /* 指向上一个节点 */
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;

/* 链表根节点数据结构定义 */
typedef struct xLIST
{
    UBaseType_t uxNumberOfItems;       /* 链表中节点的数量 */
    ListItem_t *pxIndex;               /* 指向当前索引节点 */
    MiniListItem_t xListEnd;           /* 链表尾节点 */
} List_t;

/* 初始化节点拥有者 */
#define ListSET_LIST_ITEM_OWNER(pxListItem, pxOwner)\
        ((pxListItem)->pvOwner = (void*)(pxOwner))

/* 获取节点拥有者 */
#define ListGET_LIST_ITEM_OWNER(pxListItem)\
        ((void*)(pxListItem)->pvOwner)

/* 初始化节点排序辅助值 */
#define ListSET_LIST_ITEM_VALUE(pxListItem, xValue)\
        ((pxListItem)->xItemValue = (xValue))

/* 获取节点排序辅助值 */
#define ListGET_LIST_ITEM_VALUE(pxListItem)\
        ((pxListItem)->xItemValue)

/* 获取链表根节点的节点计数器的值 */
#define ListGET_ITEM_VALUE_OF_HEAD_ENTRY(pxList)\
        (((pxList)->xListEnd).pxNext->xItemValue)

/* 获取链表的入口节点 */
#define ListGET_HEAD_ENTRY(pxList)\
        ((pxList)->xListEnd.pxNext)

/* 获取节点的下一个节点 */
#define ListGET_NEXT(pxListItem)\
        ((pxListItem)->pxNext)

/* 获取链表的最后一个节点 */
#define ListGET_END_MARKER(pxList)\
        ((ListItem_t const *)(&((pxList)->xListEnd)))

/* 判断链表是否为空 */
#define ListLIST_IS_EMPTY(pxList)\
        ((BaseType_t)((pxList)->uxNumberOfItems == (UBaseType_t)0))

/* 获取链表的节点数 */
#define ListCURRENT_LIST_LENGTH(pxList)\
        ((pxList)->uxNumberOfItems)

/* 获取链表第一个节点的 OWNER, 即 TCB */
#define ListGET_OWNER_OF_NEXT_ENTRY(pxTCB, pxList)\
{\
    List_t *const pxConstList = (pxList);\
    /* 节点索引指向链表第一个节点 */\
    (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;\
    if ((void *)(pxConstList)->pxIndex == (void *)&((pxConstList)->xListEnd))\
    {\
        (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;\
    }\
    /* 获取节点的 OWNER/TCB */\
    (pxTCB) = (pxConstList)->pxIndex->pvOwner;\
}

void vListInitialiseItem(ListItem_t *const pxItem);
void vListInitialise(List_t *const pxList);
void vListInsertEnd(List_t *const pxList, ListItem_t *const pxNewListItem);
void vListInsert(List_t *const pxList, ListItem_t *const pxNewListItem);
UBaseType_t uxListRemove(ListItem_t *const pxItemToRemove);

#ifdef __cplusplus
}
#endif
#endif /* LIST_H */
