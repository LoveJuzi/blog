#ifndef __B_H__
#define __B_H__

#include "A.h"

class A;
class B {
    friend class A;
public:
    B(){}

    void Print() {
        print();
    }

private:
    void print() {
        cout << _aobj._a << endl;
    }

private:
    A _aobj;
};

#endif