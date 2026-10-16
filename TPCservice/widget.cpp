#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // QTcpServer(this)：指定 parent，随窗口一起销毁
    server = new QTcpServer(this);

    // 监听任意 IPv4 网卡的 PORT 端口
    server->listen(QHostAddress::AnyIPv4, PORT);

    // 有新客户端接入时触发 newConnection
    connect(server, &QTcpServer::newConnection, this, &Widget::NewClientHandler);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::NewClientHandler()
{
    // 取出一个待处理的连接（每次只能取一个，
    // 多个同时接入时 newConnection 会多次触发）
    QTcpSocket *Socket = server->nextPendingConnection();

    // 显示对端地址与端口
    // peerAddress() 取得对方 IP，peerPort() 取对方端口
    ui->ipEdit->setText(Socket->peerAddress().toString());
    ui->portEdit->setText(QString::number(Socket->peerPort()));

    // 收到数据
    connect(Socket, &QTcpSocket::readyRead, this, &Widget::clientInfoslot);

    // 尝试把处理放到子线程
    // 注意：这里传递的 Socket 是在主线程创建的，
    // Mythread 对象也在主线程，因此实际上并没有真正
    // 把处理移到子线程。详见 mythread.cpp 的详细说明。
    Mythread *t = new Mythread(Socket);
    t->start();
}

void Widget::clientInfoslot()
{
    // sender() 返回发出信号的对象，这里就是那个 Socket。
    // 用强制转换拿到它并读取数据。
    QTcpSocket *s = (QTcpSocket *)sender();
    ui->AcceptlineEdit->setText(s->readAll());
}
