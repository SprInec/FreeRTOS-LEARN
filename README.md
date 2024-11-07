# FreeRTOS-LEARN

FreeRTOS learning.

> 参考：[野火]《FreeRTOS 内核实现与应用开发实战指南》

## 一. 移植 FreeRTOS 到 CubeMX-STM32 工程

### 1. 下载 FreeRTOS 源码

在移植之前，我们首先要获取到FreeRTOS 的官方的源码包。

- 官网：http://www.freertos.org/
- 代码托管网站：https://sourceforge.net/projects/freertos/files/FreeRTOS/

我们选择 FreeRTOS 的版本 V9.0.0。

### 2. FreeRTOS 文件夹内容简介

FreeRTOS V9.0.0 解压缩后文件内容如下图所示，其中主要包含 `FreeRTOS` 和 `FreeRTOS-Plus` 这两个文件夹和一些 HTML 文件。

![image-20241030221053260](.assets/image-20241030221053260.png)

#### 2.1 FreeRTOS 文件夹

FreeRTOS 主要包含 Demo 例程和 Source 内核源码，我们就需要提取该目录下的大部分文件：

![image-20241030221349040](.assets/image-20241030221349040.png)

- FreeRTOS 文件夹下的 `Source` 文件夹里面包含的是 FreeRTOS 内核的源代码，我们移植 FreeRTOS 的时候就需要这部分源代码。

- FreeRTOS 文件夹下的 `Demo` 文件夹里面包含了 FreeRTOS 官方为各个单片机移植好的工程代码，这部分 Demo 非常有参考价值。我们把 FreeRTOS 到 STM32 的时候，`FreeRTOSConfig.h` 这个头文件就是从这里拷贝过来的。

##### 2.1.1 Source 文件夹

编号 ① 和 ③ 包含的是 FreeRTOS 的通用的头文件和 C 文件，这两部分的文件试用于各种编译器和处理器，是通用的。需要移植的头文件和 C 文件放在编号 ② `portblle` 这个文件夹。

![image-20241030221711565](.assets/image-20241030221711565.png)

我们打开 `portblle` 这个文件夹，可以看到里面很多与编译器相关的文件夹，在不同的
编译器中使用不同的支持文件。

![image-20241030221936268](.assets/image-20241030221936268.png)

- `Keil` : 文件夹里面存放的就是我们要使用的编译器 KEIL 所需要移植的文件，打开KEIL 文件夹的时候，你会看到一句话 `See-also-the-RVDS-directory.txt`，`Keil` 里面的内容跟 `RVDS` 里面的内容一样，所以我们只需要 `RVDS` 文件夹里面的内容即可。

- `MemMang` ：文件夹下存放的是跟内存管理相关的。

打开 RVDS 文件夹，下面包含了各种处理器相关的文件夹：

![image-20241030222311508](.assets/image-20241030222311508.png)

FreeRTOS 是一个软件，单片机是一个硬件，FreeRTOS 要想运行在一个单片机上面，它们就必须关联在一起，那么怎么关联？还是得通过写代码来关联，这部分关联的文件叫接口文件，通常由汇编和 C 联合编写。

这些接口文件都是跟硬件密切相关的，不同的硬件接口文件是不一样的，但都大同小异。编写这些接口文件的过程我们就叫移植，移植的过程通常由 FreeRTOS 和 MCU 原厂的人来负责，移植好的这些接口文件就放在 RVDS 这个文件夹的目录下。

以 `ARM_CM4F` 这个文件夹为例，看看里面的文件，里面只有 `port.c` 与 `portmacro.h` 两个文件，`port.c` 文件里面的内容是由 FreeRTOS 官方的技术人员为 Cortex-M4 内核的处理器写的接口文件，里面核心的上下文切换代码是由汇编语言编写而成，对技术员的要求比较高，刚开始学习的之后只需拷贝过来用即可。`portmacro.h` 则是 `port.c` 文件对应的头文件，主要是一些数据类型和宏定义。

![image-20241030223431386](.assets/image-20241030223431386.png)

`MemMang` 文件夹下存放的是跟内存管理相关的，总共有五个 `heap` 文件以及一个`readme` 说明文件，这五个 `heap` 文件在移植的时候必须使用一个，因为 FreeRTOS 在创建内核对象的时候使用的是动态分配内存，而这些动态内存分配的函数则在这几个文件里面实现，不同的分配算法会导致不同的效率与结果，后面在内存管理中会讲解每个文件的区别，由于现在是初学，所以我们选用 `heap4.c` 即可。

![image-20241030224436850](.assets/image-20241030224436850.png)

##### 2.1.2 Demo 文件夹

这个目录下内容就是 Deme 例程，我们可以直接打开里面的工程文件，各种开发平台
的完整 Demo，开发者可以方便的以此搭建出自己的项目，甚至直接使用。

![image-20241030230224786](.assets/image-20241030230224786.png)

##### 2.1.3 License 文件夹

这里面只有一个许可文件 `license.txt`，用 FreeRTOS 做产品的话就需要看看这个文件，但是我们是学习 FreeRTOS，所以暂时不需要理会这个文件。

![image-20241030230400628](.assets/image-20241030230400628.png)

#### 2.2 FreeRTOS-Plus 文件夹

FreeRTOS-Plus 文件夹里面包含的是第三方的产品，一般我们不需要使用，FreeRTOSPlus 的预配置演示项目组件（组件大多数都要收费），大多数演示项目都是在 Windows 环境中运行的，使用 FreeRTOS windows 模拟器，所以暂时不需要关注这个文件夹。

![image-20241030230528199](.assets/image-20241030230528199.png)

#### 2.3 HTML 文件

一些直接可以打开的网页文件，里面包含一些关于 FreeRTOS 的介绍，是 FreeRTOS 官方人员所写，所以都是英文的，有兴趣可以打开看看，具体相关内容可以看 HTML 文件名称。

### 3. 往 CubeMX 工程添加 FreeRTOS 源码

#### 3.1 提取 FreeRTOS 最简源码

提取后目录结构如下所示：

```c
└─FreeRTOS-Lite
   ├─include // 来自 FreeRTOS/Source/include/
   |  ├─croutine.h
   |  ├─deprecated_definitions.h
   |  ├─event_groups.h
   |  ├─FreeRTOS.h
   |  ├─list.h
   |  ├─mpu_prototypes.h
   |  ├─mpu_wrappers.h
   |  ├─portable.h
   |  ├─projdefs.h
   |  ├─queue.h
   |  ├─semphr.h
   |  ├─StackMacros.h
   |  ├─stdint.readme
   |  ├─task.h
   |  └─timers.h
   ├─port // 来自 FreeRTOS/Source/portable/
   │  ├─MemMang
   |      ├─heap_1.c
   |      ├─heap_2.c
   |      ├─heap_3.c
   |      ├─heap_4.c
   |      ├─heap_5.c
   │      └─ReadMe
   │  └─RVDS
   │      ├─ARM7_LPC21xx
   │      ├─ARM_CA9
   │      ├─ARM_CM0
   │      ├─ARM_CM3
   │      ├─ARM_CM4F
   │      ├─ARM_CM4_MPU
   │      └─ARM_CM7
   └─src // 来自 FreeRTOS/Source/
      ├─croutine.c
      ├─event_groups.c
      ├─list.c
      ├─queue.c
      ├─readme.txtc
      ├─tasks.c
      └─timers.c
```

#### 3.2 CubeMX 创建工程

这里使用 CubeMX 创建了一个 STM32F407 芯片的工程。

![image-20241101155829073](.assets/image-20241101155829073.png)

注意这里 SYS 的 Timebase Source 选择任意 TIM 时钟替换 SysTick，其余操作和配置裸机工程一致。

![image-20241101155859268](.assets/image-20241101155859268.png)

USART 可开可不开，开启后可以通过串口输出信息方便调试。

![image-20241101155931211](.assets/image-20241101155931211.png)

![image-20241101160029074](.assets/image-20241101160029074.png)

![image-20241101160104201](.assets/image-20241101160104201.png)

![image-20241101160127418](.assets/image-20241101160127418.png)

![image-20241101160201555](.assets/image-20241101160201555.png)

#### 3.3 拷贝 FreeRTOS 到工程根目录

将创建好的 `FreeRTOs-Lite` 文件夹直接整个复制到目标工程的根目录：

![image-20241031101459258](.assets/image-20241031101459258.png)

#### 3.4 拷贝 FreeRTOSConfig.h 文件到 FreeRTOS-Lite 文件夹

`FreeRTOSConfig.h` 文件是 FreeRTOS 的工程配置文件，因为 FreeRTOS 是可以裁剪的实时操作内核，应用于不同的处理器平台，用户可以通过修改这个 FreeRTOS 内核的配置头文件来裁剪 FreeRTOS 的功能，我们把它拷贝一份放在 `FreeRTOS-Lite` 这个文件夹下面。

打开 FreeRTOSv9.0.0 源码，在 `FreeRTOSv9.0.0\FreeRTOS\Demo` 文件夹下面找到 `CORTEX_STM32F103_Keil` 这个文件夹， 双击打开， 在其根目录下找到这个 `FreeRTOSConfig.h` 文件，然后拷贝到工程的 `FreeRTOS-Lite` 文件夹下即可。

![image-20241031154840804](.assets/image-20241031154840804.png)

#### 3.5 添加 FreeRTOS 源码到工程组文件夹

##### 3.5.1 新建 FreeRTOS/src 和 FreeRTOS/port 组

接下来我们在开发环境里面新建 `FreeRTOS/src` 和 `FreeRTOS/port` 两个组文件夹，其中 `FreeRTOS/src` 用于存放 `src` 文件夹的内容，`FreeRTOS/port` 用于存放 `port\MemMang` 文件夹与 `port\RVDS\ARM_CMx`文件夹的内容 `x`  表示3、4 或者7。

![image-20241031102954010](.assets/image-20241031102954010.png)

然后我们将工程文件中 FreeRTOS 的内容添加到工程中去，按照已经新建的分组添加我们的 FreeRTOS 工程源码。至此我们的 FreeRTOS 添加到工程中就已经完成，完成的效果具体见下图：

<img src=".assets/image-20241031103537618.png" alt="image-20241031103537618" style="zoom:50%;" />

##### 3.5.2 指定 FreeRTOS 头文件的路径

将 FreeRTOS 中包含头文件 (.h) 文件的路径添加到工程，如下图所示：

![image-20241031154726176](.assets/image-20241031154726176.png)

至此，FreeRTOS 的整体工程基本移植完毕，我们需要修改 FreeRTOS 配置文件，按照我们的需求来进行修改。

#### 3.6 修改 FreeRTOSConfig.h

`FreeRTOSConfig.h` 是直接从 `demo` 文件夹下面拷贝过来的，该头文件对裁剪整个FreeRTOS 所需的功能的宏均做了定义，有些宏定义被使能，有些宏定义被失能。

##### 3.6.1 FreeRTOSConfig.h 文件内容讲解

以下文件适配 **CMSIS Configuration Wizard**：

```c
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* LABEL 1 */
// 针对不同的编译器调用不同的 stdint.h 头文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif

/* LABEL 2 */
// 断言
#define vAssertCalled(char, int) printf("Error: %s, %d\r\n", char, int)
#define configASSERT(x) if ((x) == 0) vAssertCalled(__FILE__, __LINE__)

// <<< Use Configuration Wizard in Context Menu >>>

// <h> FreeRTOS 基础配置配置选项
// =====================================================================
/* LABEL 3 */
// <o> RTOS 调度器类型
//     <0=> [0] 协作式调度器(时间片)
//     <1=> [1] 抢占式调度器
// <i> 在多任务管理机制上，操作系统可以分为抢占式和协作式两种。
// <i> 协作式操作系统是任务主动释放 CPU 后，切换到下一个任务。
#define configUSE_PREEMPTION 1
// </e> !RTOS 调度器类型

/* LABEL 4 */
// <e> 时间片调度
// <i> 默认: 1 (Enable)
// <i> 时间片调度是指当一个任务运行一段时间后，操作系统会自动切换到下一个任务。
// <i> 时间片的大小由 configTICK_RATE_HZ 决定。
#define configUSE_TIME_SLICING 1
// </e> !时间片调度

/* LABEL 5 */
// <o> 下一任务执行选择方法
//     <0=> [0] 通用方法
//     <1=> [1] 特殊方法
// <i> 选择下一任务执行的方法, 0 为通用方法, 1 为特殊方法
// <i>
// <i> 某些运行 FreeRTOS 的硬件有两种方法选择下一个要执行的任务
// <i> 通用方法和特定于硬件的方法, (以下简称 " 特殊方法 " )
// <i> 
// <i> 通用方法:
// <i>   1. configUSE_PORT_OPTIMISED_TASK_SELECTION 为 0 或硬件不支持
// <i>      这种特殊方法
// <i>   2. 可以用于所有 FreeRTOS 支持的硬件
// <i>   3. 完全用 C 实现, 效率略低于特殊方法
// <i>   4. 不强制要求限制最大可用优先级数目
// <i> 
// <i> 特殊方法:
// <i>   1. 必须将 configUSE_PORT_OPTIMISED_TASK_SELECTION 设置为 1
// <i>   2. 依赖一个或多个特定架构的汇编指令 (一般是类似计算机前导零[CLZ])
// <i>      指令)
// <i>   3. 比通用方法更高效
// <i>   4. 一般强制限定最大可用优先级数目为 32
// <i> 
// <i> 一般是硬件计算前导指令, 如果所使用的 MCU 没有这些硬件指令的话此宏应
// <i> 设置为 0!
// <i>
// <i> 选择特殊方法可以提高 FreeRTOS 在特定硬件上的运行效率, 但也会增加代码量
// <i> 因此, 请根据具体硬件情况选择合适的方法!
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1

/* LABEL 6 */
// <e> 低功耗 tickless 模式
// <i> 0 Disable: 保持系统节拍 (tick) 中断一直运行
// <i> 1 Enable: 低功耗 tickless 模式
// <i> 低功耗 tickless 模式可以减少系统的功耗, 但也会降低系统的响应速度
// <i> 建议在系统空闲时使用低功耗 tickless 模式, 避免系统进入低功耗模式
#define configUSE_TICKLESS_IDLE 1
// </e> !低功耗 tickless 模式

/* LABEL 7 */
// <o> CPU 内核时钟频率
// <i> 无需修改, 自动获取
// <i> 默认: SystemCoreClock (自动获取)
// <i> 写入实际的 CPU 内核时钟频率, 即 CPU 指令执行频率, 通常为 Fclk
// <i> Fclk 为供给 CPU 内核的时钟信号, 我们所说的 CPU 主频为 XX MHZ
// <i> 就是指的这个时钟信号, 相应的, 1/Fclk 就是 CPU 时钟周期 Tclk
#define configCPU_CLOCK_HZ (SystemCoreClock)

/* LABEL 8 */
// <o> RTOS 系统节拍中断频率 (Hz)
// <i> 即一秒钟中断的次数, 每次中断 RTOS 都会进任务调度
#define configTICK_RATE_HZ ((TickType_t)1000)

/* LABEL 9 */
// <o> 可使用的最大优先级
#define configMAX_PRIORITIES (32)

/* LABEL 10 */
// <o> 空闲任务使用的堆栈大小
#define configMINIMAL_STACK_SIZE ((unsigned short )128)

/* LABEL 11 */
// <o> 任务名字字符串长度
#define configMAX_TASK_NAME_LEN	(16)

/* LABEL 12 */
// <o> 系统节拍计数器变量数据类型
//     <0=> [0] 32-bit int
//     <1=> [1] 16-bit int
// <i> TickType_t 变量用于表示系统节拍计数器的计数值
#define configUSE_16_BIT_TICKS 0

/* LABEL 13 */
// <e> 空闲任务放弃 CPU 使用权给其他同优先级的用户任务
#define configIDLE_SHOULD_YIELD	1
// </e> !空闲任务放弃 CPU 使用权给其他同优先级的用户任务

/* LABEL 14 */
// <e> 启用队列
#define configUSE_QUEUE_SETS 1
// </e> !启用队列

/* LABEL 15 */
// <e> 使能任务通知功能
#define configUSE_TASK_NOTIFICATIONS 1
// </e> !使能任务通知功能

// <e> 使能互斥信号量
#define configUSE_MUTEXES 1
// </e> !使能互斥信号量

// <e> 使能递归互斥信号量
#define configUSE_RECURSIVE_MUTEXES	1
// </e> !使能递归互斥信号量

// <e> 使能计数信号量
#define configUSE_COUNTING_SEMAPHORES 1
// </e> !使能计数信号量

/* LABEL 16 */
// <o> 可注册的信号量和消息队列个数
#define configQUEUE_REGISTRY_SIZE 10

// <e> 设置任务标签功能
#define configUSE_APPLICAITON_TASK_TAG 0
// </e> !设置任务标签功能
// </h> !FreeRTOS 基础配置配置选项


// <h> FreeRTOS 与内存申请有关配置选项
// =====================================================================
/* LABEL 17 */
// <e> 动态内存申请
#define configSUPPORT_DYNAMIC_ALLOCATION 1
// </e> !动态内存申请

// <e> 静态内存分配
#define configSUPPORT_STATIC_ALLOCATION	 1
// </e> !静态内存分配

/* LABEL 18 */
// <o> 系统总的堆大小
// <i> 单位: KB (1024 bytes)
// <i> 默认: 36KB (36 * 1024)
#define configTOTAL_HEAP_SIZE ( (size_t)( 36 * 1024 ) )
// </h> !FreeRTOS 与内存申请有关配置选项

// <h> FreeRTOS 与钩子函数有关的配置选项
// =====================================================================
/* LABEL 19 */
// <e> 使能空闲钩子
// <i> 0 Disable: 忽略空闲钩子
// <i> 1 Enable: 使用空闲钩子
// <i> 空闲钩子是一个函数, 该函数由用户来实现
// <i> FreeRTOS 规定了函数的名字和参数: void vApplicationIdleHook(void)
// <i> 这个函数在每个空闲任务周器都会被调用
// <i> 对于已经删除的 RTOS 任务, 空闲任务可以释放分配给他们的堆栈内存
// <i> 因此必须保证空闲任务可以被 CPU 执行
// <i> 使用空闲钩子函数设置 CPU 进入省电模式是很常见的
// <i> 不可以调用会引起空闲任务阻塞的 API 函数
#define configUSE_IDLE_HOOK	1
// </e> !使能空闲钩子

/* LABEL 20 */
// <e> 使能时间片钩子
// <i> 0 Disable: 忽略时间片钩子
// <i> 1 Enable: 使用时间片钩子
// <i> 时间片钩子是一个函数, 该函数由用户来实现
// <i> FreeRTOS 规定了函数的名字和参数: void vApplicationTickHook(void)
// <i> 时间片中断可以周期性的调用
// <i> 函数必须非常短小, 不能大量使用堆栈
// <i> 不能调用以 "FromISR" 或 "FROM_ISR" 结尾的 API 函数
#define configUSE_TICK_HOOK	0
// </e> !使能时间片钩子

// <e> 使能内存申请失败钩子
#define configUSE_MALLOC_FAILED_HOOK	0
// </e> !使能内存申请失败钩子

/* LABEL 21 */
// <o> 栈溢出检测方式
//     <0=> [0] 不启用
//     <1=> [1] 栈顶指针检测
//     <2=> [2] 栈底指针检测
// <i> 若使用此功能: 用户必须提供一个栈溢出钩子函数
#define configCHECK_FOR_STACK_OVERFLOW 0
// </h> !FreeRTOS 与钩子函数有关的配置选项

// <h> FreeRTOS 与运行时间和任务状态收集有关的配置选项
// =====================================================================
// <e> 运行时间统计功能
#define configGENERATE_RUN_TIME_STATS 0
// </e> !运行时间统计功能

/* 启用可视化跟踪调试 */

// <e> 可视化跟踪调试
#define configUSE_TRACE_FACILITY 0
// </e> !可视化跟踪调试

// <e> 统计格式化函数
// <i> 与运行时间统计功能同时使能时启用任务跟踪功能
// <i> 启用后会编译下面 3 个函数:
// <i> - prvWriteNametoBuffer()
// <i> - vTaskList()
// <i> - vTaskGetRunTimeStats()
#define configUSE_STATS_FORMATTING_FUNCTIONS 1
// </e> !统计格式化函数
// </h> !FreeRTOS 与运行时间和任务状态收集有关的配置选项

// <h> FreeRTOS 与协程有关的配置选项
// =====================================================================
/* LABEL 22 */
// <e> 使能协程功能
// <i> 启用后必须添加文件 croutine.c 
#define configUSE_CO_ROUTINES 0
// </e> !使能协程功能

/* LABEL 23 */ 
// <o> 最大协程优先级数目
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )
// </h> !FreeRTOS 与协程有关的配置选项

// <h> FreeRTOS 与软件定时器有关的配置选项
// =====================================================================
// <e> 使能软件定时器
#define configUSE_TIMERS 1
// </e> !使能软件定时器

/* LABEL 24 */
// <o> 软件定时器优先级
// <i> 此处配置的数值是最大优先级减去的数值
// <i> 默认: 1 (configMAX_PRIORITIES - 1)
#define configTIMER_TASK_PRIORITY	( configMAX_PRIORITIES - 1 )

/* LABEL 25 */
// <o> 软件定时器队列长度
#define configTIMER_QUEUE_LENGTH	10

// <o> 软件定时器任务栈大小
// <i> 默认: 2 (2 * configMINIMAL_STACK_SIZE(空闲任务使用的堆栈大小))
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )
// </h> !FreeRTOS 与软件定时器有关的配置选项

// <h> FreeRTOS 可选函数配置选项
// =====================================================================
/* LABEL 26 */
// <e> INCLUDE_xTaskGetSchedulerState
#define INCLUDE_xTaskGetSchedulerState  1
// </e> !INCLUDE_xTaskGetSchedulerState

/* LABEL 27 */
// <e> INCLUDE_vTaskPrioritySet
#define INCLUDE_vTaskPrioritySet		1
// </e> !INCLUDE_vTaskPrioritySet

/* LABEL 28 */
// <e> INCLUDE_uxTaskPriorityGet
#define INCLUDE_uxTaskPriorityGet		1
// </e> !INCLUDE_uxTaskPriorityGet

/* LABEL 29 */
// <e> INCLUDE_vTaskDelete
#define INCLUDE_vTaskDelete				1
// </e> !INCLUDE_vTaskDelete


// <e> INCLUDE_vTaskCleanUpResources
#define INCLUDE_vTaskCleanUpResources	0
// </e> !INCLUDE_vTaskCleanUpResources


// <e> INCLUDE_vTaskSuspend
#define INCLUDE_vTaskSuspend			1
// </e> !INCLUDE_vTaskSuspend


// <e> INCLUDE_vTaskDelayUntil
#define INCLUDE_vTaskDelayUntil			1
// </e> !INCLUDE_vTaskDelayUntil


// <e> INCLUDE_vTaskDelay
#define INCLUDE_vTaskDelay				1
// </e> !INCLUDE_vTaskDelay


// <e> INCLUDE_eTaskGetState
#define INCLUDE_eTaskGetState			1
// </e> !INCLUDE_eTaskGetState

// <e> INCLUDE_xTimerPendFunctionCall
#define INCLUDE_xTimerPendFunctionCall	1
// </e> !INCLUDE_xTimerPendFunctionCall
// </h> !FreeRTOS 可选函数配置选项

// <h> FreeRTOS 与中断有关的配置选项
// =====================================================================
/* LABEL 30 */
#ifdef __NVIC_PRIO_BITS
#if (__NVIC_PRIO_BITS == 4)
#define configPRIO_BITS 4
#elif (__NVIC_PRIO_BITS == 3)
#define configPRIO_BITS 5
#else
/* LABEL 31 */
// <o> 中断优先级位数
#define configPRIO_BITS 4
#endif // (__NVIC_PRIO_BITS == 4)
#endif // __NVIC_PRIO_BITS

/* LABEL 32 */
// <o> 中断最低优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY	15

/* LABEL 33 */
// <o> 系统可管理的最高中断优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5
// </h> !FreeRTOS 与中断有关的配置选项

/* LABEL 34 */
#define configKERNEL_INTERRUPT_PRIORITY \
    (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << configPRIO_BITS)
/* LABEL 35 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << configPRIO_BITS)

// <h> FreeRTOS 与中断服务函数有关的配置选项
// =====================================================================
/* LABEL 36 */
#if (configUSE_TRACE_FACILITY == 1)
#include "trcRecorder.h"
// <e> 启用被 Trace 源码调用的可选函数
#define INCLUDE_xTaskGetCurrentTaskHandle 1
// </e> !启用被 Trace 源码调用的可选函数
#endif
// </h> !FreeRTOS 与中断服务函数有关的配置选项

// <<< end of configuration section >>>
#endif /* FREERTOS_CONFIG_H */
```

- LABEL 1 ：针对不同的编译器调用不同的 `stdint.h` 文件，在 MDK 中，我们默认的是 `__CC_ARM`。

- LABEL 2 ：**断言**简介：在使用 C 语言编写工程代码时，我们总会对某种假设条件进行检查，断言就是用于在代码中捕捉这些假设，可以将断言看作是异常处理的一种高级形式。断言表示为一些布尔表达式，程序员相信在程序中的某个特定表达式值为真。可以在任何时候启用和禁用断言验证，因此可以在测试时启用断言，而在发布时禁用断言。同样，程序投入运行后，最终用户在遇到问题时可以重新启用断言。它可以快速发现并定位软件问题，同时对系统错误进行自动报警。断言可以对在系统中隐藏很深，用其它手段极难发现的问题可以用断言来进行定位，从而缩短软件问题定位时间，提高系统的可测性。实际应用时，可根据具体情况灵活地设计断言。这里只是使用宏定义实现了断言的功能，断言作用很大，特别是在调试的时候，而 FreeRTOS 中使用了很多断言接口 `configASSERT`，所以我们需要实现断言，把错误信息打印出来从而在调试中快速定位，打印信息的内容是xxx 文件 xxx 行 `( __FILE__, __LINE__)`。

- LABEL 3 ：置1：FreeRTOS 使用抢占式调度器；置0：FreeRTOS 使用协作式调度器（时间片）。

    - **抢占式调度**：在这种调度方式中，系统总是选择优先级最高的任务进行调度，并且 一旦高优先级的任务准备就绪之后，它就会马上被调度而不等待低优先级的任务主动放弃CPU，高优先级的任务抢占了低优先级任务的CPU 使用权，这就是抢占，在实际操作系统中，这样子的方式往往是最适用的。
    - **协作式调度**：是由任务主动放弃CPU，然后才进行任务调度。

    在多任务管理机制上，操作系统可以分为抢占式和协作式两种。 协作式操作系
    统是任务主动释放 CPU后，切换到下一个任务。任务切换的时机完全取决于正在运行的任务。

