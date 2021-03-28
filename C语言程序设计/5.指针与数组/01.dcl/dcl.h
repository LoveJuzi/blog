#include <stdio.h>
#include <string.h>

// 定义分词最大长度
#define MAXTOKEN 100

// 定义分词类型
enum
{
    NAME,
    PARENS,
    BRACKETS,
    LEFTPARENS,
    RIGHTPARENS,
    POINTER,
    END
};

// 字符读取函数
int getch();
void ungetch();

// 分词处理函数
void parseToken();
char *getToken();
int getTokentype();

// 分词分析函数
void initDatatype();
void initDcl();
void dcl();
char *getOut();
char *getName();
char *getDatatype();

// 分词反解析函数
void initUndcl();
void undcl();