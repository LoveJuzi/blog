# make 基础

## makefile的作用

makefile定义一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行其他更复杂的功能操作。

## 关于程序的编译和链接

1. 首相要把源文件编译成 中间代码文件，这个动作叫 编译
2. 然后把大量的中间代码文件合成 可执行文件，这个动作叫 链接

## Makefile 介绍

基本规则：

    1. 如果这个工程没有编译过，那么我们的所有C文件都要编译并被链接
    2. 如果这个工程的某几个C文件被修改，那么我们只编译被修改的C文件，并链接目标程序
    3. 如果这个工程的头文件被改变了，那么我们需要编译引用了这个头文件的C文件，并链接目标程序

## 基本规则组织方式

```txt
target ... : prerequisites ...
command
...
...
```

target : 目标文件，可以是 ObjectFile, 也可以是 可执行文件，还可以是 一个标签
prerequisites : 生成 taget 所需要的文件或是目标
command : make 需要执行的命令

这里需要解释一下：

    taget这一个或多个目标文件依赖于prerequisites中的文件，其生成的规则定义在command中。也就是说，prerequisites中如果有一个以上的文件比target文件要新的话，那么commmand所定义的命令就会执行。

三个特殊的变量：

    ```txt
    $@  : 目标文件
    $^  : 所有的依赖文件
    $<  : 第一个依赖文件
    ```

## make的工作原理

1. make 会在当前目录下找名字叫“Makefile”或“makefile”的文件
2. 如果找到，它会找文件中的第一个目标文件（target)，并把这个文件作为最终的目标文件
3. 如果最终目标文件不存在，或者是目标文件的依赖文件要比最终文件要新，那么，他会执行后面定义的命令重新生成最终的目标文件
4. 如果所依赖的文件也存在，那么make会在当前文件中目标的依赖性，如果找到则再根据哪一个规则生成依赖文件
5. 当然，你的C文件和H文件是存在的啦，于是make会生成.o文件（中间代码文件），然后再用.o文件声明make的终极任务，也就是目标文件edit了

## makefile中使用变量

```txt
objects = main.o kbd.o command.o display.o \
             insert.osearch.o files.o utils.o 
edit : $(objects)
        cc -o edit $(objects)
main.o : main.c defs.h
        cc -c main.c
kbd.o : kbd.c defs.h command.h
        cc -c kbd.c
command.o : command.c defs.h command.h
        cc -c command.c
display.o : display.c defs.h buffer.h
        cc -c display.c
insert.o : insert.c defs.h buffer.h
        cc -c insert.c
search.o : search.c defs.h buffer.h
        cc -c search.c
files.o : files.c defs.h buffer.h command.h
        cc -c files.c
utils.o : utils.c defs.h
        cc -c utils.c
clean :
        rm edit $(objects)
```

其中的 objects 就是一个变量，引用一个变量的值，使用语法 $(变量名)

## 高级技术，让make自动推导

make可以自动推导文件以及文件依赖关系后面的命令。。。（我是真的服了）

make看到一个.o文件，就会自动的把.c文件加载依赖关系中，并且后面的cc指令也会被推导出来

```txt
objects = main.o kbd.o command.o display.o \
          insert.o search.o files.o utils.o

edit : $(objects)
        cc -o edit $(objects)

main.o : defs.h
kbd.o : defs.h command.h
command.o : defs.h command.h
display.o : defs.h buffer.h
insert.o : defs.h buffer.h
search.o : defs.h buffer.h
files.o : defs.h buffer.h command.h
utils.o : defs.h

.PHONY : clean
clean :
        rm edit $(objects)
```

上面的推导也是一个“隐晦规则”
.PHONY 表示 clean 是一个伪目标

## Makefile 里有什么

包含5个东西：显示规则，隐晦规则，变量定义，文件指示，注释

显示规则：

    显示规则说明了，如何生成一个或多个目标文件。

隐晦规则：

    有make自动推导

变量定义：

    类似C语言中的宏定义

文件指示

    一个是在一个Makefile中引用另一个Makefile，类似C语言的include；另一个是指根据某些情况指定Makefile中的有效部分，类似C语言中的预编译#if一样；还有就是定义一个多行的命令

注释

    只有行注释

## include


include<filename> filename可以是当前操作系统Shell的文件模式（可以包含路径和通配符）

## 环境变量 MAKEFILES

建议不使用这个环境变量

## make的工作方式

1. 读入所有的Makefile
2. 读入被include的其他Makefile
3. 初始化文件中的变量
4. 推导隐晦规则，并分析所有规则
5. 为所有的目标文件创建依赖关系链
6. 根据依赖关系，决定哪些目标要重新生成
7. 执行生成命令

## Makefile书写规则

两条主要原则：

    1. 编写依赖关系
    2. 构造生成方法


## 在规则中使用通配符号

```txt
~      :  表示HOME目录，  ~smh/test 表示宿主目录smh下的test目录
*      :  表示任意字串
```

引入两个概念
```txt
$?     自动化变量
objects := $(wildcard *.o)    wildcard 关键字
```

## 重点概念：文件搜寻

VPATH 变量

定义VPATH变量以后，如果make在当前目录寻找不到文件，那么会在VPATH定义的目录中寻找

```txt
VPATH = src:../headers
```

vpath 关键字

