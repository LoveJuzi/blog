package main

import "fmt"

func f(A []rune, B []rune) []rune {
	if len(A) == 0 && len(B) == 0 {
		return nil
	}

	C := make([]rune, 0)

	LA := len(A) - 1
	LB := len(B) - 1
	carry := 0

	for {
		a := 0
		b := 0
		c := 0

		if LA >= 0 {
			a = int(A[LA] - '0')
			LA -= 1
		}

		if LB >= 0 {
			b = int(B[LB] - '0')
			LB -= 1
		}

		c = a + b + carry

		if c >= 10 {
			carry = 1
			c -= 10
		} else {
			carry = 0
		}

		C = append(C, rune(c)+'0')

		if LA < 0 && LB < 0 && carry == 0 {
			break
		}
	}

	head := 0
	tail := len(C) - 1
	for head < tail {
		C[head], C[tail] = C[tail], C[head]
		head += 1
		tail -= 1
	}

	return C
}

func main() {
	for ch := range f([]rune{'1', '2', '3'}, []rune{'3'}) {
		fmt.Printf("%s ", string(ch))
	}
}
