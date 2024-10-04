/**
 * @file list.c
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.04
 * 
 * @copyright Copyright (c) 2024 SprInec
 * 
 */

#include "list.h"

void vListInitialiseItem(ListItem_t * const pxItem )
{
    /* 初始化该节点所在的链表为空,表示节点还没有插入任何链表 */
    pxItem->pvContainer = NULL;
}
