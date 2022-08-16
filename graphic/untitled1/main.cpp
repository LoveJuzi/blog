#include <iostream>
#include <vector>


class A {
private:
    int x = 100;

public:
    class AInner {
    public:
        AInner(A* a1) : a(a1) {}
        void output() {
           std::cout << a->x << std::endl;
        }

    private:
        A* a;
    };

    AInner* b;

    void creatInner() {
        b = new AInner(this);
    }

    void setX(int x) { this->x = x; }
};


int main(int argc, char *argv[])
{
    A a;
    a.creatInner();
    a.b->output();
    a.setX(10000);
    a.b->output();

    return 0;
}
