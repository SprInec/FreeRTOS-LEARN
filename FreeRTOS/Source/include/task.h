/**
 * @file task.h
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TASK_H
#define TASK_H
#ifdef __cplusplus
extern "C" {
#endif

#include "portmacro.h"
#include "projdefs.h"
#include "list.h"

#define taskYIELD() portYIELD()

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 15

typedef void * TaskHandle_t;

extern List_t pxReadyTasksLists[configMAX_PRIORITIES];

// FIXME: 测试用
/* 任务控制块 */
typedef struct tskTaskControlBlock
{
    volatile StackType_t *pxTopOfStack;       /* 栈顶 */
    ListItem_t xStateListItem;                /* 任务节点 */
    StackType_t *pxStack;                     /* 任务栈起始地址 */
    char pcTaskName[configMAX_TASK_NAME_LEN]; /* 任务名称 */
} tskTCB;
typedef tskTCB TCB_t;

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                               const char *const pcName,
                               const uint32_t ulStackDepth,
                               void *const pvParameters,
                               StackType_t *const puxStackBuffer,
                               TCB_t *const pxTaskBuffer);

void prvInitialiseTaskLists(void);
void vTaskStartScheduler(void);
void vTaskSwitchContext(void);

#ifdef __cplusplus
}
#endif
#endif /* TASK_H */
