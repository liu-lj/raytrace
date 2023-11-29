# callg/raytrace

## 介绍
我的 Ray Tracing in One Weekend 实现

## 使用说明

1.  项目使用 [xmake](https://gitee.com/tboox/xmake/) 构建。安装 xmake 后使用以下命令编译：
```bash
$ cd raytrace # cd 到项目命令
$ xmake # 使用 xmake 编译
```
运行程序命令：`xmake run`。

2. 程序接受 0~3 个输入作为参数，分别为：

| 参数 | 默认值 | 说明 |
| --- | --- | --- |
| angle | 2 | 光线发散的角度 |
| foucsDist | 10 | 焦距 |
| imageDist | 0.1 | 传感器-透镜距离 |

若使用 Ctrl+D 结束输入，程序将会使用默认值。