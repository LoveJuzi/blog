#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <iostream>

using namespace std;

class ThQueue
{
public:
    ThQueue() {}
    void push(int e)
    {
        unique_lock<mutex> lck(this->mtx);
        this->Q.push(e);
        this->cv.notify_one();
    }
    int pop()
    {
        unique_lock<mutex> lck(this->mtx);

        while (this->Q.empty()) {
            this->cv.wait(lck);
        }
        int e = this->Q.front();
        this->Q.pop();
        return e;
    }

private:
    queue<int> Q;
    mutex mtx;
    condition_variable cv;
};

void f1(ThQueue * thq) {
    cout << "f1" << endl;
    for (int i=0; i<1000; i++) {
        thq->push(i);
    }
}

void f2(ThQueue * thq) {
    cout << "f2" << endl;
    for (int i=1000; i<2000; i++) {
        thq->push(i);
    }
}

void f3(ThQueue * thq) {
    cout << "f3" << endl;
    while (1) {
        int e = thq->pop();
        if (e == -9999) {
            break;
        }
        cout << e << endl;
    }
}

int main()
{
    ThQueue thq;
    thread th1(f1, &thq);
    thread th2(f2, &thq);
    thread th3(f3, &thq);
    th1.join();
    th2.join();
    thq.push(-9999);
    th3.join();
    return 0;
}