/**
 * @file list_insert_demo.c
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "list.h"

struct xLIST List_Test;

struct xLIST_ITEM List_Item1;
struct xLIST_ITEM List_Item2;
struct xLIST_ITEM List_Item3;

int main(void)
{
    vListInitialise(&List_Test);

    vListInitialiseItem(&List_Item1);
    List_Item1.xItemValue = 1;

    vListInitialiseItem(&List_Item2);
    List_Item2.xItemValue = 2;

    vListInitialiseItem(&List_Item3);
    List_Item3.xItemValue = 3;

    vListInsert(&List_Test, &List_Item2);
    vListInsert(&List_Test, &List_Item1);
    vListInsert(&List_Test, &List_Item3);

    for (;;)
    {

    }
}
