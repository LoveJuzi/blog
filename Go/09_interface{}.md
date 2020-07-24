# interface{}

## 简介

研究golang，interface是一个绕不过的砍。

interface 本身也可以理解成一个类似结构体的东西，只不过这个结构体存了两个值，一个是指向某个对象的类型信息，另一个是指向某个对象的值信息。

golang中任何的变量的对象都有两部分信息，一部分是类型信息，一部分是值信息。所以 nil != nil !!!

golang是强类型语言，比较操作都是依据严格的变量的类型和对应的值进行比较，只有相同的变量类型才能比较，而nil是无类型的值。

## 接口的构建

对于切片和map对象的构建过程，我在内存管理的部分说过了。接口也有构建过程，它的构建方式有点特殊:

1. 首先检查变量对象是否实现了接口中申明的方法集，如果没有，编译报错
2. 申请接口类型变量
3. 将变量对象的类型和值存入接口类型变量

根据接口的构建过程，我们容易知道，对于空接口interface{}而言，任何变量对象都是实现的，所以，任何变量对象都可以构建空接口。

现在的问题是，接口对接口赋值的时候，会发生什么？

答案是，首先接口会利用自身的变量类型信息，检查是否满足新接口的要求，如果符合那么将自己的信息拷贝到新接口中，还是上述接口构建的三个步骤！！！

例子如下：

```golang
package main

import (
  "fmt"
)

type InterfaceA interface {
  A()
}

type InterfaceB interface {
  B()
}

type Person struct {
  Name string
}

func (p *Person) A() {
  fmt.Println("A")
}

func main() {
  person := &Person{Name: "周二"}
  var a InterfaceA = person
  a.A()
  var b InterfaceB = a
  b.B()
}
```

简单解释一下：

1. 首先我们构建了一个person对象 “person := &Person{Name: "周二"}”
2. 然后我们使用person对象构建了一个接口a对象 “var a InterfaceA = person”
3. 我们成功的调用了person中的A方法，通过接口a对象的调用 “a.A()”
4. 我们无法构建接口b对象，因为person对象没有实现B方法！！！所以 “  var b InterfaceB = a” 编译报错！！！
