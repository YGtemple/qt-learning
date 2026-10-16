#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    socket = new QTcpSocket;

    // ── 连接成功：隐藏连接窗，打开聊天窗 ──
    connect(socket, &QTcpSocket::connected, this, [this]() {
        QMessageBox::information(this, "connection successful",
                                 "connection server successful");
        this->hide();
        // socket 移交给 chat 窗口使用。
        // 注意：这里没设置 Qt::WA_DeleteOnClose，
        // 关闭时就只是隐藏，对象不会释放。
        chat *c = new chat(socket);
        c->show();
    });

    // ── 连接失败：弹出错误信息 ──
    // 注意：Qt 5.15 起 error() 已废弃，改用 errorOccurred()
    connect(socket, &QTcpSocket::errorOccurred, this, [this]() {
        QString errormessage = socket->errorString();
        QMessageBox::warning(this, "connection failed", errormessage);
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_cancelButton_clicked()
{
    this->close();
}

void Widget::on_connectButton_clicked()
{
    // 从输入框取 IP 与端口
    QString ip = ui->iplineEdit->text();
    QString port = ui->portlineEdit->text();

    // connectToHost 的第二个参数是 quint16（2 字节），
    // 所以用 toUShort() 而不是 toInt()，与端口号取值范围一致。
    socket->connectToHost(QHostAddress(ip), port.toUShort());
}
