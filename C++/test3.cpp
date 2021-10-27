#include <iostream>

using namespace std;

class A {
    public:
    A(int v) : a(v) {}
    virtual void Print() {
        cout << this->a << endl;
    }

    private:
    int a;
};

int main() {
    int a = 10;
    int c = 20;
    int & b = a;
    cout << b << endl;
    b = c;
    cout << b << endl;

    A d(200);
    A e(300);
    A & f = d;
    f.Print();
    f = e;
    f.Print();
}
