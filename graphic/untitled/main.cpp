#include <iostream>

int main()
{
    // 1. 生成随机四位数
    int a1 = 1, a2 = 2, a3 = 3, a4 = 4;

    // 2. 游戏主循环, 最多猜测10次
    int cnt = 10;
    for (;cnt >0; --cnt) {
        // 2.1 获取用户输入的数字
        int f;
        std::cin >> f;

        int b4 = f % 10;
        f /= 10;
        int b3 = f % 10;
        f /= 10;
        int b2 = f % 10;
        f /= 10;
        int b1 = f % 10;

        // 2.2 检测用户输入是否正确
        if (b1 == 0 || b2 == 0 || b3 == 0 || b4 == 0) {
            std::cout << "input error" << std::endl;
            continue;
        }

        if (b1 == b2 || b1 == b3 || b1 == b4 || b2 == b3 || b2 == b4 || b3 == b4) {
            std::cout << "input error" << std::endl;
            continue;
        }

        // 2.3 检测用户数字是否匹配
        int cntA = 0;
        int cntB = 0;
        for (int i=0; i<4; ++i) {
            if (b1 == a1) {
                cntA += 1;
            } else if (b1 == a2 || b1 == a3 || b1 == a4) {
                cntB += 1;
            }

            int temp = b1;
            b1 = b2, b2 = b3, b3 = b4, b4 = temp;
            temp = a1;
            a1 = a2, a2 = a3, a3 = a4, a4 = temp;
        }

        // 2.4 给出匹配结果
        if (cntA < 4) {
            std::cout << cntA << "A" << cntB << "B" << std::endl;
            continue;
        }

        // 2.5 游戏胜利
        std::cout << "You Win! Number is " << a1 << a2 << a3 << a4 << std::endl;

        break;
    }

    // 3 超过猜测次数，游戏失败
    if (cnt == 0) {
        std::cout << "Game Over!!!"<< std::endl;
    }

    return 0;
}
