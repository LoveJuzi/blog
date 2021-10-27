#ifndef __B_H__
#define __B_H__

#include "A.h"
#include <iostream>

using namespace std;

class A;
class B {
    friend class A;
public:
    B();
    ~B();

    void Print() {
        print();
    }

private:
    void print();

private:
    A * _aobj;
};

#endif