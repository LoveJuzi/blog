#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

#define NUM 10
#define NUMLEN 4
#define GAMETEIMS 10

int main() {
    srand((unsigned)time(NULL));
    int a[NUM] = { 0 }; 
    int targetNum = 0;

    {
        int b[NUM] = { 0 };
        b[0] = -1;
        targetNum = 0;
        for (int i=0; i<NUMLEN; ++i) {
            int temp = rand() % 10;
            if (b[temp] != 0) {
                --i;
                continue;
            }
            targetNum = targetNum * 10 + temp;
            b[temp] = i+1;
        }
        memcpy(a, b, NUM * sizeof(int));
    }

    std::cout << "input number:" << std::endl;
    int count = GAMETEIMS;
    for (; count>0; --count) {
        int c = 0;
        std::cin >> c;

        bool ok = true;
        int A = 0;
        int B = 0;
        int b[NUM] = {0};
        memcpy(b, a, NUM * sizeof(int));
        for (int i=NUMLEN; i>0; --i) {
            int temp = c % 10;
            c /= 10;
            if (b[temp] == -1) {
                ok = false;
                break;
            } else if (b[temp] == i) {
                ++A;
            } else if (b[temp] != 0) {
                ++B;
            }
            b[temp] = -1;
        }

        if (!ok) {
            std::cout << "input error!!!\n" << std::endl;
            continue;
        }

        std::cout << A << "A"  << B << "B\n" << std::endl;

        if (A == NUMLEN) break;
    }

    if (count > 0) {
        std::cout << "You Win" << std::endl;
    } else {
        std::cout << "\ntarget number is " << targetNum << std::endl;
        std::cout << "Game Over" << std::endl;
    }

    return 0;
}
