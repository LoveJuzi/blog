# OpenGL_Matrix



## C++ protected 继承

C++ 的继承访问属性，有三个 `public` 、`protected` 和 `private`

其中，隐私级别属性级别 `public < protected < private` 

也就是说，如果使用 `protected` 继承，那么父类的所有方法在子类中，至少是 `protected` 级别的隐私级别

| **父类中的访问属性** | **继承方式**                 | **子类中的访问属性** |
| -------------------- | ---------------------------- | -------------------- |
| **private**          | **public/protected/private** | **不允许访问**       |
| **public**           | **public**                   | **public**           |
| **public**           | **protected**                | **protected**        |
| **public**           | **private**                  | **private**          |
| **protected**        | **public**                   | **protected**        |
| **protected**        | **protected**                | **protected**        |
| **protected**        | **private**                  | **private**          |

请理解上述表格

基于以上理解对于 `QOpenGLFunctions_3_3_Core` 我们采用 `protected` 继承方式，从而达到对 `OpenGL` 函数封装的效果

C++ 语言自由度是非常的高，请好好的学这门语言，语言特性都是一点点累计出来的

## QOpenGLContext QOpenGLFunctions

我在编写代码的时候，发现 Qt 在初始化的时候调用了 **[initializeOpenGLFunctions](https://doc.qt.io/qt-5/qopenglfunctions.html#initializeOpenGLFunctions)**，这玩意如果不执行，后面 OpenGL 的函数都调用失败了。。。

真心操蛋，不知道这个函数到底干了什么，目前只能在主类中进行了。。。（还有很多东西要学习，在暂时丢掉这些无关紧要的东西）

## tree 生成目录

发现一个好用的命令，可以直接生成目录，我使用了另一种方式

```sh
alias treeqt 'tree -P "*.cpp|*.h|Makefile|*.glsl" -I "*.moc.cpp" --prune --noreport'
```

## vim 插入模式下 backspace 不能使用

```shell
set nocompatible 
set backspace=indent,eol,start
```

## vim no tag found

其实很简单，只要把对应的 tags 文件的设置到 tags 属性就可以了

## vim 键盘映射

这应该是最后一个了解 vim 的过程了

[vim中的各种map](https://blog.csdn.net/zzyczzyc/article/details/86529138)

| map  | noremap  | unmap  | mapclear  |
| ---- | -------- | ------ | --------- |
| nmap | nnoremap | nunmap | nmapclear |
| vmap | vnoremap | vunmap | vmapclear |
| imap | inoremap | iunmap | imapclear |
| cmap | cnoremap | cunmap | cmapclear |

我们这里主要使用 `cnoremap`，命令模式映射，非递归的形式

[Multiple commands at once](https://vim.fandom.com/wiki/Multiple_commands_at_once)

[vim的几种模式和按键映射](http://haoxiang.org/2011/09/vim-modes-and-mappin/)

经过折腾，这部分内容以后不做研究，主要是意义不大，vim 的主要任务还是做为一个编辑器，大部分的其他系统命令还是应该使用命令行工具，这里我可以直接开一个终端窗口就可以了

