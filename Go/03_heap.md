# heap

## 简介

标准库容器实现了了heap的相关接口，所以，只要实现了相关的heap接口，就能够操作堆的结构了。

## 引入

import "container/heap"

## heap.Interface

```golang
type Interface interface {
  sort.Interface
  Push(x interface{}) // add x as element Len()
  Pop() interface{}   // remove and return element Len() - 1.
}
```

## sort.Interface

```golang
type Interface interface {
  // Len is the number of elements in the collection.
  Len() int
  // Less reports whether the element with
  // index i should sort before the element with index j.
  Less(i, j int) bool
  // Swap swaps the elements with indexes i and j.
  Swap(i, j int)
}
```

## heap对外的函数

1. Init
2. Push
3. Pop
4. Remove
5. Fix

## 具体操作方式

1. 首先需要定义相关的接口，只有定义了相关的接口，才能够使用相应的函数。
2. 相关的对外函数，也就是外部能够使用的一些函数，也就是特定的相关功能。
