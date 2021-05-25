#include "dcl.h"

#define MAXTOKEN 100

static char token[MAXTOKEN]; // 存入读取的分词
static int tokentype;        // 存入读取的分词类型

void parseToken();

char *getToken()
{
    return token;
}

int getTokentype()
{
    return tokentype;
}

int isSpace(char c)
{
    return c == ' ' || c == '\t';
}

int isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int isDigit(char c)
{
    return c >= '0' && c <= '9';
}

int isPointer(char c)
{
    return c == '*';
}

int isLeftParens(char c)
{
    return c == '(';
}

int isRightParens(char c)
{
    return c == ')';
}

int isEnd(char c)
{
    return c == '\n';
}

int isAlnum(char c)
{
    return isAlpha(c) || isDigit(c);
}

int isLeftBrackets(char c)
{
    return c == '[';
}

int isRightBrackets(char c)
{
    return c == ']';
}

void parseToken()
{
    // 跳过空白符号
    int c = getch();
    for (;;)
    {
        if (!isSpace(c))
        {
            break;
        }
        c = getch();
    }

    if (isLeftParens(c))
    {
        c = getch();
        if (isRightParens(c))
        {
            strcpy(token, "()");
            tokentype = PARENS;
            return;
        }
        else
        {
            ungetch(c);
            strcpy(token, "(");
            tokentype = LEFTPARENS;
            return;
        }
    }
    else if (isLeftBrackets(c))
    {
        char *p = token;
        for (;;)
        {
            *p = c;
            ++p;
            if (isRightBrackets(c))
            {
                break;
            }
            c = getch();
        }
        *p = '\0';
        // p is token
        tokentype = BRACKETS;
        return;
    }
    else if (isAlpha(c))
    {
        char *p = token;
        for (;;)
        {
            *p = c;
            ++p;
            c = getch();
            if (!isAlnum(c))
            {
                break;
            }
        }

        ungetch(c);
        *p = '\0';
        // p is token
        tokentype = NAME;
        return;
    }
    else if (isPointer(c))
    {
        strcpy(token, "*");
        tokentype = POINTER;
        return;
    }
    else if (isRightParens(c))
    {
        strcpy(token, ")");
        tokentype = RIGHTPARENS;
        return;
    }
    else if (isEnd(c))
    {
        token[0] = '\n';
        token[1] = '\0';
        tokentype = END;
        return;
    }
    else
    {
        token[0] = 'c';
        token[1] = '\0';
        tokentype = c;
        return;
    }
}