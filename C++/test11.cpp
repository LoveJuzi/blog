#include <iostream>

using namespace std;

int main() {
    int a = 10;
    const auto && c = a;
    cout << c << endl;
    return 0;
}