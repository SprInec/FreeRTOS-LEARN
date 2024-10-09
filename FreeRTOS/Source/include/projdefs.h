/**
 * @file projdefs.h
 * @author SprInec (JulyCub@163.com)
 * @brief 
 * @version 0.1
 * @date 2024.10.06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PROJDEFS_H
#define PROJDEFS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "portmacro.h"

typedef void (* TaskFunction_t)(void * arg);

#define pdFALSE ((BaseType_t)0)
#define pdTRUE ((BaseType_t)1)

#define pdPASS (pdTRUE)
#define pdFAIL (pdFALSE)

#ifdef __cplusplus
}
#endif
#endif /* PROJDEFS_H */