- LABEL 4 ：使能时间片调度(默认式使能的)。当优先级相同的时候，就会采用时间片调度，这意味着 RTOS 调度器总是运行处于最高优先级的就绪任务，在每个 FreeRTOS 系统节拍中断时在相同优先级的多个任务间进行任务切换。如果宏 `configUSE_TIME_SLICING` 设置为 0，FreeRTOS 调度器仍然总是运行处于最高优先级的就绪任务，但是当 RTOS 系统节拍中断发生时，相同优先级的多个任务之间不再进行任务切换，而是在执行完高优先级的任务之后才进行任务切换。一般来说，FreeRTOS 默认支持 32 个优先级，很少情况会把 32 个优先级全用完，所以，官方建议采用抢占式调度。

- LABEL 5 ：FreeRTOS 支持两种方法选择下一个要执行的任务：一个是软件方法扫描就绪链表， 这种方法我们通常称为通用方法，`configUSE_PORT_OPTIMISED_TASK_SELECTION` 为 0 或者硬件不支持特殊方法，才使用通用方法获取下一个即将运行的任务，通用方法可以用于所有 FreeRTOS 支持的硬件平台，因为这种方法是完全用 C 语言实现，所以效率略低于特殊方法，但不强制要求限制最大可用优先级数目； 另一个是硬件方式查找下一个要运行的任务， 必须将`configUSE_PORT_OPTIMISED_TASK_SELECTION` 设置为 1，因为是必须依赖一个或多个特定架构的汇编指令（一般是类似计算前导零 [CLZ] 指令，在M3、M4、M7 内核中都有，这个指令是用来计算一个变量从最高位开始的连续零的个数），所以效率略高于通用方法，但受限于硬件平台，一般强制限定最大可用优先级数目为 32，这也是FreeRTOS 官方为什么推荐使用 32 位优先级的原因。

- LABEL 6 ：低功耗 tickless 模式。

    - 置 1：使能低功耗 tickless 模式；
    - 置 0：保持系统节拍（tick）中断一直运行

    如果不是用于低功耗场景，我们一般置 0 即可。

- LABEL 7 ：配置 CPU 内核时钟频率，也就是 CPU 指令执行频率，通常称为 Fclk ， Fclk 为供给 CPU 内核的时钟信号，我们所说的 cpu 主频为 XX MHz，就是指的这个时钟信号，相应的，1/Fclk 即为 CPU 时钟周期。
- LABEL 8 ：FreeRTOS 系统节拍中断的频率。表示操作系统每 1 秒钟产生多少个tick，tick 即是操作系统节拍的时钟周期，时钟节拍就是系统以固定的频率产生中断（时基中断），并在中断中处理与时间相关的事件，推动所有任务向前运行。时钟节拍需要依赖于硬件定时器，在 STM32 裸机程序中经常使用的 SysTick 时钟是 MCU 的内核定时器， 通常都使用该定时器产生操作系统的时钟节拍。在FreeRTOS 中，系统延时和阻塞时间都是以 tick 为单位，配置、`configTICK_RATE_HZ` 的值可以改变中断的频率，从而间接改变了 FreeRTOS 的时钟周期（T=1/f）。我们设置为1000，那么FreeRTOS 的时钟周期为 1ms，过高的系统节拍中断频率也意味着FreeRTOS 内核占用更多的 CPU 时间，因此会降低效率，一般配置为 100~1000 即可。
- LABEL 9 ：可使用的最大优先级，默认为 32 即可，官方推荐的也是 32。每一个任务都必须被分配一个优先级，优先级值从 0~ （configMAX_PRIORITIES - 1）之间。**低优先级数值表示低优先级任务**。空闲任务的优先级为 0（tskIDLE_PRIORITY），因此它是最低优先级任务。FreeRTOS 调度器将确保处于就绪态的高优先级任务比同样处于就绪状态的低优先级任务优先获取处理器时间。换句话说，**FreeRTOS 运行的永远是处于就绪态的高优先级任务。处于就绪状态的相同优先级任务使用时间片调度机制共享处理器时间。**

- LABEL 10 ：空闲任务默认使用的堆栈大小，默认为 128 字即可（在M3、M4、
    M7 中为128*4 字节），堆栈大小不是以字节为单位而是以字为单位的，比如在32 位架构下，栈大小为 100 表示栈内存占用 400 字节的空间。

- LABEL 11 ： 任务名字字符串长度，这个宏用来定义该字符串的最大长度。这里定义的长度包括字符串结束符 `\0`。

- LABEL 12 ：系统节拍计数器变量数据类型，1 表示为 16 位无符号整形，0 表示为 32 位无符号整形，STM32 是 32 位机器，所以默认使用为0 即可，这个值位数的大小决定了能计算多少个 tick，比如假设系统以 1ms 产生一个 tick 中断的频率计时，那么 32 位无符号整形的值则可以计算 4294967295 个 tick，也就是系统从 0 运行到 4294967.295 秒的时候才溢出，转换为小时的话，则能运行1193 个小时左右才溢出，当然，溢出就会重置时间，这点完全不用担心；而假如使用 16 位无符号整形的值，只能计算 65535 个 tick，在 65.535 秒之后就会溢出，然后重置。

- LABEL 13 ：控制任务在空闲优先级中的行为，空闲任务放弃 CPU 使用权给其他同优先级的用户任务。仅在满足下列条件后，才会起作用：

    1. 启用抢占式调度；

    2. 用户任务优先级与空闲任务优先级相等。

    一般不建议使用这个功能，能避免尽量避免。

    1. 设置用户任务优先级比空闲任务优先级高

    2. 这个宏定义配置为 0。

- LABEL 14 ：启用消息队列，消息队列是 FreeRTOS 的 IPC 通信的一种，用于
    传递消息。

- LABEL 15 ：开启任务通知功能，默认开启。每个 FreeRTOS 任务具有一个 32位的通知值，FreeRTOS 任务通知是直接向任务发送一个事件，并且接收任务的通知值是可以选择的，任务通过接收到的任务通知值来解除任务的阻塞状态（假如因等待该任务通知而进入阻塞状态）。相对于队列、二进制信号量、计数信号量或事件组等 IPC 通信，使用任务通知显然更灵活。官方说明：相比于使用信号量解除任务阻塞，使用任务通知可以快 45%（使用 GCC编译器，-o2 优化级别），并且使用更少的 RAM。

    > FreeRTOS 官方说明：Unblocking an RTOS task with a direct notification is 45% fasterand uses less RAM than unblocking a task with a binary semaphore.

- LABEL 16 ：设置可以注册的信号量和消息队列个数，用户可以根据自己需要
    修改即可，RAM 小的芯片尽量裁剪得小一些。、
- LABEL 17 ：支持动态分配申请，一般在系统中采用的内存分配都是动态内存
    分配。FreeRTOS 同时也支持静态分配内存，但是常用的就是动态分配了。
- LABEL 18 ：FreeRTOS 内核总计可用的有效的 RAM 大小，不能超过芯片的RAM 大小，一般来说用户可用的内存大小会小于 `configTOTAL_HEAP_SIZE` 定义的大小，因为系统本身就需要内存。每当创建任务、队列、互斥量、软件定时器或信号量时，FreeRTOS 内核会为这些内核对象分配 RAM ， 这里的 RAM 都属于 `configTOTAL_HEAP_SIZE` 指定的内存区。

- LABEL 19 ：配置空闲钩子函数，钩子函数是类似一种回调函数，在任务执行到某个点的时候，跳转到对应的钩子函数执行，这个宏定义表示是否启用空闲任务钩子函数， 这个函数由用户来实现， 但是 FreeRTOS 规定了函数的名字和参数： `void vApplicationIdleHook(void)`，我们自定义的钩子函数不允许出现阻塞的情况。
- LABEL 20 ：配置时间片钩子函数，与空闲任务钩子函数一样。这个宏定义表示是否启用时间片钩子函数，这个函数由用户来实现，但是 FreeRTOS 规定了函数的名字和参数：`void vApplicationTickHook(void)`，我们自定义的钩子函数不允许出现阻塞的情况。同时需要知道的是 `xTaskIncrementTick` 函数在`xPortSysTickHandler` 中断函数中被调用的。因此，`vApplicationTickHook()`函数执行的时间必须很短才行，同时不能调用任何不是以 `FromISR` 或 `FROM_ISR` 结尾的API 函数。

- LABEL 21 ：这个宏定义大于 0 时启用堆栈溢出检测功能，如果使用此功能，用户必须提供一个栈溢出钩子函数，如果使用的话，此值可以为 1 或者 2，因为有两种栈溢出检测方法。使用该功能，可以分析是否有内存越界的情况。
- LABEL 22 ：启用协程，启用协程以后必须添加文件 `croutine.c`，默认不使用，因为 FreeRTOS 不对协程做支持了。
- LABEL 23 ：协程的有效优先级数目，当 `configUSE_CO_ROUTINES` 这个宏定义有效的时候才有效，默认即可。
- LABEL 24 ：配置软件定时器任务优先级为最高优先级（`configMAX_PRIORITIES-1`) 。
- LABEL 25 ：软件定时器队列长度，也就是允许配置多少个软件定时器的数量，其实 FreeRTOS 中理论上能配置无数个软件定时器，因为软件定时器是不基于硬件的。
- LABEL 26 ：必须将 `INCLUDE_XTaskGetSchedulerState` 这个宏定义设置为 1 才能使用 `xTaskGetSchedulerState()` 这个 API 函数接口。

- LABEL 27：`INCLUDE_VTaskPrioritySet` 这个宏定义必须设置为 1 才能使 `vTaskPrioritySet()` 这个 API 函数接口。
- LABEL 28 ：`INCLUDE_uxTaskPriorityGet` 这个宏定义必须设置为 1 才能使`uxTaskPriorityGet()` 这个 API 函数接口。
- LABEL 29 ：`INCLUDE_vTaskDelete` 这个宏定义必须设置为 1 才能使`vTaskDelete()` 这个 API 函数接口。其他都是可选的宏定义，根据需要自定义即可。
- LABEL 30 ：定义 `__NVIC_PRIO_BITS` 表示配置 FreeRTOS 使用多少位作为中断优先级，在 STM32 中使用 4 位作为中断的优先级。
- LABEL 31 ：如果没有定义，那么默认就是4 位。

- LABEL 32 ：配置中断最低优先级是 15 （ 一般配置为15 ） 。`configLIBRARY_LOWEST_INTERRUPT_PRIORITY` 是用于配置 `SysTick` 与`PendSV` 的。

    注意了：这里是中断优先级，**中断优先级的数值越小，优先级越高**。而FreeRTOS 的任务优先级是，**任务优先级数值越小，任务优先级越低**。

- LABEL 33 ：配置系统可管理的最高中断优先级为 5 ，`configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY` 是用于配置 `basepri` 寄存器的，当 `basepri` 设置为某个值的时候，会让系统不响应比该优先级低的中断，而优先级比之更高的中断则不受影响。就是说当这个宏定义配置为 5 的时候，中断优先级数值在0、1、2、3、4 的这些中断是不受 FreeRTOS 管理的，不可被屏蔽，也不能调用 FreeRTOS 中的 API 函数接口，而中断优先级在 5 到 15 的这些中断是受到系统管理，可以被屏蔽的。

- LABEL 34 ：对需要配置的 `SysTick` 与 `PendSV` 进行偏移（因为是高4 位才有效），在 `port.c` 中会用到 `configKERNEL_INTERRUPT_PRIORITY` 这个宏定义来配置 `SCB_SHPR3`（ 系统处理优先级寄存器，地址为：0xE000 ED20），具体见下图。

    ![image-20241031151211814](.assets/image-20241031151211814.png)

- LABEL 35 ：`configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY` 是用于配置`basepri` 寄存器的，让 FreeRTOS 屏蔽优先级数值大于这个宏定义的中断（数值越大，优先级越低），而 `basepri` 的有效位为高 4 位，所以需要进行偏移，因为 STM32 只使用了优先级寄存器中的 4 位，所以要以最高有效位对齐，具体见下图。

    还需要注意的是：中断优先级 0（具有最高的逻辑优先级）不能被 `basepri` 寄存器屏蔽，因此，`configMAX_SYSCALL_INTERRUPT_PRIORITY`  绝不可以设置成 0。

    ![image-20241031151451285](.assets/image-20241031151451285.png)

    **为什么要屏蔽中断?**

    先了解一下什么是临界段！**临界段用一句话概括就是一段在执行的时候不能被中断的代码段。**在 FreeRTOS 里面，这个临界段最常出现的就是对全局变量的操作，全局变量就好像是一个枪把子，谁都可以对他开枪，但是我开枪的时候，你就不能开枪，否则就不知道是谁命中了靶子。

    **那么什么情况下临界段会被打断？一个是系统调度，还有一个就是外部中断。**在FreeRTOS 中，系统调度，最终也是产生 `PendSV` 中断，在 `PendSV Handler` 里面实现任务的切换，所以还是可以归结为中断。既然这样，FreeRTOS 对临界段的保护就很有必要了，在必要的时候将中断屏蔽掉，但是又必须保证某些特别紧急的中断的处理，比如像无人机的碰撞检测。

    `PRIMASK` 和 `FAULTMAST` 是Cortex-M 内核里面三个中断屏蔽寄存器中的两个，还有一个是 `BASEPRI`，有关这三个寄存器的详细用法见下面表格 ：

    |   名字    | 功能描述                                                     |
    | :-------: | ------------------------------------------------------------ |
    |  PRIMASK  | 这是个只有单一比特的寄存器。 在它被置 1 后，就关掉所有可屏蔽的异常，只剩下 NMI 和硬 FAULT 可以响应。它的缺省值是 0，表示没有关中断。 |
    | FAULTMASK | 这是个只有 1 个位的寄存器。当它置 1 时，只有 NMI 才能响应，所有其它的异常，甚至是硬 FAULT，也通通闭嘴。它的缺省值也是 0，表示没有关异常。 |
    |  BASEPRI  | 这个寄存器最多有 9 位（ 由表达优先级的位数决定）。它定义了被屏蔽优先级的阈值。当它被设成某个值后，所有优先级号大于等于此值的中断都被关（优先级号越大，优先级越低）。但若被设成 0，则不关闭任何中断， 0 也是缺省值。 |

- LABEL 36 ：`configUSE_TRACE_FACILITY` 这个宏定义是用于FreeRTOS 可视化调试软件 `Tracealyzer` 需要的东西， 我们现在暂时不需要， 将`configUSE_TRACE_FACILITY` 定义为 0 即可。

##### 3.6.2 FreeRTOSConfig.h 文件修改

`FreeRTOSConfig.h` 头文件的内容修改的不多，具体是：

- 修改与对应 MCU 的头文件，如果是使用 STM32F4 的MCU，则包含F4 的头文件  `#include "stm32f40x.h"`
- 包含串口的头文件 `“bsp_usart.h”`，因为在 `FreeRTOSConfig.h` 中实现了断言操作，需要打印一些信息。

其他根据需求修改即可。

> **NOTE** 
>
> 虽然 FreeRTOS 中默认是打开很多宏定义的，但是用户还是要根据需要选择打开与关闭，因为这样子的系统会更适合用户需要，更严谨与更加节省系统源。

```c
// <o> MCU 型号选择
//     <0=> STM32 F1
//     <1=> STM32 F4
//     <2=> STM32 H7
// <i> 选择所使用的 MCU 型号
#define configMCU_TYPE 1

#if configMCU_TYPE == 0
#include "stm32f1xx_hal.h"
#elif configMCU_TYPE == 1
#include "stm32f4xx_hal.h"
#elif configMCU_TYPE == 2
#include "stm32h7xx_hal.h"
#endif

// <e> 开启断言
#define configUSE_ASSERT_INFO 0
// </e> !开启断言

#if configUSE_ASSERT_INFO == 1
#include "bsp_usart.h"
#define vAssertCalled(char, int) printf("Error: %s, %d\r\n", char, int)
#define configASSERT(x) if ((x) == 0) vAssertCalled(__FILE__, __LINE__)
#endif
```

##### 3.7 修改 stm32f40x_it.c

`SysTick` 中断服务函数是一个非常重要的函数，FreeRTOS 所有跟时间相关的事情都在里面处理，`SysTick` 就是 FreeRTOS 的一个心跳时钟，驱动着 FreeRTOS 的运行，就像人的心跳一样，假如没有心跳，我们就相当于“死了”，同样的，FreeRTOS 没有了心跳，那么它就会卡死在某个地方，不能进行任务调度，不能运行任何的东西，因此我们需要实现一个 FreeRTOS 的心跳时钟，FreeRTOS 帮我们实现 `SysTick` 的启动的配置：在 `port.c` 文件中已经实现 `vPortSetupTimerInterrupt()` 函数，并且 FreeRTOS 通用的 `SysTick` 中断服务函数也实现了：在 `port.c` 文件中已经实现 `xPortSysTickHandler()` 函数，所以移植的时候只需要我们在 `stm32f40x_it.c` 文件中**实现对应（STM32）平台上的 `SysTick_Handler()` 函数**（最简单的办法就是直接在 `SysTick_Handler` 中引用 `xPortSysTickHandler` 函数）即可。

FreeRTOS 为开发者考虑得特别多，`PendSV_Handler()` 与 `SVC_Handler()` 这两个很重要的函数都帮我们实现了，在 `port.c` 文件中已经实 `xPortPendSVHandler()` 与 `vPortSVCHandler()` 函数， 防止我们自己实现不了， 那么在 `stm32f40x_it.c`  中就需要我们**在 `PendSV_Handler()` 与`SVC_Handler()` 这两个函数中引用`xPortPendSVHandler()` 与 `vPortSVCHandler()`** 。

```c
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim7;

/* USER CODE BEGIN EV */
extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);
extern void vPortSVCHandler(void);
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */
	vPortSVCHandler();
  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */
	
  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */
    xPortPendSVHandler();
  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}


/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
#if (INCLUDE_xTaskGetSchedulerState == 1 )
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
#endif /* INCLUDE_xTaskGetSchedulerState */
		xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
		}
#endif /* INCLUDE_xTaskGetSchedulerState */
  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */

  /* USER CODE END TIM7_IRQn 0 */
  HAL_TIM_IRQHandler(&htim7);
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}c

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

```

至此，我们的 FreeRTOS 基本移植完成。

##### 3.8 修改 main.c

在 `main.c` 文件中引入头文件 `FreeRTOS.h` 和 `task.h` ： 

```c
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
```

##### 3.9 编译

>  :triangular_flag_on_post: 注意：所用的编译器版本为 AC5，若用 AC6 编译器则存在汇编与 C 混合编写语法上的更改，直接编译会报错，需要针对 `port.c` 及`portmacro.h` 中涉及 `__asm` , `__dsb`, `__isb`的代码进行语法修改。

将程序编译好，验证无误即可。

## 二. 创建任务

