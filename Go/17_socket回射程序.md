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
