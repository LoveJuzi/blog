#ifndef __A_H__
#define __A_H__

#include "B.h"

class A {
    friend void B::print();
public:
    A() : _a(10) {}

private:
    int _a;
};

#endif