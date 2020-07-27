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

## 与闭包的问题
