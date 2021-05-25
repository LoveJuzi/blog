#include <stdio.h>
#include <stdlib.h>

#define MAXTOP 100

#define ELELEN 100

void push(double f);
double pop();
int getElement(char ele[], char s[], int idx);
int isNumber(char s[]);
int isDigit(char ch);
void delNumber(char s[]);
int isOp(char s[]);
void delOp(char s[]);
int isAdd(char ch);
void delAdd();
int isSub(char ch);
void delSub();
int isMul(char ch);
void delMul();
int isDiv(char ch);
void delDiv();

int main()
{
    char s[] = "1 2 - 4 5 + *";
    int idx = 0;
    char ele[ELELEN];
    for (;;)
    {
        if (s[idx] == '\0')
        {
            break;
        }
        idx = getElement(ele, s, idx);
        if (isNumber(ele))
        {
            delNumber(ele);
        }
        else if (isOp(ele))
        {
            delOp(ele);
        }
        else
        {
            // 报错
        }
    }
    printf("%f\n", pop());
    return 0;
}

// I/O函数
int getElement(char ele[], char s[], int idx)
{
    char sep = ' ';

    int i = 0;

    for (;;)
    {
        if (s[idx] != sep)
        {
            break;
        }
        idx++;
    }

    for (;;)
    {
        if (s[idx] == '\0')
        {
            return idx;
        }

        if (s[idx] == sep)
        {
            ele[i] = '\0';
            return idx;
        }

        ele[i] = s[idx];
        i++;
        idx++;
    }

    return idx;
}

int sp = 0;
double val[MAXTOP];

void push(double f)
{
    if (sp > MAXTOP)
    {
        return;
    }
    val[sp++] = f;
}

double pop()
{
    if (sp > 0)
    {
        return val[--sp];
    }

    return 0.0; // 表示错误
}

int isNumber(char s[])
{
    if (isDigit(s[0]))
    {
        return 1;
    }
    return 0;
}

int isDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

void delNumber(char s[])
{
    push(atof(s));
}

int isOp(char s[])
{
    if (s[1] != '\0')
    {
        return 0;
    }

    if (s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/')
    {
        return 1;
    }

    return 0;
}

void delOp(char s[])
{
    if (isAdd(s[0]))
    {
        delAdd();
    }
    else if (isSub(s[0]))
    {
        delSub();
    }
    else if (isMul(s[0]))
    {
        delMul();
    }
    else if (isDiv(s[0]))
    {
        delDiv();
    }
}

int isAdd(char ch)
{
    return ch == '+';
}

void delAdd()
{
    push(pop() + pop());
}

int isSub(char ch)
{
    return ch == '-';
}

void delSub()
{
    push(0.0 - pop() + pop());
}

int isMul(char ch)
{
    return ch == '*';
}

void delMul()
{
    push(pop() * pop());
}

int isDiv(char ch)
{
    return ch == '/';
}

void delDiv()
{
    push(1.0 / pop() * pop());
}
