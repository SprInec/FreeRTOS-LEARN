/**
 * @file task_demo.c
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "FreeRTOSConfig.h"

/* 定义任务栈 */
#define TASK1_STACK_SIZE 128
StackType_t Task1Stack[TASK1_STACK_SIZE];

#define TASK2_STACK_SIZE 128
StackType_t Task2Stack[TASK2_STACK_SIZE];

/* 定义任务标志 */
portCHAR flag1 = 0;
portCHAR flag2 = 0;

/* 定义任务控制块 */
TCB_t Task1TCB;
TCB_t Task2TCB;

TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;

/**
 * @brief 软件延时
 * 
 * @param count 
 */
void delay(uint32_t count)
{
    for (; count != 0; count--);
}

void Task1_Entry(void *p_arg)
{
    for (;;)
    {
        flag1 = 1;
        delay(100);
        flag1 = 0;
        delay(100);
    }
}

void Task2_Entry(void *p_arg)
{
    for (;;)
    {
        flag2 = 1;
        delay(100);
        flag2 = 0;
        delay(100);
    }
}

int main(void)
{
    prvInitialiseTaskLists();

    Task1_Handle =
        xTaskCreateStatic((TaskFunction_t)Task1_Entry,
                          (char *)"Task1",
                          (uint32_t)TASK1_STACK_SIZE,
                          (void *)NULL,
                          (StackType_t *)Task1Stack,
                          (TCB_t *)&Task1TCB);

    /* 将任务添加到就绪列表 */
    vListInsertEnd(&(pxReadyTasksLists[1]),
                   &(((TCB_t*)&Task1TCB)->xStateListItem));

    Task2_Handle =
        xTaskCreateStatic((TaskFunction_t)Task2_Entry,
                          (char *)"Task2",
                          (uint32_t)TASK2_STACK_SIZE,
                          (void *)NULL,
                          (StackType_t *)Task2Stack,
                          (TCB_t *)&Task2TCB);

    /* 将任务添加到就绪列表 */
    vListInsertEnd(&(pxReadyTasksLists[2]),
                   &(((TCB_t*)&Task2TCB)->xStateListItem));
}