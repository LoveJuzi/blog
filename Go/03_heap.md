# heap

## 简介

标准库容器实现了了heap的相关接口，所以，只要实现了相关的heap接口，就能够操作堆的结构了。

## 引入

import "container/heap"

## heap.Interface

1. func Init(h Interface)
2. func Pop(h Interface) interface{}
3. func Push(h Interface, x interface{})
4. func Remove(h Interface, i int) interface{}
5. type interface
