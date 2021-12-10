package main

import "fmt"

func natureNumber() (<-chan int, func()) {
	ch := make(chan int)
	exitCh := make(chan struct{}, 1)
	exitCh2 := make(chan struct{}, 1)

	go func() {
		N := 0
	LOOP:
		for {
			select {
			case <-exitCh:
				break LOOP

			case ch <- N:
				N++
			}
		}
		exitCh2 <- struct{}{}
	}()

	exitFunc := func() {
		exitCh <- struct{}{}
		<-exitCh2
	}

	return ch, exitFunc
}

func main() {
	N, exitN := natureNumber()
	defer exitN()

	fmt.Println(<-N)
	fmt.Println(<-N)
	fmt.Println(<-N)
	fmt.Println(<-N)
	fmt.Println(<-N)
}
