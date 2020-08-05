# socket 反射程序

## 简介

本来socket这部分的学习，我应该是使用C语言来学习的，但是C语言学习的时候缺少各种包，且涉及到的网络函数使用有各种各样的限制，从原理上来说，不论是golang还是C，socket的基本原理都是类似的，本着只是对原理的进一步理解，决定使用golang语言进行学习，golang对网络这部分正好也封装了一个网络包，本文也是对这个网络包进行学习。

网络部分主要涉及如下几个重要问题：

  1. 连接（寻址）
  2. 数据包传输（路由原理）
  3. I/O复用
  4. 非阻塞I/O
  5. DNS
  6. 广播等其他的细节的东西

网络部分逻辑方面并不难理解，只是涉及的概念太多，需要串起来才行。

## 时间反射程序

之前使用C语言写网络程序，各种复杂度，但是golang天然的支持goroutine和channel，从而可以对不同的I/O处理采用goroutine进行处理，各个I/O之间，使用channel进行通信。

### 服务端程序

```golang
package main

import (
  "fmt"
  "net"
  "os"
  "time"
)

func main() {
  serve()
}

func serve() {
  tcpServer, err := net.ResolveTCPAddr("tcp4", ":8080")
  if err != nil {
    // 记录错误日志
    os.Exit(1)
  }
  listener, err := net.ListenTCP("tcp", tcpServer)
  if err != nil {
    // 记录错误日志
    os.Exit(1)
  }
  for {
    // conn 就是已经建立连接的socket对象
    conn, err := listener.Accept()
    if err != nil {
      // 记录错误日志
      continue
    }

    go handle(conn) // 服务处理程序
  }
}

func handle(conn net.Conn) {
  // 一般是服务端做主动关闭，防止客户端恶意保持长连接
  // 服务端关闭连接的时候，也会清空socket的发送缓冲区
  defer func() { conn.Close() }()

  socketSendCh := make(chan string, 1)
  defer close(socketSendCh)

  done1 := make(chan struct{})
  done2 := make(chan struct{})
  defer close(done1)
  defer close(done2)

  defer func() {
    if err := recover(); err != nil {
      // 记录异常日志
    }
  }()

  go generateTime(socketSendCh, done1)
  go sendBuff(conn, socketSendCh, done2)

  <-done1
  <-done2
}

func generateTime(socketSendCh chan string, done chan struct{}) {
  defer func() { done <- struct{}{} }()
  // 生成时间，然后向socketSendCh发送数据
  timeTemplate := "2006-01-02 15:04:05" //常规类型

  t := time.Now().Format(timeTemplate)

  fmt.Println(t)

  socketSendCh <- t
}

func sendBuff(conn net.Conn, socketSendCh chan string, done chan struct{}) {
  defer func() { done <- struct{}{} }()

  // 从socketSendCh去除数据，发送数据到客户端
  buff := <-socketSendCh

  // 发送数据到客户端
  conn.Write([]byte(buff))
}
```

### 客户端程序

```golang
package main

import (
  "fmt"
  "io/ioutil"
  "net"
  "os"
)

func main() {
  client()
}

func client() {
  server := "127.0.0.1:8080"
  addr, err := net.ResolveTCPAddr("tcp4", server)
  if err != nil {
    // 记录错误日志
    os.Exit(1)
  }

  //建立tcp连接
  conn, err := net.DialTCP("tcp4", nil, addr)
  if err != nil {
    // 记录错误日志
    os.Exit(1)
  }

  handle(conn) // 无需启动一个协程，因为客户端只需要处理一个连接
}

func handle(conn net.Conn) {
  standardOutputCh := make(chan string, 1)
  defer close(standardOutputCh)
  done1 := make(chan struct{})
  defer close(done1)
  done2 := make(chan struct{})
  defer close(done2)

  go receiveBuff(conn, standardOutputCh, done1)
  go printTime(standardOutputCh, done2)

  <-done1
  <-done2
}

func receiveBuff(conn net.Conn, standardOutputCh chan string, done chan struct{}) {
  defer func() { done <- struct{}{} }()
  //接收响应
  response, err := ioutil.ReadAll(conn)
  if err != nil {
    // 记录错误日志
    // 此处不退出程序，因为可能是服务端提前关闭导致的一个错误
  }
  standardOutputCh <- string(response)
}

func printTime(standardOutputCh chan string, done chan struct{}) {
  defer func() { done <- struct{}{} }()
  t := <-standardOutputCh
  fmt.Println(t)
}
```

