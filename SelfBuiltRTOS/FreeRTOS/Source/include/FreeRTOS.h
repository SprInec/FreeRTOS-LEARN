#ifndef FREERTOS_H
#define FREERTOS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOSConfig.h"
#include "projdefs.h"
#include "portable.h"
#include "list.h"

/* 编译器版本 */
#define COMPILER_VERSION 5

/* 任务控制块 */
typedef struct tskTaskControlBlock
{
    volatile StackType_t *pxTopOfStack;         /* 任务栈顶指针 */
    ListItem_t xStateListItem;                  /* 任务节点 */
    StackType_t *pxStack;                       /* 任务栈起始地址 */
    char pcTaskName[configMAX_TASK_NAME_LEN];   /* 任务名称 */
} tskTCB;
typedef tskTCB TCB_t;

#ifdef __cplusplus
}
#endif
#endif /* FREERTOS_H */
