#include <iostream>

using namespace std;

class A {
    public:

    A() : m_a(10) {}

    A(int a) : m_a(a) {}
    
    void Print() {
        cout << this->m_a << endl;
    }

    private:

    int m_a;
};

class B : public A {
    public:

    B() : A(30) {

    }
};

int main() {
    B b;
    b.Print();
    return 0;
}