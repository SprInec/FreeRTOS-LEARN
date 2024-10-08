/**
 * @file portmacro.h
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PORTMACRO_H
#define PORTMACRO_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"
#include "FreeRTOSConfig.h"

/* 数据类型重定义 */
#define portCHAR        char
#define portFLOAT       float
#define portDOUBLE      double
#define portLONG        long
#define portSHORT       short
#define portSTACK_TYPE  uint32_t
#define portBASE_TYPE   long

typedef portSTACK_TYPE  StackType_t;
typedef long            BaseType_t;
typedef unsigned long   UBaseType_t;

#if ( configUSE_16_BIT_TICKS == 1 )
typedef uint16_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffff
#else
typedef uint32_t TickType_t;
#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif

#define portNVIC_INT_CTRL_REG (*((volatile uint32_t*)0xe000ed04))
#define portNVIC_PENDSVSET_BIT (1UL << 28UL)

#define portSY_FULL_READ_WRITE (15)

#define portYIELD()\
{\
    /* 触发 PendSV, 产生上下文切换 */\
    portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
    __dsb(portSY_FULL_READ_WRITE);\
    __isb(portSY_FULL_READ_WRITE);\
}

#ifdef __cplusplus
}
#endif
#endif /* PORTMACRO_H */
