#include <iostream>

using namespace std;

// 要求 T 是指针类型，不能是数组类型的指针
template <class T>
class SmartPointer {
private:
    T * _ptr; // 需要被包装的指针
    int * _count; // 智能指针的引用计数

public:
    SmartPointer(T * ptr = nulptr);
    SmartPointer(const SmartPointer & oth);
    ~SmartPointer();

    SmartPointer & operator=(const SmartPointer & oth);

    T & operator*();

    T * operator->();

private:
    // 辅助函数，用来释放引用计数
    void release();
}

template <class T>
SmartPointer<T>::SmartPointer(T * ptr) : _ptr(ptr), _count(new int(0)) {
    if (_ptr != nullptr) {
        _count++; // 引用计数+1
    }
}

template <class T>
SmartPointer<T>::SmartPointer(const SmartPointer & oth) {
    _ptr = oth._ptr;
    _count = oth._count;
    *_count++; // 引用计数+1
}

template <class T>
SmartPointer & SmartPointer<T>::operator = (const SmartPointer & oth) {
    if (this == &oth) {
        return *this;
    }

    release();

    _ptr = oth._ptr;
    _count = oth._count;
    *_count++; // 引用计数+1

    return *this;
}

template <class T>
SmartPointer<T>::~SmartPointer() {
    release();
}

template <class T>
void SmartPointer<T>::release() {
    if (*_count == 0) {
        delete _count;
        _count = nullptr;
        return;
    }
    *_count--; // 引用计数-1
    if (*_count == 0) {
        delete _ptr;
        _ptr = nullptr;
        delete _count;
        _count = nullptr;
    }
}

template <class T>
T & SmartPointer<T>::operator *() {
    assert(this->_ptr == nullptr);
    return *(this->ptr);
}

template <class T>
T * SmartPointer<T>::operator -> () {
    assert(this->ptr == nullptr);
    return this->ptr;
}

int main() {
    SmartPointer<int> a(new int(10));
    SmartPointer<int> b;

    cout << *a << endl;
    return 0;
}