#include "chat.h"
#include "ui_chat.h"

chat::chat(QTcpSocket *s, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chat)
{
    ui->setupUi(this);
    Socket = s;
}

chat::~chat()
{
    delete ui;
}

void chat::on_clearButton_clicked()
{
    ui->inputlineEdit->clear();
}

void chat::on_sendButton_clicked()
{
    QByteArray ba;
    // 显式 toUtf8()：网络传输的是字节流，
    // QString 是 UTF-16，不转会出问题
    ba.append(ui->inputlineEdit->text().toUtf8());
    Socket->write(ba);
}
