# channel

## 简介

goroutine 的同步工具除了使用传统的同步锁机制，还可以使用管道。对于管道的使用，golang有一些特殊的处理。

## 关闭管道能否读

## 关闭管道能否写

## 管道的关闭

## 缓冲管道和非缓冲管道

## 只读管道和只写管道的应用

[golang读取关闭channel遇到的问题/如何优雅关闭channel](https://studygolang.com/articles/12763?fr=sidebar)
[go 关闭channel分析](https://blog.csdn.net/tenfer_fan/article/details/54932798)

I/O 管道描述符

主线程申请 标准输出管道，socket输入管道，socket输出管道

## 多生产者多消费者模型

```golang
package main

import (
  "fmt"
  "math/rand"
  "strconv"
  "time"
)

func bgtask(f func(), ech chan<- struct{}, rfs ...func()) {
  go func() {
    defer recover()                      // 捕获一切异常
    defer func() { ech <- struct{}{} }() // 发送退出信号
    defer func() {                       // 捕获任务panic
      if err := recover(); err != nil {
        // 记录异常日志
        fmt.Println(err)

        for _, v := range rfs {
          v()
        }
      }
    }()

    f()
  }()
}

func synctask(f func(), rfs ...func()) <-chan struct{} {
  d := make(chan struct{})
  go func() {
    defer recover()                    // 捕获一切异常
    defer func() { d <- struct{}{} }() // 发送退出信号
    ch := make(chan struct{})          // 后台任务退出信号
    bgtask(f, ch, rfs...)              // 后台处理任务
    <-ch                               // 等待后台任务结束
  }()
  return d
}

func taskMPCMain(m, n int) func() {
  return func() {
    MPCMain(m, n)
  }
}

func taskProducerFactory(ch1 <-chan string, ch chan<- string, m int) func() {
  return func() {
    ProducerFactory(ch1, ch, m)
  }
}

func taskProducerMachine(ch1 <-chan string, ch chan<- string) func() {
  return func() {
    ProducerMachine(ch1, ch)
  }
}

func taskConsumerFactory(ch <-chan string, n int) func() {
  return func() {
    ConsumerFactory(ch, n)
  }
}

func taskConsumerMachine(ch <-chan string) func() {
  return func() {
    ConsumerMachine(ch)
  }
}

// SIZE 生产者消费者通道的大小
const SIZE int = 1 << 10

// MPCMain 主程序
func MPCMain(m, n int) {
  ch1 := make(chan string, SIZE)
  ch := make(chan string, SIZE)

  // 异常恢复代码，先下游后上游
  T1 := synctask(taskProducerFactory(ch1, ch, m), func() { close(ch) }, func() { close(ch1) })
  T2 := synctask(taskConsumerFactory(ch, n), func() { close(ch) })

  for i := 0; i < 100000; i++ { // 分发任务
    ch1 <- strconv.Itoa(i)
  }

  // 任务控制流
  close(ch1) // 通知T1任务结束
  <-T1       // 等待T1任务结束
  close(ch)  // 通知T2任务结束
  <-T2       // 等待T2任务结束
}

// ProducerFactory 用来启动生产者
func ProducerFactory(ch1 <-chan string, ch chan<- string, m int) {
  ch2 := make(chan string, m)
  T := make([]<-chan struct{}, m)
  for i := range T {
    T[i] = synctask(taskProducerMachine(ch2, ch), func() { close(ch) }, func() { close(ch2) })
  }
  for v := range ch1 { // 需要传递一下，因为close无法处理<-chan
    ch2 <- v
  }

  // 任务控制流
  close(ch2)         // 通知T任务结束
  for i := range T { // 等待T任务结束
    <-T[i]
  }
}

// ProducerMachine 每个Machine都会处理一次请求
func ProducerMachine(ch1 <-chan string, ch chan<- string) {
  for v := range ch1 {
    ch <- Producer(v)
  }
}

// ConsumerFactory 用来启动消费者
func ConsumerFactory(ch <-chan string, n int) {
  ch2 := make(chan string, n)
  T := make([]<-chan struct{}, n)
  for i := range T {
    T[i] = synctask(taskConsumerMachine(ch2), func() { close(ch2) })
  }
  for v := range ch { // 需要传递一下，因为close无法处理<-chan
    ch2 <- v
  }

  // 任务控制流
  close(ch2)         // 通知T任务结束
  for i := range T { // 等待T任务结束
    <-T[i]
  }
}

// ConsumerMachine 每个Machine都会处理一次请求
func ConsumerMachine(ch <-chan string) {
  for v := range ch {
    Consumer(v)
  }
}

// Producer 生产者加工
func Producer(src string) (dst string) {
  dst = src + "HHHHHHHH"
  return
}

// Consumer 消费者消费
func Consumer(s string) {
  fmt.Println(s)
  time.Sleep(time.Second * time.Duration(rand.Intn(5)+1))
}

func main() {
  <-synctask(taskMPCMain(3, 10))
}
```

golang 语言虽然原声支持并发，但是golang的并发也是有陷阱的，目前来说有以下几点：

  1. goroutine由于死锁导致goroutine资源消耗
  2. goroutine异常导致整个进程终止

所以，并发不是你想开，想开就能开！！！

goroutine遵循的是CSP理论，具体的实现方式在goroutine已经介绍过了。

本节主要的内容是：如何安全的使用goroutine？

这是一个比较大的话题，为了安全使用goroutine，我们需要明白，什么情况下，我们需要使用goroutine？或者可以改成，goroutine到底能够执行什么类型的计算任务？

解答这两个基本问题的时候，我发现，我都不能给出一个很好的答案，主要的一个原因是，我发现我无法协调goroutine的运转，或者更准确的说，如何调度goroutine，保证goroutine的同步性。

channel很自然的就出现了。

到这里，你会明白，如果想比较自然的协调goroutine的运转，channel是一个非常好用的工具，这里也体现了，channel优于锁的机制。

举个比较类似的例子，Shell中有一个东西和channel很像，没错，就是管道，管道从某个意义上说也是协调了各个进程的运转。但是，channel和管道不一样，区别我们后面说，但是，管道写的代码能不能改成共享文件的方式？答案是可以的，但是共享文件的方式是不是很不自然，同时也不是很好写。共享文件的方式就类似使用锁共享内存的方式，我做这个比喻，只是想让大家对channel和锁的区别有个潜在认识。

所以，做个总结，管道能做的事情，锁能做，锁能做的事情，管道也能做。如果别人问你，那我如何抉择？是使用锁，还是使用管道？这个问题的回答，不是网上常见的，哪个自然就使用哪个。这个回答是不对的。

如果你对问题的思考，是以通信的方式进行的，那么使用管道，如果你对问题的思考，是以共享内存的方式进行的，那么使用锁？不要在以通信方式思考的代码使用锁，也不要在以共享内存方式的代码中使用管道！！！（这个就是自然的方式，不过笼统的说自然，而不说什么是自然，都是耍流氓）

golang的主张是什么呢？

golang提倡是以通信的方式实现共享内存，也就是说，golang提倡多以通信的方式思考问题，以通信的方式实现goroutine的协调任务！！！

在介绍goroutine的章节中，我也实现了一个多生产者多消费者的代码，不过那个代码有不少的问题，具体的我也不详细说了，通过几天对channel的思考，我重新写了一个版本，这个版本有我对goroutine和channel的结合使用的一些理解。

现在来回答“goroutine到底能够执行什么类型的计算任务”？

基于我对编程的理解，编程首先要做的不是写实际的算法，而是要对现有的问题，进行任务划分。有一个好的任务划分，每个任务之间再通过特定的方式进行协调，达到最终解决现有的问题的目的。golang协调各个任务使用的就是通信手段。

启动一个新的任务，我们使用goroutine，任务间的协调，使用channel。

为了更好的解释多生产者多消费者模型的代码，我们先对任务这个概念做一点补充。

计算机的任务可以分成后台任务和同步任务。

所谓的后台任务就是，任务的结束后，启动任务的那个父任务并不能知道子任务结束了，为了保证后台任务和其对应的父任务的协调状态，我在设计的时候，使用了一个退出信号的通道，当后台任务结束了，就会往退出信号队列发送一个信号，父任务在通过捕获这个信号（通常还是要启动一个新的后台任务），这样就能够保证任务的协调性。看下代码：

```golang
func bgtask(f func(), ech chan<- struct{}, rfs ...func()) {
  go func() {
    defer recover()                      // 捕获一切异常
    defer func() { ech <- struct{}{} }() // 发送退出信号
    defer func() {                       // 捕获任务panic
      if err := recover(); err != nil {
        // 记录异常日志
        fmt.Println(err)

        for _, v := range rfs {
          v()
        }
      }
    }()

    f()
  }()
}
```

bgtask就是一个后台任务的启动函数，这个函数有三个类型的参数。

第一个参数表示实际任务函数，通常这个函数都是一个包裹函数，包裹函数就是对实际业务函数的封装，我们这里限定包裹函数都是taskXXX这种命名方式，比如代码中的“taskProducerFactory”、“taskConsumerFactory”等等都是包裹函数。

第二个参数表示退出信号接收通道，从代码中可以看到，当我们的实际任务函数执行完毕后，bgtask中的goroutine就会退出，从而调用defer语句，defer的第二条语句就是用来发送退出信号。

```golang
defer func() { ech <- struct{}{} }() // 发送退出信号
```

这里顺便解释一下第一条语句的作用，由于defer语句都有可能产生异常，比如上面发送退出信号这条语句，可能ech这个channel被关闭了，从而发送的时候就会异常，为了保证goroutine不会异常退出，我们就继续捕获异常，不过这时候，我们不处理defer的异常，仅仅是调用recover()进行恢复一下就行了。

```golang
defer recover()                      // 捕获一切异常
```

第三个参数表示异常恢复函数集，解释一下，当任务函数执行出现异常了，此时，我们需要做两件事情：

  1. 日志记录这个异常，方便以后查错
  2. 调用恢复函数集，主要的目的是协调各个任务函数，比如，如果后台任务是一个关联了某个channel的任务，由于关联了某个channel，也就是说该任务可能是channel的上游或者下游，不过不管是上游还是下游，我们通过关闭这个channel，从而通知上游或者下游，说我异常退出了，如果channel关闭了，写channel也会异常，从而引起其他关联任务的异常退出，读channel不会异常，但是会有一个关闭标记，读任务可能会异常退出，也可能正常退出。

从这个函数功能的介绍，这个函数并不能保证无死锁的情况发生，也并不能完美的处理所有异常，所以，以后还有待改进！

为了有个更好的代码，代码都需要不断的重构，重构的前提就是不断的思考，思考我们有什么，我们没什么。

所谓的同步任务，父任务在等待同步任务的完成，父任务会阻塞，父任务这个阻塞特性，是同步任务和后台任务的一个区别。

```golang
func synctask(f func(), rfs ...func()) <-chan struct{} {
  d := make(chan struct{})
  go func() {
    defer recover()                    // 捕获一切异常
    defer func() { d <- struct{}{} }() // 发送退出信号
    ch := make(chan struct{})          // 后台任务退出信号
    bgtask(f, ch, rfs...)              // 后台处理任务
    <-ch                               // 等待后台任务结束
  }()
  return d
}
```

synctask函数就是一个同步任务函数，这个函数使用了bgtask函数，使用这个函数的目的是，这样就不需要在该函数中处理业务异常的代码了。

synctask函数有两个类型的参数，这两个类型的参数和bgtask函数中对应的类型是相同的。

第一个参数类型是包裹任务函数，第二个参数类型是异常恢复函数集。

synctask函数和bgtask函数最大的不同是，synctask函数返回一个同步信号channel，当synctask中的任务函数完成了，synctask函数会发送一个同步信号到同步信号channel中，父任务可以读取这个信号，如果这个信号没有发送，父任务会一直阻塞在读过程。

那么如何使用这两个函数呢？我以synctask函数为例子进行讲解。

1. 任务函数

    ```golang
    func ProducerMachine(ch1 <-chan string, ch chan<- string) {
      for v := range ch1 {
        ch <- Producer(v)
      }
    }
    ```

2. 包裹任务函数

    ```golang
    func taskProducerMachine(ch1 <-chan string, ch chan<- string) func() {
      return func() {
        ProducerMachine(ch1, ch)
      }
    }
    ```

3. 启动任务

    ```golang
    T[i] = synctask(taskProducerMachine(ch2, ch), func() { close(ch) }, func() { close(ch2) })
    ```

    这里解释一下，“taskProducerMachine(ch2, ch)”表示任务函数，“func() { close(ch) }, func() { close(ch2) }”表示异常恢复函数集，由于任务函数有引用了两个channel，当任务函数异常了，我们需要在异常恢复函数集中关闭关联的channel，目的就是用来协调其他任务的运行（可能是异常退出，也可能是正常退出，也可能其他任务还会继续运行，这个主要看程序的可靠性设计）

golang可以很好的写正交代码，上面的使用方式也可以看出正交的优雅性。

有了这个基础，现在解释一下多生产者多消费者模型代码的解析。

首先，我们进行任务划分，划分成两个子任务，一个是生产任务，一个是消费任务。

生产任务负责接收一个初始产品，对这个初始产品初级加工，产生一个新的产品。

```golang
func Producer(src string) (dst string) {
  dst = src + "HHHHHHHH"
  return
}
```

消费任务负责接收生产环节生产的产品，然后进行消费处理。

```golang
func Consumer(s string) {
  fmt.Println(s)
  time.Sleep(time.Second * time.Duration(rand.Intn(5)+1))
}
```

为了协调这两个任务，我们引入一个产品channel，上游是生产机器，下游是消费机器，生产机器调用生产任务，将生产的产品放入产品channel，消费机器从产品channel读取产品，使用消费任务进行消费。

```golang
func ProducerMachine(ch1 <-chan string, ch chan<- string) {
  for v := range ch1 {
    ch <- Producer(v)
  }
}

func ConsumerMachine(ch <-chan string) {
  for v := range ch {
    Consumer(v)
  }
}
```

解释一下ProducerMachine函数的第一个参数的作用，生产机器需要从外部引入一些原始产品。

为了协调多机器的问题，比如有m个生产者n个消费者的问题，我们引入工厂，工厂负责启动m台或者n台机器。

```golang
func ProducerFactory(ch1 <-chan string, ch chan<- string, m int) {
  ch2 := make(chan string, m)
  T := make([]<-chan struct{}, m)
  for i := range T {
    T[i] = synctask(taskProducerMachine(ch2, ch), func() { close(ch) }, func() { close(ch2) })
  }
  for v := range ch1 { // 需要传递一下，因为close无法处理<-chan
    ch2 <- v
  }

  // 任务控制流
  close(ch2)         // 通知T任务结束
  for i := range T { // 等待T任务结束
    <-T[i]
  }
}
```

ProducerFactory函数就是一个生产工厂，首先引入一个临时channel--ch2，ch2的作用就是连接ProducerMachine和ProducerFactory的任务关系，当ProducerMachine出现异常了，我们会关闭ch2，从而导致ProducerFactory任务中的写ch2操作异常，也就是导致下面的语句异常

```golang
for v := range ch1 { // 需要传递一下，因为close无法处理<-chan
  ch2 <- v
}
```

关闭ch的目的是为了协调ProducerMachine和ConsumerMachine任务的关系，ConsumerMachine相关的代码如下：

```golang
for v := range ch {
  Consumer(v)
}
```

关闭ch会引起这个循环正常退出，这个是range channel的一个特性。

在解释一下任务控制流

```golang
close(ch2)         // 通知T任务结束
for i := range T { // 等待T任务结束
  <-T[i]
}
```

任务控制流表示的是，当任务都正常运行的时候，我们的任务应该如何流转，比如此处，我们首先关闭ch2，用于通知ProducerMachine任务结束

```golang
for v := range ch1 {
  ch <- Producer(v)
}
```

然后，我们等待所有生产机器的退出信号，当所有任务生产机器都完成工作了，ProducerFactory任务正常结束。

类似的我们有ConsumerFactory函数

```golang
func ConsumerFactory(ch <-chan string, n int) {
  ch2 := make(chan string, n)
  T := make([]<-chan struct{}, n)
  for i := range T {
    T[i] = synctask(taskConsumerMachine(ch2), func() { close(ch2) })
  }
  for v := range ch { // 需要传递一下，因为close无法处理<-chan
    ch2 <- v
  }

  // 任务控制流
  close(ch2)         // 通知T任务结束
  for i := range T { // 等待T任务结束
    <-T[i]
  }
}
```

最后我们需要一个主函数用来连接ProducerFactory和ConsumerFactory的关系

```golang
func MPCMain(m, n int) {
  ch1 := make(chan string, SIZE)
  ch := make(chan string, SIZE)

  // 异常恢复代码，先下游后上游
  T1 := synctask(taskProducerFactory(ch1, ch, m), func() { close(ch) }, func() { close(ch1) })
  T2 := synctask(taskConsumerFactory(ch, n), func() { close(ch) })

  for i := 0; i < 100000; i++ { // 分发任务
    ch1 <- strconv.Itoa(i)
  }

  // 任务控制流
  close(ch1) // 通知T1任务结束
  <-T1       // 等待T1任务结束
  close(ch)  // 通知T2任务结束
  <-T2       // 等待T2任务结束
}
```

这个函数很干净，首先申请channel，然后启动任务，执行任务分发，最后做下任务控制流。golang语言处理的真是漂亮。

这里解释一下分发任务：golang分发任务都是通过channel的，分发到channel也是为了进行任务协调。

小小的总结一下：golang解决问题的方式总体来说就两个大的步骤，任务划分和任务协调！！！

任务划分的任务如果都是同步任务，使用上面例子的方式就能够有效的进行任务协调，那么，如果是后台任务，任务协调又该怎么做？后面再说。

一个好的任务划分也会减轻任务协调的压力，如果任务划分不合理，那么任务协调也就变的更困难，如果任务协调失败，整个任务就可能失败。

那么，怎么才能有个好的任务划分呢？理解实际问题是解决这个划分的一个出路，不清楚实际问题，无法抽象，那么都没有办法进行任务划分！！！
