# ERROR LOG

> 该文档记录工程中所遇到的报错及解决方案

- [x] 2024/10/06 2:18

```
Error: L6218E: Undefined symbol Image$$ARM_LIB_STACK$$ZI$$Limit (referred from startup_armcm4.o).
```

<img src=".assets/image-20241006022211406.png" alt="image-20241006022211406" style="zoom:50%;" />

参考链接：https://blog.csdn.net/luofeng66ok/article/details/104869417

---

- [x] 2024/10/08 17:24

```
__asm 汇编和C混合代码报错
```

![image-20241008215818931](.assets/image-20241008215818931.png)

参考链接：https://blog.csdn.net/u010058695/article/details/114529090

---

- [x] 2024/10/09 11:58

```
error: invalid operand for instruction
<inline asm>(1): note: instantiated into assembly here
    1 |         dsb portSY_FULL_READ_WRITE
      |             ^~~~~~~~~
```

**问题：** 在当前的内联汇编写法中：

```c
__asm("dsb portSY_FULL_READ_WRITE");
__asm("isb portSY_FULL_READ_WRITE");
```

`portSY_FULL_READ_WRITE` 作为一个宏变量直接插入汇编指令中，这不符合 ARM 内联汇编的语法。内联汇编通常需要直接使用常量或者通过占位符将 C 语言中的宏或者变量传递给汇编指令。

**解决方案：**应该将 `portSY_FULL_READ_WRITE` 替换为具体的立即数，或者通过占位符将其插入内联汇编中。改写后的正确写法如下：

```C
#define portSY_FULL_READ_WRITE (15)

#define portYIELD()\
{\
    /* 触发 PendSV, 产生上下文切换 */\
    portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
    __asm volatile ("dsb %0" :: "i" (portSY_FULL_READ_WRITE));\
    __asm volatile ("isb %0" :: "i" (portSY_FULL_READ_WRITE));\
}
```

**解释：**

1. `__asm volatile`：确保编译器不会优化掉这段汇编代码。
2. `"dsb %0"`：`%0` 是占位符，用于在内联汇编中插入后面的立即数。
3. `"i" (portSY_FULL_READ_WRITE)`：`"i"` 表示这是一个立即数，将 `portSY_FULL_READ_WRITE` 替换为 `0xF` 传递给汇编指令。

---

- [x] 2024/10/09 12:04

```
error: invalid instruction
   60 |     __asm("PRESERVE8");
      |           ^
<inline asm>(1): note: instantiated into assembly here
    1 |         PRESERVE8
      |         ^~~~~~~~~
```

**`PRESERVE8` 指令的定义**：`PRESERVE8` 是 ARM 架构中用于确保使用 8 字节对齐的指令，常用于保护调用约定。它应该在适当的上下文中使用，通常是在某些编译器和架构的支持下。

**错误原因：**`ARM v6.21` 编译器不支持该语法

**解决方案：** 替换为以下写法

```C
__asm volatile(".p2align 3");
```

---

- [x] 2024/10/09 12:11

```
error: unsupported relocation type
  131 |     __asm("mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY");
      |           ^
<inline asm>(1): note: instantiated into assembly here
    1 |         mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
      |         ^
```

**立即数问题**：ARM 汇编中的 `mov` 指令需要一个具体的立即数，而不是一个宏或变量。`configMAX_SYSCALL_INTERRUPT_PRIORITY` 是在 C 语言中定义的宏，它的值在编译时会被替换，但直接在汇编指令中使用可能导致编译器无法生成正确的指令。

**汇编指令的限制**：某些汇编指令在处理立即数时有特定的格式要求。直接使用宏作为立即数会导致编译器无法解析。

**解决方案：**使用占位符替换宏，通过内联汇编中的占位符传递宏的值，这样编译器在处理时可以将宏的值替换到汇编指令中。修改代码如下：

```C
__asm volatile ("mov r0, %0" :: "i"(configMAX_SYSCALL_INTERRUPT_PRIORITY));
```

