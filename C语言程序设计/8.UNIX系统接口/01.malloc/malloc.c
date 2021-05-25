#include "malloc.h"
#include <alloc.h>

#define MALLOC 1024

typedef long Align;

union header
{
    struct
    {
        union header *ptr;
        unsigned size;

    } s;
    Align x;
};

typedef union header Header;

static Header base;   // 空闲链表的头
static Header *freep; // 空闲链表的初始化指针

static Header *morecore(unsigned nu);

int isExactly(Header *p, unsigned nunits)
{
    return p->s.size == nunits;
}

int isEnough(Header *p, unsigned nunits)
{
    return p->s.size > nunits;
}

void init()
{
    if (freep != NULL)
    {
        return;
    }

    base.s.ptr = &base;
    freep = &base;
}

void *malloc2(unsigned nbytes)
{
    unsigned nunits = 0;

    // 计算需要申请的块的大小
    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    init();

    // 在空闲链表中查找
    Header *prevp = freep;
    Header *p = prevp->s.ptr;
    for (;;)
    {
        if (isExactly(p, nunits))
        {
            prevp->s.ptr = p->s.ptr;
            return (void *)(p + 1);
        }
        else if (isEnough(p, nunits))
        {
            p->s.size -= nunits;
            p += p->s.size;
            p->s.size = nunits;
            return (void *)(p + 1);
        }
        else if (p == freep)
        {
            p = morecore(nunits);
            if (p == NULL)
            {
                // 需要报错
                return NULL;
            }
        }

        prevp = p;
        p = prevp->s.ptr;
    }
}

void free2(void *ap)
{
    Header *p, *nextp; // 需要寻找的插入位置
    Header *bp;        // 基数指针

    bp = (Header *)(ap)-1;

    p = freep;
    nextp = p->s.ptr;
    for (;;)
    {
        if (p < bp && bp < nextp)
        {
            break;
        }

        // 这种情况表明，指针走到从尾到头的情况
        if (p >= nextp && (p < bp || bp < nextp))
        {
            break;
        }

        p = nextp;
        nextp = p->s.ptr;
    }

    if (bp + bp->s.size == nextp) // 与nextp合并
    {
        bp->s.size += nextp->s.size;
        bp->s.ptr = nextp->s.ptr;
    }
    else
    {
        bp->s.ptr = nextp;
    }

    if (p + p->s.size == bp) // 与p合并
    {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    }
    else
    {
        p->s.ptr = bp;
    }

    freep = p;
}

static Header *morecore(unsigned nu)
{
    if (nu < MALLOC)
    {
        nu = MALLOC;
    }

    char *cp;
    Header *up;

    cp = sbrk(nu * sizeof(Header));
    if (cp == (char *)-1)
    {
        return NULL;
    }

    up = (Header *)cp;
    up->s.size = nu;
    free2((void *)(up + 1));
    return freep;
}
