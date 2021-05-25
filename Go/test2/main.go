package main

import (
	"fmt"
	smhbao2 "test2/smhbao"
	"test2/smhbao3"
	"test2/smhbao3/smhbao3child"
)

func main() {
	smhbao2.Quicksort()
	smhbao3.MergerSort()

	t := smhbao3.A{B: 100}
	fmt.Println(t)

	smhbao3child.HeapSort()
}
