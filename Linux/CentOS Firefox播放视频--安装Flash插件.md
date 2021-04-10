# CentOS Firefox播放视频--安装Flash插件



[Centos7 中firefox安装flash插件](https://blog.51cto.com/u_1929297/2398742)

这里需要注意的是，需要将对应的动态库插件放入到用户的根目录下，还有系统根目录下

恩，最终的测试还是不好使用。。。

换个思路，我无非就是希望使用一个浏览器，不是 chrome 的浏览器看视频，我查查有没有其他的浏览器可以在 centos 下使用，放弃对 firefox 的幻想



在本文中我们将向您介绍如何在[香港服务器](https://www.a5idc.net/)的CentOS 8系统上安装Brave Browser。Brave浏览器是一个免费的开源浏览器。它速度快，安全，通过阻止跟踪器来保护隐私，并且仍然基于chromium，所以你有所有你可能正在寻找的扩展和功能。
步骤1.首先，让我们首先确保您的系统是最新的。
sudo dnf install epel-release
sudo dnf update
sudo dnf install dnf-plugins-core
步骤2.在CentOS 8上安装Brave Browser。
现在，我们在安装之前添加Brave存储库：
sudo dnf config-manager --add-repo https://brave-browser-rpm-release.s3.brave.com/x86_64/
sudo rpm --import https://brave-browser-rpm-release.s3.brave.com/brave-core.asc
完成后，只需使用以下命令来安装Brave：
sudo dnf install brave-browser
步骤3.访问CentOS系统上的Brave浏览器。
成功安装后，通过键入brave-browser或图形界面从命令行启动它。加载欢迎页面后，单击“我们开始吧”，然后按照屏幕上的说明从当前浏览器导入书签和设置，也可以设置默认搜索引擎。
至此，您已经成功安装了Brave。欢迎访问A5互联https://www.a5idc.net/了解更多。



```shell
sudo dnf install dnf-plugins-core

sudo dnf config-manager --add-repo https://brave-browser-rpm-release.s3.brave.com/x86_64/

sudo rpm --import https://brave-browser-rpm-release.s3.brave.com/brave-core.asc

sudo dnf install brave-browser
```

## centos yum 的速度太慢

