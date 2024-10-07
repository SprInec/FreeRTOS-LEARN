/**
 * @file list.c
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.04
 * 
 * @copyright Copyright (c) 2024 
 */

#include "list.h"

/**
 * @brief 链表节点初始化
 * 
 * @param pxItem 节点
 */
void vListInitialiseItem(ListItem_t *const pxItem)
{
    /* 初始化该节点所在的链表为空,表示节点还没有插入任何链表 */
    pxItem->pvContainer = NULL;
}

/**
 * @brief 链表根节点初始化/链表初始化
 * 
 * @param pxList 链表/根节点
 */
void vListInitialise(List_t *const pxList )
{
    pxList->pxIndex = (ListItem_t *) &(pxList->xListEnd);
    pxList->xListEnd.xItemValue = portMAX_DELAY;
    pxList->xListEnd.pxNext = (ListItem_t *) &(pxList->xListEnd);
    pxList->xListEnd.pxPrevious = (ListItem_t *) &(pxList->xListEnd);
    pxList->uxNumberOfItems = (UBaseType_t)0U;
}

/**
 * @brief 将节点插入链表尾部/头部
 * 
 * @param pxList 被插链表/根节点
 * @param pxNewListItem 新节点
 */
void vListInsertEnd(List_t *const pxList, ListItem_t *const pxNewListItem)
{
    ListItem_t *const pxIndex = pxList->pxIndex;

    pxNewListItem->pxNext = pxIndex;
    pxNewListItem->pxPrevious = pxIndex->pxPrevious;
    pxIndex->pxPrevious->pxNext = pxNewListItem;
    pxIndex->pxPrevious = pxNewListItem;

    /* 记住该节点所在的链表 */
    pxNewListItem->pvContainer = (void *)pxList;

    /* 更新链表长度 */
    (pxList->uxNumberOfItems)++;
}

/**
 * @brief 将节点按照升序排列插入到链表
 *
 * @details 将节点按照升序排列插入到链表,如果有两个节点的值相同,则新节点在旧节点的后面插入
 * @param pxList 被插链表/根节点
 * @param pxNewListItem 新节点
 */
void vListInsert(List_t *const pxList, ListItem_t *const pxNewListItem)
{
    ListItem_t *pxIterator;

    /* 获取节点的排序辅助值 */
    const TickType_t xValueOfInsertion = pxNewListItem->xItemValue;

    /* 寻找节点要插入的位置 */
    if (xValueOfInsertion == portMAX_DELAY)
    {
        pxIterator = pxList->xListEnd.pxPrevious;
    }
    else
    {
        for (pxIterator = (ListItem_t *)&(pxList->xListEnd);
             pxIterator->pxNext->xItemValue <= xValueOfInsertion;
             pxIterator = pxIterator->pxNext)
        {
            /* 无任务,不断迭代只为找到要插入的位置 */
        }
    }

    /* 根据升序排列, 插入节点 */
    pxNewListItem->pxNext = pxIterator->pxNext;
    pxNewListItem->pxNext->pxPrevious = pxNewListItem;
    pxNewListItem->pxPrevious = pxIterator;
    pxIterator->pxNext = pxNewListItem;

    /* 记住该节点所在的链表 */
    pxNewListItem->pvContainer = (void *)pxList;

    /* 更新链表长度 */
    (pxList->uxNumberOfItems)++;
}

/**
 * @brief 将节点从链表删除
 * 
 * @param pxItemToRemove 待删节点
 * @return UBaseType_t 链表中剩余节点的个数
 */
UBaseType_t uxListRemove(ListItem_t *const pxItemToRemove)
{
    /* 获取节点所在链表 */
    List_t *const pxLis = (List_t *)pxItemToRemove->pvContainer;

    /* 将指定节点从链表删除 */
    pxItemToRemove->pxNext->pxPrevious = pxItemToRemove->pxPrevious;
    pxItemToRemove->pxPrevious->pxNext = pxItemToRemove->pxNext;

    /* 调整链表的节点索引指针 */
    if (pxLis->pxIndex == pxItemToRemove)
    {
        pxLis->pxIndex = pxItemToRemove->pxPrevious;
    }

    /* 初始化该节点的链表为空,表示该节点还没有插入任何链表 */
    pxItemToRemove->pvContainer = NULL;

    /* 更新链表长度 */
    (pxLis->uxNumberOfItems)--;

    /* 返回链表中剩余节点的个数 */
    return pxLis->uxNumberOfItems;
}
