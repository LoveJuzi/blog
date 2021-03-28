#include "dcl.h"

#define OUTSIZE 1000

static char out[OUTSIZE];
static char name[MAXTOKEN];
static char datatype[MAXTOKEN];

void dirdcl();

void initDcl()
{
    out[0] = '\0';
}

char *getOut()
{
    return out;
}

char *getName()
{
    return name;
}

char *getDatatype()
{
    return datatype;
}

void initDatatype()
{
    parseToken();
    if (getTokentype() == EOF)
    {
        return;
    }
    strcpy(datatype, getToken());
}

void dcl(void)
{
    int ns = 0;

    for (;;)
    {
        parseToken();
        if (getTokentype() != POINTER)
        {
            break;
        }
        ns++;
    }

    dirdcl();

    for (;;)
    {
        if (ns == 0)
        {
            break;
        }

        strcat(out, " pointer to");
        ns--;
    }
}

void dirdcl()
{
    if (getTokentype() == LEFTPARENS)
    {
        dcl();
        if (getTokentype() != RIGHTPARENS)
        {
            printf("error: missing \n");
        }
    }
    else if (getTokentype() == NAME)
    {
        strcpy(name, getToken());
    }
    else
    {
    }

    for (;;)
    {
        parseToken();
        if (getTokentype() == PARENS)
        {
            strcat(out, " function returning");
        }
        else if (getTokentype() == BRACKETS)
        {
            strcat(out, " array");
            strcat(out, getToken());
            strcat(out, " of");
        }
        else
        {
            break;
        }
    }
}

void initUndcl()
{
    parseToken();
}

void undcl()
{
    char temp[OUTSIZE * 2];
    for (;;)
    {
        if (getTokentype() == END)
        {
            break;
        }

        if (getTokentype() == NAME)
        {
            sprintf(temp, "%s %s", getToken(), out);
            strcpy(out, temp);
        }
        else if (getTokentype() == PARENS || getTokentype() == BRACKETS)
        {
            strcat(out, getToken());
        }
        else if (getTokentype() == POINTER)
        {
            sprintf(temp, "(*%s)", out);
            strcpy(out, temp);
        }

        parseToken();
    }
}