## 输入回射程序

### 服务端程序

```golang
package main

import (
  "fmt"
  "io"
  "net"
  "os"
)

func main() {
  serve()
}

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

func taskHandle(conn net.Conn) func() {
  return func() {
    Handle(conn)
  }
}

func taskRecieveBuff(conn net.Conn, sktSendCh chan string, sdOutCh chan string) func() {
  return func() {
    RecieveBuff(conn, sktSendCh, sdOutCh)
  }
}

func taskSendBuff(conn net.Conn, sktSendCh chan string) func() {
  return func() {
    SendBuff(conn, sktSendCh)
  }
}

func taskPrintBuff(ch <-chan string) func() {
  return func() {
    PrintBuff(ch)
  }
}

func serve() {
  // 申请IP地址空间 绑定端口
  tcpServer, err := net.ResolveTCPAddr("tcp4", ":8080")
  if err != nil {
    // 记录错误日志
    os.Exit(1)
  }

  // 开启监听
  listner, err := net.ListenTCP("tcp", tcpServer)
  if err != nil {
    // 记录错误日志
    os.Exit(1)
  }

  // 等待连接
  for {
    conn, err := listner.Accept()
    if err != nil {
      // 记录错误日志
      continue
    }

    // 这其实是一个后台任务，但是由于我们不关注退出信号，
    // 使用同步任务进行模拟一下
    synctask(taskHandle(conn))
  }
}

// Handle 各种资源的管理都在handle中处理
func Handle(conn net.Conn) {
  defer func() { conn.Close() }() // 服务端关闭连接

  ch1 := make(chan string)
  ch2 := make(chan string)

  T1 := synctask(taskRecieveBuff(conn, ch1, ch2), func() { close(ch1) }, func() { close(ch2) })
  T2 := synctask(taskSendBuff(conn, ch1), func() { close(ch1) })
  T3 := synctask(taskPrintBuff(ch2), func() { close(ch2) })

  <-T1       // 等待T1任务结束
  close(ch1) // 通知T2任务结束
  close(ch2) // 通知T3任务结束
  <-T2       // 等待T2任务结束
  <-T3       // 等待T3任务结束
}

// RecieveBuff 从socket的接收缓冲区读取数据
func RecieveBuff(conn net.Conn, ch1 chan string, ch2 chan string) {
  var ba [1 << 10]byte
  for {
    n, err := conn.Read(ba[:])
    if err != nil {
      if err == io.EOF {
        fmt.Println("接收到EOF")
        break
      }
      // 记录错误日志
      break
    }
    ch1 <- string(ba[:n])
    ch2 <- string(ba[:n])
  }
}

// SendBuff 向socket的发送缓冲区发送数据
func SendBuff(conn net.Conn, ch <-chan string) {
  for v := range ch {
    _, err := conn.Write([]byte(v))
    if err != nil {
      // 记录错误日志
      break
    }
  }
}

// PrintBuff 标准输出输出内容
func PrintBuff(ch <-chan string) {
  for v := range ch {
    fmt.Println(v)
  }
}
```

### 客户端程序

