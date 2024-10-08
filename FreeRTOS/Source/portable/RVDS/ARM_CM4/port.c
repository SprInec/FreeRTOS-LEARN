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

#include "portmacro.h"
#include "projdefs.h"
#include "list.h"
#include "task.h"

#define portINITIAL_XPSR (0x01000000)
#define portSTART_ADDRESS_MASK ((StackType_t)0xfffffffeUL)

#define portNVIC_SYSPRI2_REG (*((volatile uint32_t *)0xE000ED20))

#define portNVIC_PENDSV_PRI (((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 16UL)
#define portNVIC_SYSTICK_PRI (((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 24UL)

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

void prvStartFirstTask(void)
{
    PRESERVE8

    __asm("ldr r0, = 0xE000ED08");
    __asm("ldr r0, [r0]");
    __asm("ldr r0, [r0]");

    __asm("msr msp, r0");

    __asm("cpsie i");
    __asm("cpsie f");

    __asm("dsb");
    __asm("isb");

    __asm("svc 0");
    __asm("nop");
    __asm("nop");
}

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

void vPortSVCHandler(void)
{
    // extern pxCurrentTCB;

    __asm("PRESERVE8");

    __asm("ldr r3, =pxCurrentTCB");
    __asm("ldr r1, [r3]");
    __asm("ldr r0, [r1]");
    __asm("ldmia r0!, {r4-r11}");
    __asm("msr psp, r0");
    __asm("isb");
    __asm("mov r0, #0");
    __asm("msr basepri, r0");
    __asm("orr r14, #0xd");

    __asm("bx r14");
}

/**
 * @brief 实现任务切换 
 */
void xPortPendSVHandler(void)
{
    // extern pxCurrentTCB;
    // extern vTaskSwitchContext;

    __asm("PRESERVE8");

    __asm("mrs r0, psp");
    __asm("isb");

    __asm("ldr r3, =pxCurrentTCB");
    __asm("ldr r2, [r3]");

    __asm("stmdb r0!, {r4-r11}");
    __asm("str r0, [r2]");

    __asm("stmdb sp!, {r3, r4}");
    __asm("mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY");
    __asm("msr basepri, r0");
    __asm("dsb");
    __asm("isb");
    __asm("bl vTaskSwitchContext");
    __asm("mov r0, #0");
    __asm("msr basepri, r0");
    __asm("ldmia sp!, {r3, r14}");

    __asm("ldr r1, [r3]");
    __asm("ldr r0, [r1]");
    __asm("ldmia r0!, {r4-r11}");
    __asm("msr psp, r0");
    __asm("isb");
    __asm("bx r14");
    __asm("nop");
}