> 后续的工程中的相关 BSP 直接使用了 *[libstm](https://github.com/SprInec/libstm)* 库的代码。
>
> 该文档编写时所用的开发板为：*大越创新 DevEBox-STM32F407VET6 开发板*

### 1. 硬件初始化

由 CubeMX 生成的工程代码中，`main.c` 的 `main` 函数中已经包含了相关配置的硬件初始化，我们在此基础上添加 BSP 相关的初始化代码即可，这里以 LED 闪烁实验进行测试：

```c
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_config.h"  // libstm 库的配置头文件
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  BSP_LED_Init();  // libstm 库中 LED 的初始化代码
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

```

主要是添加了 libstm 库的配置头文件 `bsp_config.h` 以及 libstm 库中 LED 的初始化函数 `BSP_LED_Init()`。

> libstm 库的 `bsp_config.h` 头文件同样兼容了 CMSIS Configuration Wizard，支持可视化配置，在 `bsp_config.h` 的 RTOS 选项中选择 FreeRTOS 即可自动包含 FreeRTOS 的相关头文件。 

执行到 `BSP_LED_Init()` 函数的时候，操作系统完全都还没有涉及到，即 `BSP_LED_Init()` 函数所做的工作跟我们以前编写的裸机工程里面的硬件初始化工作是一致的。可以简单理解为在创建任务并开启调度器之前，都处于裸机编程的范畴，在此阶段我们可以使用裸机开发的相关经验，对硬件初始化及一些相关的配置进行串口输出或者 LED，LCD 等方式的调试。

### 2. 创建单任务 - SRAM 静态内存

 这里，我们创建一个单任务，任务使用的栈和任务控制块都使用静态内存，即预先定义好的全局变量，这些预先定义好的全局变量都存在内部的 SRAM 中。

#### 2.1 定义任务函数

任务实际上就是一个无限循环且不带返回值的 C 函数。目前，我们创建一个这样的任务，让开发板上面的 LED 灯以100ms 的频率闪烁：

```c
void LED_Task(void *parameter)
{
    while(1)
    {
        __BSP_LED2_Toggle();
        vTaskDelay(pdMS_TO_TICKS(100));
        __BSP_LED2_Toggle();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
```

- 任务必须是一个死循环，否则任务将通过 LR 返回，如果 LR 指向了非法的内存就会产生 `HardFault_Handler`，而 FreeRTOS 指向一个死循环，那么任务返回之后就在死循环中执行，这样子的任务是不安全的，所以避免这种情况，**任务一般都是死循环并且无返回值的**。只执行一次的任务在执行完毕要记得及时删除。
- 任务里面的延时函数必须使用 FreeRTOS 里面提供的延时函数如 `vTaskDelay()`，并不能使用我们裸机编程中的 `HAL_Delay()`。
    - FreeRTOS 里面的延时是阻塞延时，即调用 `vTaskDelay()` 函数的时候，当前任务会被挂起，调度器会切换到其它就绪的任务，从而实现多任务。
    - 如果使用裸机编程中的那种延时，那么整个任务就成为了一个死循环，如果恰好该任务的优先级是最高的，那么系统永远都是在这个任务中运行，比它优先级更低的任务无法运行，根本无法实现多任务。

#### 2.2 空闲任务与定时器任务堆栈函数实现

当我们使用了静态创建任务的时候，`configSUPPORT_STATIC_ALLOCATION` 这个宏定义必须为 1 （在FreeRTOSConfig.h 文件中），若使用 Configuration Wizard 则需将 `FreeRTOs 与内存申请有关配置选项` 中的 `静态内存分配` 勾选然后 Save All 保存，若在 Keil 中则需要 Ctrl + S 保存文件：

> 下图所示的配置界面是 VSCode 中的插件 EIDE 所适配的 Configuration Wizard 可视化配置界面，该文档编写时的测试工程也是使用 VSCode + EIDE 进行的。 

![image-20241101163554495](.assets/image-20241101163554495.png)

除了使能 `configSUPPORT_STATIC_ALLOCATION` 之外，我们还需要实现两个函数：`vApplicationGetIdleTaskMemory()` 与 `vApplicationGetTimerTaskMemory()`，这两个函数是用户设定的空闲（Idle）任务与定时器（Timer）任务的堆栈大小，必须由用户自己分配，而不能是动态分配。

```c
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
static StackType_t Timer_Task_Stack[configMINIMAL_STACK_SIZE];

static StaticTask_t Idle_Task_TCB;
static StaticTask_t Timer_Task_TCB;

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) 
{
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB;
    *ppxIdleTaskStackBuffer = Idle_Task_Stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB;
    *ppxTimerTaskStackBuffer = Timer_Task_Stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH; 
}
```

#### 2.3 定义任务栈

目前我们只创建了一个任务，当任务进入延时的时候，因为没有另外就绪的用户任务，那么系统就会进入空闲任务，空闲任务是 FreeRTOS 系统自己启动的一个任务，优先级最低。**当整个系统都没有就绪任务的时候，系统必须保证有一个任务在运行，空闲任务就是为这个设计的。**当用户任务延时到期，又会从空闲任务切换回用户任务。

在 FreeRTOS 系统中，每一个任务都是独立的，他们的运行环境都单独的保存在他们的栈空间当中。那么在定义好任务函数之后，我们还要为任务定义一个栈，目前我们使用的是静态内存，所以任务栈是一个独立的全局变量。

任务的栈占用的是 MCU 内部的 RAM，当任务越多的时候，需要使用的栈空间就越大，即需要使用的
RAM 空间就越多。一个 MCU能够支持多少任务，就得看你的 RAM 空间有多少。

```c
static StackType_t AppTaskCreate_Stack[128];
static StackType_t LED_Task_Stack[128];
```

在大多数系统中需要做栈空间地址对齐，在 FreeRTOS 中是以 8 字节大小对齐，并且会检查堆栈是否已经对齐，其中 `portBYTE_ALIGNMENT` 是在 `portmacro.h` 里面定义的一个宏，其值为 8，就是配置为按 8 字节对齐，当然用户可以选择按1、2、4、8、16、32 等字节对齐，目前默认为 8。

```c
// portmacro.h
#define portBYTE_ALIGNMENT 8

#if portBYTE_ALIGNMENT == 8
#define portBYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

pxTopOfStack = pxNewTCB->pxStack + ( ulStackDepth - ( uint32_t ) 1 );
pxTopOfStack = ( StackType_t * ) ( ( ( portPOINTER_SIZE_TYPE ) pxTopOfStack ) &
               ( ~( ( portPOINTER_SIZE_TYPE ) portBYTE_ALIGNMENT_MASK ) ) );

/* 检查计算出的堆栈顶部的对齐方式是否正确。 */
configASSERT( ( ( ( portPOINTER_SIZE_TYPE ) pxTopOfStack &
            ( portPOINTER_SIZE_TYPE ) portBYTE_ALIGNMENT_MASK ) == 0UL ) );
```

#### 2.4 定义任务控制块

定义好任务函数和任务栈之后，我们还需要为任务定义一个任务控制块，通常我们称这个任务控制块为任务的身份证。在 C 代码上，任务控制块就是一个结构体，里面有非常多的成员，这些成员共同描述了任务的全部信息。

```c
static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;
```

#### 2.5 静态创建任务

**一个任务的三要素是任务主体函数，任务栈，任务控制块**，那么怎么样把这三个要素联合在一起？FreeRTOS 里面有一个叫静态任务创建函数 `xTaskCreateStatic()`，它就是干这个活的。它将任务主体函数，任务栈（静态的）和任务控制块（静态的）这三者联系在一起，让任务可以随时被系统启动。

```c
AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,
                                         (const char *)"AppTaskCreate", 
                                         (uint32_t)128,             
                                         (void *)NULL,         
                                         (UBaseType_t)1,     
                                         (StackType_t *)AppTaskCreate_Stack,
                                         (StaticTask_t *)&AppTaskCreate_TCB);
  
  if (NULL != AppTaskCreate_Handle)
  		vTaskStartScheduler();
```

自上而下解释 `xTaskCreateStatic` 函数的参数：

- 任务入口函数，即任务函数的名称，需要我们自己定义并且实现。
- 任务名字，字符串形式，最大长度由 `FreeRTOSConfig.h` 中定义的 `configMAX_TASK_NAME_LEN` 宏指定，多余部分会被自动截掉，这里任务名字最好要与任务函数入口名字一致，方便进行调试。
- 任务堆栈大小，单位为字，在 32 位的处理器下（STM32），一个字等于 4 个字节，那么任务大小就为 128 * 4 字节。
- 任务入口函数形参，不用的时候配置为 0 或者 NULL 即可。
- 任务的优先级。优先级范围根据 `FreeRTOSConfig.h` 中的宏 `configMAX_PRIORITIES` 决定，如果使能`configUSE_PORT_OPTIMISED_TASK_SELECTION` 这个宏定义，则最多支持 32 个优先级；如果不用特殊方法查找下一个运行的任务，那么则不强制要求限制最大可用优先级数目。在 FreeRTOS 中，**数值越大优先级越高，0 代表最低优先级。**
- 任务栈起始地址，只有在使用静态内存的时候才需要提供，在使用动态内存的时候会根据提供的任务栈大小自动创建。
- 任务控制块指针，在使用静态内存的时候，需要给任务初始化函数 `xTaskCreateStatic()` 传递预先定义好的任务控制块的指针。在使用动态内存的时候，任务创建函数 `xTaskCreate()` 会返回一个指针指向任务控制块，该任务控制块是 `xTaskCreate()` 函数里面动态分配的一块内存。

#### 2.6 启动任务

当任务创建好后，是处于任务就绪（Ready），在就绪态的任务可以参与操作系统的调度。但是此时任务仅仅是创建了，还未开启任务调度器，也没创建空闲任务与定时器任务（如果使能了 `configUSE_TIMERS` 这个宏定义），那这两个任务就是在启动任务调度器中实现，每个操作系统，任务调度器只启动一次，之后就不会再次执行了，FreeRTOS 中启动任务调度器的函数是 `vTaskStartScheduler()`，并且启动任务调度器的时候就不会返回，从此任务管理都由 FreeRTOS 管理，此时才是真正进入实时操作系统中的第一步。

```c
vTaskStartScheduler();
```

#### 2.7 main.c 内容全貌

```c
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED_Task_Handle;

static StackType_t AppTaskCreate_Stack[128];
static StackType_t LED_Task_Stack[128];

static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;

static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
static StackType_t Timer_Task_Stack[configMINIMAL_STACK_SIZE];

static StaticTask_t Idle_Task_TCB;
static StaticTask_t Timer_Task_TCB;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void LED_Task(void *parameter)
{
    while(1)
    {
        __BSP_LED2_Toggle();
        vTaskDelay(pdMS_TO_TICKS(100));
        __BSP_LED2_Toggle();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void AppTaskCreate(void *parameter)
{
    taskENTER_CRITICAL();
    printf("AppTaskCreate running\r\n");
    LED_Task_Handle = xTaskCreateStatic((TaskFunction_t)LED_Task,
                                        (const char *)"LED_Task",
                                        (uint32_t)128,
                                        (void *)NULL,
                                        (UBaseType_t)4,
                                        (StackType_t *)LED_Task_Stack,
                                        (StaticTask_t *)&LED_Task_TCB);
    if (NULL != LED_Task_Handle)
        printf("LED_Task created successfully\r\n");
    else
        printf("LED_Task creation failed\r\n");

    vTaskDelete(AppTaskCreate_Handle);
    taskEXIT_CRITICAL();
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) 
{
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB;
    *ppxIdleTaskStackBuffer = Idle_Task_Stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB;
    *ppxTimerTaskStackBuffer = Timer_Task_Stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH; 
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  BSP_LED_Init();
  printf("FreeRTOS Demo Application.\r\n");

  AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,
                                           (const char *)"AppTaskCreate", 
                                           (uint32_t)128,             
                                           (void *)NULL,         
                                           (UBaseType_t)1,     
                                           (StackType_t *)AppTaskCreate_Stack,
                                           (StaticTask_t *)&AppTaskCreate_TCB);
  
  if (NULL != AppTaskCreate_Handle){
      printf("Scheduler started successfully\r\n");
      vTaskStartScheduler(); // 启动调度器
  }
  else {
      printf("AppTaskCreate creation failed\r\n");
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
```

>  遇到问题：[单独一个静态任务运行卡死](#单独一个静态任务运行卡死)

#### 2.8 下载验证

程序编译好后下载到开发板验证程序，若 LED 灯闪烁，说明创建的单任务正常运行。在当前这个例程，任务的栈，任务的控制块用的都是静态内存，必须由用户预先定义，这种方法我们在使用 FreeRTOS 的时候用的比较少，通常的方法是在任务创建的时候动态的分配任务栈和任务控制块的内存空间。

### 3. 创建单任务 - SRAM 动态内存

这里，我们创建一个单任务，任务使用的栈和任务控制块是在创建任务的时候 FreeRTOS 动态分配的，并不是预先定义好的全局变量。

#### 3.1 动态内存空间的堆从哪里来

在 *创建单任务—SRAM 静态内存* 的例程中，任务控制块和任务栈的内存空间都是从内部的 SRAM 里面分配的，具体分配到哪个地址由编译器决定。现在我们开始使用动态内存，即堆，其实堆也是内存，也属于 SRAM。FreeRTOS 做法是在 SRAM 里面定义一个大数组，也就是堆内存，供 FreeRTOS 的动态内存分配函数使用，在第一次使用的时候，系统会将定义的堆内存进行初始化，这些代码在 FreeRTOS 提供的内存管理方案中实现（ `heap_1.c`、`heap_2.c`、`heap_4.c` 等 )。

```c
//系统所有总的堆大小
#define configTOTAL_HEAP_SIZE ((size_t)(36*1024))
static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
/* 如果这是第一次调用 malloc 那么堆将需要初始化，以设置空闲块列表。*/
if ( pxEnd == NULL )
{
	prvHeapInit();
} 
else {
	mtCOVERAGE_TEST_MARKER();
}
```

- 堆内存的大小为 `configTOTAL_HEAP_SIZE` ， 在 `FreeRTOSConfig.h` 中由我们自己定义，`configSUPPORT_DYNAMIC_ALLOCATION` 这个宏定义在使用 FreeRTOS 操作系统的时候必须开启。
- 从内部 SRAMM 里面定义一个静态数组 `ucHeap`，大小由 `configTOTAL_HEAP_SIZE` 这个宏决定，目前定义为36KB。定义的堆大小不能超过内部 SRAM 的总大小。
- 如果这是第一次调用 `malloc` 那么需要将堆进行初始化，以设置空闲块列表，方便以后分配内存，初始化完成之后会取得堆的结束地址，在 MemMang 中的 5 个内存分配 `heap_x.c` 文件中实现。

#### 3.2 定义任务函数

使用动态内存的时候，任务的主体函数与使用静态内存时是一样的。

```c
static void LED_Task(void* argument)
{
    while(1)
    {
        __BSP_LED1_Ficker(500);
    }
}
```

**任务必须是一个死循环**，否则任务将通过 LR 返回，如果 LR 指向了非法的内存就会产生 HardFault_Handler，而FreeRTOS 指向一个任务退出函数 `prvTaskExitError()`，里面是一个死循环，那么任务返回之后就在死循环中执行，这样子的任务是不安全的，所以避免这种情况，任务一般都是死循环并且无返回值的。我们的` AppTaskCreate` 任务，执行一次之后就进行删除，则不影响系统运行，所以，**只执行一次的任务在执行完毕要记得及时删除。**

任务里面的延时函数必须使用 FreeRTOS 里面提供的延时函数。

#### 3.3 定义任务栈

使用动态内存的时候，任务栈在任务创建的时候创建，不用跟使用静态内存那样要预先定义好一个全局的静态的栈空间，动态内存就是按需分配内存，随用随取。

#### 3.4 定义任务控制块指针

使用动态内存时候，不用跟使用静态内存那样要预先定义好一个全局的静态的任务控制块空间。任务控制块是在任务创建的时候分配内存空间创建，任务创建函数会返回一个指针，用于指向任务控制块，所以要预先为任务栈定义一个任务控制块指针，也是我们常说的任务句柄。

任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄，以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么这个句柄可以为NULL。

```c
static TaskHandle_t APPCreate_Handle = NULL;
static TaskHandle_t LED_Task_Handle = NULL;
```

#### 3.5 动态创建任务

 使用静态内存时，使用 `xTaskCreateStatic()` 来创建一个任务，而使用动态内存的时，则使用 `xTaskCreate()` 函数来创建一个任务，两者的函数名不一样，具体的形参也有区别。

```c
xReturn = xTaskCreate(AppTaskCreate,
                      "AppTaskCreate",
                      512,
                      NULL,
                      1,
                      &APPCreate_Handle);
```

#### 3.6 启动任务

```c
if (pdPASS == xReturn)
{
	printf("AppTaskCreate created successfully.\r\n")
    vTaskStartScheduler();
}
```

#### 3.7 main.c 内容全貌

```c
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static TaskHandle_t APPCreate_Handle = NULL;
static TaskHandle_t LED_Task_Handle = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void LED_Task(void* argument)
{
    while(1)
    {
        __BSP_LED1_Ficker(500);
    }
}
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();

    xReturn = xTaskCreate(LED_Task,
                          "LED_Task",
                          512,      
                          NULL,
                          2,
                          &LED_Task_Handle);

    if (pdPASS == xReturn)
        printf("LED_Task created successfully.\r\n");

    vTaskDelete(APPCreate_Handle);

    taskEXIT_CRITICAL();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  BaseType_t xReturn = pdPASS;

  BSP_LED_Init();
  printf("FreeRTOS Demo Application.\r\n");

  xReturn = xTaskCreate(AppTaskCreate,
                        "AppTaskCreate",
                        512,
                        NULL,
                        1,
                        &APPCreate_Handle);
  if (pdPASS == xReturn)
  {
      printf("AppTaskCreate created successfully.\r\n");
      vTaskStartScheduler();
  }
  else
      return -1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
```

#### 3.8 下载验证

程序编译好后下载到开发板验证程序，若 LED 灯闪烁，说明创建的单任务正常运行。

### 4. 创建多任务 - SRAM 动态内存

创建多任务只需要按照创建单任务的套路依葫芦画瓢即可，接下来我们创建两个任务，任务 1 让一个 LED 灯闪烁，任务 2 让另外一个 LED 闪烁，两个 LED 闪烁的频率不一样，两个任务的优先级不一样。

#### 4.1 main.c 内容全貌

```c
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static TaskHandle_t APPCreate_Handle = NULL;
static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t LED2_Task_Handle = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void LED1_Task(void* argument)
{
    while(1)
    {
        __BSP_LED1_Ficker(500);
    }
}

static void LED2_Task(void *argument)
{
    while (1)
    {
        __BSP_LED2_Ficker(200);
    }
}

static void AppTaskCreate(void)
{
    BaseType_t xReturn1 = pdPASS;
    BaseType_t xReturn2 = pdPASS;

    taskENTER_CRITICAL();

    xReturn1 = xTaskCreate(LED1_Task,
                          "LED1_Task",
                          512,      
                          NULL,
                          2,
                          &LED1_Task_Handle);

    xReturn2 = xTaskCreate(LED2_Task,
                          "LED2_Task",
                          512,
                          NULL,
                          3,
                          &LED2_Task_Handle);

    if (pdPASS == xReturn1 && pdPASS == xReturn2)
        printf("LED_Task created successfully.\r\n");

    vTaskDelete(APPCreate_Handle);

    taskEXIT_CRITICAL();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  BaseType_t xReturn = pdPASS;

  BSP_LED_Init();
  printf("FreeRTOS Demo Application.\r\n");

  xReturn = xTaskCreate(AppTaskCreate,
                        "AppTaskCreate",
                        512,
                        NULL,
                        1,
                        &APPCreate_Handle);
  if (pdPASS == xReturn)
  {
      printf("AppTaskCreate created successfully.\r\n");
      vTaskStartScheduler();
  }
  else
      return -1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
```

目前多任务我们只创建了两个，如果要创建 3 个、4 个甚至更多都是同样的套路，容易忽略的地方是任务栈的大小，每个任务的优先级。大的任务，栈空间要设置大一点，重要的任务优先级要设置的高一点。

#### 4.2 下载验证

程序编译好后下载到开发板验证程序，若两个 LED 灯闪烁，说明创建的单任务正常运行。

## 三. FreeRTOS 的启动流程

在目前的 RTOS 中，主要有两种比较流行的启动方式，第三种方式是 RT-Thread 中所采用的那种。

### 1. 万事俱备，只欠东风

第一种野火称之为万事俱备，只欠东风法。这种方法是在 `main` 函数中将硬件初始化，RTOS 系统初始化，所有任务的创建这些都弄好，这个称之为万事都已经准备好。最后只欠一道东风，即启动 RTOS 的调度器，开始多任务的调度，伪代码实现如下：

```c
int main (void)
{
	/* 硬件初始化 */
	HardWare_Init(); 

	/* RTOS 系统初始化 */
	RTOS_Init();

	/* 创建任务1，但任务1 不会执行，因为调度器还没有开启 */ 
	RTOS_TaskCreate(Task1);
	/* 创建任务2，但任务2 不会执行，因为调度器还没有开启 */
	RTOS_TaskCreate(Task2);

	/* ......继续创建各种任务 */

	/* 启动RTOS，开始调度 */
	RTOS_Start();
}

void Task1( void *arg )
{
	while (1)
	{
	/* 任务实体，必须有阻塞的情况出现 */
	}
}

void Task1( void *arg )
{
	while (1)
	{ 
    	/* 任务实体，必须有阻塞的情况出现 */
	}
}
```

1. 硬件初始化。硬件初始化这一步还属于裸机的范畴，我们可以把需要使用到的硬件都初始化好而且测试好，确保无误。

2. RTOS 系统初始化。比如 RTOS 里面的全局变量的初始化，空闲任务的创建等。不同的 RTOS，它们的初始化有细微的差别。

3. 创建各种任务。这里把所有要用到的任务都创建好，但还不会进入调度，因为这个时候 RTOS 的调度器还没有开启。

4. 启动 RTOS 调度器，开始任务调度。这个时候调度器就从刚刚创建好的任务中选择一个优先级最高的任务开始运行。

5. 任务实体通常是一个不带返回值的无限循环的 C 函数，函数体必须有阻塞的情况出现，不然任务（如果优先权恰好是最高）会一直在 while 循环里面执行，导致其它任务没有执行的机会。

### 2. 小心翼翼，十分谨慎

第二种野火称之为小心翼翼，十分谨慎法。这种方法是在 `main` 函数中将硬件和 RTOS 系统先初始化好，然后创建一个启动任务后就启动调度器，然后在启动任务里面创建各种应用任务，当所有任务都创建成功后，启动任务把自己删除，伪代码实现如下：

```c
int main (void)
{
	/* 硬件初始化 */
	HardWare_Init(); (1)

	/* RTOS 系统初始化 */
	RTOS_Init(); (2)
    
	/* 创建一个任务 */
	RTOS_TaskCreate(AppTaskCreate); (3)

	/* 启动RTOS，开始调度 */
	RTOS_Start(); (4)
}

/* 起始任务，在里面创建任务 */
void AppTaskCreate( void *arg ) (5)
{
	/* 创建任务1，然后执行 */
	RTOS_TaskCreate(Task1); (6)
    
	/* 当任务1 阻塞时，继续创建任务2，然后执行 */
	RTOS_TaskCreate(Task2);

	/* ......继续创建各种任务 */

	/* 当任务创建完成，删除起始任务 */
	RTOS_TaskDelete(AppTaskCreate); (7)
}

void Task1( void *arg ) (8)
{
	while (1)
	{
		/* 任务实体，必须有阻塞的情况出现 */
	}
}
void Task2( void *arg ) (9)
{
	while (1)
	{
		/* 任务实体，必须有阻塞的情况出现 */
	}
}
```

### 3. RT-Thread 启动方法

> 参考：[RT-Thread 文档中心 / RT-Thread 标准版 / 内核 / RT-Thread 启动流程](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/basic/basic?id=rt-thread-%e5%90%af%e5%8a%a8%e6%b5%81%e7%a8%8b)

不同于 FreeRTOS，`rtthread_startup()` 函数是 RT-Thread 规定的统一启动入口。一般执行顺序是：系统先从启动文件开始运行，然后进入 RT-Thread 的启动函数 `rtthread_startup()` ，最后进入用户入口函数 `main()`。

为了在进入 `main()` 之前完成 RT-Thread 系统功能初始化，RT-Thread 使用了 MDK 的扩展功能 `$Sub$$` 和 `$Super$$`。可以给 main 添加 `$Sub$$` 的前缀符号作为一个新功能函数 `$Sub$$main`，这个 `$Sub$$main` 可以先调用一些要补充在 main 之前的功能函数（这里添加 RT-Thread 系统启动，进行系统一系列初始化），再调用 `$Super$$main` 转到 `main()`  函数执行，这样可以让用户不用去管 `main()` 之前的系统初始化操作。

关于 `$Sub$$` 和 `$Super$$` 扩展功能的使用，详见 [ARM® Compiler v5.06 for µVision®armlink User Guide](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0377g/pge1362065967698.html)。

```c
/* $Sub$$main 函数 */
int $Sub$$main(void)
{
  rtthread_startup();
  return 0;
}
```

在这里 `$Sub$$main` 函数调用了 `rtthread_startup()` 函数，其中 `rtthread_startup()` 函数的代码如下所示：

```c
int rtthread_startup(void)
{
    rt_hw_interrupt_disable();

    /* 板级初始化：需在该函数内部进行系统堆的初始化 */
    rt_hw_board_init();

    /* 打印 RT-Thread 版本信息 */
    rt_show_version();

    /* 定时器初始化 */
    rt_system_timer_init();

    /* 调度器初始化 */
    rt_system_scheduler_init();

#ifdef RT_USING_SIGNALS
    /* 信号初始化 */
    rt_system_signal_init();
#endif

    /* 由此创建一个用户 main 线程 */
    rt_application_init();

    /* 定时器线程初始化 */
    rt_system_timer_thread_init();

    /* 空闲线程初始化 */
    rt_thread_idle_init();

    /* 启动调度器 */
    rt_system_scheduler_start();

    /* 不会执行至此 */
    return 0;
}
```

### 4. 孰优孰劣

有关1，2 这两种方法孰优孰劣暂时没发现，根据开发者的喜好决定。LiteOS 和 ucos 第一种和第二种都可以使用，由用户选择，FreeRTOS 则默认使用第二种。接下来详细讲解 FreeRTOS 的启动流程。

### 5. RT-Thread 启动流程

在系统上电的时候第一个执行的是启动文件里面由汇编编写的复位函数 `Reset_Handler`，复位函数的最后会调用 C 库函数 `__main`。`__main` 函数的主要工作是初始化系统的堆和栈，最后调用 C 中的 `main` 函数，从而去到 C 的世界。

```
Reset_Handler PROC
	EXPORT Reset_Handler [WEAK]
	IMPORT __main
	IMPORT SystemInit
	LDR R0, =SystemInit
	BLX R0
	LDR R0, =__main
	BX R0
	ENDP
```

#### 5.1 创建任务 xTaskCreate() 函数

在 `main()` 函数中，直接可以对 FreeRTOS 进行创建任务操作，因为 FreeRTOS 会自动帮我们做初始化的事情，比如初始化堆内存。FreeRTOS 的简单方便是在别的实时操作系统上都没有的，像 RT-Tharead，需要做很多事情；华为 LiteOS 也需要用户进行初始化内核。

这种简单的特点使得 FreeRTOS 在初学的时候变得很简单，我们自己在 `main()` 函数中直接初始化我们的板级外设——`BSP_Init()`，然后进行任务的创建即可——`xTaskCreate()`，在任务创建中，FreeRTOS 会帮我们进行一系列的系统初始化，在创建任务的时候，会帮我们初始化堆内存。

`xTaskCreate` 函数内部进行堆内存初始化：

```c
BaseType_t xTaskCreate( TaskFunction_t pxTaskCode,
						const char * const pcName,
						const uint16_t usStackDepth,
						void * const pvParameters,
						UBaseType_t uxPriority,
						TaskHandle_t * const pxCreatedTask )
{
	if ( pxStack != NULL ) {
		/* 分配任务控制块内存 */
		pxNewTCB = ( TCB_t * ) pvPortMalloc( sizeof( TCB_t ) );

		if ( pxNewTCB != NULL ) {
			/* 将堆栈位置存储在TCB 中。*/
			pxNewTCB->pxStack = pxStack;
		}
	}
    /*
	省略代码
	......
	*/
}

/* 分配内存函数 */
void *pvPortMalloc( size_t xWantedSize )
{
	BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
	void *pvReturn = NULL;
    
	vTaskSuspendAll();
	{
        /*如果这是对malloc 的第一次调用，那么堆将需要初始化来设置空闲块列表。*/
        if ( pxEnd == NULL ) {
            prvHeapInit();
        } 
        else {
            mtCOVERAGE_TEST_MARKER();
        }
        /*
        省略代码
        ......
        */
	}
}
```

在未初始化内存的时候一旦调用了 `xTaskCreate()` 函数，FreeRTOS 就会帮我们自动进行内存的初始化，内存的初始化具体见下面代码。注意，此函数是 FreeRTOS 内部调用的，目前我们暂时不用管这个函数的实现。

```c
static void prvHeapInit( void )
{
	BlockLink_t *pxFirstFreeBlock;
	uint8_t *pucAlignedHeap;
	size_t uxAddress;
	size_t xTotalHeapSize = configTOTAL_HEAP_SIZE;

	uxAddress = ( size_t ) ucHeap;
	/* 确保堆在正确对齐的边界上启动。*/
	if ( ( uxAddress & portBYTE_ALIGNMENT_MASK ) != 0 ) {
		uxAddress += ( portBYTE_ALIGNMENT - 1 );
		uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
		xTotalHeapSize -= uxAddress - ( size_t ) ucHeap;
	}

	pucAlignedHeap = ( uint8_t * ) uxAddress;

	/* xStart 用于保存指向空闲块列表中第一个项目的指针。void 用于防止编译器警告*/
	xStart.pxNextFreeBlock = ( void * ) pucAlignedHeap;
	xStart.xBlockSize = ( size_t ) 0;

	/* pxEnd 用于标记空闲块列表的末尾，并插入堆空间的末尾。*/
	uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
	uxAddress -= xHeapStructSize;
	uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
	pxEnd = ( void * ) uxAddress;
	pxEnd->xBlockSize = 0;
	pxEnd->pxNextFreeBlock = NULL;

	/* 首先，有一个空闲块，其大小可以占用整个堆空间，减去pxEnd 占用的空间。*/
	pxFirstFreeBlock = ( void * ) pucAlignedHeap;
	pxFirstFreeBlock->xBlockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
	pxFirstFreeBlock->pxNextFreeBlock = pxEnd;

	/* 只存在一个块 - 它覆盖整个可用堆空间。因为是刚初始化的堆内存*/
	xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
	xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;


	xBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * \
	heapBITS_PER_BYTE ) - 1 );
}
/*-----------------------------------------------------------*/
```

#### 5.2 vTaskStartScheduler() 函数

在创建完任务的时候，我们需要开启调度器，因为创建仅仅是把任务添加到系统中，还没真正调度，并且空闲任务也没实现，定时器任务也没实现，这些都是在开启调度函数 `vTaskStartScheduler()` 中实现的。

**为什么要空闲任务？**

因为 FreeRTOS 一旦启动，就必须要保证系统中每时每刻都有一个任务处于运行态（Runing），并且空闲任务不可以被挂起与删除，空闲任务的优先级是最低的，以便系统中其他任务能随时抢占空闲任务的 CPU 使用权。这些都是系统必要的东西，也无需用户自己实现，FreeRTOS 全部帮我们搞定了。处理完这些必要的东西之后，系统才真正开始启动。

```c
void vTaskStartScheduler( void )
{
	BaseType_t xReturn;

	/*添加空闲任务*/
#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	{
		StaticTask_t *pxIdleTaskTCBBuffer = NULL;
		StackType_t *pxIdleTaskStackBuffer = NULL;
		uint32_t ulIdleTaskStackSize;

		/*  空闲任务是使用用户提供的 RAM 创建的 - 获取
			然后 RAM 的地址创建空闲任务。这是静态创建任务，我们不用管*/
		vApplicationGetIdleTaskMemory( &pxIdleTaskTCBBuffer,
									   &pxIdleTaskStackBuffer,
									   &ulIdleTaskStackSize );
        
 		xIdleTaskHandle = xTaskCreateStatic(prvIdleTask,
											"IDLE",
											ulIdleTaskStackSize,
											( void * ) NULL,
											( tskIDLE_PRIORITY | portPRIVILEGE_BIT ),
											pxIdleTaskStackBuffer,
											pxIdleTaskTCBBuffer );

		if ( xIdleTaskHandle != NULL ) {
			xReturn = pdPASS;
		} 
        else {
			xReturn = pdFAIL;
		}
	}
#else /* 这里才是动态创建 idle 任务 */
	{
		/* 使用动态分配的RAM 创建空闲任务。 */
		xReturn = xTaskCreate( prvIdleTask, (1)
							   "IDLE", configMINIMAL_STACK_SIZE,
							   ( void * ) NULL,
							   ( tskIDLE_PRIORITY | portPRIVILEGE_BIT ),
							   &xIdleTaskHandle );
	}
#endif
    
#if ( configUSE_TIMERS == 1 )
	{
		/* 如果使能了 configUSE_TIMERS 宏定义表明使用定时器，需要创建定时器任务*/
		if ( xReturn == pdPASS ) {
			xReturn = xTimerCreateTimerTask(); (2)
		} 
        else {
			mtCOVERAGE_TEST_MARKER();
		}
	}
#endif /* configUSE_TIMERS */

	if ( xReturn == pdPASS ) {
	/* 此处关闭中断，以确保不会发生中断 在调用xPortStartScheduler（）之前或期间。 堆栈的
       创建的任务包含打开中断的状态因此，当第一个任务时，中断将自动重新启用开始运行。 */
		portDISABLE_INTERRUPTS();

#if ( configUSE_NEWLIB_REENTRANT == 1 )
		{
			/* 不需要理会，这个宏定义没打开 */
			_impure_ptr = &( pxCurrentTCB->xNewLib_reent );
		}
#endif /* configUSE_NEWLIB_REENTRANT */

		xNextTaskUnblockTime = portMAX_DELAY;
		xSchedulerRunning = pdTRUE; (3)
		xTickCount = ( TickType_t ) 0U;

		/* 如果定义了configGENERATE_RUN_TIME_STATS，则以下内容必须定义宏以配置用
	   	   于生成的计时器/计数器运行时计数器时基。目前没启用该宏定义 */
		portCONFIGURE_TIMER_FOR_RUN_TIME_STATS();

		/* 调用xPortStartScheduler 函数配置相关硬件如滴答定时器、FPU、pendsv 等 */
		if ( xPortStartScheduler() != pdFALSE ) { (4)
		/* 如果xPortStartScheduler 函数启动成功，则不会运行到这里 */
		} 
   	 else {
			/* 不会运行到这里，除非调用 xTaskEndScheduler() 函数 */
		}
	} 
    else {
		/* 只有在内核无法启动时才会到达此行，因为没有足够的堆内存来创建空闲任务或计时器任务。
	       此处使用了断言，会输出错误信息，方便错误定位 */
		configASSERT( xReturn != errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY );
	}

	/* 如果INCLUDE_xTaskGetIdleTaskHandle 设置为0，则防止编译器警告，
   	   这意味着在其他任何地方都不使用xIdleTaskHandle。暂时不用理会 */
	( void ) xIdleTaskHandle;
}
```

- (1)：动态创建空闲任务（IDLE），因为现在我们不使用静态创建，这个`configSUPPORT_STATIC_ALLOCATION` 宏定义为 0，只能是动态创建空闲任务，并且空闲任务的优先级与堆栈大小都在 `FreeRTOSConfig.h` 中由用户定义，空闲任务的任务句柄存放在静态变量 `xIdleTaskHandle` 中，用户可以调用 API 函数`xTaskGetIdleTaskHandle()` 获得空闲任务句柄。

- (2)：如果在 `FreeRTOSConfig.h` 中使能了 `configUSE_TIMERS` 这个宏定义，那么需要创建一个定时器任务，这个定时器任务也是调用 `xTaskCreate()` 函数完成创建，过程十分简单，这也是系统的初始化内容，在调度器启动的过程中发现必要初始化的东西，FreeRTOS 就会帮我们完成，真的对开发者太友好了`xTimerCreateTimerTask()` 函数具体见下面代码：

    ```c
    BaseType_t xTimerCreateTimerTask( void )
    {
    	BaseType_t xReturn = pdFAIL;
    
    	/* 检查使用了哪些活动计时器的列表，以及用于与计时器服务通信的队列，已经初始化。*/
    	prvCheckForValidListAndQueue();
    
    	if ( xTimerQueue != NULL ) {
    #if( configSUPPORT_STATIC_ALLOCATION == 1 )
    	{
    		/* 这是静态创建的，无需理会 */
    		StaticTask_t *pxTimerTaskTCBBuffer = NULL;
    		StackType_t *pxTimerTaskStackBuffer = NULL;
    		uint32_t ulTimerTaskStackSize;
    
    		vApplicationGetTimerTaskMemory(&pxTimerTaskTCBBuffer,
    									   &pxTimerTaskStackBuffer,
    									   &ulTimerTaskStackSize );
    		xTimerTaskHandle = xTaskCreateStatic(prvTimerTask,
    											 "Tmr Svc",
    											 ulTimerTaskStackSize,
    											 NULL,
    					( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                 pxTimerTaskStackBuffer,
                                                 pxTimerTaskTCBBuffer );
    
    		if ( xTimerTaskHandle != NULL )
    		{
    			xReturn = pdPASS;
    		}
    	}
    #else
    	{ 	/* 这是才是动态创建定时器任务 */
    		xReturn = xTaskCreate(prvTimerTask,
    							  "Tmr Svc",
    							  configTIMER_TASK_STACK_DEPTH,
    							  NULL,
    		           ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
    							  &xTimerTaskHandle );
    	}
    #endif /* configSUPPORT_STATIC_ALLOCATION */
    	} else {
    		mtCOVERAGE_TEST_MARKER();
    	}
    
    	configASSERT( xReturn );
        
        return xReturn;
    }
    ```

- (3)：`xSchedulerRunning` 等于 `pdTRUE`，表示调度器开始运行了，而 `xTickCount` 初始化需要初始化为0，这个 `xTickCount` 变量用于记录系统的时间，在节拍定时器（SysTick）中断服务函数中进行自加。

- (4)：调用函数 `xPortStartScheduler()` 来启动系统节拍定时器（一般都是使用SysTick）并启动第一个任务。因为设置系统节拍定时器涉及到硬件特性，因此函数 `xPortStartScheduler()` 由移植层提供（在 `port.c` 文件实现），不同的硬件架构，这个函数的代码也不相同，在 ARM_CM3 中，使用 SysTick 作为系统节拍定时器。有兴趣可以看看 `xPortStartScheduler()` 的源码内容，下面简单介绍一下相关知识：

    在 Cortex-M4 架构中，FreeRTOS 为了任务启动和任务切换使用了三个异常：SVC、PendSV 和 SysTick：

    - **SVC** （系统服务调用，亦简称系统调用）用于任务的创建和管理，通常在任务开始运行时调用。有些操作系统不允许应用程序直接访问硬件，而是通过提供一些系统服务函数，用户程序使用 SVC 发出对系统服务函数的呼叫请求，以这种方法调用它们来间接访问硬件，它就会产生一个 SVC 异常。
    - **PendSV** （可挂起系统调用）用于进行上下文切换，确保任务在切换时的状态被正确保存和恢复，完成任务切换，它是可以像普通的中断一样被挂起的，它的最大特性是如果当前有优先级比它高的中断在运行，PendSV 会延迟执行，直到高优先级中断执行完毕，这样子产生的 PendSV 中断就不会打断其他中断的运行。
    - **SysTick**  用于产生定时中断，以便调度器可以根据系统节拍来管理任务的时间片。产生系统节拍时钟，提供一个时间片，如果多个任务共享同一个优先级，则每次 SysTick 中断，下一个任务将获得一个时间片。

    关于详细的 SVC、PendSV异常描述，推荐《Cortex-M4 权威指南》一书的 “异常” 部分。

    这里将 PendSV 和 SysTick 异常优先级设置为最低，这样任务切换不会打断某个中断服务程序，中断服务程序也不会被延迟，这样简化了设计，有利于系统稳定。

    **SysTick 的优先级配置为最低，那延迟的话系统时间会不会有偏差？**

    答案是不会的，因为 SysTick 只是当次响应中断被延迟了，而 SysTick 是硬件定时器，它一直在计时，这一次的溢出产生中断与下一次的溢出产生中断的时间间隔是一样的，至于系统是否响应还是延迟响应，这个与 SysTick 无关，它照样在计时。

#### 5.3 main 函数

当我们拿到一个移植好FreeRTOS 的例程的时候，不出意外，首先看到的是 `main` 函数，当认真一看 `main` 函数里面只是创建并启动一些任务和硬件初始化。而系统初始化这些工作不需要我们实现，因为 FreeRTOS 在我们使用创建与开启调度的时候就已经偷偷帮我们做完了，如果只是使用 FreeRTOS 的话，无需关注 FreeRTOS API 函数里面的实现过程，但是我们还是建议需要深入了解 FreeRTOS 然后再去使用，避免出现问题。

```c
int main(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

	/* 开发板硬件初始化 */
	BSP_Init(); (1)
	printf("BSP Init Successfully!\r\n");
    
	/* 创建AppTaskCreate 任务 */ (2)
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,/* 任务入口函数 */
						  (const char* )"AppTaskCreate",/* 任务名字 */
						  (uint16_t )512, /* 任务栈大小 */
						  (void* )NULL,/* 任务入口函数参数 */
						  (UBaseType_t )1, /* 任务的优先级 */
						  (TaskHandle_t*)&AppTaskCreate_Handle);/*任务控制块指针*/
	/* 启动任务调度 */
	if (pdPASS == xReturn)
		vTaskStartScheduler(); /* 启动任务，开启调度 */ (3)
	else
		return -1; (4)	
    
	while (1); /* 正常不会执行到这里 */
}
```

1. 开发板硬件初始化，FreeRTOS 系统初始化是经在创建任务与开启调度器的时候完成的。
2. 在 `AppTaskCreate` 中创建各种应用任务

```c
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

	taskENTER_CRITICAL(); //进入临界区

	/* 创建LED_Task 任务 */
	xReturn = xTaskCreate((TaskFunction_t )LED1_Task, /* 任务入口函数 */
						  (const char* )"LED1_Task",/* 任务名字 */
						  (uint16_t )512, /* 任务栈大小 */
						  (void* )NULL, /* 任务入口函数参数 */
						  (UBaseType_t )2, /* 任务的优先级 */
						  (TaskHandle_t* )&LED1_Task_Handle);/* 任务控制块指针 */
	if (pdPASS == xReturn)
		printf("创建LED1_Task 任务成功!\r\n");

	/* 创建LED_Task 任务 */
	xReturn = xTaskCreate((TaskFunction_t )LED2_Task, /* 任务入口函数 */
						  (const char* )"LED2_Task",/* 任务名字 */
                          (uint16_t )512, /* 任务栈大小 */
						  (void* )NULL, /* 任务入口函数参数 */
						  (UBaseType_t )3, /* 任务的优先级 */
						  (TaskHandle_t* )&LED2_Task_Handle);/* 任务控制块指针 */
	if (pdPASS == xReturn)
		printf("创建LED2_Task 任务成功!\r\n");

	vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate 任务
	
	taskEXIT_CRITICAL(); //退出临界区
}
```

- **当创建的应用任务的优先级比 `AppTaskCreate` 任务的优先级高、低或者相等时候，程序是如何执行的？**

    假如像我们代码一样在临界区创建任务，任务只能在退出临界区的时候才执行最高优先级任务。假如没使用临界区的话，就会分三种情况：

    1. 应用任务的优先级比初始任务的优先级高，那创建完后立马去执行刚刚创建的应用任务，当应用任务被阻塞时，继续回到初始任务被打断的地方继续往下执行，直到所有应用任务创建完成，最后初始任务把自己删除，完成自己的使命。

    2. 应用任务的优先级与初始任务的优先级一样，那创建完后根据任务的时间片来执行，直到所有应用任务创建完成，最后初始任务把自己删除，完成自己的使命。

    3. 应用任务的优先级比初始任务的优先级低，那创建完后任务不会被执行，如果还有应用任务紧接着创建应用任务，如果应用任务的优先级出现了比初始任务高或者相等的情况，参考 1 和 2 的处理方式，直到所有应用任务创建完成，最后初始任务把自己删除，完成自己的使命。

- 在启动任务调度器的时候，假如启动成功的话，任务就不会有返回了，假如启动没成功，则通过 LR 寄存器指定的地址退出，在创建 `AppTaskCreate` 任务的时候，任务栈对应 LR 寄存器指向是任务退出函数`prvTaskExitError()`，该函数里面是一个死循环，这代表着假如创建任务没成功的话，就会进入死循环，该任务也不会运行。

## 四. 任务管理

### 1. 任务的基本概念

从系统的角度看，任务是竞争系统资源的最小运行单元。FreeRTOS 是一个支持多任务的操作系统。在 FreeRTOS 中，任务可以使用或等待 CPU、使用内存空间等系统资源，并独立于其它任务运行， 任何数量的任务可以共享同一个优先级， 如果宏 `configUSE_TIME_SLICING` 定义为1，处于就绪态的多个相同优先级任务将会以时间片切换的方式共享处理器。

简而言之： FreeRTOS 的任务可认为是一系列独立任务的集合。每个任务在自己的环境中运行。在任何时刻，只有一个任务得到运行，FreeRTOS 调度器决定运行哪个任务。调度器会不断的启动、停止每一个任务，宏观看上去所有的任务都在同时在执行。作为任务，不需要对调度器的活动有所了解，在任务切入切出时保存上下文环境（寄存器值、堆栈内容）是调度器主要的职责。为了实现这点，每个 FreeRTOS 任务都需要有自己的栈空间。当任务切出时，它的执行环境会被保存在该任务的栈空间中，这样当任务再次运行时，就能从堆栈中正确的恢复上次的运行环境，任务越多，需要的堆栈空间就越大，而**一个系统能运行多少个任务，取决于系统的可用的 SRAM。**

FreeRTOS 可以给用户提供多个任务单独享有独立的堆栈空间，系统可以决定任务的状态，决定任务是否可以运行，同时还能运用内核的 IPC 通信资源，实现了任务之间的通信，帮助用户管理业务程序流程。这样用户可以将更多的精力投入到业务功能的实现中。

FreeRTOS 中的任务是抢占式调度机制，高优先级的任务可打断低优先级任务，低优先级任务必须在高优先级任务阻塞或结束后才能得到调度。同时 FreeRTOS 也支持时间片轮转调度方式，只不过时间片的调度是不允许抢占任务的 CPU 使用权。

任务通常会运行在一个死循环中，也不会退出，如果一个任务不再需要，可以调用 FreeRTOS 中的任务删除 API 函数接口显式地将其删除。

### 2. 任务调度器的基本概念

FreeRTOS 中提供的任务调度器是基于优先级的全抢占式调度：在系统中除了中断处理函数、调度器上锁部分的代码和禁止中断的代码是不可抢占的之外，系统的其他部分都是可以抢占的。系统理论上可以支持无数个优先级(0 ～ N，**优先级数值越小的任务优先级越低，0 为最低优先级，分配给空闲任务使用，一般不建议用户来使用这个优先级。** 假如使能了 `configUSE_PORT_OPTIMISED_TASK_SELECTION`  这个宏（在 `FreeRTOSConfig.h` 文件定义），一般强制限定最大可用优先级数目为 32。在一些资源比较紧张的系统中，可以根据实际情况选择只支持 8 个或 32 个优先级的系统配置。在系统中，当有比当前任务优先级更高的任务就绪时，当前任务将立刻被换出，高优先级任务抢占处理器运行。

一个操作系统如果只是具备了高优先级任务能够 “ 立即 ” 获得处理器并得到执行的特点，那么它仍然不算是实时操作系统。因为这个查找最高优先级任务的过程决定了调度时间是否具有确定性，例如一个包含 n 个就绪任务的系统中，如果仅仅从头找到尾，那么这个时间将直接和 n 相关，而下一个就绪任务抉择时间的长短将会极大的影响系统的实时性。

FreeRTOS 内核中采用两种方法寻找最高优先级的任务：

- 第一种是通用的方法，在就绪链表中查找从高优先级往低查找 `uxTopPriority`，因为在创建任务的时候已经将优先级进行排序，查找到的第一个 `uxTopPriority` 就是我们需要的任务，然后通过 `uxTopPriority` 获取对应的任务控制块。
- 第二种方法则是特殊方法，利用计算前导零指令 CLZ，直接在 `uxTopReadyPriority` 这个 32 位的变量中直接得出 `uxTopPriority`，这样子就知道哪一个优先级任务能够运行，这种调度算法比普通方法更快捷，但受限于平台（在STM32 中我们就使用这种方法）。

FreeRTOS 内核中也允许创建相同优先级的任务。**相同优先级的任务采用时间片轮转方式进行调度（也就是通常说的分时调度器），时间片轮转调度仅在当前系统中无更高优先级就绪任务存在的情况下才有效。**为了保证系统的实时性，系统尽最大可能地保证高优先级的任务得以运行。**任务调度的原则是一旦任务状态发生了改变，并且当前运行的任务优先级小于优先级队列组中任务最高优先级时，立刻进行任务切换（除非当前系统处于中断处理程序中或禁止任务切换的状态）。**

### 3. 任务状态迁移

FreeRTOS 系统中的每一个任务都有多种运行状态：

![image-20241102155422015](.assets/image-20241102155422015.png)

- 创建任务→就绪态（Ready）：任务创建完成后进入就绪态，表明任务已准备就绪，随时可以运行，只等待调度器进行调度。
- 就绪态→运行态（Running）：发生任务切换时，就绪列表中最高优先级的任务被执行，从而进入运行态。
- 运行态→就绪态：有更高优先级任务创建或者恢复后，会发生任务调度，此刻就绪列表中最高优先级任务变为运行态，那么原先运行的任务由运行态变为就绪态，依然在就绪列表中，等待最高优先级的任务运行完毕继续运行原来的任务（此处可以看做是CPU 使用权被更高优先级的任务抢占了）。
- 运行态→阻塞态（Blocked）：正在运行的任务发生阻塞（挂起、延时、读信号量等待）时，该任务会从就绪列表中删除，任务状态由运行态变成阻塞态，然后发生任务切换，运行就绪列表中当前最高优先级任务。
- 阻塞态→就绪态：阻塞的任务被恢复后（任务恢复、延时时间超时、读信号量超时或读到信号量等），此时被恢复的任务会被加入就绪列表，从而由阻塞态变成就绪态；如果此时被恢复任务的优先级高于正在运行任务的优先级，则会发生任务切换，将该任务将再次转换任务状态，由就绪态变成运行态。
- 就绪态、阻塞态、运行态→挂起态（Suspended）：任务可以通过调用 `vTaskSuspend()` API 函数都可以将处于任何状态的任务挂起，被挂起的任务得不到 CPU 的使用权，也不会参与调度，除非它从挂起态中解除。
- 挂起态→就绪态：把一个挂起状态的任务恢复的唯一途径就是调用 `vTaskResume()` 或`vTaskResumeFromISR()` API 函数，如果此时被恢复任务的优先级高于正在运行任务的优先级，则会发生任务切换，将该任务将再次转换任务状态，由就绪态变成运行态。

### 4. 任务状态的概念

FreeRTOS 系统中的每一任务都有多种运行状态。系统初始化完成后，创建的任务就可以在系统中竞争一定的资源，由内核进行调度。

任务状态通常分为以下四种：

- 就绪（Ready）：该任务在就绪列表中，就绪的任务已经具备执行的能力，只等待调度器进行调度，新创建的任务会初始化为就绪态。
- 运行（Running）：该状态表明任务正在执行，此时它占用处理器，FreeRTOS 调度器选择运行的永远是处于最高优先级的就绪态任务，当任务被运行的一刻，它的任务状态就变成了运行态。
- 阻塞（Blocked）：如果任务当前正在等待某个时序或外部中断，我们就说这个任务处于阻塞状态，该任务不在就绪列表中。包含任务被挂起、任务被延时、任务正在等待信号量、读写队列或者等待读写事件等。
- 挂起态(Suspended)：处于挂起态的任务对调度器而言是不可见的，**让一个任务进入挂起状态的唯一办法就是调用 `vTaskSuspend()` 函数**；而把一个挂起状的任务恢复的唯一途径就是调用 `vTaskResume()` 或`vTaskResumeFromISR()` 函数，我们可以这么理解挂起态与阻塞态的区别，当任务有较长的时间不允许运行
    的时候，我们可以挂起任务，这样子调度器就不会管这个任务的任何信息，直到我们调用恢复任务的 API 函数；而任务处于阻塞态的时候，系统还需要判断阻塞态的任务是否超时，是否可以解除阻塞。

### 5. 常见任务函数

下面补充一些 FreeRTOS 提供给我们对任务操作的一些常用函数。

#### 5.1 任务挂起函数

##### 5.1.1 vTaskSuspend()

挂起指定任务。被挂起的任务绝不会得到 CPU的使用权，不管该任务具有什么优先级。

任务可以通过调用 `vTaskSuspend()` 函数都可以将处于任何状态的任务挂起，被挂起的任务得不到 CPU 的使用权，也不会参与调度，它相对于调度器而言是不可见的，除非它从挂起态中解除。任务挂起是我们经常使用的一个函数，代码如下：

```c
#if ( INCLUDE_vTaskSuspend == 1 )

	void vTaskSuspend( TaskHandle_t xTaskToSuspend )
	{
		TCB_t *pxTCB;

		taskENTER_CRITICAL();
		{
			/* If null is passed in here then it is the running task that is
			being suspended. */
			pxTCB = prvGetTCBFromHandle( xTaskToSuspend );

			traceTASK_SUSPEND( pxTCB );

			/* Remove task from the ready/delayed list and place in the
			suspended list. */
			if( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
			{
				taskRESET_READY_PRIORITY( pxTCB->uxPriority );
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}

			/* Is the task waiting on an event also? */
			if( listLIST_ITEM_CONTAINER( &( pxTCB->xEventListItem ) ) != NULL )
			{
				( void ) uxListRemove( &( pxTCB->xEventListItem ) );
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}

			vListInsertEnd( &xSuspendedTaskList, &( pxTCB->xStateListItem ) );
		}
		taskEXIT_CRITICAL();

		if( xSchedulerRunning != pdFALSE )
		{
			/* Reset the next expected unblock time in case it referred to the
			task that is now in the Suspended state. */
			taskENTER_CRITICAL();
			{
				prvResetNextTaskUnblockTime();
			}
			taskEXIT_CRITICAL();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		if( pxTCB == pxCurrentTCB )
		{
			if( xSchedulerRunning != pdFALSE )
			{
				/* The current task has just been suspended. */
				configASSERT( uxSchedulerSuspended == 0 );
				portYIELD_WITHIN_API();
			}
			else
			{
				/* The scheduler is not running, but the task that was pointed
				to by pxCurrentTCB has just been suspended and pxCurrentTCB
				must be adjusted to point to a different task. */
				if( listCURRENT_LIST_LENGTH( &xSuspendedTaskList ) ==
                                              uxCurrentNumberOfTasks )
				{
					/* No other tasks are ready, so set pxCurrentTCB back to
					NULL so when the next task is created pxCurrentTCB will
					be set to point to it no matter what its relative priority
					is. */
					pxCurrentTCB = NULL;
				}
				else
				{
					vTaskSwitchContext();
				}
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}

#endif /* INCLUDE_vTaskSuspend */
```

- 如果想要使用任务挂起函数 `vTaskSuspend()` 则必须将宏定义 `INCLUDE_vTaskSuspend`  配置为 1。
- `xTaskToSuspend` 是挂起指定任务的任务句柄，任务必须为已创建的任务，可以通过传递 NULL 来挂起任务自己。
- 利用任务句柄 `xTaskToSuspend` 来获取任务控制块，通过调用 `prvGetTCBFromHandle()` API 函数得到对应的任务控制块。
- 从就绪/阻塞列表中删除即将要挂起的任务。然后更新"最高优先级变量 `uxReadyPriorities`"，目的是维护这个变量，这个变量的如下功能：
    1. 在使用通用方法找到最高优先级任务时，它用来记录最高优先级任务的优先级。
    2. 在使用硬件方法找到最高优先级任务时，它的每一位（共32bit）的状态代表这个优先级上边，有没有就绪的任务，具体见 *《FreeRTOS 内核实现与应用开发实战指南》- 10.2* 小节查找最高优先级就绪任务的讲解。
- 如果任务在等待事件，也将任务从等待事件列表中移除。
- 将任务状态添加到挂起列表中。在 FreeRTOS 中有专门的列表用于记录任务的状态，记录任务挂起态的列表就是`xSuspendedTaskList`，所有被挂起的任务都会放到这个列表中。
- 重置下一个任务的解除阻塞时间。重新计算一下还要多长时间执行下一个任务，如果下个任务的解锁，刚好是被挂起的那个任务，那么就是不正确的了，因为挂起的任务对调度器而言是不可见的，所以调度器是无法对挂起态的任务进行调度，所以要重新从延时列表中获取下一个要解除阻塞的任务。
- 如果挂起的是当前运行中的任务，并且调度器已经是运行的，则需要立即切换任务。不然系统的任务就错乱了，这是不允许的。
- 调度器未运行 ( `xSchedulerRunning == pdFALSE` )，但 `pxCurrentTCB` 指向的任务刚刚被挂起，所以必须重置`pxCurrentTCB` 以指向其他可以运行的任务。

- 首先调用函数 `listCURRENT_LIST_LENGTH()` 判断一下系统中所有的任务是不是都被挂起了，也就是查看列表 `xSuspendedTaskList` 的长度是不是等于 `uxCurrentNumberOfTasks`，事实上并不会发生这种情况，因为空闲任务是不允许被挂起和阻塞的，必须保证系统中无论如何都有一个任务可以运行。
- 如果没有其他任务准备就绪，因此将 `pxCurrentTCB` 设置为 NULL，在创建下一个任务时 `pxCurrentTCB` 将重新被设置。但是实际上并不会执行到这里，因为系统中的空闲任务永远是可以运行的。
- 有其他可运行的任务，则切换到其他任务。

> **NOTE**
>
> 任务可以调用 `vTaskSuspend()` 这个函数来挂起任务自身，但是在挂起自身的时候会进行一次任务上下文切换，需要挂起自身就将 `xTaskToSuspend` 设置为 NULL 传递进来即可。无论任务是什么状态都可以被挂起，只要调用了`vTaskSuspend()` 这个函数就会挂起成功，不论是挂起其他任务还是挂起任务自身。

任务的挂起与恢复函数在很多时候都是很有用的，比如我们想暂停某个任务运行一段时间，但是我们又需要在其恢复的时候继续工作，那么删除任务是不可能的，因为删除了任务的话，任务的所有的信息都是不可能恢复的了，删除是完完全全删除了，里面的资源都被系统释放掉，但是挂起任务就不会这样子，调用挂起任务函数，仅仅是将任务进入挂起态，其内部的资源都会保留下来，同时也不会参与系统中任务的调度，当调用恢复函数的时候，整个任务立即从挂起态进入就绪态，并且参与任务的调度，如果该任务的优先级是当前就绪态优先级最高的任务，那么立即会按照挂起前的任务状态继续执行该任务，从而达到我们需要的效果，注意，是继续执行，也就是说，挂起任务之前是什么状态，都会被系统保留下来，在恢复的瞬间，继续执行。这个任务函数的使用方法是很简单的，只需把任务句柄传递进来即可，`vTaskSuspend()` 会根据任务句柄的信息将对应的任务挂起，具体见下面代码。

```c
static TaskHandle_t LED_Task_Handle = NULL;/* LED 任务句柄 */

static void KEY_Task(void* parameter)
{
	while (1) {
		if ( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON ) {
			/* K1 被按下 */
			printf("挂起LED 任务！\n");
			vTaskSuspend(LED_Task_Handle);/* 挂起LED 任务 */
		}
		vTaskDelay(20);/* 延时20 个tick */
	}
}
```

##### 5.1.2 vTaskSuspendAll()

这个函数就是比较有意思的，将所有的任务都挂起，其实源码很简单，也很有意思，不管三七二十一将调度器锁定，并且这个函数是可以进行嵌套的，说白了挂起所有任务就是挂起任务调度器。调度器被挂起后则不能进行上下文切换，但是中断还是使能的。 当调度器被挂起的时候，如果有中断需要进行上下文切换， 那么这个任务将会被挂起，在调度器恢复之后才执行切换任务。调度器恢复可以调用 `xTaskResumeAll()`  函数，调用了多少次的`vTaskSuspendAll()` 就要调用多少次 `xTaskResumeAll()` 进行恢复。

```c
void vTaskSuspendAll( void )
{
	++uxSchedulerSuspended;
}
```

`uxSchedulerSuspended` 用于记录调度器是否被挂起，该变量默认初始值为 `pdFALSE`，表明调度器是没被挂起的，每调用一次 `vTaskSuspendAll()` 函数就将变量加一，用于记录调用了多少次 `vTaskSuspendAll()` 函数。

#### 5.2 任务恢复函数

##### 5.2.1 vTaskResume()

既然有任务的挂起，那么当然一样有恢复，不然任务怎么恢复呢，任务恢复就是让挂起的任务重新进入就绪状态，恢复的任务会保留挂起前的状态信息，在恢复的时候根据挂起时的状态继续运行。如果被恢复任务在所有就绪态任务中，处于最高优先级列表的第一位，那么系统将进行任务上下文的切换。

```c
#if ( INCLUDE_vTaskSuspend == 1 )

	void vTaskResume( TaskHandle_t xTaskToResume )
	{
	TCB_t * const pxTCB = ( TCB_t * ) xTaskToResume;

		/* It does not make sense to resume the calling task. */
		configASSERT( xTaskToResume );1

		/* The parameter cannot be NULL as it is impossible to resume the
		currently executing task. */
		if( ( pxTCB != NULL ) && ( pxTCB != pxCurrentTCB ) )
		{
			taskENTER_CRITICAL();
			{
				if( prvTaskIsTaskSuspended( pxTCB ) != pdFALSE )
				{
					traceTASK_RESUME( pxTCB );

					/* As we are in a critical section we can access the ready
					lists even if the scheduler is suspended. */
					( void ) uxListRemove(  &( pxTCB->xStateListItem ) );
					prvAddTaskToReadyList( pxTCB );

					/* We may have just resumed a higher priority task. */
					if( pxTCB->uxPriority >= pxCurrentTCB->uxPriority )
					{
						/* This yield may not cause the task just resumed to run,
						but will leave the lists in the correct state for the
						next yield. */
						taskYIELD_IF_USING_PREEMPTION();
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			taskEXIT_CRITICAL();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}

#endif /* INCLUDE_vTaskSuspend */
```

- 如果想要使用任务恢复函数 `vTaskResume()` 则必须将宏定义 `INCLUDE_vTaskSuspend` 配置为1，因为任务挂起只能通过调用 `vTaskSuspend()` 函数进行挂起， 没挂起的任务就无需恢复， 当需要调用 `vTaskSuspend()` 函数就必须使能 `INCLUDE_vTaskSuspend` 这个宏定义，所以想要使用 FreeRTOS 的任务挂起与恢复函数就必须将这个宏定义配置为 1。
- `xTaskToResume` 是恢复指定任务的任务句柄。
- 根据 `xTaskToResume` 任务句柄获取对应的任务控制块。
- 检查要恢复的任务是存在，如果不存在，调用恢复任务函数没有任何意义。
- `pxTCB` 任务控制块指针不能为 NULL，肯定要已经挂起的任务才需要恢复，同时要恢复的任务不能是当前正在运行的任务，因为当前正在运行（运行态）的任务不需要恢复，只能恢复处于挂起态的任务。
- 进入临界区，防止被打断。
- 判断要恢复的任务是否真的被挂起了，如果被挂起才需要恢复，没被挂起那当然也不需要恢复。
- 将要恢复的任务从挂起列表中删除。在 FreeRTOS 中有专门的列表用于记录任务的状态，记录任务挂起态的列表就是 `xSuspendedTaskList`，现在恢复任务就将要恢复的任务从列表中删除。
- 将要恢复的任务添加到就绪列表中去，任务从挂起态恢复为就绪态。FreeRTOS 也是有专门的列表记录处于就绪态的任务， 这个列表就是 `pxReadyTasksLists`。
- 如果恢复的任务优先级比当前正在运行的任务优先级更高，则需要进行任务的切换，调用`taskYIELD_IF_USING_PREEMPTION()` 进行一次任务切换。
- 退出临界区。

`vTaskResume()` 函数用于恢复挂起的任务。无论任务在挂起时候调用过多少次这个 `vTaskSuspend()` 函数，也只需调用一次 `vTaskResume ()` 函数即可将任务恢复运行，当然，无论调用多少次的 `vTaskResume()` 函数，也只在任务是挂起态的时候才进行恢复。

```c
static TaskHandle_t LED_Task_Handle = NULL;/* LED 任务句柄 */

static void KEY_Task(void* parameter)
{
	while (1) {
		if ( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON ) {
		/* K2 被按下 */
		printf("恢复LED 任务！\n");
		vTaskResume(LED_Task_Handle);/* 恢复LED 任务！ */
		}
		vTaskDelay(20);/* 延时20 个tick */
	}
}
```

##### 5.2.2 xTaskResumeFromISR()

`xTaskResumeFromISR()` 与 `vTaskResume()` 一样都是用于恢复被挂起的任务，不一样的是 **`xTaskResumeFromISR()` 专门用在中断服务程序中**。无论通过调用一次或多次 `vTaskSuspend()` 函数而被挂起的任务，也只需调用一次 `xTaskResumeFromISR()` 函数即可解挂。要想使用该函数必须在 `FreeRTOSConfig.h` 中把`INCLUDE_vTaskSuspend` 和 `INCLUDE_vTaskResumeFromISR` 都定义为 1 才有效。任务还没有处于挂起态的时候，调用 `xTaskResumeFromISR()` 函数是没有任何意义的。

```c
#if ( ( INCLUDE_xTaskResumeFromISR == 1 ) && ( INCLUDE_vTaskSuspend == 1 ) )

	BaseType_t xTaskResumeFromISR( TaskHandle_t xTaskToResume )
	{
	BaseType_t xYieldRequired = pdFALSE;
	TCB_t * const pxTCB = ( TCB_t * ) xTaskToResume;
	UBaseType_t uxSavedInterruptStatus;

		configASSERT( xTaskToResume );

		/* RTOS ports that support interrupt nesting have the concept of a
		maximum	system call (or maximum API call) interrupt priority.
		Interrupts that are	above the maximum system call priority are keep
		permanently enabled, even when the RTOS kernel is in a critical section,
		but cannot make any calls to FreeRTOS API functions.  If configASSERT()
		is defined in FreeRTOSConfig.h then
		portASSERT_IF_INTERRUPT_PRIORITY_INVALID() will result in an assertion
		failure if a FreeRTOS API function is called from an interrupt that has
		been assigned a priority above the configured maximum system call
		priority.  Only FreeRTOS functions that end in FromISR can be called
		from interrupts	that have been assigned a priority at or (logically)
		below the maximum system call interrupt priority.  FreeRTOS maintains a
		separate interrupt safe API to ensure interrupt entry is as fast and as
		simple as possible.  More information (albeit Cortex-M specific) is
		provided on the following link:
		http://www.freertos.org/RTOS-Cortex-M3-M4.html */
		portASSERT_IF_INTERRUPT_PRIORITY_INVALID();

		uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
		{
			if( prvTaskIsTaskSuspended( pxTCB ) != pdFALSE )
			{
				traceTASK_RESUME_FROM_ISR( pxTCB );

				/* Check the ready lists can be accessed. */
				if( uxSchedulerSuspended == ( UBaseType_t ) pdFALSE )
				{
					/* Ready lists can be accessed so move the task from the
					suspended list to the ready list directly. */
					if( pxTCB->uxPriority >= pxCurrentTCB->uxPriority )
					{
						xYieldRequired = pdTRUE;
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					( void ) uxListRemove( &( pxTCB->xStateListItem ) );
					prvAddTaskToReadyList( pxTCB );
				}
				else
				{
					/* The delayed or ready lists cannot be accessed so the task
					is held in the pending ready list until the scheduler is
					unsuspended. */
					vListInsertEnd( &( xPendingReadyList ), &( pxTCB->xEventListItem ) );
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		portCLEAR_INTERRUPT_MASK_FROM_ISR( uxSavedInterruptStatus );

		return xYieldRequired;
	}

#endif /* ( ( INCLUDE_xTaskResumeFromISR == 1 ) && ( INCLUDE_vTaskSuspend == 1 ) ) */
```

- `xTaskToResume` 是恢复指定任务的任务句柄。
- 定义一个是否需要进行任务切换的变量 `xYieldRequired`，默认为 `pdFALSE`，当任务恢复成功并且需要任务切换的话则重置为 `pdTRUE`，以表示需要进行任务切换。
- 根据 `xTaskToResume` 任务句柄获取对应的任务控制块。
- 定义一个变量 `uxSavedInterruptStatus` 用于保存关闭中断的状态。
- 检查要恢复的任务是存在，如果不存在，调用恢复任务函数没有任何意义。
- 调用 `portSET_INTERRUPT_MASK_FROM_ISR()` 函数设置 `basepri` 寄存器用于屏蔽系统可管理的中断，防止被处理被其他中断打断，当 `basepri` 设置为 `configMAX_SYSCALL_INTERRUPT_PRIORITY` 的时候（该宏在`FreeRTOSConfig.h` 中定义，现在配置为 5），会让系统不响应比该优先级低的中断，而优先级比之更高的中断则不受影响。就是说当这个宏定义配置为 5 的时候，中断优先级数值在0、1、2、3、4 的这些中断是不受 FreeRTOS 管理的，不可被屏蔽，而中断优先级在 5 到 15 的中断是受到系统管理，可用被屏蔽的。
- 判断要恢复的任务是否真的被挂起了，如果被挂起才需要恢复，没被挂起那当然也不需要恢复。
- 检查可以访问的就绪列表，检查调度器是否被挂起，如果没有被挂起，则继续执行下面两条的程序内容。
- 如果刚刚恢复的任务优先级比当前任务优先级更高需要进行一次任务的切换，重置 `xYieldRequired = pdTRUE ` 表示需要进行任务切换。
- 可以访问就绪列表，因此可以将任务从挂起列表中删除，然后添加到就绪列表中。
- 因为 `uxSchedulerSuspended` 调度器被挂起，无法访问就绪列表，因此任务将被添加到待处理的就绪列表中，直到调度器被恢复再进行任务的处理。
- 调用 `portCLEAR_INTERRUPT_MASK_FROM_ISR()` 函数清除 `basepri` 的设置，恢复屏蔽的中断。
- 返回 `xYieldRequired` 结果，在外部选择是否进行任务切换。使用 `xTaskResumeFromISR()` 的时候有几个需要注意的地方：
    1. 当函数的返回值为 `pdTRUE` 时：恢复运行的任务的优先级等于或高于正在运行的任务， 表明在中断服务函数退出后必须进行一次上下文切换， 使用 `portYIELD_FROM_ISR()` 进行上下文切换。当函数的返回值为`pdFALSE` 时：恢复运行的任务的优先级低于当前正在运行的任务，表明在中断服务函数退出后不需要进行上下文切换。
    2. `xTaskResumeFromISR()` **通常被认为是一个危险的函数，因为它的调用并非是固定的，中断可能随时来来临。**所以，**`xTaskResumeFromISR()` 不能用于任务和中断间的同步**，如果中断恰巧在任务被挂起之前到达，这就会导致一次中断丢失（任务还没有挂起，调用 `xTaskResumeFromISR()` 函数是没有意义的，只能等下一次中断）。这种情况下，**可以使用信号量或者任务通知来同步就可以避免这种情况。**

```c
void vAnExampleISR( void )
{
	BaseType_t xYieldRequired;
    
	/* 恢复被挂起的任务 */
	xYieldRequired = xTaskResumeFromISR( xHandle );

	if ( xYieldRequired == pdTRUE ) {
		/* 执行上下文切换， ISR 返回的时候将运行另外一个任务 */
		portYIELD_FROM_ISR();
	}
}
```

##### 5.2.3 xTaskResumeAll()

当调用了 `vTaskSuspendAll()` 函数将调度器挂起，想要恢复调度器的时候我们就需要调用 `xTaskResumeAll()`函数：

```c
BaseType_t xTaskResumeAll( void )
{
TCB_t *pxTCB = NULL;
BaseType_t xAlreadyYielded = pdFALSE;

	/* If uxSchedulerSuspended is zero then this function does not match a
	previous call to vTaskSuspendAll(). */
	configASSERT( uxSchedulerSuspended );

	/* It is possible that an ISR caused a task to be removed from an event
	list while the scheduler was suspended.  If this was the case then the
	removed task will have been added to the xPendingReadyList.  Once the
	scheduler has been resumed it is safe to move all the pending ready
	tasks from this list into their appropriate ready list. */
	taskENTER_CRITICAL();
	{
		--uxSchedulerSuspended;

		if( uxSchedulerSuspended == ( UBaseType_t ) pdFALSE )
		{
			if( uxCurrentNumberOfTasks > ( UBaseType_t ) 0U )
			{
				/* Move any readied tasks from the pending list into the
				appropriate ready list. */
				while( listLIST_IS_EMPTY( &xPendingReadyList ) == pdFALSE )
				{
					pxTCB = ( TCB_t * ) listGET_OWNER_OF_HEAD_ENTRY( ( &xPendingReadyList ) );
					( void ) uxListRemove( &( pxTCB->xEventListItem ) );
					( void ) uxListRemove( &( pxTCB->xStateListItem ) );
					prvAddTaskToReadyList( pxTCB );

					/* If the moved task has a priority higher than the current
					task then a yield must be performed. */
					if( pxTCB->uxPriority >= pxCurrentTCB->uxPriority )
					{
						xYieldPending = pdTRUE;
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}
				}

				if( pxTCB != NULL )
				{
					/* A task was unblocked while the scheduler was suspended,
					which may have prevented the next unblock time from being
					re-calculated, in which case re-calculate it now.  Mainly
					important for low power tickless implementations, where
					this can prevent an unnecessary exit from low power
					state. */
					prvResetNextTaskUnblockTime();
				}

				/* If any ticks occurred while the scheduler was suspended then
				they should be processed now.  This ensures the tick count does
				not	slip, and that any delayed tasks are resumed at the correct
				time. */
				{
					UBaseType_t uxPendedCounts = uxPendedTicks; /* Non-volatile copy. */

					if( uxPendedCounts > ( UBaseType_t ) 0U )
					{
						do
						{
							if( xTaskIncrementTick() != pdFALSE )
							{
								xYieldPending = pdTRUE;
							}
							else
							{
								mtCOVERAGE_TEST_MARKER();
							}
							--uxPendedCounts;
						} while( uxPendedCounts > ( UBaseType_t ) 0U );

						uxPendedTicks = 0;
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}
				}

				if( xYieldPending != pdFALSE )
				{
					#if( configUSE_PREEMPTION != 0 )
					{
						xAlreadyYielded = pdTRUE;
					}
					#endif
					taskYIELD_IF_USING_PREEMPTION();
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
	taskEXIT_CRITICAL();

	return xAlreadyYielded;
}
```

- 断言，如果 `uxSchedulerSuspended` 为 0，则此函数与先前对 `vTaskSuspendAll()` 的调用次数不匹配，也就是说明没有调用过 `vTaskSuspendAll()`函数，不需要调用 `xTaskResumeAll()`恢复调度器。

- 进入临界区。
- 每调用一次 `vTaskSuspendAll()` 函数就会将 `uxSchedulerSuspended` 变量加一，那么调用对应的`xTaskResumeAll()` 肯定就是将变量减一。
- 如果调度器恢复正常工作，也就是调度器没有被挂起，就可以将所有待处理的就绪任务从待处理就绪列表`xPendingReadyList` 移动到适当的就绪列表中。
- 当待处理就绪列表 `xPendingReadyList` 中是非空的时候，就需要将待处理就绪列表中的任务移除，添加到就绪列表中去。
- 如果移动的任务的优先级高于当前任务，需要进行一次任务的切换，重置 `xYieldPending = pdTRUE` 表示需要进行任务切换。
- 在调度器被挂起时，任务被解除阻塞，这可能阻止了重新计算下一个解除阻塞时间，在这种情况下，需要重置下一个任务的解除阻塞时间。调用 `prvResetNextTaskUnblockTime()` 函数将从延时列表中获取下一个要解除阻塞的任务。
- 如果在调度器挂起这段时间产生滴答定时器的计时，并且在这段时间有任务解除阻塞，由于调度器的挂起导致没法切换任务，当恢复调度器的时候应立即处理这些任务。这样既确保了滴答定时器的计数不会滑动，也保证了所有在延时的任务都会在正确的时间恢复。
- 调用 `xTaskIncrementTick()` 函数查找是否有待进行切换的任务，如果有则应该进行任务切换。
- 如果需要任务切换， 则调用 `taskYIELD_IF_USING_PREEMPTION()` 函数发起一次任务切换。

- 退出临界区。

`xTaskResumeAll` 函数的使用方法很简单， 但是要注意， 调用了多少次 vTaskSuspendAll()函数就必须同样调用多少次 `xTaskResumeAll()` 函数。

```c
void vDemoFunction( void )
{
    vTaskSuspendAll();
    /* 处理xxx 代码 */
	vTaskSuspendAll();	
    /* 处理xxx 代码 */
	vTaskSuspendAll();
	/* 处理xxx 代码 */

	xTaskResumeAll();
	xTaskResumeAll();
	xTaskResumeAll();
}
```

#### 5.3 任务删除函数 vTaskDelete()

`vTaskDelete()` 用于删除一个任务。当一个任务删除另外一个任务时，形参为要删除任务创建时返回的任务句柄，如果是删除自身， 则形参为 NULL。 要想使用该函数必须在 `FreeRTOSConfig.h` 中把 `INCLUDE_vTaskDelete` 定义为 1，删除的任务将从所有就绪，阻塞，挂起和事件列表中删除。

```c
#if ( INCLUDE_vTaskDelete == 1 )

	void vTaskDelete( TaskHandle_t xTaskToDelete )
	{
	TCB_t *pxTCB;

		taskENTER_CRITICAL();
		{
			/* If null is passed in here then it is the calling task that is
			being deleted. */
			pxTCB = prvGetTCBFromHandle( xTaskToDelete );

			/* Remove task from the ready list. */
			if( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
			{
				taskRESET_READY_PRIORITY( pxTCB->uxPriority );
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}

			/* Is the task waiting on an event also? */
			if( listLIST_ITEM_CONTAINER( &( pxTCB->xEventListItem ) ) != NULL )
			{
				( void ) uxListRemove( &( pxTCB->xEventListItem ) );
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}

			/* Increment the uxTaskNumber also so kernel aware debuggers can
			detect that the task lists need re-generating.  This is done before
			portPRE_TASK_DELETE_HOOK() as in the Windows port that macro will
			not return. */
			uxTaskNumber++;

			if( pxTCB == pxCurrentTCB )
			{
				/* A task is deleting itself.  This cannot complete within the
				task itself, as a context switch to another task is required.
				Place the task in the termination list.  The idle task will
				check the termination list and free up any memory allocated by
				the scheduler for the TCB and stack of the deleted task. */
				vListInsertEnd( &xTasksWaitingTermination, &( pxTCB->xStateListItem ) );

				/* Increment the ucTasksDeleted variable so the idle task knows
				there is a task that has been deleted and that it should therefore
				check the xTasksWaitingTermination list. */
				++uxDeletedTasksWaitingCleanUp;

				/* The pre-delete hook is primarily for the Windows simulator,
				in which Windows specific clean up operations are performed,
				after which it is not possible to yield away from this task -
				hence xYieldPending is used to latch that a context switch is
				required. */
				portPRE_TASK_DELETE_HOOK( pxTCB, &xYieldPending );
			}
			else
			{
				--uxCurrentNumberOfTasks;
				prvDeleteTCB( pxTCB );

				/* Reset the next expected unblock time in case it referred to
				the task that has just been deleted. */
				prvResetNextTaskUnblockTime();
			}

			traceTASK_DELETE( pxTCB );
		}
		taskEXIT_CRITICAL();

		/* Force a reschedule if it is the currently running task that has just
		been deleted. */
		if( xSchedulerRunning != pdFALSE )
		{
			if( pxTCB == pxCurrentTCB )
			{
				configASSERT( uxSchedulerSuspended == 0 );
				portYIELD_WITHIN_API();
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
	}

#endif /* INCLUDE_vTaskDelete */
```

- 如果想要使用任务删除函数 `vTaskDelete()` 则必须在 `FreeRTOSConfig.h` 中将宏定义 `INCLUDE_vTaskDelete` 配置为 1，`xTaskToDelete` 是删除指定任务的任务句柄。

- 利用任务句柄 `xTaskToDelete` 来获取任务控制块，通过调用 `prvGetTCBFromHandle()` 函数得到对应的任务控制块。如果如果 `xTaskToDelete` 为NULL 则会删除任务自身。
- 将任务从就绪列表中删除，如果删除后就绪列表的长度为 0，当前没有就绪的任务，应该调用`taskRESET_READY_PRIORITY()` 函数清除任务的最高就绪优先级变量 `uxTopReadyPriority` 中的位。

- 如果当前任务在等待事件，那么将任务从事件列表中移除。

- 如果此时删除的任务是任务自身的话，那么删除任务函数不能在任务本身内完成，因为需要上下文切换到另一个任务。所以需要将任务放在结束列表中（`xTasksWaitingTermination`），空闲任务会检查结束列表并在空闲任务中释放删除任务的控制块和已删除任务的堆栈内存。

- 增加 `uxDeletedTasksWaitingCleanUp` 变量的值，该变量用于记录有多少个任务需要释放内存，以便空闲任务知道有多少个已删除的任务需要进行内存释放，空闲任务会检查结束列表 `xTasksWaitingTermination` 并且释放对应删除任务的内存空间，空闲任务调用 `prvCheckTasksWaitingTermination()` 函数进行这些相应操作， 该函数是 FreeRTOS 内部调用的函数，在 `prvIdleTask` 中调用，该函数用户无需理会，将其源码附下用于学习：

    ```c
    static void prvCheckTasksWaitingTermination( void )
    {
    
    	/** THIS FUNCTION IS CALLED FROM THE RTOS IDLE TASK **/
    
    	#if ( INCLUDE_vTaskDelete == 1 )
    	{
    		BaseType_t xListIsEmpty;
    
    		/* ucTasksDeleted is used to prevent vTaskSuspendAll() being called
    		too often in the idle task. */
    		while( uxDeletedTasksWaitingCleanUp > ( UBaseType_t ) 0U )
    		{
    			vTaskSuspendAll();
    			{
    				xListIsEmpty = listLIST_IS_EMPTY( &xTasksWaitingTermination );
    			}
    			( void ) xTaskResumeAll();
    
    			if( xListIsEmpty == pdFALSE )
    			{
    				TCB_t *pxTCB;
    
    				taskENTER_CRITICAL();
    				{
    					pxTCB = ( TCB_t * ) listGET_OWNER_OF_HEAD_ENTRY( ( &xTasksWaitingTermination ) );
    					( void ) uxListRemove( &( pxTCB->xStateListItem ) );
    					--uxCurrentNumberOfTasks;
    					--uxDeletedTasksWaitingCleanUp;
    				}
    				taskEXIT_CRITICAL();
    
    				prvDeleteTCB( pxTCB );
    			}
    			else
    			{
    				mtCOVERAGE_TEST_MARKER();
    			}
    		}
    	}
    	#endif /* INCLUDE_vTaskDelete */
    }
    ```

    - `uxDeletedTasksWaitingCleanUp` 这个变量的值用于记录需要进行内存释放的任务个数，只有在需要进行释放的时候才进入循环查找释放的任务，防止在空闲任务中过于频繁地调用 `vTaskSuspendAll()`。
    - 挂起任务调度器。
    - 检查结束列表 `xTasksWaitingTermination` 中的任务个数是否为空。
    - 如果结束列表是非空的话就根据 `xTasksWaitingTermination` 中的任务获取对应的任务控制块。
    - 将任务从状态列表中删除。
    - 当前任务个数减一，并且 `uxDeletedTasksWaitingCleanUp` 的值也减一，直到为 0 退出循环。
    - 调用 `prvDeleteTCB()` 函数释放任务控制块与堆栈空间。这个函数的作用是在任务删除自身的时候才起作用，删除其他任务的时候是直接在删除函数中将其他任务的内存释放掉，不需要在空闲任务中释放。
    - 删除的任务并非自身， 则将当前任务个数减一，`uxCurrentNumberOfTasks` 是全局变量，用于记录当前的任务总数量。
    - 调用 `prvDeleteTCB()` 函数释放任务控制块与堆栈空间。此处与在空闲任务中用法一致。
    - 重置下一个任务的解除阻塞时间。重新计算一下还要多长时间执行下一个任务，如果下个任务的解锁，刚好是被删除的任务，那么这就是不正确的，因为删除的任务对调度器而言是不可见的，所以调度器是无法对删除的任务进行调度，所以要重新从延时列表中获取下一个要解除阻塞的任务。调用 `prvResetNextTaskUnblockTime()` 函数从延时列表的头部来获取下一个要解除任务的 TCB，延时列表按延时时间排序。
    - 退出临界区。
    - 如删除的是当前的任务，则需要发起一次任务切换。

**删除任务时，只会自动释放内核本身分配给任务的内存。应用程序（而不是内核）分配给任务的内存或任何其他资源必须是删除任务时由应用程序显式释放。**怎么理解这句话？就好像在某个任务中我申请了一大块内存，但是没释放就把任务删除，这块内存在任务删除之后不会自动释放的，所以我们**应该在删除任务之前就把任务中的这些资源释放掉，然后再进行删除，否则很容易造成内存泄漏。**

```c
TaskHandle_t DeleteHandle;

if (xTaskCreate(DeleteTask,
				"DeleteTask",
				STACK_SIZE,
				NULL,
				PRIORITY,
                &DeleteHandle) != pdPASS )
{
	/* 创建任务失败，因为没有足够的堆内存可分配。 */
}

void DeleteTask( void )
{
	/* 用户代码 xxxxx */
	/* ............ */

	/* 删除任务本身 */
	vTaskDelete( NULL );
}

/* 在其他任务删除DeleteTask 任务 */
vTaskDelete( DeleteHandle );
```

#### 5.4 任务延时函数

##### 5.4.1 vTaskDelay()

`vTaskDelay()` 在我们任务中用得非常之多，每个任务都必须是死循环，并且是必须要有阻塞的情况，否则低优先级的任务就无法被运行了。要想使用 FreeRTOS 中的 `vTaskDelay()`函数必须在 `FreeRTOSConfig.h` 中把`INCLUDE_vTaskDelay` 定义为 1 来使能。

```c
void vTaskDelay( const TickType_t xTicksToDelay )
```

`vTaskDelay()` 用于阻塞延时，调用该函数后，任务将进入阻塞状态，进入阻塞态的任务将让出 CPU 资源。延时的时长由形参 `xTicksToDelay` 决定，**单位为系统节拍周期**， 比如系统的时钟节拍周期为1ms，那么调用`vTaskDelay(1)` 的延时时间则为 1ms。

`vTaskDelay()` 延时是**相对性的延时**，它指定的延时时间是从调用 `vTaskDelay()` 结束后开始计算的，经过指定的时间后延时结束。比如 `vTaskDelay(100)`， 从调用 `vTaskDelay()` 结束后，任务进入阻塞状态，经过100 个系统时钟节拍周期后，任务解除阻塞。因此，`vTaskDelay()` 并不适用与周期性执行任务的场合。此外，其它任务和中断活动， 也会影响到 `vTaskDelay()` 的调用（比如调用前高优先级任务抢占了当前任务），进而影响到任务的
下一次执行的时间。

```c
#if ( INCLUDE_vTaskDelay == 1 )

	void vTaskDelay( const TickType_t xTicksToDelay )
	{
	BaseType_t xAlreadyYielded = pdFALSE;

		/* A delay time of zero just forces a reschedule. */
		if( xTicksToDelay > ( TickType_t ) 0U )
		{
			configASSERT( uxSchedulerSuspended == 0 );
			vTaskSuspendAll();
			{
				traceTASK_DELAY();

				/* A task that is removed from the event list while the
				scheduler is suspended will not get placed in the ready
				list or removed from the blocked list until the scheduler
				is resumed.

				This task cannot be in an event list as it is the currently
				executing task. */
				prvAddCurrentTaskToDelayedList( xTicksToDelay, pdFALSE );
			}
			xAlreadyYielded = xTaskResumeAll();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		/* Force a reschedule if xTaskResumeAll has not already done so, we may
		have put ourselves to sleep. */
		if( xAlreadyYielded == pdFALSE )
		{
			portYIELD_WITHIN_API();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}

#endif /* INCLUDE_vTaskDelay */
```

- 延时时间 `xTicksToDelay` 要大于 0 个 tick，否则会进行强制切换任务。

- 挂起任务调度器。将任务添加到延时列表中，`prvAddCurrentTaskToDelayedList()`。
- 恢复任务调度器。
- 强制切换任务，调用 `portYIELD_WITHIN_API()` 函数将 `PendSV` 的 bit28  置 1。

```c
static void prvAddCurrentTaskToDelayedList( TickType_t xTicksToWait, (1)
											const BaseType_t xCanBlockIndefinitely ) (2)
{
	TickType_t xTimeToWake;
	const TickType_t xConstTickCount = xTickCount; (3)

	/* 在将任务添加到阻止列表之前，从就绪列表中删除任务，
	   因为两个列表都使用相同的列表项。 */
	if ( uxListRemove( &( pxCurrentTCB->xStateListItem ) ) == ( UBaseType_t ) 0 ) { (4)
		portRESET_READY_PRIORITY( pxCurrentTCB->uxPriority,
		uxTopReadyPriority );
	} else {
		mtCOVERAGE_TEST_MARKER();
	}

#if ( INCLUDE_vTaskSuspend == 1 )
	{
		if ( ( xTicksToWait == portMAX_DELAY ) &&
		( xCanBlockIndefinitely != pdFALSE ) ) { (5)
		/* 支持挂起，则将当前任务挂起，
		   直接将任务添加到挂起列表，而不是延时列表！*/
			vListInsertEnd( &xSuspendedTaskList,
			&( pxCurrentTCB->xStateListItem ) ); (6)
		} else {
			/* 计算唤醒任务的时间 */
			xTimeToWake = xConstTickCount + xTicksToWait;
            /* 列表项将按唤醒时间顺序插入 */
			listSET_LIST_ITEM_VALUE(
				&( pxCurrentTCB->xStateListItem ), xTimeToWake );

			if ( xTimeToWake < xConstTickCount ) { (8)
				/* 唤醒时间如果溢出了，则会添加到延时溢出列表中 */
				vListInsert( pxOverflowDelayedTaskList,
					&( pxCurrentTCB->xStateListItem ) );
			} else {
				/* 没有溢出，添加到延时列表中 */
				vListInsert( pxDelayedTaskList,
					&( pxCurrentTCB->xStateListItem ) ); (9)

				/* 如果进入阻塞状态的任务被放置在被阻止任务列表的头部，
				   也就是下一个要唤醒的任务就是当前任务，那么就需要更新
				   xNextTaskUnblockTime 的值 */
				if ( xTimeToWake < xNextTaskUnblockTime ) { (10)
						xNextTaskUnblockTime = xTimeToWake;
				} else {
					mtCOVERAGE_TEST_MARKER();
				}
			}
		}
	}
}
```

- `xTicksToWait` 表示要延时多长时间，单位为系统节拍周期。

- `xCanBlockIndefinitely` 表示是否可以永久阻塞，如果 `pdFALSE` 表示不允许永久阻塞，也就是不允许挂起当然任务，而如果是 `pdTRUE`，则可以永久阻塞。

- 获取当前调用延时函数的时间点。

- 在将任务添加到阻止列表之前，从就绪列表中删除任务，因为两个列表都使用相同的列表项。调用`uxListRemove()` 函数将任务从就绪列表中删除。

- 支持挂起， 则将当前任务挂起， 此操作必须将 `INCLUDE_vTaskSuspend` 宏定义使能，并且`xCanBlockIndefinitely` 为 `pdTRUE`。

- 调用 `vListInsertEnd()` 函数直接将任务添加到挂起列表 `xSuspendedTaskList`，而不是延时列表。

- 计算唤醒任务的时间。

- 唤醒时间如果溢出了，则会将任务添加到延时溢出列表中，任务的延时由两个列表来维护，一个是用于延时溢出情况，另一个用于非溢出情况。

    ```c
    PRIVILEGED_DATA static List_t * volatile pxDelayedTaskList;
    PRIVILEGED_DATA static List_t * volatile pxOverflowDelayedTaskList;
    ```

- 如果唤醒任务的时间没有溢出，就会将任务添加到延时列表中，而不是延时溢出列表。
- 如果下一个要唤醒的任务就是当前延时的任务，那么就需要重置下一个任务的解除阻塞时间`xNextTaskUnblockTime` 为唤醒当前延时任务的时间。

任务的延时在实际中运用特别多，因为需要暂停一个任务，让任务放弃 CPU，延时结束后再继续运行该任务，如果任务中没有阻塞的话，比该任务优先级低的任务则无法得到 CPU 的使用权，就无法运行。

```c
void vTaskA( void * pvParameters )
{
	while (1) {
		// ...
		// 这里为任务主体代码
		// ...

		/* 调用相对延时函数,阻塞1000 个tick */
		vTaskDelay( 1000 );
	}
}
```

##### 5.4.2 vTaskDelayUntil()

在 FreeRTOS 中，除了相对延时函数，还有**绝对延时**函数 `vTaskDelayUntil()`，这个绝对延时常用于较精确的周期运行任务，比如我有一个任务，希望它以固定频率定期执行，而不受外部的影响，任务从上一次运行开始到下一次运行开始的时间间隔是绝对的，而不是相对的。

```c
#if ( INCLUDE_vTaskDelayUntil == 1 )

void vTaskDelayUntil( TickType_t * const pxPreviousWakeTime,
					  const TickType_t xTimeIncrement );
```

要想使用该函数必须在 `FreeRTOSConfig.h` 中把 `INCLUDE_vTaskDelayUntil` 定义为 1 来使能。

`vTaskDelayUntil()` 与 `vTaskDelay ()` 一样都是用来实现任务的周期性延时。但 `vTaskDelay ()` 的延时是相对的，是不确定的，它的延时是等 `vTaskDelay ()` 调用完毕后开始计算的。并且 `vTaskDelay ()` 延时的时间到了之后，如果有高优先级的任务或者中断正在执行，被延时阻塞的任务并不会马上解除阻塞，所有每次执行任务的周期并不完全确定。而 `vTaskDelayUntil()` 延时是绝对的，适用于周期性执行的任务。当( `*pxPreviousWakeTime + xTimeIncrement` )时间到达后，`vTaskDelayUntil()` 函数立刻返回，如果任务是最高优先级的，那么任务会立马解除阻塞，所以说 `vTaskDelayUntil()` 函数的延时是绝对性的。

```c
#if ( INCLUDE_vTaskDelayUntil == 1 )

	void vTaskDelayUntil( TickType_t * const pxPreviousWakeTime, const TickType_t xTimeIncrement )
	{
	TickType_t xTimeToWake;
	BaseType_t xAlreadyYielded, xShouldDelay = pdFALSE;

		configASSERT( pxPreviousWakeTime );
		configASSERT( ( xTimeIncrement > 0U ) );
		configASSERT( uxSchedulerSuspended == 0 );

		vTaskSuspendAll();
		{
			/* Minor optimisation.  The tick count cannot change in this
			block. */
			const TickType_t xConstTickCount = xTickCount;

			/* Generate the tick time at which the task wants to wake. */
			xTimeToWake = *pxPreviousWakeTime + xTimeIncrement;

			if( xConstTickCount < *pxPreviousWakeTime )
			{
				/* The tick count has overflowed since this function was
				lasted called.  In this case the only time we should ever
				actually delay is if the wake time has also	overflowed,
				and the wake time is greater than the tick time.  When this
				is the case it is as if neither time had overflowed. */
				if( ( xTimeToWake < *pxPreviousWakeTime ) && ( xTimeToWake > xConstTickCount ) )
				{
					xShouldDelay = pdTRUE;
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				/* The tick time has not overflowed.  In this case we will
				delay if either the wake time has overflowed, and/or the
				tick time is less than the wake time. */
				if( ( xTimeToWake < *pxPreviousWakeTime ) || ( xTimeToWake > xConstTickCount ) )
				{
					xShouldDelay = pdTRUE;
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}

			/* Update the wake time ready for the next call. */
			*pxPreviousWakeTime = xTimeToWake;

			if( xShouldDelay != pdFALSE )
			{
				traceTASK_DELAY_UNTIL( xTimeToWake );

				/* prvAddCurrentTaskToDelayedList() needs the block time, not
				the time to wake, so subtract the current tick count. */
				prvAddCurrentTaskToDelayedList( xTimeToWake - xConstTickCount, pdFALSE );
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		xAlreadyYielded = xTaskResumeAll();

		/* Force a reschedule if xTaskResumeAll has not already done so, we may
		have put ourselves to sleep. */
		if( xAlreadyYielded == pdFALSE )
		{
			portYIELD_WITHIN_API();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}

#endif /* INCLUDE_vTaskDelayUntil */
```

- 指针，指向一个变量，该变量保存任务最后一次解除阻塞的的时刻。第一次使用时，该变量必须初始化为当前时间，之后这个变量会在 `vTaskDelayUntil()` 函数内自动更新。

- 周期循环时间。当时间等于 ( `*pxPreviousWakeTime + xTimeIncrement` )时，任务解除阻塞。如果不改变参数 `xTimeIncrement` 的值，调用该函数的任务会按照固定频率执行。

- 获取开始进行延时的时间点。

- 计算延时到达的时间，也就是唤醒任务的时间，由于变量 `xTickCount` 与 `xTimeToWake` 可能会溢出，所以程序必须检测各种溢出情况，并且要保证延时周期不得小于任务主体代码执行时间，才能保证绝对延时的正确性，具体见下面的几种溢出情况。

- `pxPreviousWakeTime` 中保存的是上次唤醒时间，唤醒后需要一定时间执行任务主体代码，如果上次唤醒时间大于当前时间，说明节拍计数器溢出了。

- 如果本次任务的唤醒时间小于上次唤醒时间，但是大于开始进入延时的时间，进入延时的时间与任务唤醒时间都已经溢出了，这样子就可以看做没有溢出，其实也就是保了证周期性延时时间大于任务主体代码的执行时间，具体见下图：

    注意记住下面单词表示的含义：

    xTimeIncrement：任务周期时间。

    pxPreviousWakeTime：上一次唤醒任务的时间点。

    xTimeToWake：本次要唤醒任务的时间点。

    xConstTickCount：进入延时的时间点。

    ![image-20241103162158874](.assets/image-20241103162158874.png)

- 只是唤醒时间 `xTimeToWake` 溢出的情况，或者是 `xTickCount` 与 `xTimeToWake` 都没溢出的情况，都是符合要求的，因为都保证了周期性延时时间大于任务主体代码的执行时间。

    ![image-20241103162244768](.assets/image-20241103162244768.png)

    ![image-20241103162256178](.assets/image-20241103162256178.png)

    从上面三幅图可以看出无论是溢出还是没有溢出，都要求在下次唤醒任务之前，当前任务主体代码必须被执行完。也就是说任务执行的时间必须小于任务周期时间 `xTimeIncrement`，总不能存在任务周期为 10ms 的任务，其主体代码执行时间为 20ms，这样子根本执行不完任务主体代码。计算的唤醒时间合法后，就将当前任务加入延时列表，同样延时列表也有两个。每次产生系统节拍中断，都会检查这两个延时列表，查看延时的任务是否到期，如果时间到，则将任务从延时列表中删除，重新加入就绪列表，任务从阻塞态变成就绪态，如果此时的任务优先级是最高的，则会触发一次上下文切换。

- 更新上一次唤醒任务的时间 `pxPreviousWakeTime`。
- `prvAddCurrentTaskToDelayedList()` 函数需要的是阻塞时间而不是唤醒时间，因此减去当前的进入延时的时间 `xConstTickCount`。
- 强制执行一次上下文切换。

下面看看 `vTaskDelayUntil()` 的使用方法，注意了，`vTaskDelayUntil()` 的使用方法与 `vTaskDelay()` 不一样：

```c
void vTaskA( void * pvParameters )
{
	/* 用于保存上次时间。调用后系统自动更新 */
	static portTickType PreviousWakeTime;
	/* 设置延时时间，将时间转为节拍数 */
	const portTickType TimeIncrement = pdMS_TO_TICKS(1000);

	/* 获取当前系统时间 */
	PreviousWakeTime = xTaskGetTickCount();

	while (1)
    {
		/* 调用绝对延时函数,任务时间间隔为1000 个tick */
		vTaskDelayUntil( &PreviousWakeTime，TimeIncrement );

		// ...
		// 这里为任务主体代码
		// ...

	}
}
```

> 注意：在使用的时候要将延时时间转化为系统节拍，在任务主体之前要调用延时函数。

任务会先调用 `vTaskDelayUntil()` 使任务进入阻塞态，等到时间到了就从阻塞中解除，然后执行主体代码，任务主体代码执行完毕。会继续调用 `vTaskDelayUntil()` 使任务进入阻塞态，然后就是循环这样子执行。即使任务在执行过程中发生中断，那么也不会影响这个任务的运行周期，仅仅是缩短了阻塞的时间而已，到了要唤醒的时间依旧会将任务唤醒。

### 6. 任务的设计要点

作为一个嵌入式开发人员，要对自己设计的嵌入式系统要了如指掌，任务的优先级信息，任务与中断的处理，任务的运行时间、逻辑、状态等都要知道，才能设计出好的系统，所以，在设计的时候需要根据需求制定框架。在设计之初就应该考虑下面几点因素：任务运行的上下文环境、任务的执行时间合理设计。

FreeRTOS 中程序运行的上下文包括：

- 中断服务函数。

- 普通任务。

- 空闲任务。

1. 中断服务函数

    中断服务函数是一种需要特别注意的上下文环境，它运行在非任务的执行环境下（一般为芯片的一种特殊运行模式（也被称作特权模式）），在这个上下文环境中**不能使用挂起当前任务的操作，不允许调用任何会阻塞运行的API 函数接口。**另外需要注意的是，**中断服务程序最好保持精简短小，快进快出，一般在中断服务函数中只做标记事件的发生，然后通知任务，让对应任务去执行相关处理**，因为中断服务函数的优先级高于任何优先级的任务，如果中断处理时间过长，将会导致整个系统的任务无法正常运行。所以**在设计的时候必须考虑中断的频率、中断的处理时间**等重要因素，以便配合对应中断处理任务的工作。

2. 任务

    任务看似没有什么限制程序执行的因素，似乎所有的操作都可以执行。但是做为一个优先级明确的实时系统，如果一个任务中的程序出现了**死循环操作（此处的死循环是指没有阻塞机制的任务循环体）**，那么比这个任务优先级低的任务都将无法执行，当然也包括了空闲任务，因为死循环的时候，任务不会主动让出 CPU，低优先级的任务是不可能得到 CPU 的使用权的，而高优先级的任务就可以抢占 CPU。这个情况在实时操作系统中是必须注意的一点，所以**在任务中不允许出现死循环**。如果一个任务只有就绪态而无阻塞态，势必会影响到其他低优先级任务的执行，所以**在进行任务设计时，就应该保证任务在不活跃的时候，任务可以进入阻塞态以交出 CPU使用权**，这就需要我们自己明确知道什么情况下让任务进入阻塞态，保证低优先级任务可以正常运行。在实际设计中，一般会将紧急的处理事件的任务优先级设置得高一些。

3. 空闲任务

    空闲任务（idle 任务）是 FreeRTOS 系统中没有其他工作进行时自动进入的系统任务。因为处理器总是需要代码来执行——所以至少要有一个任务处于运行态。FreeRTOS 为了保证这一点，当调用 `vTaskStartScheduler()`时，调度器会自动创建一个空闲任务，空闲任务是一个非常短小的循环。**用户可以通过空闲任务钩子方式，在空闲任务上钩入自己的功能函数。通常这个空闲任务钩子能够完成一些额外的特殊功能，例如系统运行状态的指示，系统省电模式等。**除了空闲任务钩子，FreeRTOS 系统还把空闲任务用于一些其他的功能，比如当系统删除一个任务或一个动态任务运行结束时，在执行删除任务的时候，并不会释放任务的内存空间，只会将任务添加到结束列表中，真正的系统资源回收工作在空闲任务完成，**空闲任务是唯一一个不允许出现阻塞情况的任务，因为FreeRTOS 需要保证系统永远都有一个可运行的任务。**

    对于空闲任务钩子上挂接的空闲钩子函数，它应该满足以下的条件：

    - 永远不会挂起空闲任务；
    - 不应该陷入死循环，需要留出部分时间用于系统处理系统资源回收。

4. 任务的执行时间

    任务的执行时间一般是指两个方面，一是任务从开始到结束的时间，二是任务的周期。

    在系统设计的时候这两个时间候我们都需要考虑，例如，对于事件 A 对应的服务任务 Ta，系统要求的实时响应指标是10ms，而 Ta 的最大运行时间是1ms，那么 10ms 就是任务 Ta 的周期了，1ms 则是任务的运行时间，简单来说任务Ta 在10ms 内完成对事件 A的响应即可。此时，系统中还存在着以 50ms 为周期的另一任务 Tb，它每次运行的最大时间长度是100us。在这种情况下，即使把任务 Tb 的优先级抬到比Ta 更高的位置，对系统的实时性指标也没什么影响，因为即使在 Ta 的运行过程中，Tb 抢占了Ta 的资源，等到 Tb 执行完毕，消耗的时间也只不过是 100us，还是在事件 A 规定的响应时间内(10ms)，Ta 能够安全完成对事件A的响应。但是假如系统中还存在任务 Tc，其运行时间为 20ms，假如将 Tc的优先级设置比Ta 更高，那么在 Ta 运行的时候，突然间被 Tc 打断，等到 Tc 执行完毕，那 Ta 已经错过对事件 A（10ms）的响应了，这是不允许的。所以**在设计的时候，必须考虑任务的时间，一般来说处理时间更短的任务优先级应设置更高一些。**

### 7. 任务管理实验

```c
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static TaskHandle_t APPCreate_Handle = NULL;
static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t LED2_Task_Handle = NULL;
static TaskHandle_t Key0_Task_Handle = NULL;
static TaskHandle_t Key1_Task_Handle = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void LED1_Task(void* argument)
{
    while(1)
    {
        __BSP_LED1_Ficker(500);
    }
}

static void LED2_Task(void *argument)
{
    while (1)
    {
        __BSP_LED2_Ficker(200);
    }
}

static void Key0_Task(void *argument)
{
    while (1)
    {
        if (BSP_KEY_Read(0) ==  GPIO_PIN_RESET)
        {
            printf("LED1 Task Suspend.\r\n");
            vTaskSuspend(LED1_Task_Handle);
        }
        vTaskDelay(100);
    }
}

static void Key1_Task(void *argument)
{
    while (1)
    {
        if (BSP_KEY_Read(1) == GPIO_PIN_RESET)
        {
            printf("LED1 Task Resume.\r\n");
            vTaskResume(LED1_Task_Handle);
        }
        vTaskDelay(100);
    }
}

static void AppTaskCreate(void)
{
    BaseType_t xReturn1 = pdPASS;
    BaseType_t xReturn2 = pdPASS;
    BaseType_t xReturn3 = pdPASS;
    BaseType_t xReturn4 = pdPASS;

    taskENTER_CRITICAL();

    xReturn1 = xTaskCreate(LED1_Task,
                          "LED1_Task",
                          256,      
                          NULL,
                          2,
                          &LED1_Task_Handle);

    xReturn2 = xTaskCreate(LED2_Task,
                          "LED2_Task",
                          256,
                          NULL,
                          3,
                          &LED2_Task_Handle);

    xReturn3 = xTaskCreate(Key0_Task,
                          "Key0_Task",
                          256,
                          NULL,
                          5,
                          &Key0_Task_Handle);

    xReturn4 = xTaskCreate(Key1_Task,
                          "Key1_Task",
                          256,
                          NULL,
                          5,
                          &Key1_Task_Handle);

    if (pdPASS == xReturn1 && pdPASS == xReturn2 &&
        pdPASS == xReturn3 && pdPASS == xReturn4)
        printf("LED_Task created successfully.\r\n");

    vTaskDelete(NULL);

    taskEXIT_CRITICAL();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  BaseType_t xReturn = pdPASS;

  BSP_LED_Init();
  BSP_KEY_Init();

  printf("FreeRTOS Demo Application.\r\n");

  xReturn = xTaskCreate(AppTaskCreate,
                        "AppTaskCreate",
                        512,
                        NULL,
                        1,
                        &APPCreate_Handle);
  if (pdPASS == xReturn)
  {
      printf("AppTaskCreate created successfully.\r\n");
      vTaskStartScheduler();
  }
  else
      return -1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

```

## 五. 消息队列

### 5.1 消息队列的基本概念

队列又称消息队列，是一种常用于任务间通信的数据结构，队列可以在任务与任务间、中断和任务间传递信息，实现了任务接收来自其他任务或中断的不固定长度的消息，任务能够从队列里面读取消息，当队列中的消息是空时，读取消息的任务将被阻塞，用户还可以指定阻塞的任务时间 `xTicksToWait`，在这段时间中，如果队列为空，该任务将保持阻塞状态以等待队列数据有效。当队列中有新消息时，被阻塞的任务会被唤醒并处理新消息；当等待的时间超过了指定的阻塞时间，即使队列中尚无有效数据，任务也会自动从阻塞态转为就绪态。消息队列是一种异步的通信方式。

通过消息队列服务，任务或中断服务例程可以将一条或多条消息放入消息队列中。同样，一个或多个任务可以从消息队列中获得消息。当有多个消息发送到消息队列时，通常是将先进入消息队列的消息先传给任务，也就是说，任务先得到的是最先进入消息队列的消息，即先进先出原则（FIFO），但是也支持后进先出原则（LIFO）。

FreeRTOS 中使用队列数据结构实现任务异步通信工作，具有如下特性：

- 消息支持先进先出方式排队，支持异步读写工作方式。
- 读写队列均支持超时机制。
- 消息支持后进先出方式排队，往队首发送消息（LIFO）。
- 可以允许不同长度（不超过队列节点最大值）的任意类型消息。
- 一个任务能够从任意一个消息队列接收和发送消息。
- 多个任务能够从同一个消息队列接收和发送消息。
- 当队列使用结束后，可以通过删除队列函数进行删除。

### 5.2 消息队列的运作机制

创建消息队列时 FreeRTOS 会先给消息队列分配一块内存空间，这块内存的大小等于消息队列控制块大小加上（单个消息空间大小与消息队列长度的乘积），接着再初始化消息队列，此时消息队列为空。FreeRTOS 的消息队列控制块由多个元素组成，当消息队列被创建时，系统会为控制块分配对应的内存空间，用于保存消息队列的一些信息如消息的存储位置，头指针 `pcHead`、尾指针 `pcTail`、消息大小 `uxItemSize` 以及队列长度 `uxLength` 等。同时每个消息队列都与消息空间在同一段连续的内存空间中，在创建成功的时候，这些内存就被占用了，只有删除了消息队列的时候，这段内存才会被释放掉，创建成功的时候就已经分配好每个消息空间与消息队列的容量，无法更改，每个消息空间可以存放不大于消息大小 `uxItemSize` 的任意类型的数据，所有消息队列中的消息空间总数即是消息队列的长度，这个长度可在消息队列创建时指定。

任务或者中断服务程序都可以给消息队列发送消息，当发送消息时，如果队列未满或者允许覆盖入队，FreeRTOS 会将消息拷贝到消息队列队尾，否则，会根据用户指定的阻塞超时时间进行阻塞，在这段时间中，如果队列一直不允许入队，该任务将保持阻塞状态以等待队列允许入队。当其它任务从其等待的队列中读取入了数据（队列未满），该任务将自动由阻塞态转移为就绪态。当等待的时间超过了指定的阻塞时间，即使队列中还不允许入队，任务也会自动从阻塞态转移为就绪态，此时发送消息的任务或者中断程序会收到一个错误码 `errQUEUE_FULL`。

发送紧急消息的过程与发送消息几乎一样，唯一的不同是，当发送紧急消息时，发送的位置是消息队列队头而非队尾，这样，接收者就能够优先接收到紧急消息，从而及时进行消息处理。

当某个任务试图读一个队列时，其可以指定一个阻塞超时时间。在这段时间中，如果队列为空，该任务将保持阻塞状态以等待队列数据有效。当其它任务或中断服务程序往其等待的队列中写入了数据，该任务将自动由阻塞态转移为就绪态。当等待的时间超过了指定的阻塞时间，即使队列中尚无有效数据，任务也会自动从阻塞态转移为就绪态。

当消息队列不再被使用时，应该删除它以释放系统资源，一旦操作完成，消息队列将被永久性的删除。

消息队列的运作过程具体见下图：

![image-20241107155049097](.assets/image-20241107155049097.png)

### 5.3 消息队列的阻塞机制

很简单，因为 FreeRTOS 已经为我们做好了，我们直接使用就好了，每个对消息队列读写的函数，都有这种机制，我称之为阻塞机制。假设有一个任务 A 对某个队列进行读操作的时候（也就是我们所说的出队），发现它没有消息，那么此时任务A 有 3 个选择：

- 第一个选择，任务 A 扭头就走，既然队列没有消息，那我也不等了，干其它事情去，这样子任务 A不会进入阻塞态；
- 第二个选择，任务A还是在这里等等吧，可能过一会队列就有消息，此时任务 A会进入阻塞状态，在等待着消息的道来，而任务 A 的等待时间就由我们自己定义，比如设置 1000 个系统时钟节拍 tick 的等待，在这 1000 个tick 到来之前任务 A 都是处于阻塞态，当阻塞的这段时间任务A等到了队列的消息，那么任务A就会从阻塞态变成就绪态，如果此时任务 A 比当前运行的任务优先级还高，那么，任务 A 就会得到消息并且运行；假如 1000 个tick 都过去了，队列还没消息，那任务 A 就不等了，从阻塞态中唤醒，返回一个没等到消息的错误代码，然后继续执行任务 A 的其他代码；
- 第三个选择，任务 A 死等，不等到消息就不走了，这样子任务 A 就会进入阻塞态，直到完成读取队列的消息。

而在发送消息操作的时候，为了保护数据，当且仅当队列允许入队的时候，发送者才能成功发送消息；队列中无可用消息空间时，说明消息队列已满，此时，系统会根据用户指定的阻塞超时时间将任务阻塞，在指定的超时时间内如果还不能完成入队操作，发送消息的任务或者中断服务程序会收到一个错误码errQUEUE_FULL，然后解除阻塞状态；当然，只有在任务中发送消息才允许进行阻塞状态，而在中断中发送消息不允许带有阻塞机制的，需要调用在中断中发送消息的API 函数接口，因为发送消息的上下文环境是在中断中，不允许有阻塞的情况。

假如有多个任务阻塞在一个消息队列中，那么这些阻塞的任务将按照任务优先级进行排序，优先级高的任务将优先获得队列的访问权。

### 5.4 消息队列的应用场景

消息队列可以应用于发送不定长消息的场合，包括任务与任务间的消息交换，队列是 FreeRTOS 主要的任务间通讯方式，可以在任务与任务间、中断和任务间传送信息，发送到队列的消息是通过拷贝方式实现的，这意味着队列存储的数据是原数据，而不是原数据的引用。

### 5.5 消息队列控制块

FreeRTOS 的消息队列控制块由多个元素组成，当消息队列被创建时，系统会为控制块分配对应的内存空间，用于保存消息队列的一些信息如消息的存储位置，头指针 `pcHead`、尾指针 `pcTail`、消息大小 `uxItemSize` 以及队列长度 `uxLength`，以及当前队列消息个数 `uxMessagesWaiting` 等。

```c
typedef struct QueueDefinition {
	int8_t *pcHead; (1)
	int8_t *pcTail; (2)
	int8_t *pcWriteTo; (3)
        
	union {
		int8_t *pcReadFrom; (4)
		UBaseType_t uxRecursiveCallCount; (5)
	} u;

	List_t xTasksWaitingToSend; (6)
	List_t xTasksWaitingToReceive; (7)

	volatile UBaseType_t uxMessagesWaiting; (8)
	UBaseType_t uxLength; (9)
	UBaseType_t uxItemSize; (10)

    volatile int8_t cRxLock; (11)
	volatile int8_t cTxLock; (12)

#if( ( configSUPPORT_STATIC_ALLOCATION == 1 )&&( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
	uint8_t ucStaticallyAllocated;
#endif

#if ( configUSE_QUEUE_SETS == 1 )
	struct QueueDefinition *pxQueueSetContainer;
#endif

#if ( configUSE_TRACE_FACILITY == 1 )
	UBaseType_t uxQueueNumber;
	uint8_t ucQueueType;
#endif

} xQUEUE;

typedef xQUEUE Queue_t;
```

1. `pcHead` 指向队列消息存储区起始位置，即第一个消息空间。

2. `pcTail` 指向队列消息存储区结束位置地址。

3. `pcWriteTo` 指向队列消息存储区下一个可用消息空间。

4. `pcReadFrom` 与 `uxRecursiveCallCount` 是一对互斥变量，使用联合体用来确保两个互斥的结构体成员不会同时出现。当结构体用于队列时，`pcReadFrom` 指向出队消息空间的最后一个，见文知义，就是读取消息时候是从 `pcReadFrom` 指向的空间读取消息内容。

5. 当结构体用于互斥量时，`uxRecursiveCallCount` 用于计数，记录递归互斥量被“调用”的次数。

6. `xTasksWaitingToSend` 是一个发送消息阻塞列表，用于保存阻塞在此队列的任务，任务按照优先级进行排序，由于队列已满，想要发送消息的任务无法发送消息。

7. `xTasksWaitingToReceive` 是一个获取消息阻塞列表，用于保存阻塞在此队列的任务，任务按照优先级进行排序，由于队列是空的，想要获取消息的任务无法获取到消息。

8. `uxMessagesWaiting` 用于记录当前消息队列的消息个数，如果消息队列被用于信号量的时候，这个值就表示有效信号量个数。

9. `uxLength` 表示队列的长度，也就是能存放多少消息。

10. `uxItemSize` 表示单个消息的大小。

11. 队列上锁后，储存从队列收到的列表项数目，也就是出队的数量，如果队列没有上锁，设置为`queueUNLOCKED`。

12. 队列上锁后，储存发送到队列的列表项数目，也就是入队的数量，如果队列没有上锁，设置为`queueUNLOCKED`。

    这两个成员变量为 `queueUNLOCKED` 时，表示队列未上锁；当这两个成员变量为 `queueLOCKED_UNMODIFIED` 时，表示队列上锁

### 5.6 消息队列常用函数讲解

使用队列模块的典型流程如下：

- 创建消息队列。
- 写队列操作。
- 读队列操作。
- 删除队列。

#### 5.6.1 消息队列创建函数 xQueueCreate()

`xQueueCreate()` 用于创建一个新的队列并返回可用于访问这个队列的队列句柄。队列句柄其实就是一个指向队列数据结构类型的指针。

队列就是一个数据结构，用于任务间的数据的传递。每创建一个新的队列都需要为其分配 RAM，一部分用于存储队列的状态，剩下的作为队列消息的存储区域。使用 `xQueueCreate()` 创建队列时，使用的是动态内存分配，所以要想使用该函数必须在 `FreeRTOSConfig.h` 中把 `configSUPPORT_DYNAMIC_ALLOCATION` 定义为 1 来使能，这是个用于使能动态内存分配的宏，通常情况下，在 FreeRTOS 中，凡是创建任务，队列，信号量和互斥量等内核对象都需要使用动态内存分配，所以这个宏默认在 `FreeRTOS.h` 头文件中已经使能（即定义为1）。如果想使用静态内存，则可以使用 `xQueueCreateStatic()` 函数来创建一个队列。使用静态创建消息队列函数创建队列时需要的形参更多，需要的内存由编译的时候预先分配好，一般很少使用这种方法。`xQueueCreate()` 函数原型具体见下面代码，使用说明见下面表格：

```c
#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
#define xQueueCreate( uxQueueLength, uxItemSize ) \
	xQueueGenericCreate( ( uxQueueLength ), ( uxItemSize ), ( queueQUEUE_TYPE_BASE ) )
#endif
```

![image-20241107160813508](.assets/image-20241107160813508.png)

从函数原型中，我们可以看到，创建队列真正使用的函数是 `xQueueGenericCreate()`，消息队列创建函数，顾名思义，就是创建一个队列，与任务一样，都是需要先创建才能使用的东西，FreeRTOS 肯定不知道我们需要什么样的队列，比如队列的长度，消息的大小这些信息都是需要我们自己定义的，FreeRTOS 提供给我们这个创建函数，爱怎么搞都是我们自己来实现，下面是 `xQueueGenericCreate()` 函数源码：

```c
#if( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

	QueueHandle_t xQueueGenericCreate( const UBaseType_t uxQueueLength,
									   const UBaseType_t uxItemSize,
									   const uint8_t ucQueueType )
	{
		Queue_t *pxNewQueue;	
        size_t xQueueSizeInBytes;
		uint8_t *pucQueueStorage;

        configASSERT( uxQueueLength > ( UBaseType_t ) 0 );

        if ( uxItemSize == ( UBaseType_t ) 0 ) {
        	/* 消息空间大小为0*/
        	xQueueSizeInBytes = ( size_t ) 0; (1)
        } else {
            /* 分配足够消息存储空间，空间的大小为队列长度*单个消息大小 */
            xQueueSizeInBytes = ( size_t ) ( uxQueueLength * uxItemSize ); (2)
        }
        /* 向系统申请内存，内存大小为消息队列控制块大小+消息存储空间大小 */
        pxNewQueue=(Queue_t*)pvPortMalloc(sizeof(Queue_t)+xQueueSizeInBytes); (3)

        if ( pxNewQueue != NULL ) {
        	/* 计算出消息存储空间的起始地址 */
       	 	pucQueueStorage = ( ( uint8_t * ) pxNewQueue ) + sizeof( Queue_t ); (4)

    #if( configSUPPORT_STATIC_ALLOCATION == 1 )
        	{
            pxNewQueue->ucStaticallyAllocated = pdFALSE;
        	}
    #endif

            prvInitialiseNewQueue( uxQueueLength, (5)
            uxItemSize,
            pucQueueStorage,
            ucQueueType,
            pxNewQueue );
        }

        return pxNewQueue;
    }

#endif
```

1. 如果 `uxItemSize` 为 0，也就是单个消息空间大小为 0，这样子就不需要申请内存了，那么 `xQueueSizeInBytes` 也设置为 0 即可，设置为 0 是可以的，用作信号量的时候这个就可以设置为 0。

2. `uxItemSize` 并不是为 0，那么需要分配足够存储消息的空间，内存的大小为队列长度*单个消息大小。

3. FreeRTOS 调用 `pvPortMalloc()` 函数向系统申请内存空间，内存大小为消息队列控制块大小加上消息存储空间大小，因为这段内存空间是需要保证连续的，具体见下图。

    ![image-20241107161211663](.assets/image-20241107161211663.png)

4. 计算出消息存储内存空间的起始地址，因为(3)步骤中申请的内存是包含了消息队列控制块的内存空间，但是我们存储消息的内存空间在消息队列控制块后面。

5. 调用 `prvInitialiseNewQueue()` 函数将消息队列进行初始化。其实 `xQueueGenericCreate()` 主要是用于分配消息队列内存的，消息队列初始化函数源码具体见如下代码。

    ```c
    static void prvInitialiseNewQueue( const UBaseType_t uxQueueLength, (1)
                                       const UBaseType_t uxItemSize, (2)
                                       uint8_t *pucQueueStorage, (3)
                                       const uint8_t ucQueueType, (4)
                                       Queue_t *pxNewQueue ) (5)
    {
    	( void ) ucQueueType;
    
    	if ( uxItemSize == ( UBaseType_t ) 0 ) {
    	/* 没有为消息存储分配内存,但是pcHead 指针不能设置为NULL,
    	   因为队列用作互斥量时,pcHead 要设置成NULL。
           这里只是将pcHead 指向一个已知的区域 */
    		pxNewQueue->pcHead = ( int8_t * ) pxNewQueue; (6)
    	} else {
    		/* 设置pcHead 指向存储消息的起始地址 */
    		pxNewQueue->pcHead = ( int8_t * ) pucQueueStorage; (7)
    	}
    
    	/* 初始化消息队列控制块的其他成员 */
    	pxNewQueue->uxLength = uxQueueLength; (8)
    	pxNewQueue->uxItemSize = uxItemSize;
    	/* 重置消息队列 */
    	( void ) xQueueGenericReset( pxNewQueue, pdTRUE ); (9)
    
    #if ( configUSE_TRACE_FACILITY == 1 )
    	{
    		pxNewQueue->ucQueueType = ucQueueType;
    	}
    #endif
    
    #if( configUSE_QUEUE_SETS == 1 )
    {
    	pxNewQueue->pxQueueSetContainer = NULL;
    }
    #endif
    
    	traceQUEUE_CREATE( pxNewQueue );
    }
    ```

    1. 消息队列长度。
    2. 单个消息大小。
    3. 存储消息起始地址。
    4. 消息队列类型：
        - queueQUEUE_TYPE_BASE：表示队列 。
        - queueQUEUE_TYPE_SET：表示队列集合 。
        - queueQUEUE_TYPE_MUTEX：表示互斥量。
        - queueQUEUE_TYPE_COUNTING_SEMAPHORE：表示计数信号量 。
        - queueQUEUE_TYPE_BINARY_SEMAPHORE：表示二进制信号量 。
        - queueQUEUE_TYPE_RECURSIVE_MUTEX ：表示递归互斥量。
    5. 消息队列控制块。
    6. 如果没有为消息队列分配存储消息的内存空间，而且 `pcHead` 指针不能设置为 NULL，因为队列用作互斥量时，`pcHead` 要设置成NULL，这里只能将 `pcHead `指向一个已知的区域，指向消息队列控制块`pxNewQueue`。
    7. 如果分配了存储消息的内存空间，则设置 `pcHead` 指向存储消息的起始地址 `pucQueueStorage`。
    8. 初始化消息队列控制块的其他成员，消息队列的长度与消息的大小。
    9. 重置消息队列，在消息队列初始化的时候，需要重置一下相关参数

创建完成的消息队列示意图：

![image-20241107162703180](.assets/image-20241107162703180.png)

**在创建消息队列的时候，是需要用户自己定义消息队列的句柄的，但是注意了，定义了队列的句柄并不等于创建了队列，创建队列必须是调用消息队列创建函数进行创建**（可以是静态也可以是动态创建），否则，以后根据队列句柄使用消息队列的其它函数的时候会发生错误，创建完成会返回消息队列的句柄，用户通过句柄就可使用消息队列进行发送与读取消息队列的操作，如果返回的是 NULL 则表示创建失败，消息队列创建函数 `xQueueCreate()` 使用实例具体如下：

```c
QueueHandle_t Test_Queue =NULL;

#define QUEUE_LEN 4 /* 队列的长度，最大可包含多少个消息 */
#define QUEUE_SIZE 4 /* 队列中每个消息大小（字节） */

BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

taskENTER_CRITICAL(); //进入临界区

/* 创建Test_Queue */
Test_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* 消息队列的长度 */
						  (UBaseType_t ) QUEUE_SIZE);/* 消息的大小 */
if (NULL != Test_Queue)
	printf("创建Test_Queue 消息队列成功!\r\n");

taskEXIT_CRITICAL(); //退出临界区
```

#### 5.6.2 消息队列静态创建函数 xQueueCreateStatic()

`xQueueCreateStatic()` 用于创建一个新的队列并返回可用于访问这个队列的队列句柄。队列句柄其实就是一个指向队列数据结构类型的指针。

队列就是一个数据结构，用于任务间的数据的传递。每创建一个新的队列都需要为其分配 RAM ， 一部分用于存储队列的状态， 剩下的作为队列的存储区。使用 `xQueueCreateStatic()` 创建队列时，使用的是静态内存分配，所以要想使用该函数必须在 `FreeRTOSConfig.h` 中把 `configSUPPORT_STATIC_ALLOCATION` 定义为 1 来使能。这是个用于使能静态内存分配的宏，需要的内存在程序编译的时候分配好，由用户自己定义，其实创建过程与`xQueueCreate()` 都是差不多的。

![image-20241107163113140](.assets/image-20241107163113140.png)

使用示例：

```c
/* 创建一个可以最多可以存储10 个64 位变量的队列 */
#define QUEUE_LENGTH 10
#define ITEM_SIZE sizeof( uint64_t )

/* 该变量用于存储队列的数据结构 */
static StaticQueue_t xStaticQueue;

/* 该数组作为队列的存储区域，大小至少有uxQueueLength * uxItemSize 个字节 */
uint8_t ucQueueStorageArea[ QUEUE_LENGTH * ITEM_SIZE ];

void vATask( void *pvParameters )
{
	QueueHandle_t xQueue;

	/* 创建一个队列 */
	xQueue = xQueueCreateStatic( QUEUE_LENGTH, /* 队列深度 */
								 ITEM_SIZE, /* 队列数据单元的单位 */
								 ucQueueStorageArea,/* 队列的存储区域 */
								 &xStaticQueue ); /* 队列的数据结构 */
	/* 剩下的其他代码 */
}
```

#### 5.6.3 消息队列删除函数 vQueueDelete()

队列删除函数是根据消息队列句柄直接删除的，删除之后这个消息队列的所有信息都会被系统回收清空，而且不能再次使用这个消息队列了，但是需要注意的是，如果某个消息队列没有被创建，那也是无法被删除的，动脑子想想都知道，没创建的东西就不存在，怎么可能被删除。`xQueue`  是 `vQueueDelete()` 函数的形参，是消息队列句柄，表示的是要删除哪个想队列。

```c
void vQueueDelete( QueueHandle_t xQueue )
{
	Queue_t * const pxQueue = ( Queue_t * ) xQueue;

	/* 断言 */
	configASSERT( pxQueue ); (1)
	traceQUEUE_DELETE( pxQueue );

#if ( configQUEUE_REGISTRY_SIZE > 0 )
{
	/* 将消息队列从注册表中删除，我们目前没有添加到注册表中，暂时不用理会 */
	vQueueUnregisterQueue( pxQueue ); (2)
}
#endif

#if( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )&& ( configSUPPORT_STATIC_ALLOCATION == 0 ) ) {
	/* 因为用的消息队列是动态分配内存的，所以需要调用 vPortFree 来释放消息队列的内存 */
	vPortFree( pxQueue ); (3)
}
}
```

1. 对传入的消息队列句柄进行检查，如果消息队列是有效的才允许进行删除操作。
2. 将消息队列从注册表中删除，我们目前没有添加到注册表中，暂时不用理会。
3. 因为用的消息队列是动态分配内存的，所以需要调用 `vPortFree()` 函数来释放消息队列的内存。

消息队列删除函数 `vQueueDelete()` 的使用也是很简单的，只需传入要删除的消息队列的句柄即可，调用函数时，系统将删除这个消息队列。需要注意的是调用删除消息队列函数前，系统应存在 `xQueueCreate()` 或`xQueueCreateStatic()` 函数创建的消息队列。此外 `vQueueDelete()` 也可用于删除信号量。如果删除消息队列时，有任务正在等待消息，则不应该进行删除操作（官方说的是不允许进行删除操作，但是源码并没有禁止删除的操作，使用的时候注意一下就行了）。

使用示例：

```c
#define QUEUE_LENGTH 5
#define QUEUE_ITEM_SIZE 4

int main( void )
{
	QueueHandle_t xQueue;
	/* 创建消息队列 */
	xQueue = xQueueCreate( QUEUE_LENGTH, QUEUE_ITEM_SIZE );

	if ( xQueue == NULL ) {
		/* 消息队列创建失败 */
	} else {
		/* 删除已创建的消息队列 */
		vQueueDelete( xQueue );
	}
}
```

#### 5.6.4 向消息队列发送消息函数

任务或者中断服务程序都可以给消息队列发送消息，当发送消息时，如果队列未满或者允许覆盖入队，FreeRTOS 会将消息拷贝到消息队列队尾，否则，会根据用户指定的阻塞超时时间进行阻塞，在这段时间中，如果队列一直不允许入队，该任务将保持阻塞状态以等待队列允许入队。当其它任务从其等待的队列中读取入了数据（队列未满），该任务将自动由阻塞态转为就绪态。当任务等待的时间超过了指定的阻塞时间，即使队列中还不允许入队，任务也会自动从阻塞态转移为就绪态，此时发送消息的任务或者中断程序会收到一个错误码 `errQUEUE_FULL`。

发送紧急消息的过程与发送消息几乎一样，唯一的不同是，当发送紧急消息时，发送的位置是消息队列队头而非队尾，这样，接收者就能够优先接收到紧急消息，从而及时进行消息处理。

其实消息队列发送函数有好几个，都是使用宏定义进行展开的，有些只能在任务调用，有些只能在中断中调用，具体见下面讲解。

##### 5.6.4.1 xQueueSend() 与 xQueueSendToBack()

```c
#define xQueueSend( xQueue, pvItemToQueue, xTicksToWait ) \
					xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), \
									   ( xTicksToWait ), queueSEND_TO_BACK )
```

```c
#define xQueueSendToBack( xQueue, pvItemToQueue, xTicksToWait ) \
						  xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), \
											 ( xTicksToWait ), queueSEND_TO_BACK )
```

`xQueueSend()` 是一个宏，宏展开是调用函数 `xQueueGenericSend()`，这个函数在后面会详细讲解其实现过程。该宏是为了向后兼容没有包含 `xQueueSendToFront()` 和 `xQueueSendToBack()` 这两个宏的 FreeRTOS 版本。`xQueueSend()` 等同于 `xQueueSendToBack()`。

`xQueueSend()` 用于向队列尾部发送一个队列消息。消息以拷贝的形式入队，而不是以引用的形式。**该函数绝对不能在中断服务程序里面被调用，中断中必须使用带有中断保护功能的 `xQueueSendFromISR()` 来代替**。`xQueueSend()` 函数的具体说明见下面表格 ，应用实例具体见下面代码。

![image-20241107164117907](.assets/image-20241107164117907.png)

```c
static void Send_Task(void* parameter)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	uint32_t send_data1 = 1;
	uint32_t send_data2 = 2;
	while (1) {
		if ( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON ) {
			/* K1 被按下 */
			printf("发送消息send_data1！\n");
 			xReturn = xQueueSend( Test_Queue, /* 消息队列的句柄 */
								  &send_data1,/* 发送的消息内容 */
 								  0 ); /* 等待时间 0 */
			if (pdPASS == xReturn)
 				printf("消息send_data1 发送成功!\n\n");
 		}
 		if ( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON ) {
			/* K2 被按下 */
 			printf("发送消息send_data2！\n");
			xReturn = xQueueSend( Test_Queue, /* 消息队列的句柄 */
                                  &send_data2,/* 发送的消息内容 */
								  0 ); /* 等待时间 0 */
			if (pdPASS == xReturn)
				printf("消息send_data2 发送成功!\n\n");
		}
 		vTaskDelay(20);/* 延时20 个tick */
 	}
}
```

##### 5.6.4.2 xQueueSendFromISR() 与 xQueueSendToBackFromISR()

```c
#define xQueueSendToFrontFromISR(xQueue,pvItemToQueue,pxHigherPriorityTaskWoken) \
		xQueueGenericSendFromISR( ( xQueue ), ( pvItemToQueue ),\
								  ( pxHigherPriorityTaskWoken ), queueSEND_TO_FRONT )
```

`xQueueSendToBackFromISR` 等同于 `xQueueSendFromISR ()`。

```c
#define xQueueSendToBackFromISR(xQueue,pvItemToQueue,pxHigherPriorityTaskWoken) \
		xQueueGenericSendFromISR( ( xQueue ), ( pvItemToQueue ), \
							      ( pxHigherPriorityTaskWoken ), queueSEND_TO_BACK )
```

`xQueueSendFromISR()` 是一个宏，宏展开是调用函数 `xQueueGenericSendFromISR()`。该宏是 `xQueueSend()`的中断保护版本，用于在中断服务程序中向队列尾部发送一个队列消息，等价于 `xQueueSendToBackFromISR()`。`xQueueSendFromISR()`函数具体说明见下面表格，使用实例具体见下面代码。

![image-20241107164625002](.assets/image-20241107164625002.png)

```c
void vBufferISR( void )
{
	char cIn;
	BaseType_t xHigherPriorityTaskWoken;

	/* 在ISR 开始的时候，我们并没有唤醒任务 */
    xHigherPriorityTaskWoken = pdFALSE;

	/* 直到缓冲区为空 */
	do {
		/* 从缓冲区获取一个字节的数据 */
		cIn = portINPUT_BYTE( RX_REGISTER_ADDRESS );

		/* 发送这个数据 */
		xQueueSendFromISR( xRxQueue, &cIn, &xHigherPriorityTaskWoken );

	} while ( portINPUT_BYTE( BUFFER_COUNT ) );

	/* 这时候buffer 已经为空，如果需要则进行上下文切换 */
	if ( xHigherPriorityTaskWoken ) {
		/* 上下文切换，这是一个宏，不同的处理器，具体的方法不一样 */
		taskYIELD_FROM_ISR ();
	}
}
```

##### 5.6.4.3 xQueueSendToFront()

```c
#define xQueueSendToFront( xQueue, pvItemToQueue, xTicksToWait ) \
		xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), \
						   ( xTicksToWait ), queueSEND_TO_FRONT )
```

`xQueueSendToFron()` 是一个宏， 宏展开也是调用函数 `xQueueGenericSend()` 。`xQueueSendToFront()` 用于向队列队首发送一个消息。消息以拷贝的形式入队，而不是以引用的形式。**该函数绝不能在中断服务程序里面被调用，而是必须使用带有中断保护功能的 `xQueueSendToFrontFromISR ()` 来代替**。`xQueueSendToFron()` 函数的具体说明见下面表格，使用方式与 `xQueueSend()` 函数一致。

![image-20241107164949180](.assets/image-20241107164949180.png)

##### 5.6.4.4 xQueueSendToFrontFromISR()

```c
#define xQueueSendToFrontFromISR( xQueue,pvItemToQueue,pxHigherPriorityTaskWoken ) \
		xQueueGenericSendFromISR( ( xQueue ), ( pvItemToQueue ),
							      ( pxHigherPriorityTaskWoken ), queueSEND_TO_FRONT )
```

`xQueueSendToFrontFromISR()` 是一个宏， 宏展开是调用函数 `xQueueGenericSendFromISR()`。该宏是`xQueueSendToFront()` 的中断保护版本，用于在中断服务程序中向消息队列队首发送一个消息。`xQueueSendToFromISR()`函数具体说明见下面表格，使用方式与 `xQueueSendFromISR()` 函数一致。

![image-20241107165411369](.assets/image-20241107165411369.png)

##### 5.6.4.5 通用消息队列发送函数 xQueueGenericSend() (任务)

上面看到的那些在任务中发送消息的函数都是 `xQueueGenericSend()` 展开的宏定义，真正起作用的就是`xQueueGenericSend()` 函数，根据指定的参数不一样，发送消息的结果就不一样，下面一起看看任务级的通用消息队列发送函数的实现过程：

```

```

##### 5.6.4.6 消息队列发送函数 xQueueGenericSendFromISR()（中断）

既然有任务中发送消息的函数，当然也需要有在中断中发送消息函数，其实这个函数跟 `xQueueGenericSend()` 函数很像， 只不过是执行的上下文环境是不一样的，`xQueueGenericSendFromISR()` 函数只能用于中断中执行，是不带阻塞机制的。

```

```

#### 5.6.5 从消息队列读取消息函数

当任务试图读队列中的消息时，可以指定一个阻塞超时时间，当且仅当消息队列中有消息的时候，任务才能读取到消息。在这段时间中，如果队列为空，该任务将保持阻塞状态以等待队列数据有效。当其它任务或中断服务程序往其等待的队列中写入了数据，该任务将自动由阻塞态转为就绪态。当任务等待的时间超过了指定的阻塞时间，即使队列中尚无有效数据，任务也会自动从阻塞态转移为就绪态。

##### 5.6.5.1 xQueueReceive() 与 xQueuePeek()

```c
#define xQueueReceive( xQueue, pvBuffer, xTicksToWait ) \
		xQueueGenericReceive( ( xQueue ), ( pvBuffer ), \
							  ( xTicksToWait ), pdFALSE )
```

`xQueueReceive()` 是一个宏， 宏展开是调用函数 `xQueueGenericReceive()` 。`xQueueReceive()` 用于从一个队列中接收消息并把消息从队列中删除。接收的消息是以拷贝的形式进行的，所以我们必须提供一个足够大空间的缓冲区。具体能够拷贝多少数据到缓冲区，这个在队列创建的时候已经设定。该函数绝不能在中断服务程序里面被调用，而是必须使用带有中断保护功能的 `xQueueReceiveFromISR ()` 来代替。

![image-20241107170052869](.assets/image-20241107170052869.png)

使用示例：

```c
static void Receive_Task(void* parameter)
{
	BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdPASS */
	uint32_t r_queue; /* 定义一个接收消息的变量 */
	while (1) {
		xReturn = xQueueReceive( Test_Queue, /* 消息队列的句柄 */
								 &r_queue, /* 发送的消息内容 */
								 portMAX_DELAY); /* 等待时间 一直等 */
	if (pdTRUE== xReturn)
		printf("本次接收到的数据是：%d\n\n",r_queue);
	else
		printf("数据接收出错,错误代码: 0x%lx\n",xReturn);
	}
}
```

看到这里，有人就问了如果我接收了消息不想删除怎么办呢？其实，你能想到的东西，FreeRTOS 看到也想到了，如果不想删除消息的话，就调用 `xQueuePeek()` 函数。其实这个函数与 `xQueueReceive()` 函数的实现方式一样，连使用方法都一样，只不过 `xQueuePeek()` 函数接收消息完毕不会删除消息队列中的消息而已。

```c
#define xQueuePeek( xQueue, pvBuffer, xTicksToWait ) \
		xQueueGenericReceive( ( xQueue ), ( pvBuffer ), \
							  ( xTicksToWait ), pdTRUE )
```

##### 5.6.5.2 xQueueReceiveFromISR() 与 xQueuePeekFromISR()

`xQueueReceiveFromISR()` 是 `xQueueReceive ()` 的中断版本，用于在中断服务程序中接收一个队列消息并把消息从队列中删除；`xQueuePeekFromISR()` 是 `xQueuePeek()` 的中断版本，用于在中断中从一个队列中接收消息，但并不会把消息从队列中移除。

说白了这两个函数只能用于中断，是不带有阻塞机制的，并且是在中断中可以安全调用。

![image-20241107170547001](.assets/image-20241107170547001.png)

![image-20241107170559745](.assets/image-20241107170559745.png)

![image-20241107170616785](.assets/image-20241107170616785.png)

使用示例：

```c
QueueHandle_t xQueue;

/* 创建一个队列，并往队列里面发送一些数据 */
void vAFunction( void *pvParameters )
{
	char cValueToPost;
	const TickType_t xTicksToWait = ( TickType_t )0xff;

	/* 创建一个可以容纳10 个字符的队列 */
	xQueue = xQueueCreate( 10, sizeof( char ) );
	if ( xQueue == 0 ) {
		/* 队列创建失败 */
	}

	/* ... 任务其他代码 */
	
	/* 往队列里面发送两个字符如果队列满了则等待xTicksToWait 个系统节拍周期*/
	cValueToPost = 'a';
	xQueueSend( xQueue, ( void * ) &cValueToPost, xTicksToWait );
    cValueToPost = 'b';
	xQueueSend( xQueue, ( void * ) &cValueToPost, xTicksToWait );
    
	/* 继续往队列里面发送字符当队列满的时候该任务将被阻塞*/
	cValueToPost = 'c';
	xQueueSend( xQueue, ( void * ) &cValueToPost, xTicksToWait );
}

/* 中断服务程序：输出所有从队列中接收到的字符 */
void vISR_Routine( void )
{
	BaseType_t xTaskWokenByReceive = pdFALSE;
	char cRxedChar;

	while ( xQueueReceiveFromISR( xQueue,
                                 ( void * ) &cRxedChar,
                                 &xTaskWokenByReceive) ) {

		/* 接收到一个字符，然后输出这个字符 */
		vOutputCharacter( cRxedChar );
	
		/* 如果从队列移除一个字符串后唤醒了向此队列投递字符的任务，
	   	   那么参数xTaskWokenByReceive 将会设置成pdTRUE，这个循环无论重复多少次，
       	   仅会有一个任务被唤醒 */
	}

	if ( xTaskWokenByReceive != pdFALSE ) {
	/* 我们应该进行一次上下文切换，当ISR 返回的时候则执行另外一个任务 */
    	/* 这是一个上下文切换的宏，不同的处理器，具体处理的方式不一样 */
		taskYIELD ();
	}
}
```

##### 5.6.5.3 从队列读取消息函数 xQueueGenericReceive()

由于在中断中接收消息的函数用的并不多，我们只讲解在任务中读取消息的函数——`xQueueGenericReceive()`

```

```

### 5.7 消息队列使用注意事项

在使用 FreeRTOS 提供的消息队列函数的时候，需要了解以下几点：
1. 使用 `xQueueSend()`、`xQueueSendFromISR()`、`xQueueReceive()` 等这些函数之前应先创建需消息队列，并根据队列句柄进行操作。
2. 队列读取采用的是先进先出（FIFO）模式，会先读取先存储在队列中的数据。当然也 FreeRTOS 也支持后进先出（LIFO）模式，那么读取的时候就会读取到后进队列的数据。
3. 在获取队列中的消息时候，我们必须要定义一个存储读取数据的地方，并且该数据区域大小不小于消息大小，否则，很可能引发地址非法的错误。
4. 无论是发送或者是接收消息都是以拷贝的方式进行，如果消息过于庞大，可以将消息的地址作为消息进行发送、接收。
5. 队列是具有自己独立权限的内核对象，并不属于任何任务。所有任务都可以向同一队列写入和读出。一个队列由多任务或中断写入是经常的事，但由多个任务读出倒是用的比较少。

### 5.7 消息队列实验

消息队列实验是在 FreeRTOS 中创建了两个任务，一个是发送消息任务，一个是获取消息任务，两个任务独立运行，发送消息任务是通过检测按键的按下情况来发送消息，假如发送消息不成功，就把返回的错误情代码在串口打印出来，另一个任务是获取消息任务，在消息队列没有消息之前一直等待消息，一旦获取到消息就把消息打印在串口调试助手里。

```c
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_config.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define QUEUE_LEN  4
#define QUEUE_SIZE 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static TaskHandle_t APPCreate_Handle = NULL;
static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t Key0_Task_Handle = NULL;

QueueHandle_t Test_Queue = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void LED1_Task(void* argument)
{
	BaseType_t xReturn = pdTRUE;
	uint32_t r_queue;

	while(1)
    {
		xReturn = xQueueReceive(Test_Queue, &r_queue, portMAX_DELAY);
		if (pdTRUE == xReturn) {
			printf("LED1 Task received data: %d.\r\n", r_queue);
			__BSP_LED1_Ficker(100);
		}
		else {
			printf("LED1 Task receive data failed. %d\r\n", xReturn);
		}
		vTaskDelay(20);
    }
}

static void Key0_Task(void *argument)
{
	BaseType_t xReturn = pdTRUE;
	uint32_t send_data1 = 1;
	uint32_t send_data2 = 2;

	while (1)
    {
        if (BSP_KEY_Read(0) ==  GPIO_PIN_RESET)
        {
            printf("Send data: %d.\r\n", send_data1);
			xReturn = xQueueSend(Test_Queue, &send_data1, portMAX_DELAY);
        }
		else if (BSP_KEY_Read(1) ==  GPIO_PIN_RESET)
        {
            printf("Send data: %d.\r\n", send_data2);
			xReturn = xQueueSend(Test_Queue, &send_data2, portMAX_DELAY);
        }
        vTaskDelay(1000);
    }
}

static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();

	Test_Queue = xQueueCreate((UBaseType_t)QUEUE_LEN,
							  (UBaseType_t)QUEUE_SIZE);

	if (NULL != Test_Queue)
		printf("Test Queue created successfully.\r\n");

	xReturn = xTaskCreate(LED1_Task,
                          "LED1_Task",
                          256,      
                          NULL,
                          4,
                          &LED1_Task_Handle);

    if (pdPASS == xReturn)
		printf("LED1 Task created successfully.\r\n");

	xReturn = xTaskCreate(Key0_Task,
                          "Key0_Task",
                          256,
                          NULL,
                          3,
                          &Key0_Task_Handle);
	
	if (pdPASS == xReturn)
		printf("Key0 Task created successfully.\r\n\n");

    vTaskDelete(NULL);

    taskEXIT_CRITICAL();
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  BaseType_t xReturn = pdPASS;

  BSP_LED_Init();
  BSP_KEY_Init();

  printf("FreeRTOS Demo Application.\r\n");

  xReturn = xTaskCreate(AppTaskCreate,
                        "AppTaskCreate",
                        512,
                        NULL,
                        1,
                        &APPCreate_Handle);
  if (pdPASS == xReturn)
  {
      printf("AppTaskCreate created successfully.\r\n");
      vTaskStartScheduler();
  }
  else
      return -1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
```

## 六. 信号量

回想一下，是否在裸机编程中这样使用过一个变量：用于标记某个事件是否发生，或者标志一下某个东西是否正在被使用，如果是被占用了的或者没发生，我们就不对它进行操作。

### 6.1 信号量基本概念

信号量（Semaphore）是一种实现任务间通信的机制，可以实现任务之间同步或临界资源的互斥访问，常用于协助一组相互竞争的任务来访问临界资源。在多任务系统中，各任务之间需要同步或互斥实现临界资源的保护，信号量功能可以为用户提供这方面的支持。

抽象的来讲，信号量是一个非负整数，所有获取它的任务都会将该整数减一（获取它当然是为了使用资源），当该整数值为零时，所有试图获取它的任务都将处于阻塞状态。通常一个信号量的计数值用于对应有效的资源数，表示剩下的可被占用的互斥资源数。其值的含义分两种情况：

- 0：表示没有积累下来的释放信号量操作，且有可能有在此信号量上阻塞的任务。、
- 正值，表示有一个或多个释放信号量操作。

#### 6.1.1 二值信号量

二值信号量既可以用于临界资源访问也可以用于同步功能。

二值信号量和互斥信号量（以下使用互斥量表示互斥信号量）非常相似，但是有一些细微差别：**互斥量有优先级继承机制**，二值信号量则没有这个机制。这使得**二值信号量更偏向应用于同步功能**（任务与任务间的同步或任务和中断间同步），而**互斥量更偏向应用于临界资源的访问**。

用作同步时，信号量在创建后应被置为空，任务 1 获取信号量而进入阻塞，任务 2 在某种条件发生后，释放信号量，于是任务 1 获得信号量得以进入就绪态，如果任务 1 的优先级是最高的，那么就会立即切换任务，从而达到了两个任务间的同步。同样的，在中断服务函数中释放信号量，任务 1 也会得到信号量，从而达到任务与中断间的同步。

还记得我们经常说的中断要快进快出吗，在裸机开发中我们经常是在中断中做一个标记，然后在退出的时候进行轮询处理，这个就是类似我们使用信号量进行同步的，当标记发生了，我们再做其他事情。在 FreeRTOS 中我们用信号量用于同步，任务与任务的同步，中断与任务的同步，可以大大提高效率。

可以将二值信号量看作只有一个消息的队列，因此这个队列只能为空或满（因此称为二值），我们**在运用的时候只需要知道队列中是否有消息即可，而无需关注消息是什么。**

#### 6.1.2 计数信号量

二进制信号量可以被认为是长度为 1 的队列，而计数信号量则可以被认为长度大于1的队列，信号量使用者依然不必关心存储在队列中的消息，只需关心队列是否有消息即可。

顾名思义，计数信号量肯定是用于计数的，在实际的使用中，我们常将计数信号量用于事件计数与资源管理。每当某个事件发生时，任务或者中断将释放一个信号量（信号量计数值加1），当处理被事件时（一般在任务中处理），处理任务会取走该信号量（信号量计数值减1），信号量的计数值则表示还有多少个事件没被处理。此外，系统还有很多资源，我们也可以使用计数信号量进行资源管理，信号量的计数值表示系统中可用的资源数目，任务必须先获取到信号量才能获取资源访问权，当信号量的计数值为零时表示系统没有可用的资源，但是要注意，在使用完资源的时候必须归还信号量，否则当计数值为 0 的时候任务就无法访问该资源了。

计数型信号量允许多个任务对其进行操作，但限制了任务的数量。比如有一个停车场，里面只有 100 个车位，那么能停的车只有 100 辆，也相当于我们的信号量有 100 个，假如一开始停车场的车位还有 100 个，那么每进去一辆车就要消耗一个停车位，车位的数量就要减一，对应的，我们的信号量在使用之后也需要减一，当停车场停满了 100 辆车的时候，此时的停车位为0，再来的车就不能停进去了，否则将造成事故，也相当于我们的信号量为0，后面的任务对这个停车场资源的访问也无法进行，当有车从停车场离开的时候，车位又空余出来了，那么，后面的车就能停进去了，我们信号量的操作也是一样的，当我们释放了这个资源，后面的任务才能对这个资源进行访问。

#### 6.1.3 互斥信号量

互斥信号量其实是特殊的二值信号量，由于其特有的优先级继承机制从而使它更适用于简单互锁，也就是保护临界资源（什么是优先级继承在后续相信讲解）。

用作互斥时，信号量创建后可用信号量个数应该是满的，任务在需要使用临界资源时，（临界资源是指任何时刻只能被一个任务访问的资源），先获取互斥信号量，使其变空，这样其他任务需要使用临界资源时就会因为无法获取信号量而进入阻塞，从而保证了临界资源的安全。

在操作系统中，我们使用信号量的很多时候是为了给临界资源建立一个标志，信号量表示了该临界资源被占用情况。这样，当一个任务在访问临界资源的时候，就会先对这个资源信息进行查询，从而在了解资源被占用的情况之后，再做处理，从而使得临界资源得到有效的保护。

#### 6.1.4 递归信号量

递归信号量，见文知义，递归嘛，就是可以重复获取调用的，本来按照信号量的特性，每获取一次可用信号量个数就会减少一个，但是递归则不然，对于已经获取递归互斥量的任务可以重复获取该递归互斥量，该任务拥有递归信号量的所有权。任务成功获取几次递归互斥量，就要返还几次，在此之前递归互斥量都处于无效状态，其他任务无法获取，只有持有递归信号量的任务才能获取与释放。

### 6.2 二值信号量应用场景

在嵌入式操作系统中二值信号量是任务间、任务与中断间同步的重要手段，信号量使用最多的一般都是二值信号量与互斥信号量（互斥信号量在下一章讲解）。为什么叫二值信号量呢？因为信号量资源被获取了，信号量值就是 0，信号量资源被释放，信号量值就是 1，把这种只有 0 和 1 两种情况的信号量称之为二值信号量。

在多任务系统中，我们经常会使用这个二值信号量，比如，某个任务需要等待一个标记，那么任务可以在轮询中查询这个标记有没有被置位，但是这样子做，就会很消耗 CPU 资源并且妨碍其它任务执行，更好的做法是任务的大部分时间处于阻塞状态（允许其它任务执行），直到某些事件发生该任务才被唤醒去执行。可以使用二进制信号量实现这种同步，当任务取信号量时，因为此时尚未发生特定事件，信号量为空，任务会进入阻塞状态；当事件的条件满足后，任务/中断便会释放信号量，告知任务这个事件发生了，任务取得信号量便被唤醒去执行对应的操作，任务执行完毕并不需要归还信号量，这样子的 CPU 的效率可以大大提高，而且实时响应也是最快的。

再比如某个任务使用信号量在等中断的标记的发生，在这之前任务已经进入了阻塞态，在等待着中断的发生，当在中断发生之后，释放一个信号量，也就是我们常说的标记，当它退出中断之后，操作系统会进行任务的调度，如果这个任务能够运行，系统就会把等待这个任务运行起来，这样子就大大提高了我们的效率。

二值信号量在任务与任务中同步的应用场景：假设我们有一个温湿度的传感器，假设是1s 采集一次数据，那么我们让他在液晶屏中显示数据出来，这个周期也是要1s 一次的，如果液晶屏刷新的周期是100ms 更新一次，那么此时的温湿度的数据还没更新，液晶屏根本无需刷新，只需要在1s 后温湿度数据更新的时候刷新即可，否则 CPU 就是白白做了多次的无效数据更新，CPU 的资源就被刷新数据这个任务占用了大半，造成CPU 资源浪费，如果液晶屏刷新的周期是10s 更新一次，那么温湿度的数据都变化了10 次，液晶屏才来更新数据，那拿这个产品有啥用，根本就是不准确的，所以，还是需要同步协调工作，在温湿度采集完毕之后，进行液晶屏数据的刷新，这样子，才是最准确的，并且不会浪费CPU的资源。

同理，二值信号量在任务与中断同步的应用场景：我们在串口接收中，我们不知道啥时候有数据发送过来，有一个任务是做接收这些数据处理，总不能在任务中每时每刻都在任务查询有没有数据到来，那样会浪费CPU 资源，所以在这种情况下使用二值信号量是很好的办法，当没有数据到来的时候，任务就进入阻塞态，不参与任务的调度，等到数据到来了，释放一个二值信号量，任务就立即从阻塞态中解除，进入就绪态，然后运行的时候处理数据，这样子系统的资源就会很好的被利用起来。

### 6.3 二值信号量运作机制

创建信号量时，系统会为创建的信号量对象分配内存，并把可用信号量初始化为用户自定义的个数， 二值信号量的最大可用信号量个数为1。

二值信号量获取，任何任务都可以从创建的二值信号量资源中获取一个二值信号量，获取成功则返回正确，否则任务会根据用户指定的阻塞超时时间来等待其它任务/中断释放信号量。在等待这段时间，系统将任务变成阻塞态，任务将被挂到该信号量的阻塞等待列表中。

在二值信号量无效的时候，假如此时有任务获取该信号量的话，那么任务将进入阻塞状态，具体见下图：

![image-20241107184310757](.assets/image-20241107184310757.png)

假如某个时间中断/任务释放了信号量，其过程具体见下图，那么，由于获取无效信号量而进入阻塞态的任务将获得信号量并且恢复为就绪态，其过程具体见下图：

![image-20241107184334677](.assets/image-20241107184334677.png)

![image-20241107184341244](.assets/image-20241107184341244.png)



### 6.4 计数信号量运作机制

计数信号量可以用于资源管理，允许多个任务获取信号量访问共享资源，但会限制任务的最大数目。访问的任务数达到可支持的最大数目时，会阻塞其他试图获取该信号量的任务，直到有任务释放了信号量。这就是计数型信号量的运作机制，虽然计数信号量允许多个任务访问同一个资源，但是也有限定，比如某个资源限定只能有3 个任务访问，那么第4 个任务访问的时候，会因为获取不到信号量而进入阻塞，等到有任务（比如任务1）释放掉该资源的时候，第4 个任务才能获取到信号量从而进行资源的访问，其运作的机制具体见下图：

![image-20241107184446961](.assets/image-20241107184446961.png)

### 6.5 信号量控制块

信号量 API 函数实际上都是宏，它使用现有的队列机制，这些宏定义在 `semphr.h` 文件中，如果使用信号量或者互斥量，需要包含 `semphr.h` 头文件。所以 FreeRTOS 的信号量控制块结构体与消息队列结构体是一模一样的，只不过结构体中某些成员变量代表的含义不一样而已，我们会具体讲解一下哪里与消息队列不一样。先来看看信号量控制块，具体见下面代码：

```c
typedef struct QueueDefinition {
	int8_t *pcHead;
	int8_t *pcTail;
	int8_t *pcWriteTo;

	union {
		int8_t *pcReadFrom;
		UBaseType_t uxRecursiveCallCount;
	} u;

	List_t xTasksWaitingToSend;
	List_t xTasksWaitingToReceive;

	volatile UBaseType_t uxMessagesWaiting; (1)
	UBaseType_t uxLength; (2)
 	UBaseType_t uxItemSize; (3)

 	volatile int8_t cRxLock;
	volatile int8_t cTxLock;

#if( ( configSUPPORT_STATIC_ALLOCATION == 1 )&&( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
 	uint8_t ucStaticallyAllocated;
#endif

#if ( configUSE_QUEUE_SETS == 1 )
 	struct QueueDefinition *pxQueueSetContainer;
#endif

#if ( configUSE_TRACE_FACILITY == 1 )
 	UBaseType_t uxQueueNumber;
 	uint8_t ucQueueType;
#endif

} xQUEUE;

typedef xQUEUE Queue_t;
```

1. 如果控制块结构体是用于消息队列：`uxMessagesWaiting` 用来记录当前消息队列的消息个数；如果控制块结构体被用于信号量的时候，这个值就表示有效信号量个数，有以下两种情况：
    - 如果信号量是二值信号量、互斥信号量，这个值是1 则表示有可用信号量，如果是 0 则表示没有可用信号量。
    - 如果是计数信号量，这个值表示可用的信号量个数，在创建计数信号量的时候会被初始化一个可用信号量个数 `uxInitialCount`，最大不允许超过创建信号量的初始值 `uxMaxCount`。
2. 如果控制块结构体是用于消息队列：`uxLength` 表示队列的长度，也就是能存放多少消息；如果控制块结构体被用于信号量的时候，`uxLength` 表示最大的信号量可用个数，会有以下两种情况：
    - 如果信号量是二值信号量、互斥信号量，`uxLength` 最大为1，因为信号量要么是有效的，要么是无效的。
    - 如果是计数信号量，这个值表示最大的信号量个数，在创建计数信号量的时候将由用户指定这个值`uxMaxCount`。
3. 如果控制块结构体是用于消息队列：`uxItemSize` 表示单个消息的大小；如果控制块结构体被用于信号量的时候，则无需存储空间，为 0 即可。

### 6.6 常用信号量函数接口讲解

#### 6.6.1 创建信号量函数

##### 1. 创建二值信号量 xSemaphoreCreateBinary()

`xSemaphoreCreateBinary()` 用于创建一个二值信号量，并返回一个句柄。其实二值信号量和互斥量都共同使用一个类型 `SemaphoreHandle_t` 的句柄（.h 文件79 行），该句柄的原型是一个 `void` 型的指针。使用该函数创建的二值信号量是空的， 在使用函数 `xSemaphoreTake()` 获取之前必须先调用函数 `xSemaphoreGive()` 释放后才可以获取。如果是使用老式的函数 `vSemaphoreCreateBinary()` 创建的二值信号量，则为1，在使用之前不用先释放。要想使用该函数必须在 `FreeRTOSConfig.h` 中把宏 `configSUPPORT_DYNAMIC_ALLOCATION` 定义为1，即开启动态内存分配。其实该宏在 `FreeRTOS.h` 中默认定义为1，即所有 FreeRTOS 的对象在创建的时候都默认使用动态内存分配方案。











## OTHER

#### STM32 + FreeRTOS + Cpp 混合编程

>  参考：[stm32 + freertos + c++混合编程方法](https://blog.csdn.net/yuhu0012/article/details/136510811)



#### 通过预定义宏根据编译器和其版本来编写条件编译的代码

在 C/C++ 中，可以使用预定义宏（如 `__xx__`）来识别当前使用的编译器及其版本。每个编译器通常都有一组特定的预定义宏，以下是一些常见编译器及其相应的预定义宏示例：

1. **GCC (GNU Compiler Collection)**：

    - `__GNUC__`：主版本号

    - `__GNUC_MINOR__`：次版本号

    - `__GNUC_PATCHLEVEL__`：修订号

    - 示例：

        ```c
        #if defined(__GNUC__)
        #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
        #endif
        ```

2. **MSVC (Microsoft Visual C++)**：

    - `_MSC_VER`：表示版本号，例如：1920表示 Visual Studio 2019。

    - 示例：

        ```c
        #ifdef _MSC_VER
        #if _MSC_VER >= 1920 // Visual Studio 2019
        // 代码
        #endif
        #endif
        ```

3. **Clang**：

    - `__clang__`：定义为 Clang 编译器存在时。

    - `__clang_major__`、`__clang_minor__`、`__clang_patchlevel__`：版本信息。

    - 示例：

        ```c
        #ifdef __clang__
        #define CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
        #endif
        ```

4. **Keil**：

    - `__ARMCC_VERSION`：Keil ARM 编译器的版本信息。

    - 示例：

        ```c
        #ifdef __ARMCC_VERSION
        // 代码
        #endif
        ```

使用这些宏，可以根据编译器的版本执行不同的代码。示例如下：

```c
#if defined(__GNUC__)
    // GCC-specific code
#elif defined(_MSC_VER)
    // MSVC-specific code
#elif defined(__clang__)
    // Clang-specific code
#elif defined(__ARMCC_VERSION)
    // Keil-specific code
#else
    #error "Unsupported compiler"
#endif
```

通过这种方式，你可以根据编译器和其版本来编写条件编译的代码，从而确保代码在不同编译环境下的兼容性。



## ERROR LOG

#### 单独一个静态任务运行卡死

- [x] 2024/11/1 17:50

解决方法：

1. 将 `AppTaskCreate` 的任务优先级设置为  7 以上，将 `LED_Task` 优先级设置为 8 以上， `LED_Task` 优先级需大于等于 `AppTaskCreate` 的任务优先级。
2. 将创建任务时的堆栈大小增大为 256，即 256 * 4 字节。

主要与任务优先级有关，通常是由于 FreeRTOS 的任务调度和任务创建顺序产生的：

- **优先级反转或任务“饿死”**

    当 `AppTaskCreate` 和 `LED_Task` 的优先级设置较低时，空闲任务或其他较高优先级的任务可能会占用系统的时间片，导致低优先级任务得不到调度。

    如果将 `AppTaskCreate` 设置为低优先级，而 `LED_Task` 的优先级相同或更低，系统可能会陷入一个状态：调度器始终等待高优先级任务先完成，而不会执行低优先级任务。这种情况被称为**任务饿死**（starvation）。

- **任务的创建顺序和资源竞争**

    在 FreeRTOS 中，任务的创建和优先级会影响它们的调度。因为 `LED_Task` 是在 `AppTaskCreate` 中创建的，确保 `AppTaskCreate` 先被调度执行才能成功创建 `LED_Task`。

    将 `AppTaskCreate` 设置为较高优先级可以确保它立即获得调度机会，创建 `LED_Task` 并退出。这解释了为什么只有在 `AppTaskCreate` 优先级较高时才能创建成功。

-  **任务切换的触发条件**

    由于 FreeRTOS 的任务调度依赖于任务切换指令或延时函数（如 `vTaskDelay` 或 `vTaskDelayUntil`），较低优先级的任务在等待调度时可能无法及时切换。如果 `AppTaskCreate` 和 `LED_Task` 的优先级低于空闲任务，它们的执行可能会受到影响。确保 `AppTaskCreate` 和 `LED_Task` 优先级足够高，可以在 `vTaskStartScheduler` 后立即调度。
    
    

#### Keil AC5 编译器对于宏函数的嵌套语句块语法报错

- [x] 2024/11/02 12:18

**原定义：**

```c
#define __BSP_LED1_Ficker(__TIME__) ({	__BSP_LED1_Toggle(); 		\
										vTaskDelay(__TIME__); 	    \
										__BSP_LED1_Toggle(); 		\
										vTaskDelay(__TIME__); })
```

**报错：**

```bash
Error:  #29: expected an expression
```

**原因：**

定义的宏函数使用了 GCC 的语法（即 “ 嵌套语句块 ” ），这在某些编译器中可能会引起问题。Keil AC5 可能不支持这种语法，因此出现了“expected an expression”的错误。

**解决方法：**

将宏函数的定义更改为 `do {...} while(0)` 的结构，如：

```c
#define __BSP_LED1_Ficker(__TIME__) do {	\
	__BSP_LED1_Toggle(); 		            \
	vTaskDelay(__TIME__); 	                \
	__BSP_LED1_Toggle(); 		            \
	vTaskDelay(__TIME__);                   \
	} while(0)
```

使用 `do { ... } while(0)` 结构作为宏定义的方式，有几个优点和原因：

1. **兼容性**：这种结构在不同的编译器中通常更兼容。某些编译器可能对带有嵌套语句块的宏处理不当，而 `do { ... } while(0)` 结构是标准的 C 语法。
2. **单个语句效果**：使用 `do { ... } while(0)` 可以将整个宏定义视为一个单一的语句。在使用宏的地方，即使宏内部有多条语句，调用时也不会引起语法错误。这可以防止在使用宏时出现意外的分号或其他语法问题。
3. **代码可读性**：将整个宏包裹在 `do { ... } while(0)` 中，可以提高代码的可读性和维护性。其他程序员在查看代码时，更容易理解这是一个完整的操作。
4. **避免副作用**：如果在宏中使用了表达式，`do { ... } while(0)` 可以有效地限制其作用域，避免在多次调用时可能导致的副作用。

因此，推荐在定义宏时使用这种结构，特别是在涉及多条语句的情况下。这可以提高代码的可靠性和可移植性。
