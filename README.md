# qt-learning · Qt6 C++ 渐进式练习集

> 一句话简介：从空窗口到 TCP 客户端/服务端与多线程的 Qt6 学习路径，每个练习附技术要点与踩坑记录。

## 一、这是什么

7 个 Qt 练习项目的源码归档（剔除编译产物后每个仅 5-8 个文件）。不是教程，是**学习过程的原始记录**——包括当时的错误写法和注释掉的试错代码。

按依赖关系排成一条路径：

```
01 ──▶ Qtimer / Timer ──▶ file ──▶ calculate ──▶ TCPclient ⇄ TPCservice
入门      定时器            文件IO     手写解析器      网络 + 多线程
```

## 二、各项目说明

| 项目 | 主题 | 源文件 |
|---|---|---|
| `01` | 空窗口 + 自定义组件（Qt 工程结构入门） | 5 |
| `Qtimer` | QTimer 定时器用法 | 5 |
| `Timer` | 定时器变体（对比 Qtimer 的两种写法） | 5 |
| `file` | 文件读写与 QFileDialog | 5 |
| `calculate` | **手写表达式解析**的计算器 | 5 |
| `TCPclient` | QTcpSocket 客户端 + 聊天窗口 | 8 |
| `TPCservice` | QTcpServer 服务端 + QThread | 7 |

## 三、值得单独说的两个

### `calculate` — 不用正则，手写字符串解析

原始版本用 `QRegExp` 解析表达式，后来改成纯字符串处理。核心函数 `calculate()` 有 70 行，注释写得很细：

```cpp
// 从索引1开始查找，避免误判负数的负号（如"-3+5"中的"-"是负号而非运算符）
for (int i = 1; i < exprNoSpace.length(); ++i) {
    QChar c = exprNoSpace.at(i);
    if (c == '+' || c == '-' || c == '*' || c == '/') {
        opIndex = i;
        op = c.toLatin1();
        break;  // 只处理第一个找到的运算符（确保是二元运算）
    }
}
```

**为什么从索引 1 开始**——这是手写解析最容易踩的坑。`-3+5` 里第一个 `-` 是负号不是运算符，从 0 开始找会把它误判成运算符，于是左边是空字符串、右边是 `3+5`，转换直接失败。

后面还有几处防御：

```cpp
// 没找到运算符，但整个串能转成数字（如输入 "-123"）
if (opIndex == -1) {
    bool convertOk;
    num1 = exprNoSpace.toDouble(&convertOk);
    if (convertOk) { ok = true; return num1; }
    return 0;
}

// 除零检查
case '/':
    if (num2 == 0) { ok = false; return 0; }
    return num1 / num2;
```

用 `bool& ok` 输出参数传递成功/失败状态，而不是用异常或哨兵值（返回 0 无法区分「结果就是 0」和「计算出错」）。这是 C++ 里常见的错误处理约定。

**局限**：只支持单个二元运算，`1+2*3` 会因为「只处理第一个运算符」而得到 3 而不是 7。真正的表达式求值需要双栈（操作数栈 + 运算符栈）或递归下降。这个练习止步于二元运算。

### `TPCservice` — 一处跨线程隐患

服务端在 `NewClientHandler()` 里为每个客户端连接起一个线程：

```cpp
void Widget::NewClientHandler()
{
    QTcpSocket * Socket = server->nextPendingConnection();
    // 启动线程
    Mythread * t = new Mythread(Socket);
    t->start();
}
```

`Mythread::run()` 里做信号连接：

```cpp
void Mythread::run()
{
    connect(socket, &QTcpSocket::readyRead, this, &Mythread::clientInfoslot);
}
```

**这里有个 Qt 的经典问题**：`socket` 是在主线程用 `nextPendingConnection()` 创建的，`QThread` 对象 `t` 本身也生活在主线程（`new Mythread(...)` 在 `NewClientHandler` 里调用）。`run()` 虽在子线程执行，但 `connect` 建立的是 `socket → this` 的连接，而 `this` 的线程亲和性仍是主线程。

结果：`clientInfoslot()` 实际会在**主线程**的事件循环里被调用，多线程没有起到分担作用。而且 `Mythread` 继承了 `QThread`，`run()` 结束后线程就退出了——`connect` 建立的连接无法长期存活。

**正确做法**是「moveToThread」模式：

```cpp
// 让 worker 对象（而非 QThread 子类）承载业务逻辑
auto *worker = new SocketWorker;          // QObject 子类
worker->moveToThread(thread);             // 把 worker 移到子线程
connect(thread, &QThread::started, worker, &SocketWorker::setup);
thread->start();
```

关键是**继承 QThread 并重写 run() 只适合「一次性的循环任务」**；需要长期响应信号的对象应该用 `moveToThread`，让对象本身运行在子线程的事件循环里。

这个项目保留的是学习阶段的写法，注释里能看到当时的思考痕迹。

### `TCPclient` — 连接状态的两个信号

```cpp
connect(socket, &QTcpSocket::connected, this, [this]() {
    QMessageBox::information(this, "connection successful", "connection server successful");
    this->hide();
    chat* c = new chat(socket);
    c->show();
});
connect(socket, &QTcpSocket::errorOccurred, this, [this]() {
    QString errormessage = socket->errorString();
    QMessageBox::warning(this, "connection failed", errormessage);
});
```

连接成功后把主窗口 `hide()` 掉、弹出聊天窗口，socket 通过构造函数传给 `chat`。这里 `chat` 窗口没有设置 `Qt::WA_DeleteOnClose`，关闭时不会自动释放——`new` 出来的对象会泄漏。

端口转换用的是：

```cpp
socket->connectToHost(QHostAddress(ip), port.toUShort());
// 注释：uint = unsigned int, 16 = 2byte = short
```

`connectToHost` 第二个参数是 `quint16`，所以从 `QLineEdit` 取出的字符串要转 `toUShort()` 而不是 `toInt()`。

## 四、文件结构

```
qt-learning/
├── 01/                 空窗口入门
├── Qtimer/             定时器
├── Timer/              定时器变体
├── file/               文件读写
├── calculate/          手写解析计算器
├── TCPclient/          TCP 客户端
└── TPCservice/         TCP 服务端 + 多线程
```

每个目录是独立的 qmake 工程（`.pro` + `.cpp` + `.h` + `.ui`），剔除 `build/`、`x64/`、`*.user` 后上传。

相关的完整项目见 [rongyan/qt-desktop](https://github.com/YGtemple/rongyan)（融研智笔 Qt 桌面端，1000+ 行、三栏布局、网络请求与状态持久化）。

## 五、编译运行

```bash
cd calculate
qmake calculate.pro
make          # 或 mingw32-make / nmake
```

需要 Qt6（Core / Gui / Widgets / Network）。TCP 示例需先启动 `TPCservice` 再连 `TCPclient`，默认端口 8000。

## 六、这份记录的价值

这 7 个练习覆盖了 Qt 学习的完整路径：窗口与布局 → 信号槽 → 定时器 → 文件 IO → 字符串处理 → 网络 → 多线程。

保留原始写法的原因：**注释掉的试错代码和当时的错误解法，比正确版本更能说明问题在哪里**。比如 `calculate` 里那句「替换原来的正则表达式解析部分」、`TPCservice` 里线程模型的隐患，都是学习路径上的真实痕迹。

## 许可证

MIT License
