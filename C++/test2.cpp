#include <iostream>

using namespace std;

int main() {
    int b = 10;
    int const * a;
    int const c = 200;
    a = &b;
    cout << b << endl;

    cout << *a << endl;
    return 0;
}