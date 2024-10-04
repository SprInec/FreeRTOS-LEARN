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

#ifndef _LIST_H
#define _LIST_H
#ifdef __cplusplus
extern "C" {
#endif

struct xLIST_ITEM
{
    TickType_t xItemValue;
    struct xLIST_ITEM * pxNext;
    struct xLIST_ITEM * pxPrevious;
    void * pvOwner;
    void * pvContainer;
};
typedef struct xLIST_ITEM ListItem_t;

#ifdef __cplusplus
}
#endif
#endif /* _LIST_H */