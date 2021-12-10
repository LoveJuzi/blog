package main

import (
	"fmt"
)

func prime() <-chan int {
	rt := make(chan int)

	var primeImpl func(<-chan int, chan<- int)
	primeImpl = func(ch <-chan int, rt chan<- int) {
		p := <-ch // 获取质数
		rt <- p   // 输出质数

		ch1 := make(chan int, 1)
		go primeImpl(ch1, rt) // 执行下一个质数筛选

		for d := range ch {
			if d%p != 0 { // 筛选非倍数的数字
				ch1 <- d
			}
		}
	}

	go func() {
		ch := make(chan int, 1)
		go primeImpl(ch, rt)
		for N := 2; ; N++ {
			ch <- N
		}
	}()

	return rt
}

func main() {
	pch := prime()

	// 打印前100个质数
	for i := 0; i < 100000; i++ {
		fmt.Printf("%d ", <-pch)
	}
	fmt.Printf("\n")
}
