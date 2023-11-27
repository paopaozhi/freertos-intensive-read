# GCC链接文件

在嵌入式系统中，使用ARM GCC（GNU Compiler Collection）时，链接器文件（linker script）对于配置程序的内存布局和分配是至关重要的。链接器文件告诉链接器如何组织程序的各个部分，包括代码、数据、堆栈等，以及它们在内存中的位置。

## 链接文件详解

对于链接文件其实主要一个功能就是指明程序链接的位置。

对于嵌入式的链接文件往往是复杂的，但是对于用户来说，其实并不需要过于关系，因为对于普通的开发者来说只需要根据模板链接文件进行部分简单的更改，所以，这里只对部分简单且重要的部分进行解释说明。

### 重要字段

`ENTRY(Reset_Handler)`: 指定程序的入口地址为Reset_Handler，该函数在启动文件中，作为上电运行的第一个函数。

`MEMORY`: 定义了程序所用到的内存区域，包括RAM和Flash。在这里，RAM起始地址为0x20000000，长度为20K，Flash起始地址为0x8000000，长度为128K。

`SECTIONS`: 定义了不同的代码和数据段以及它们在内存中的放置。具体的内容包括：

`.isr_vector`: 向量表的存放位置，通常在Flash中。

`.text`: 存放代码的位置，包括代码、只读数据以及初始化代码段。

`.rodata`: 存放只读数据的位置。

`.data`: 存放初始化数据的位置，这里通过`AT> FLASH`表示数据同时在`RAM`和`Flash`中，但在运行时使用`RAM`中的数据。

`.ARM.extab` 和 `.ARM`: 用于异常处理。

`.bss`: 存放未初始化数据的位置。

`._user_heap_stack`: 用于检查RAM是否足够，包括堆和栈的定义。
`PROVIDE`语句用于定义一些全局符号，如`end`和`_end`，这些符号在程序中可以使用。

`.ARM.attributes`: 存放ARM特有的属性信息。

`/DISCARD/`: 从编译器库中删除特定的部分，以减小生成的可执行文件的大小。

### RAM和FLASH定义

```
ENTRY(Reset_Handler)

/* 用户模式堆栈的最高地址 */
_estack = ORIGIN(RAM) + LENGTH(RAM); /* end of "RAM" Ram type memory */

_Min_Heap_Size = 0xC00; /* required amount of heap */
_Min_Stack_Size = 0x400; /* required amount of stack */

/* Memories definition */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 20K
  FLASH    (rx)    : ORIGIN = 0x8000000,   LENGTH = 128K
}
```

- `ENTRY(Reset_Handler)`: 定义程序的入口点为`Reset_Handler`，这是程序启动时执行的第一个函数。这通常包含了一些硬件初始化和启动代码。
- `_estack = ORIGIN(RAM) + LENGTH(RAM);`: 定义用户模式堆栈的最高地址，即RAM的结束地址。这个地址通常用作堆栈的起始地址。
- `_Min_Heap_Size`和`_Min_Stack_Size`: 定义了程序运行所需的最小堆和栈的大小。这对于检查系统是否有足够的RAM可用是很有用的。
- `MEMORY`部分定义了两个内存区域，一个是RAM，一个是FLASH。每个区域都有一个起始地址（`ORIGIN`）和一个长度（`LENGTH`）。

### 中断向量表

```c
.isr_vector :
{
. = ALIGN(4);
KEEP(*(.isr_vector)) /* Startup code */
. = ALIGN(4);
} >FLASH
```

- `.isr_vector`: 这个段定义了中断向量表，它包含程序启动时将执行的中断服务例程。这通常存储在Flash中。

### text段

```c
.text :
{
  . = ALIGN(4);
  *(.text)           /* .text sections (code) */
  *(.text*)          /* .text* sections (code) */
  *(.glue_7)         /* glue arm to thumb code */
  *(.glue_7t)        /* glue thumb to arm code */
  *(.eh_frame)

  KEEP (*(.init))
  KEEP (*(.fini))

  . = ALIGN(4);
  _etext = .;        /* define a global symbols at end of code */
} >FLASH
```

- `.text`: 这个段包含了程序的代码部分，包括标准的代码段（`.text`）和汇编代码段（`*(.glue_7)` `*(.glue_7t)`）。

```c
.rodata :
{
  . = ALIGN(4);
  *(.rodata)         /* .rodata sections (constants, strings, etc.) */
  *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  . = ALIGN(4);
} >FLASH
```

- `.rodata`: 这个段通常包含只读数据，如常量和字符串。

```c
.data :
{	
  . = ALIGN(4);
  _sdata = .;        /* create a global symbol at data start */
  *(.data)           /* .data sections */
  *(.data*)          /* .data* sections */
  *(.RamFunc)        /* .RamFunc sections */
  *(.RamFunc*)       /* .RamFunc* sections */

  . = ALIGN(4);
  _edata = .;        /* define a global symbol at data end */
} >RAM AT> FLASH
```

- `.data`: 这个段包含了初始化数据，如全局变量的初始值。`AT> FLASH`表示数据同时存在于RAM和Flash中，但在运行时使用RAM中的数据。

```c
.bss :
{
  /* This is used by the startup in order to initialize the .bss section */
  _sbss = .;         /* define a global symbol at bss start */
  __bss_start__ = _sbss;
  *(.bss)
  *(.bss*)
  *(COMMON)

  . = ALIGN(4);
  _ebss = .;         /* define a global symbol at bss end */
  __bss_end__ = _ebss;
} >RAM
```

- `.bss`: 这个段包含未初始化的全局变量。在程序启动时，这个段会被清零。

```c
._user_heap_stack :
{
  . = ALIGN(8);
  PROVIDE ( end = . );
  PROVIDE ( _end = . );
  . = . + _Min_Heap_Size;
  . = . + _Min_Stack_Size;
  . = ALIGN(8);
} >RAM
```

- `._user_heap_stack`: 这个段用于检查是否有足够的RAM剩余，包括堆和栈的定义。

```c
/DISCARD/ :
{
  libc.a ( * )
  libm.a ( * )
  libgcc.a ( * )
}
```

- `/DISCARD/`: 这个部分从编译器库中删除一些信息，以减小生成的可执行文件的大小。通常，这些信息在程序运行时并不需要，因为它们已经被包含在了启动文件中。

```c
.ARM.attributes 0 : { *(.ARM.attributes) }
```

- `.ARM.attributes`: 这个段包含了ARM体系结构的一些特殊属性信息。

这只是对你的链接器脚本的简要解释。确保根据你的具体需求和目标平台进行调整。
