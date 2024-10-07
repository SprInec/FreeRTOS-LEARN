/**
 * @file port.c
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#define portINITIAL_XPSR (0x01000000)
#define portSTART_ADDRESS_MASK ((StackType_t)0xfffffffeUL)

#include "portmacro.h"

static void prvTaskExitError(void)
{
    /* 函数停止在这里 */
    for (;;);
}

/**
 * @brief 
 * 
 * @param pxTopOfStack 
 * @param pxCode 
 * @param pvParameters 
 * @return StackType_t* 
 */
StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters)
{
    /* 异常发生时, 自动加载到 CPU 寄存器的内容 */
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t)pxCode & portSTART_ADDRESS_MASK;
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t)prvTaskExitError;
    pxTopOfStack -= 5; /* R12 R3 R2 and R1 默认初始化为0 */
    *pxTopOfStack = (StackType_t)pvParameters;

    /* 异常发生时, 手动加载到 CPU 寄存器的内容 */
    pxTopOfStack -= 8;

    /* 返回栈顶指针, 此时pxTopOfStack指向空闲栈 */
    return pxTopOfStack;
}

#define portNVIC_SYSPRI2_REG         (*((volatile uint32_t*)0xE000ED20))

#define portNVIC_PENDSV_PRI          ((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 16 UL)
#define portNVIC_SYSTICK_PRI         ((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 24 UL)

BaseType_t xPortStartScheduler(void)
{
    /* 配置 PendSV 和 SysTick 的中断优先级为最低 */
    portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;

    /* 启动第一个任务, 不再返回 */
    prvStartFirstTask();

    /* 不应运行到这里 */
    return 0;
}

__asm void prvStartFirstTask(void)
{
    PRESERVE8

    ldr r0, = 0xE000ED08
    ldr r0, [r0]
    ldr r0, [r0]

    msr msp, r0

    cpsie i
    cpsie f
    
    dsb
    isb

    svc 0
    nop
    nop
}