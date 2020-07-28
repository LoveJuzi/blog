# goroutine

## CSP模型

golang CSP模型有三个主要元素：M，P，G。

M：Machine的缩写，直接关联一个内核线程。
P：Processor的缩写，类似处理器的功能，处理M的上下文问题。
G：Goroutine的缩写，网上说是一个协程，具体的代码逻辑数据都存储在Goroutine中。

M,P,G三者有如下这些关系：

1. P需要绑定一个M，P才能正常工作。当M阻塞了，P会脱离M，P会重新找一个空闲M进行绑定。当M阻塞结束后，M会去偷一个P，如果没有偷到，M会将Goroutine放入到全局Goroutine队列中。
2. 每个P都有一个Goroutine队列，该队列用于存储Goroutine。系统还有一个全局的Goroutine，用于处理一些特殊情况。
3. P每次从自己的队列中取一个Goroutine，将其运行到其绑定的M上，如果自己的队列是空的，那么会从全局Goroutine中取Goroutine，如果全局Goroutine也是空的，P会从其他P的队列中取Goroutine，通常会取一半到自己的对列中。

[The Go scheduler](https://morsmachine.dk/go-scheduler)

![01.jpg](./img/01.jpg)

![02.jpg](./img/02.jpg)

![03.jpg](./img/03.jpg)

![04.jpg](./img/04.jpg)

如果想明白goroutine的运行过程，需要理解它的CSP模型，即便不能查看源码，但是逻辑过程网上还是有的。明白了它的逻辑过程，剩下的就比较简单的。（goroutine是其他语言没有的一种模型，如果不清楚，而是以其他语言的的方式进行理解，这些都是不够充分的，有些代码就比较混乱）

## Hello World

```golang
package main

import (
  "fmt"
  "time"
)

func hello() {
  fmt.Println("Hello world")
}

func main() {
  go hello()
  time.Sleep(time.Second * 1)
}
```

启动一个新的goroutine非常的简单，使用“go” 语句就可以了。

## 引入channel

```golang
package main

import (
  "fmt"
)

func hello(ch chan int) {
  fmt.Println("Hello world")
  ch <- 0
}

func main() {
  ch := make(chan int)
  defer close(ch)
  go hello(ch)
  <-ch
}
```

首先，channel也是一个类似结构体的东西，channel的构建过程一半使用make的方式。

channel含有如下信息

1. 管道大小，默认是0(阻塞管道)，make可以指定一个大小。
2. 管道内部有个管道锁，用于解决并发引起的读写问题。
3. 管道需要进行关闭。（傻逼设计，由于需要手动关闭，导致golang的代码变得复杂！！！）

## PC例子

```golang
package main

import "fmt"

// SIZE 生产者消费者通道的大小
const SIZE int = 1 << 4

func main() {
  ch := make(chan string, SIZE)
  pchs := make(chan struct{})
  cchs := make(chan struct{})

  defer close(ch)   // 这个会最后一个释放
  defer close(pchs) // 这个会第二个释放
  defer close(cchs) // 这个会优先释放

  go Producer(ch, pchs) // 启动生产者
  go Consumer(ch, cchs) // 启动消费者

  <-pchs // 等待生产者生产完毕

  ch <- "" // 关闭消费者，需要等待生产者生产完毕

  <-cchs // 等待消费者消费完毕
}

// Producer 生产者
func Producer(ch chan string, chs chan struct{}) {
  fmt.Println("生产者开始生产...")
  for i := 0; i < 50; i++ {
    product := fmt.Sprintf("产品%d", i+1)
    fmt.Println("生产======>", product)
    ch <- product
  }
  fmt.Println("生产者结束生产...")
  chs <- struct{}{}
}

// Consumer 消费者
func Consumer(ch chan string, chs chan struct{}) {
  fmt.Println("消费者开始消费...")
  var product string
  for {
    select {
    case product = <-ch:
      if product == "" {
        goto DONE
      }
      fmt.Println("消费======>", product)
    }
  }
DONE:
  fmt.Println("消费者结束消费...")
  chs <- struct{}{}
}
```

这是一个单生产者单消费者的例子，这个例子有如下一些特点：

1. 效率不如锁
2. 代码结构清晰简单
3. 为了维护生产者和消费者的关系，需要一个主函数
4. 使用了golang的一些高级特性，比如 goroutine, defer, channel, select 等等，而这些特性多多少少都会有一些陷阱

## 多PC问题

```golang
package main

import "fmt"

func main() {
  MPCMain(3, 10) // 3个生产者，10个消费者
}

// SIZE 生产者消费者通道的大小
const SIZE int = 1 << 10

// MPCMain 多生产者消费者主程序
// m 表示生产者的个数，n表示消费者的个数
func MPCMain(m, n int) {
  ch := make(chan string, SIZE)
  pchs := make([]chan struct{}, m)
  cchs := make([]chan struct{}, n)

  for i := 0; i < m; i++ {
    pchs[i] = make(chan struct{})
  }
  for i := 0; i < n; i++ {
    cchs[i] = make(chan struct{})
  }

  defer close(ch)
  defer func() {
    for _, v := range pchs {
      close(v)
    }
  }()
  defer func() {
    for _, v := range cchs {
      close(v)
    }
  }()

  for idx, v := range pchs {
    go Producer(ch, v, idx) // 启动生产者
  }
  for idx, v := range cchs {
    go Consumer(ch, v, idx) // 启动消费者
  }

  for i := 0; i < m; i++ {
    <-pchs[i]
  }
  for i := 0; i < n; i++ {
    ch <- "" // 关闭消费者，需要等待生产者生产完毕
  }
  for i := 0; i < n; i++ {
    <-cchs[i]
  }
}

// Producer 生产者
func Producer(ch chan string, chs chan struct{}, id int) {
  defer func() {
    // 处理异常
    chs <- struct{}{}
  }()
  fmt.Printf("生产者%d开始生产...\n", id)
  for i := 0; i < 50; i++ {
    product := fmt.Sprintf("产品%d", i+1)
    fmt.Printf("生产%d======>%s\n", id, product)
    ch <- product
  }
  fmt.Printf("生产者%d结束生产...\n", id)
}

// Consumer 消费者
func Consumer(ch chan string, chs chan struct{}, id int) {
  defer func() {
    // 处理异常
    chs <- struct{}{}
  }()
  fmt.Printf("消费者%d开始消费...\n", id)
  var product string
  for {
    select {
    case product = <-ch:
      if product == "" {
        goto DONE
      }
      fmt.Printf("消费%d======>%s\n", id, product)
    }
  }
DONE:
  fmt.Printf("消费者%d结束消费...\n", id)
}
```

可以看到，生产者消费者的主体代码并没有太大的变化，我们仅仅改变的就是资源管理部分的代码，抽象了一个管理函数“MPCMain“，我们没有抽象一个对象什么的，因为我秉承认为，golang还是一个以函数为中心的这么一个语言，大部分的情况，使用函数就能够解决问题。

凡是符合生产者消费者模型，都可以使用上述的代码机制进行编码。个人感觉这种编码方式还是比较干净的。

主要设计思路如下：

1. 不论生产者还是消费者值进行通道关联
2. 通常需要准备两种类型的通道，数据通道和状态通道，数据通道用于生产者和消费者的关联，状态通道用于管理者和生产者消费者的关联。
3. 使用了一个小技巧，0值数据表示关闭消费者
4. 最后注意一点：这种设计是在一个进程中进行设计的，对于网络的形式，需要采用网络的设计方式！！！通常需要拆分管理函数，管道可能需要使用消息队列代替。

## RW问题

```golang
package main

import (
  "fmt"
  "math/rand"
  "sync"
  "time"
)

// SIZE 队列大小
const SIZE = 1 << 10

// RWMain 读者写者主程序
// readerFinished 管道看似多余，其实，这是一种通信管理方式
// 通过这种方式，我们将计数的操作都限定在主管理函数
func RWMain(ch chan int8, done chan struct{}) {
  rch := make(chan int8, SIZE)                 // 读者队列
  wch := make(chan int8, SIZE)                 // 写者队列
  ws := make(chan struct{}, 1)                 // 写程序排斥
  wg := &sync.WaitGroup{}                      // 正在读的读者计数器
  readerFinished := make(chan struct{}, 1<<31) // 读者完成队列
  defer func() {
    // 处理异常
    close(readerFinished)
    close(ws)
    close(wch)
    close(rch)
  }()

  go func() { // 读者读取完毕，需要通知计数减少
    for {
      select {
      case <-readerFinished:
        wg.Done()
      }
    }
  }()
  waitAllReaderExit := func() {
    wg.Wait()
  }
  waitWriterExit := func() {
    ws <- struct{}{} // 检查是否有写者
    <-ws
  }

  go generateReader(rch, readerFinished)
  go generateWriter(wch, ws)

  var rw int8
  for { // 分发任务
    select {
    case rw = <-ch:
      waitWriterExit()
      if rw == 0 {
        wg.Add(1)
        rch <- rw
      } else if rw == 1 {
        ws <- struct{}{}
        waitAllReaderExit() // 等待所有的读者退出
        wch <- rw
      } else {
        goto DONE
      }
    }
  }
DONE:
  waitAllReaderExit()
  done <- struct{}{}
}

// 生成读者
func generateReader(ch chan int8, wch chan struct{}) {
  var r int8
  for {
    select {
    case r = <-ch:
      fmt.Println("读者队列长度：", len(ch))
      go Reader(r, wch)
    }
  }
}

// 生成写者
func generateWriter(ch chan int8, ws chan struct{}) {
  var w int8
  for {
    select {
    case w = <-ch:
      fmt.Println("写者队列长度：", len(ch)+1)
      go Writer(w, ws)
    }
  }
}

// Reader 读者
func Reader(r int8, wch chan struct{}) {
  fmt.Println("读者正在读...", r)
  t := time.Second * time.Duration(rand.Intn(5)+2)
  fmt.Println("读时长：", t)
  time.Sleep(t)
  wch <- struct{}{}
}

// Writer 写者
func Writer(w int8, ws chan struct{}) {
  fmt.Println("写者正在写...", w)
  t := time.Second * time.Duration(rand.Intn(5)+2)
  fmt.Println("写时长：", t)
  time.Sleep(t)
  <-ws
}

func main() {
  ch := make(chan int8, SIZE) // 读写主程序的接收队列
  done := make(chan struct{})
  defer close(ch)
  defer close(done)

  go RWMain(ch, done)

  task := []int8{0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1}
  for _, v := range task {
    ch <- v
  }

  <-done
}
```

读者写者问题也是一个常见的竞争模型。

例子中，我几乎全部使用了golang推荐的channel机制，除了读者的计数部分，使用了sync包的功能，从这里也说明不是所有的竞争操作都必须使用channel机制。

上述代码和多生产者和多消费者整体的框架是一致的，一个主管理函数，一些工作函数。

主函数的一个主要工作就是用来协调写者和读者的关系。主要内容如下：

1. 主函数有一个主队列，这个队列排队一些读写任务，0表示读任务，1表示写任务。
2. 主函数通过对主队列中任务进行分发，读任务分发到读者队列，写任务分发到写者队列。
3. 主函数分发有如下限制：
   1. 如果当前有写者在工作，那么不进行任何任务分发，等待写者完成任务
   2. 如果当前分发读者任务，主函数需要给读者任务进行计数，当读者任务完成后，主函数需要减去该任务数
   3. 如果当前分发写者任务，首先标识有写者进行写任务，禁止主函数继续分发任务，然后，主函数需要等待所有的读者任务完成，也就是读者的任务计数归0，然后才分发写者任务
   4. 读者不断的从读者队列读取读者任务，每完成一个读者任务，需要通知主函数一个读者任务已完成，读者计数减一
   5. 写者不断的从写者队列读取写者任务，写任务完成，需要通知主函数继续分发新的任务

通过使用channel机制，主函数，读者工作函数，写者工作函数都进行了解藕，这样易于多任务的重构。

由于协调工作都由主函数完成，这样也减轻了工作函数的负担。

最后提一点，并不是所有的资源竞争都要用channel，必要的时候使用锁机制也是可以的，尤其是无法对竞争资源进一步划分，比如此例子中的读者计数器。

例子中的“readerFinished”的用法是可以借鉴的，这就是一个协调协程状态的一个channel。

## 线程安全队列---条件变量的方法

```golang
package main

import (
  "fmt"
  "sync"
)

// TQueue 线程安全队列
type TQueue struct {
  Data     []int
  Len      int
  beginIdx int
  endIdx   int

  popmutex  sync.Mutex
  pushmutex sync.Mutex
  fullCond  *sync.Cond
  emptyCond *sync.Cond
}

// NewTQueue 生成一个队列，需要指定队列的大小
func NewTQueue(l int) *TQueue {
  tq := &TQueue{
    Data:      make([]int, l+1),
    Len:       l + 1,
    beginIdx:  0,
    endIdx:    0,
    popmutex:  sync.Mutex{},
    pushmutex: sync.Mutex{},
  }
  tq.fullCond = sync.NewCond(&tq.pushmutex)
  tq.emptyCond = sync.NewCond(&tq.popmutex)
  return tq
}

func idxplusplus(idx int, l int) int {
  return (idx + 1) % l
}

// IsEmpty 队列是否为空
func IsEmpty(tq *TQueue) bool {
  return tq.beginIdx == tq.endIdx
}

// IsFull 队列是否为满
func IsFull(tq *TQueue) bool {
  return tq.beginIdx == idxplusplus(tq.endIdx, tq.Len)
}

// Push 向队列中添加一个元素
// 当队列满的时候需要条件等待
func Push(tq *TQueue, e int) {
  tq.pushmutex.Lock()
  defer tq.pushmutex.Unlock()

  for IsFull(tq) {
    tq.fullCond.Wait()
  }

  tq.Data[tq.endIdx] = e
  tq.endIdx = idxplusplus(tq.endIdx, tq.Len)

  tq.emptyCond.Signal()
}

// Pop 从队列中提取一个元素
// 当队列空时需要条件等待
func Pop(tq *TQueue) int {
  tq.popmutex.Lock()
  defer tq.popmutex.Unlock()

  for IsEmpty(tq) {
    tq.emptyCond.Wait()
  }

  e := tq.Data[tq.beginIdx]
  tq.beginIdx = idxplusplus(tq.beginIdx, tq.Len)

  tq.fullCond.Signal()

  return e
}

func main() {
  done1 := make(chan struct{})
  done2 := make(chan struct{})
  tq := NewTQueue(5)

  go func() {
    for i := 0; i < 50; i++ {
      fmt.Println("======>", i)

      Push(tq, i)
    }
    done1 <- struct{}{}
  }()
  go func() {
    for {
      e := Pop(tq)
      fmt.Println(e)

      if e == 49 {
        break
      }
    }
    done2 <- struct{}{}
  }()

  <-done1
  <-done2
}
```

## 线程安全队列---channel的方法

```golang
package main

import (
  "fmt"
)

// TQueue 线程安全队列
type TQueue struct {
  Data     []int
  Len      int
  beginIdx int
  endIdx   int

  popmutex  chan struct{}
  pushmutex chan struct{}
  fullch    chan struct{} //用于检查队列的满
  emptych   chan struct{} //用于检查队列的空
}

// NewTQueue 生成一个队列，需要指定队列的大小
func NewTQueue(l int) *TQueue {
  return &TQueue{
    Data:      make([]int, l+1),
    Len:       l + 1,
    beginIdx:  0,
    endIdx:    0,
    fullch:    make(chan struct{}, l),
    emptych:   make(chan struct{}, l),
    popmutex:  make(chan struct{}, 1),
    pushmutex: make(chan struct{}, 1),
  }
}

func idxplusplus(idx int, l int) int {
  return (idx + 1) % l
}

// IsEmpty 队列是否为空
func IsEmpty(tq *TQueue) bool {
  return tq.beginIdx == tq.endIdx
}

// IsFull 队列是否为满
func IsFull(tq *TQueue) bool {
  return tq.beginIdx == idxplusplus(tq.endIdx, tq.Len)
}

// Push 向队列中添加一个元素
// 当队列满的时候需要条件等待
func Push(tq *TQueue, e int) {
  tq.fullch <- struct{}{}
  defer func() {
    tq.emptych <- struct{}{}
  }()

  tq.pushmutex <- struct{}{}
  defer func() {
    <-tq.pushmutex
  }()

  tq.Data[tq.endIdx] = e
  tq.endIdx = idxplusplus(tq.endIdx, tq.Len)
}

// Pop 从队列中提取一个元素
// 当队列空时需要条件等待
func Pop(tq *TQueue) int {
  <-tq.emptych
  defer func() {
    <-tq.fullch
  }()

  tq.popmutex <- struct{}{}
  defer func() {
    <-tq.popmutex
  }()

  e := tq.Data[tq.beginIdx]
  tq.beginIdx = idxplusplus(tq.beginIdx, tq.Len)

  return e
}

func main() {
  done1 := make(chan struct{})
  done2 := make(chan struct{})
  tq := NewTQueue(5)

  go func() {
    for i := 0; i < 50; i++ {
      fmt.Println("======>", i)

      Push(tq, i)
    }
    done1 <- struct{}{}
  }()
  go func() {
    for {
      e := Pop(tq)
      fmt.Println(e)

      if e == 49 {
        break
      }
    }
    done2 <- struct{}{}
  }()

  <-done1
  <-done2
}
```

上面是基于两种不同的方式，求解线程安全队列，一种是基于条件变量，一种是基于channel的。实现方面，两者的复杂度差不多，主要说说两者的差别。

使用条件变量的情况，必须使用互斥锁进行搭配，条件变量控制的就是互斥锁的资源状态，条件变量等待的时候，当前会放弃互斥锁的所有权，并且会把自己添加到条件变量唤醒队列中，等到有唤醒信号的时候，从唤醒队列中重新唤醒，然后重新竞争锁，如果得到锁的所有权，重新检查条件，如果不符合条件继续上锁，重复流程。

使用channel的情况略有不同，首先预检查资源是否充足，充足后，再检查是否读权限或写权限，如果有，那么就进行读或写，操作完毕后，还需要将预检查的资源写入到对端资源管道，从而保证同步。

基于上述的一些例子，我们有如下的一些总结：

1. 从任务角度划分，可以使用channel进行任务的解藕，这个是基于通信的理论
2. 从内存共享的角度，channel可以充当信号量的功能，从而可以等价锁和条件变量的一些代码
3. 由于条件变量的局限性，或者不便性，对于计数规则，推荐使用sync包的WaitGroup的功能，这个是协程安全的计数器

好了，有了以上程序的一个应用，现在解决另一个问题：golang到底使用channel还是锁？

基于上述的一些研究，我给的答案是，如果做任务划分，使用channel，如果处理内存共享问题，可以使用channel，也可以使用锁，如果处理线程安全计数问题，直接使用sync包中的WaitGroup的功能，不使用channel，也不使用锁。

channel不是万能的，但是经验告诉你，使用channel会更容易对代码进行维护，写程序的大部分时间不是在开发时间，而是在维护时间！！！（这个是我多年的编程经验告诉我的）

## 使用channel实现读写锁
