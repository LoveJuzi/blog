package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	inputReader := bufio.NewReader(os.Stdin)

	s := ""
	start := 0
	end := 0

	s, _ = inputReader.ReadString('\n')
	fmt.Scanf("%d", &start)
	fmt.Scanf("%d", &end)

	// fmt.Println(s)
	// fmt.Println(start)
	// fmt.Println(end)

	s = reverseWords(s, start, end)

	fmt.Print(s)
}

func reverseWords(s string, start int, end int) string {
	if len(s) == 0 {
		return "EMPTY"
	}

	sa := make([]rune, 0)
	for _, v := range s {
		sa = append(sa, v)
	}

	// 分离单词
	wds := make([]string, 0)
	for i := 0; i < len(sa); i++ {
		if sa[i] == ' ' {
			continue
		}
		j := i
		wd := ""
		for j < len(sa) {
			if sa[j] == ' ' || j+1 == len(sa) {
				wds = append(wds, wd)
				break
			}
			wd = wd + string(sa[j])
			j++
		}
		i = j
	}

	if start < 0 || end >= len(wds) {
		return "EMPTY"
	}

	for i, j := start, end; i < j; i, j = i+1, j-1 {
		wds[i], wds[j] = wds[j], wds[i]
	}

	rt := ""
	for _, v := range wds {
		rt += v
		rt += " "
	}

	rt = rt[:len(rt)-1]

	return rt
}
