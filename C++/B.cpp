#include "B.h"
#include "A.h"

B::B() {
    _aobj = new A();
}

B::~B() {
    delete _aobj;
}

void B::print() {
    cout << _aobj->_a << endl;
}
