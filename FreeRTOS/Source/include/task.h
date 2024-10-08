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

typedef void * TaskHandle_t;

extern List_t pxReadyTasksLists[configMAX_PRIORITIES];

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                               const char *const pcName,
                               const uint32_t ulStackDepth,
                               void *const pvParameters,
                               StackType_t *const puxStackBuffer,
                               TCB_t *const pxTaskBuffer);

void prvInitialiseTaskLists(void);

#ifdef __cplusplus
}
#endif
#endif /* TASK_H */