```golang
package main

import (
	"bufio"
	"fmt"
	"io"
	"net"
	"os"
)

func main() {
	client()
}

func bgtask(f func(), ech chan<- struct{}, rfs ...func()) {
	go func() {
		defer func() { recover() }()         // 捕获一切异常
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
		defer func() { recover() }()       // 捕获一切异常
		defer func() { d <- struct{}{} }() // 发送退出信号
		ch := make(chan struct{})          // 后台任务退出信号
		bgtask(f, ch, rfs...)              // 后台处理任务
		<-ch                               // 等待后台任务结束
	}()
	return d
}

func taskScanBuff(ch chan<- string) func() {
	return func() {
		ScanBuff(ch)
	}
}

func taskSendBuff(conn net.Conn, ch <-chan string) func() {
	return func() {
		SendBuff(conn, ch)
	}
}

func taskRecieveBuff(conn net.Conn, ch chan<- string) func() {
	return func() {
		RecieveBuff(conn, ch)
	}
}

func taskPrintBuff(ch <-chan string) func() {
	return func() {
		PrintBuff(ch)
	}
}

func client() {
	// 连接的地址
	server := "127.0.0.1:8080"
	// 连接
	addr, err := net.ResolveTCPAddr("tcp4", server)
	if err != nil {
		// 记录错误日志
		os.Exit(1)
	}

	conn, err := net.DialTCP("tcp", nil, addr)
	if err != nil {
		// 记录错误日志
		os.Exit(1)
	}

	handle(conn)
}

func handle(conn net.Conn) {
	// 通信channel
	sktSendCh := make(chan string, 1)
	stdOutCh := make(chan string, 1)

	// 任务分发
	// 针对不同的任务类型，我们仅仅需要重新编写不同的任务模板，也就是task的实现方式
	// golang语言使用这种方式，可以很好的进行任务并发
	synctask(taskScanBuff(sktSendCh), func() { close(sktSendCh) })
	T2 := synctask(taskSendBuff(conn, sktSendCh), func() { close(sktSendCh) })
	T3 := synctask(taskRecieveBuff(conn, stdOutCh), func() { close(stdOutCh) })
	T4 := synctask(taskPrintBuff(stdOutCh), func() { close(stdOutCh) })

	// 任务执行顺序控制流
	// 任务控制流最好在主程中进行描述，这样便于后期维护
	// 当然，为了优雅，也可以写到具体的任务中，但是这样会导致一个问题
	// 当任务流变的复杂的时候，具体任务的控制流就变得复杂，不利于维护
	<-T3             // 等待T3任务结束
	close(sktSendCh) // 通知T2结束任务
	close(stdOutCh)  // 通知T4结束任务
	<-T2             // 等待T2结束任务
	<-T4             // 等待T3结束任务
}

// ScanBuff 从标准输入读入数据
func ScanBuff(ch chan<- string) {
	// 读文件
	// 读文件就有EOF标识符
	var b string = ""
	for {
		fmt.Scanf("%s", &b)
		ch <- b
	}
}

// SendBuff 向socket的发送缓冲区发送数据
func SendBuff(conn net.Conn, ch <-chan string) {
	for b := range ch {
		var err error
		writer := bufio.NewWriter(conn)
		_, err = writer.WriteString(b)
		if err != nil {
			// 记录错误日志
			panic("write error")
		}

		err = writer.Flush()
		if err != nil {
			// 记录错误日志
			panic("write flush error")
		}
	}
}

// RecieveBuff 从socket的接收缓冲区接收数据
func RecieveBuff(conn net.Conn, ch chan<- string) {
	var ba [1 << 10]byte
	for {
		n, err := conn.Read(ba[:])
		if err != nil {
			if err == io.EOF {
				fmt.Println("接收到EOF")
				break
			}
			// 记录错误日志
			break
		}
		ch <- string(ba[:n])
	}
}

// PrintBuff 打印数据到标准输出
func PrintBuff(ch <-chan string) {
	for b := range ch {
		fmt.Printf(b)
		fmt.Println("")
	}
}
```
