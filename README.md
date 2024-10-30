# FreeRTOS-NANO
FreeRTOS learning.

> 参考：[野火]《FreeRTOS 内核实现与应用开发实战指南》

## 一. 移植 FreeRTOS 到 STM32

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

### 3. 往裸机工程添加 FreeRTOS 源码

#### 3.1 提取 FreeRTOS 最简源码

