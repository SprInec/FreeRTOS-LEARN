/**
 * @file FreeRTOSConfig.h
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef FREERTOSCONFIG_H
#define FREERTOSCONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

// FIXME: 测试用
#include "list.h"

#define configUSE_16_BIT_TICKS 0

#define configMAX_TASK_NAME_LEN 16

#define configSUPPORT_STATIC_ALLOCATION 1

#define configMAX_PRIORITIES 5

#define configKERNEL_INTERRUPT_PRIORITY 255

/* 修改 FreeRos 中 SVC, PendSV 和 SysTick 中断服务函数的名称 */
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
#define xPortSVCHandler SVC_Handler

        // FIXME: 测试用
        /* 任务控制块 */
        typedef struct tskTaskControlBlock
{
    volatile StackType_t *pxTopOfStack;                             /* 栈顶 */
    ListItem_t            xStateListItem;                           /* 任务节点 */
    StackType_t          *pxStack;                                  /* 任务栈起始地址 */
    char                  pcTaskName[configMAX_TASK_NAME_LEN];      /* 任务名称 */
} tskTCB;
typedef tskTCB TCB_t;

#ifdef __cplusplus
}
#endif
#endif /* FREERTOSCONFIG_H */
