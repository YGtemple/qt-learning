#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    pixid = 39;

    // 原代码用绝对路径指向本地截图目录，已改为相对路径。
    // 请自行把图片放到 assets/frames/ 下，命名 38.jpg / 39.jpg / 40.jpg。
    QPixmap pix("assets/frames/38.jpg");
    ui->label->setPixmap(pix);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_startButton_clicked()
{
    // 开启定时器，保存返回的编号
    timeid = this->startTimer(TIMEOUT);
}

/**
 * 定时器事件处理器。
 *
 * 与 QTimer（见 Qtimer 项目）的区别：
 *
 *   QTimer 方式      面向对象，timeout 信号 → 槽
 *                     可读性好，可以同时多个独立定时器
 *
 *   timerEvent 方式  重写虚函数，所有定时器都进这一个函数，
 *                     需要用 timerId() 区分是哪一个。
 *                     适合简单的单定时器场景，代码更短。
 *
 * 下面这个前置判断是必须的：
 * 如果界面里有多个定时器，不判断 id 的话每个定时器
 * 超时都会让图片翻帧，速度会成倍变快。
 */
void Widget::timerEvent(QTimerEvent* event)
{
    // 不是自己启动的那个定时器就忽略
    if (event->timerId() != timeid) {
        return;
    }

    QString path("assets/frames/");
    path += QString::number(pixid);
    path += ".jpg";

    QPixmap pix(path);
    ui->label->setPixmap(pix);
    pixid++;

    if (pixid == 41) {
        pixid = 38;   // 循环回第一帧
    }
}

void Widget::on_stopButton_clicked()
{
    this->killTimer(timeid);
}
