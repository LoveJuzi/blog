#include <stdio.h>

#define MAXLEIN 1000

int getline2(char s[], int lim);
int strindex(char s[], char t[]);
int strsub(char s[], char t[], int k);

char pattern[] = "ould";

int main()
{
    char s[MAXLEIN];
    // 读取所有行的方法
    while (getline2(s, MAXLEIN) > 0)
    {
        if (strindex(s, pattern) != -1)
        {
            printf("%s\n", s);
        }
    }

    return 0;
}

// I/O 相关函数
// 1. 读取至多 lim 个字符
// 2. 读取到行标志返回
// 3. 读取到文件结束标志返回
//
// 函数返回读取到字符个数
int getline2(char s[], int lim)
{
    int cnt = 0;
    char c;
    for (int i = 0; i < lim; i++) // 最多读取这么多的字符
    {
        if ((c = getchar()) == EOF)
        {
            break;
        }

        if (c == '\n')
        {
            break;
        }

        s[cnt] = c;
        cnt++;
    }
    s[cnt] = '\0'; // 添加C语言的字符串的结束标志

    return cnt;
}

// s 是被匹配字符串
// t 是模式串
// 如果 t 在 s 中，返回 t 在 s 的索引
// 如果不在 返回 -1
int strindex(char s[], char t[])
{
    int k = 0;

    for (;;)
    {
        if (s[k] == '\0')
        {
            break;
        }

        if (strsub(s, t, k) != -1)
        {
            return k;
        }

        k++;
    }

    return -1;
}

// 校验 t 是不是 s 的一个子串
// k 是校验的位置
// 如果是，返回t的长度
// 如果不是，返回-1
int strsub(char s[], char t[], int k)
{
    if (t[0] == '\0')
    {
        return -1;
    }

    int j = 0;
    for (;;)
    {
        if (t[j] == '\0')
        {
            break;
        }

        if (s[k] == '\0')
        {
            break;
        }

        if (s[k] != t[j])
        {
            break;
        }

        k++;
        j++;
    }

    if (t[j] == '\0')
    {
        return j;
    }

    return -1;
}