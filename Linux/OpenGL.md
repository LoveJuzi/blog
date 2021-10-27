# OpenGl

[OpenGL教程](http://www.opengl-tutorial.org/)

我记得很久以前，我看到国一个关于 OpenGL 的教程，那个教程还是有代码的，现在找不到了。

虽然，目前 3.0 是趋势，但是，公司使用的是旧版本，我不纠结，我其实需要学习的是旧版本，因为，工作需要的原因。

## 可编程管线和固定管线

[[GLES] 固定管线与可编程管线的差别](https://www.jianshu.com/p/756006001031)

[MarkDown flow流程图示例](https://www.jianshu.com/p/f28c94cf1204)

这篇文章主要解释了什么是“固定管线”，什么是“可编程管线”。

图形图像数据经历各种操作过程后，在画面显示的最终结果，一般要经历以下两个过程：

1. 顶点渲染：用于渲染出形状
2. 像素渲染：用于形状中填充颜色

```flow
start=>start: 数据
op1=>operation: 顶点渲染
op2=>operation: 像素渲染
end=>end: 画面
start(right)->op1(right)->op2(right)->end
```

## 概括介绍 OpengGL 的渲染管线

基本渲染流程如下：

```flow
s=>start: 顶点数据
vs=>operation: 顶点着色器
sa=>operation: 图元装备
gs=>operation: 几何着色器
rz=>operation: 光栅化
fs=>operation: 片段着色器
tab=>operation: 测试与混合
end=>end: 画面
s->vs->sa->gs->rz->fs->tab->end
```

> 顶点数组对象： Vertex Array Object, VAO
>
> 顶点缓冲对象： Vertex Buffer Object, VBO
>
> 索引缓冲对象： Element Buffer Object, EBO 或 Index Buffer Object, IBO

上述流程图就是基本的图形渲染流程，总体上可分为两个步骤：

1. 把你的 3D 坐标转换为 2D 坐标
2. 把 2D 坐标转变为实际有颜色的像素

> 在现代OpenGL中，我们**必须**定义至少一个顶点着色器和一个片段着色器（因为GPU中没有默认的顶点/片段着色器）。出于这个原因，刚开始学习现代OpenGL的时候可能会非常困难，因为在你能够渲染自己的第一个三角形之前已经需要了解一大堆知识了。在本节结束你最终渲染出你的三角形的时候，你也会了解到非常多的图形编程知识。





## C++ virtual override 关键字

`virtual` 虽然平时使用比较多，但是在实际使用的时候，还是缺少一点理论依据，这里做一些使用规范的说明：

1. C++ 中不允许在类外面的实现或定义函数的时候，添加 `virtual` 或 `override` 等关键字
2. 通常我写代码的时候，如果涉及到的是虚函数我会在美个子类中声明一下，其实这种做法是不对的

C++ 中当某个成员函数申明了 `virtual`，表示这个成员函数是个虚函数，如果子类重写了这个虚函数，那么，在实际的子类对象实例化后，就会调用子类重写的这个虚函数，子类重写的时候并不需要在显示申明 `virtual`，从而，这样会带来两个问题：

1. 如果子类没有申明 `virtual` ，那么我们读代码的时候，很难区分一个函数是不是重写了父类的虚函数
2. 如果子类申明了 `virtual`，但是，父类其实没有这个虚方法，从而也容易引发歧义

C++11 引入 override 关键字，就是用来解决这个问题，如果是父类，那么我们使用 `virtual` 申明虚方法，如果是子类，我们使用 override 申明我们需要重写父类的虚方法

```c++
struct A{
    virtual void f1(int) const;
    virtual void f2();
    void f3();
};
 
struct B : A {
    void f1(int) const override;    //正确，f1与基类中的f1匹配
    void f2(int) override;    //错误，A中没有形如f2(int)的函数
    void f3() override;    //错误，f3()不是虚函数
    void f4() override;    //错误，A中没有名为f4的虚函数
};
```

这个例子能够很好的解释我们的上述遇到的问题。

[c++11之override关键字](https://blog.csdn.net/ypshowm/article/details/89231390)



## firefox vim 插件

主要是为了方面浏览网页，更少的接触鼠标











