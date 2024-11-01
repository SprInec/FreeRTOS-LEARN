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

> **NOTE** ：
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

> libstm 库的 bsp_config.h 头文件同样兼容了 CMSIS Configuration Wizard，支持可视化配置，在 bsp_config.h 的 RTOS 选项中选择 FreeRTOS 即可自动包含 FreeRTOS 的相关头文件。 

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







## ERROR LOG

### 单独一个静态任务运行卡死

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
