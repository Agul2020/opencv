# OpenCV 数字图像处理 
## 参考内容
- 数字图像处理（第三版）——何东建
## 环境安装
- [VSCode opencv 环境安装](https://www.cnblogs.com/uestc-mm/p/12758110.html)    - - -转载自 [小淼博客](https://www.cnblogs.com/uestc-mm/)

- [Visual Studio opencv 环境安装](https://blog.csdn.net/ivan_9/article/details/112984941) ---转载自[ivan_9的博客](https://blog.csdn.net/ivan_9)

## 测试环境
- 编译器： TDM-GCC 10.3.0
- 软件版本 OpenCV 4.5.3

## 读入、显示和存储图像
- ### 输入文件名时采用的是相对路径，要注意相关用法

    - 以`./`开头，代表当前目录和文件目录在同一个目录里，“./”也可以省略不写！

    - 以`../`开头：向上走一级，代表目标文件在当前文件所在的上一级目录；

    - 以`../../`开头：向上走两级，代表父级的父级目录，也就是上上级目录，再说明白点，就是上一级目录的上一级目录

    - 以`/`开头，代表根目录

- ### 运行效果
    ![图片加载失败](https://s3.bmp.ovh/imgs/2021/09/d45ad7ebe616e884.png "运行效果截图")
    