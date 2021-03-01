#include <iostream>
#include <vector>

using namespace std;

struct A {
    A() {
        cout << "构造函数" << endl;
    }

    A(int a) {
        cout << "自定义构造函数" << endl;
    }

    A(const A & oth) {
        cout << "拷贝构造函数" << endl;
    }

    A(A && oth) {
        cout << "移动构造" << endl;
    }

    A & operator = (const A & oth) {
        cout << "复制构造函数" << endl;
        return *this;
    }

    A & operator = (A && oth) {
        cout << "移动赋值操作符" << endl;
        return *this;
    }
};

A f() {
    A a = A(10);
    return a;
}

int main() {
    A a = f();

    // vector<A> vec;
    // vec.push_back(f());

    // A b;
     
    // b = f();

    return 0;
}