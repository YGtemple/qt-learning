<div align="right">

[English](README.md) · **[简体中文](README.zh-CN.md)**

</div>

# qt-learning

Qt 6 练习集，从空窗口到 TCP 客户端 / 服务端与多线程。

<sub>C++ · Qt 6 · 7 个项目</sub>

---

## 里面有什么

七个独立 Qt 工程，每个是学习路径上的一步：

```
01 → Qtimer / Timer → file → calculate → TCPclient ⇄ TPCservice
```

| 项目 | 主题 |
|---|---|
| `01` | 窗口与自定义控件基础 |
| `Qtimer` / `Timer` | 定时器——同一个效果的两种 Qt API |
| `file` | 文件读写与对话框 |
| `calculate` | 手写表达式解析 |
| `TCPclient` | `QTcpSocket` 客户端与聊天窗口 |
| `TPCservice` | `QTcpServer` 加 `QThread` |

## 两处值得看

**`calculate`**——不用正则库的解析。查找运算符的循环从索引 1 开始而不是 0，因为 `-3+5` 在位置 0 上的减号不是运算符。从 0 开始会把它误读成运算符，左边成了空字符串，转换直接失败。

**`TPCservice`**——一处值得研究的线程问题。socket 在主线程创建，`QThread` 对象也生活在主线程，所以 `run()` 里建立的连接实际上会在主线程投递。笔记解释了原因，以及正确的 `moveToThread` 写法。

## 构建

```bash
cd calculate
qmake calculate.pro
make          # 或 mingw32-make / nmake
```

需要 Qt 6，含 Core、Gui、Widgets、Network 模块。

## 许可证

MIT
