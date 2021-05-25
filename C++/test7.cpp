#include <iostream>

using namespace std;

class A {
public:
    A() : m_a(10) {
        cout << "无参构造函数" << endl;
    }

    A(const A &oth) : m_a(oth.m_a) {
        cout << "拷贝构造函数" << endl;
    }

    A & operator= (const A & oth) {
        cout << "赋值构造函数" << endl;
        this->m_a = oth.m_a;
        return *this;
    }

private:
    int m_a;
};

int main() {
    A a1;
    A a2(a1);
    A a3 = a1;
    a3 = a1;
}