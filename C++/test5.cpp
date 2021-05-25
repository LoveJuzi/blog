#include <iostream>

using namespace std;

class A {
public:
    A() : this->m_a(10) {
        
    }

private:
    int m_a;
};

int main() {
    return 0;
}