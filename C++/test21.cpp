#include <iostream>

using namespace std;



class A {
public:
    A(int a) : _a(a) {}

    friend int operator > (const A & a1, const A & a2);
    // int operator > (const A & oth) {
    //     return _a > oth._a;
    // }

private:
    int _a;
};

int operator > (const A & a1, const A & a2) {
    return a1._a > a2._a;
}

int main() {
    A a1(30);
    A a2(20);

    if (a1 > a2) {
        cout << "hello world" << endl;
    }
    return 0;
}