```txt
1. vpath <pattern> <directories>   为符合模式<pattern>的文件指定搜索目录<directories>
2. vpath <pattern>                 清除符合模式<pattern>的文件的搜索目录
3. vpath                           清除所有已被设置好了的文件搜索目录
```

```txt
vpath %.h ../headers
```

该语句表示，要求make在“../headers”目录下搜索所有以“.h”结尾的文件。（如果某文件在当前目录没有找到的话）

## 伪目标

## 多目标

```txt
   bigoutput littleoutput : text.g

           generate text.g -$(subst output,,$@) > $@
```

相当于

```txt
   bigoutput : text.g

           generate text.g -big > bigoutput

   littleoutput : text.g

           generate text.g -little > littleoutput
```

引入概念 函数 自动化变量

其中，-$(subst output,,$@)中的“$”表示执行一个Makefile的函数，函数名为subst，后面的为参数。关于函数，将在后面讲述。这里的这个函数是截取字符串的意思，“$@”表示目标的集合，就像一个数组，“$@”依次取出目标，并执于命令。

## 静态模式

非常强大的功能

## 自动生成依赖性

这个功能也非常的强大

## 显示命令

## 命令执行

## 命令出错

## 嵌套执行make

系统级别的环境变量 ：  SHELL  MAKEFLAGS

## 定义命令包

## 变量的基础

就是定义的时候初始化，引用的使用是$符号

## 变量中的变量

```txt
ifeq (0,${MAKELEVEL})
cur-dir := $(shell pwd)
whoami := $(shell whoami)
host-type := $(shell arch)
MAKE := ${MAKE} host-type=${host-type} whoami=${whoami}
endif
```

对于系统变量“MAKELEVEL”，其意思是，如果我们的make有一个嵌套执行的动作（参见前面的“嵌套使用make”），那么，这个变量会记录了我们的当前Makefile的调用层数。

```txt
FOO ?= bar
```

相当于

```txt
ifeq ($(origin FOO), undefined)
FOO = bar
endif
```

## 变量的高级用法

### 变量的替换

### 变量的值当成变量

## 追加变量值

## override指示符

如果有变量是通常make的命令行参数设置的，那么Makefile中对这个变量的赋值会被忽略。如果你想在Makefile中设置这类参数的值，那么，你可以使用“override”指示符。其语法是：

```txt
override <variable> = <value>
override <variable> := <value>
```

## 多行变量

```txt
define two-lines
echo foo
echo $(bar)
endef
```

## 环境变量

## 目标变量

```txt
prog : CFLAGS = -g
prog : prog.o foo.o bar.o
$(CC) $(CFLAGS) prog.o foo.o bar.o


prog.o : prog.c
$(CC) $(CFLAGS) prog.c


foo.o : foo.c
$(CC) $(CFLAGS) foo.c


bar.o : bar.c
$(CC) $(CFLAGS) bar.c
```

在这个示例中，不管全局的$(CFLAGS)的值是什么，在prog目标，以及其所引发的所有规则中（prog.o foo.o bar.o的规则），$(CFLAGS)的值都是“-g”

## 模式变量

```txt
%.o : CFLAGS = -O
```

## 条件判断

三个关键字 : ifeq、else和endif

ifeq的意思表示条件语句的开始，并指定一个条件表达式，表达式包含两个参数，以逗号分隔，表达式以圆括号括起。else表示条件表达式为假的情况。endif表示一个条件语句的结束，任何一个条件表达式都应该以endif结束。

```txt

libs_for_gcc = -lgnu
normal_libs =


ifeq ($(CC),gcc)
libs=$(libs_for_gcc)
else
libs=$(normal_libs)
endif


foo: $(objects)
$(CC) -o foo $(objects) $(libs)
```

## 条件判断语法

```txt
<conditional-directive>
<text-if-true>
endif
```

或者

```txt
<conditional-directive>
<text-if-true>
else
<text-if-false>
endif
```

其中条件判断关键字有：ifeq ifneq ifdef ifndef

## 函数的调用语法

```txt
$(<function> <arguments> )
```

或是

```txt
${<function> <arguments>}
```

例子：

```txt
comma:= ,
empty:=
space:= $(empty) $(empty)
foo:= a b c
bar:= $(subst $(space),$(comma),$(foo))
```

## 字符串处理函数

## 文件名操作函数

## foreach 函数

## if 函数

## call 函数

## origin函数

## shell函数

## 控制make的函数

## 关于命令参数变量

```txt
ARFLAGS
函数库打包程序AR命令的参数。默认值是“rv”。
ASFLAGS
汇编语言编译器参数。（当明显地调用“.s”或“.S”文件时）。
CFLAGS
C语言编译器参数。
CXXFLAGS
C++语言编译器参数。
COFLAGS
RCS命令参数。
CPPFLAGS
C预处理器参数。（ C 和 Fortran 编译器也会用到）。
FFLAGS
Fortran语言编译器参数。
GFLAGS
SCCS “get”程序参数。
LDFLAGS
链接器参数。（如：“ld”）
LFLAGS
Lex文法分析器参数。
PFLAGS
Pascal语言编译器参数。
RFLAGS
Ratfor 程序的Fortran 编译器参数。
YFLAGS
Yacc文法分析器参数。
```

## 模式规则

## 规则链

