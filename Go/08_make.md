# make

说make，其实还是想说golang的内存问题

## 简介

这篇文章主要还是想用来分析golang的内存问题。

## 基本分类

1. 基础类型
2. 数组类型
3. 结构体类型
4. 切片类型
5. map类型
6. 指针类型

## 堆栈简介

嗯，说实在的golang有没有堆栈的区分，我是怀疑态度的，因为这部分对开发人员是透明的。

我看过一些文章，它们是这样说的：

    在编写高效率的程序时，存储位置的确会有影响。如果可以，go语言编译器将会分配函数的局部变量到函数的栈帧上。然而，如果编译器不能证实局部变量在函数返回后，不会再被引用，编译器将必须分配该变量到垃圾回收堆上，以避免出现悬挂指针错误。如果一个局部变量非常大，那么存储在堆上比存储在栈上也更加合理。

从这里可以看出，当我们申请一个变量的时候，golang优先从栈空间进行内存申请，如果不符合栈内存申请的要求，那么golang就会从堆空间进行内存申请。

栈空间可以理解成一段固定的内存，每个函数都会有一个初始化的大小。
对空间可以理解成一段不固定的内存，这段内存所有函数都有机会使用。

除了上面的区别，栈空间的内存回收是快速的，不依赖gc，而堆空间的内存回收是缓慢的，依赖gc。

所以，为了编写高效率的golang程序，我们需要简单的了解，什么时候是在栈上申请内存，如何减少从堆上申请内存。

## 基本类型

int 类， float 类， bool， byte， rune 这些都是优先从栈上申请空间。这些基本类型都可以取地址，从而有对应的指针类型。

指针类型也是优先从栈上申请空间，但是指针类型所指向的内存，可能是栈空间的，也可能是堆空间。

## 数组类型

数组类型和切片很像，我在网上也看到过一些解释它们的区别，但是都不能够说服我。

数组类型有两个值：一个数组长度，一个数组元素类型（通常是基本类型或结构体类型）。

现在的问题是

  1. 数组类型一般是栈空间还是堆空间？
  2. 数组类型有对应的指针类型吗？

这两个问题，我写了一个简单的测试代码，代码如下：

    ```golang
    package memory

    import (
      "fmt"
      "testing"
    )

    func printArray(arr [1 << 10]int) {
      fmt.Println(arr)
    }

    func printArrayByptr(arr *[1 << 10]int) {
      fmt.Println(arr)
    }

    func Test_array(t *testing.T) {
      arr := [1 << 10]int{}
      printArrayByptr(&arr)
      printArray(arr)
      t.Error(arr)
    }
    ```

1. 数组类型是从栈上获取空间的，证据就是当数组的大小编程 1<< 31 的时候，系统给错误提示了

        ```txt
        Running tool: /usr/local/go/bin/go test -timeout 30s godemo/language/memory -run ^Test_array$

        # godemo/language/memory [godemo/language/memory.test]
        /root/go/src/godemo/language/memory/memory_test.go:16:6: stack frame too large (>1GB): 16384 MB locals + 0 MB args
        FAIL  godemo/language/memory [build failed]
        FAIL
        ```

2. 数组类型有对应的指针类型，但是，我们一般不这么用，如果传参的时候，参数是数组类型，那么依据golang的参数传值性质，数组类型将全值copy，所以，这种类型是低效率的传参方式。

经过测试，我发现，golang还是有堆栈的区分的，golang对基础类型，数组类型都是从栈空间进行内存申请，除非明确要求从堆上进行内存申请。另外，基本所有的类型都支持指针类型。

这里提一句，目前所有提到类型都是可读可写的。

## 结构体类型

结构体类型是golang对外提供的一种自定义类型，结构体类型和基本类型性质类似，结构体类型也是从栈空间申请内存，结构体类型也有对应的指针类型。

结构体类型可以理解成一个是其他类型的一种集合。

之所以要说结构体类型，是为了更好的理解golang的切片类型和map类型。

## 切片类型

要理解切片类型，需要有几个基本的计算机概念的知识：

1. 结构体
2. 泛型
3. 运算符重载
4. 构建

为什么网上说那么多切片和数组的区别，都没有办法说的清楚，主要还是它涉及的基础计算机概念比较多。

1. 不论你怎么解释切片是一个类似数组的东西，你都没有办法否定，切片本身是个结构体，只是切片实现了一个类似动态数组的东西。
2. golang本身不支持泛型，但是切片支持，这个是依赖与语言内置的功能，有了泛型，所以切片类似数组类型，可以自定义一个自己指定的类型。
3. 切片之所以有类似数组访问的一些语法，比如方括号“[]”语法，主要是因为切片内置重载了方括号的运算符。
4. 构建部分是比较复杂的，主要是因为，生成切片的方式比较多，比如数组的切片语法“arr[:]”， 还有直接定义法“[]int{}”，还有make内建构造器“ make([]int, 10, 10)”。

明白了以上4点以后，再来解释切片和数组的关系就比较容易了。

切片是一个结构体，这个结构体含有三个基本元素，一个是指向一片连续内存空间的指针，这片内存空间是依赖于指定的类型，另一个元素，是一个长度，这个长度表示连续内从空间有效数据的长度大小，还有一个容量元素，表示连续内存空间的总长度。

