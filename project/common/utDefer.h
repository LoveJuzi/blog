#ifndef UTDEFER_H
#define UTDEFER_H

template<typename Function>
struct Deferer{
    Function _f;
    
    Deferer(Function f) : _f(f) {}
    ~Deferer()          { _f(); }
};

template<typename Function>
Deferer<Function> deferFunc(Function f) {
    return Deferer<Function>(f);
}

#define __DEFER_1__(x, y)  x##y
#define __DEFER_2__(x, y)  __DEFER_1__(x, y)
#define __DEFER_3__(x)     __DEFER_2__(x, __COUNTER__)
#define utDefer(expr)      auto __DEFER_3__(_defered_option_) = deferFunc([&](){expr;})

#endif /* UTDEFER_H */
