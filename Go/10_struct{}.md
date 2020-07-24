# struct{}

## 简介

当我看到这个东西的时候，我的第一反应是蒙掉了！！！咋还有这种奇葩的东西的存在！

空结构的一个简单应用就是可以省内存！

参考网址：[空结构应用](https://blog.csdn.net/weixin_44328662/article/details/86501900)

golang 只内置了map，没有内置set，使用空结构，可以实现了简单的set。

map[int]struct{} 相当于 set[int]
