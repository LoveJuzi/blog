#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <mutex>
#include <memory>

template<typename T>
class Singleton
{
public:
    static T* getInstancePtr() {
        static std::once_flag flag;
        std::call_once(flag, [&]() {
            _instance.reset(new T());
        });
        return _instance.get();
    }

protected:
    Singleton() {};
    ~Singleton() {};

private:
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

private:
    static std::unique_ptr<T>  _instance;
};

template<typename T>
std::unique_ptr<T> Singleton<T>::_instance = NULL;

#endif // SINGLETON_H_

