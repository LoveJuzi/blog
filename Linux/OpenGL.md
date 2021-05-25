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