当连续内存空间不够用的时候，切片会怎么办呢？这里需要引入另一个内置函数append。当append发现，切片的空间不够用了，append会干一件事情，申请一个新的切片，然后将原有切片的内容深拷贝到新的切片中，所以，这就是为什么append的返回值是一个切片的原因。

现在来解释目前的三种构造切片的方式，来研究切片中连续内存空间的是在栈上，还是在堆上。

1. arr[:]：数组的切片语法生成的切片，数组本身空间是在栈上的，切片中的连续内存空间就沿用了数组的空间，所以连续内存空间就是在栈上。
2. []int{}：直接定义法，该定义法并不会申请连续内存空间地址，因为其容量是0
3. make：内置函数make的方法，比较特殊，因为golang对make使用使用了函数重载，也就是说make的形式比较多样，但是，不管怎么样，只要make申请了一片连续空间，这部分的空间都是在堆上的。

还有一个append函数，append函数也会生成一个切片，这个切片的连续内存的位置和make是类似的。

说几个注意点：

1. 由于数组切片语法，导致可以通过切片的手段直接修改数组的内容，所以，我们不使用数组的指针这种方式，修改数组的内容，当然也有特殊的情况（方法定义的时候）
2. 一个很重要的东西，就是切片的操作只有增加空间语法，没有减少空间的语法。
3. 还是要在提一句，切片是结构体，所以切片是在栈上的，当然可以在堆上，需要使用new语法。

## map类型

map类型的工作原理和切片类似，这里不做讨论，需要记住的是，map类型的内部实现可能是一个哈希表，不是红黑树。

## 指针类型

golang的指针类型需要单独说一下，主要的原因是，golang的指针类型不同于C中的指针类型的应用，虽然他们都能干类似的事情，但是对于golang来说，指针类型一类特定的类型。

1. 指针类型的构建有两种形式：取地址法和new操作符
2. 取地址法：变量既可以是在栈上，也可以是在堆上，无所谓，因为不论在何处，总有一个内存地址，它们的区别，无外乎是操作系统对内存区的管理方式的不同。
3. new操作符，这个操作符会在堆上申请内存空间，同时返回该存储空间的地址值。

看一个例子：

    ```golang
    // Node 一个有效的地址
    type Node struct {
      a int
      b int
    }

    // ModifyA 修改 a 的值
    func ModifyA(n Node) {
      n.a++
    }
    ```

问题是，调用ModifyA能否修改从外面传入的对象中的a的值？

答案是不能的。原因也很简单，golang的函数参数都是值传递的，而a会是一个新的地址空间中的一个新的变量。

另外一个问题，如何做，才能真正修改？

答案是使用指针类型，此处就是指针类型的第一个用法。

指针类型还有另一个用法，就是作为方法的接收者。

我们知道，golang有三个重要的元素：函数、接口和方法。其中，方法是需要指定一个接收者，这个接收者是有条件的。

    接收者只能是具体的类型对象及该类型的对应的指针类型两种。

也就是说，如果接收者的非指针类型如果还是一个指针类型是不行的。

现在引入一个问题，函数是什么类型？

答案是，函数是一个类似结构体的类型，函数本身的值不是指针类型，虽然函数本身的值可能是一个函数指针的值。

所以函数类型是可以作为方法接收者。这点非常重要！！！

由于函数可以作为方法接收者，所以函数是可以实现接口的！！！

当使用指针类型作为函数接收者，那么就可以更改对象内部的值。

再引入一个问题：什么时候使用函数？什么时候使用接口？什么时候使用方法？对应的参数或者接收者，什么使用指针类型？

这个需要自己体会，这是一个大的议题。

最后还有一个问题：指针的指针可以吗？指针的指针有实际的用处吗？

可以有指针的指针，但是基本不这么用！！！原因很简单，之所以用指针，主要是为了减少不必要的拷贝（这是一个非常重要的原因），还有一个用处就是修改指针引用位置的值，所以，使用指针的指针就没有什么用。另外，如果真的想使用指针的指针，怎么做？结构体封装！！！

以上的讨论都是基于golang的，是基于golang现有的语法进行讨论的，其他语言的使用方式或者应用方式可能不一样，所以，抛开语言来讨论都是耍流氓！

最近想到了一个新的问题：栈空间的内存回收是怎么做的？  =====> 这个问题一定要解决

当我想到这个问题的时候，我一度怀疑我之前对栈和堆的理解出错了，后来经过网上查资料，发现了一个新的概念“逃逸分析”

## 逃逸分析

参考地址：[逃逸分析](https://www.cnblogs.com/sunsky303/p/11121657.html)

通过这篇文章我们知道如下信息：

1. 逃逸分析的用处（为了性能）

    最大的好处应该是减少gc的压力，不逃逸的对象分配在栈上，当函数返回时就回收了资源，不需要gc标记清除。
    因为逃逸分析完后可以确定哪些变量可以分配在栈上，栈的分配比堆快，性能好
    同步消除，如果你定义的对象的方法上有同步锁，但在运行时，却只有一个线程在访问，此时逃逸分析后的机器码，会去掉同步锁运行。

2. go消除了堆和栈的区别

    go在一定程度消除了堆和栈的区别，因为go在编译的时候进行逃逸分析，来决定一个对象放栈上还是放堆上，不逃逸的对象放栈上，可能逃逸的放堆上。

所以，我一直感觉golang是不是没有堆栈区别，现在给出结论：

  golang是有堆栈区别的，只是经过逃逸分析，进一步混淆了堆栈的区别！！！
