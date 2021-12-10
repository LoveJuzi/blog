package main

import (
	"fmt"
)

func producer() <-chan int {
	ch := make(chan int, 10)
	go func() {
		defer func() { recover() }()
		defer close(ch)
		for i := 1; i <= 100; i++ {
			ch <- i
		}
	}()

	return ch
}

func consumer(d int) int {
	fmt.Println(d)
	return d * 5
}

func sum(s int, d int) int {
	return s + d
}

func main() {
	job1N := 8
	jobs1 := make(chan struct{}, job1N)
	for i := 0; i < job1N; i++ {
		jobs1 <- struct{}{}
	}

	job2N := 1
	jobs2 := make(chan struct{}, job2N)
	for i := 0; i < job2N; i++ {
		jobs2 <- struct{}{}
	}

	s := 0

	job2 := func(d int) {
		defer func() { recover() }()
		defer func() { jobs2 <- struct{}{} }()

		s = sum(s, d)
	}

	job1 := func(d int) {
		defer func() { recover() }()
		defer func() { jobs1 <- struct{}{} }()
		d = consumer(d)
		<-jobs2
		go job2(d)
	}

	for d := range producer() {
		<-jobs1
		go job1(d)
	}

	for i := 0; i < job1N; i++ {
		<-jobs1
	}

	for i := 0; i < job2N; i++ {
		<-jobs2
	}

	fmt.Printf("sum is: %d\n", s)
}
