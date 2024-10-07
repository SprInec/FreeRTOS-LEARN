/**
 * @file task.c
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "FreeRTOSConfig.h"
#include "task.h"

static void prvInitialiseNewTask( TaskFunction_t pxTaskCode, 
                                  const char *const pcName,
                                  const uint32_t ulStackDepth,
                                  void *const pvParameters,
                                  TaskHandle_t * const pxCreatedTask,
                                   TCB_t * const pxNewTCB )
{
    StackType_t *pxTopOfStack;
    UBaseType_t x;

    /* 获取栈顶地址 */
    pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - (uint32_t)1);
    /* 向下做 8 字节对齐 */
    pxTopOfStack = (StackType_t *)((uint32_t)pxTopOfStack & (~((uint32_t)0x0007)));

}

#if (configSUPPORT_STATIC_ALLOCATION == 1)

/**
 * @brief 
 *
 * @param pxTaskCode 任务入口/任务函数名称
 * @param pcName 任务名称, 字符串形式, 方便调试
 * @param ulStackDepth 任务栈大小, 单位为字
 * @param pvParameters 任务形参
 * @param puxStackBuffer 任务栈起始地址
 * @param pxTaskBuffer 任务控制块指针
 * @return TaskHandle_t
 */
TaskHandle_t xTaskCreateStatic( TaskFunction_t pxTaskCode, 
                                const char *const pcName,
                                const uint32_t ulStackDepth,
                                void *const pvParameters,
                                StackType_t *const puxStackBuffer,
                                TCB_t *const pxTaskBuffer )
{
    TCB_t *pxNewTCB;
    TaskHandle_t xReturn;

    if ((pxTaskBuffer != NULL) && (puxStackBuffer != NULL))
    {
        pxNewTCB = (TCB_t *)pxTaskBuffer;
        pxNewTCB->pxStack = (StackType_t *)puxStackBuffer;

        /* 创建新任务 */
        prvInitialiseNewTask( pxTaskCode,   /* 任务入口 */
                              pcName,       /* 任务名称, 字符串形式 */
                              ulStackDepth, /* 任务栈大小, 单位为字 */
                              pvParameters, /* 任务形参 */
                              &xReturn,     /* 任务句柄 */
                              pxNewTCB );   /* 任务栈起始地址 */
    }
    else
    {
        xReturn = NULL;
    }

    /* 返回任务句柄, 如果任务创建成功, 此时xReturn应指向任务控制块 */
    return xReturn;
}

#endif /* configSUPPORT_STATIC_ALLOCATION */
