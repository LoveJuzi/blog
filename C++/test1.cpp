#include <iostream>

using namespace std;

union test1
{
    int a;
    char b[4];
};



int main() {
    test1 t;
    t.a = 1;
    cout << t.b[0] << endl;
}
