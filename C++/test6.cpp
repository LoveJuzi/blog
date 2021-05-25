#include <iostream>

using namespace std;

class A {
public:
    A() {
        this->m_a = 30;
    }
    void Print();
private:
    int m_a;
};

void A::Print() {
    cout << this->m_a << endl;
}

int main() {
    A a;
    a.Print();
}