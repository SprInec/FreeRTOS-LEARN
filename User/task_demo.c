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