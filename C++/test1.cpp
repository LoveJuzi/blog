#include <iostream>

using namespace std;

static int A = 10;

int PrintB() {
    cout << B << endl;
}

static int B = 100;

int main() {
    cout << "hello world" << endl;
}
