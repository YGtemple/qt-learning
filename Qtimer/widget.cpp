#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 堆区创建（无 parent，需在析构中处理；
    // 更推荐 new QTimer(this) 交给对象树管理）
    timer = new QTimer;

    picid = 39;

    // 初始显示一帧。
    // 注意：原代码用的是指向本地截图目录的绝对路径，
    // 已改为相对路径；请自行把图片放到 assets/frames/ 下，
    // 命名为 39.jpg / 40.jpg / 41.jpg。
    QImage img;
    img.load("assets/frames/39.jpg");
    ui->label->setPixmap(QPixmap::fromImage(img));

    // 信号槽四要素：谁发出、发什么信号、谁处理、怎么处理
    connect(timer, &QTimer::timeout, this, &Widget::timeoutslot);
}

Widget::~Widget()
{
    delete ui;
}

/** 启动周期定时器（需要手动 stop 才会停） */
void Widget::on_startButton_clicked()
{
    timer->start(TIMEOUT);
}

/** 定时器超时：切换到下一帧，循环播放 39→41 */
void Widget::timeoutslot()
{
    QString path("assets/frames/");
    path += QString::number(picid);
    path += ".jpg";

    QImage img;
    img.load(path);
    ui->label->setPixmap(QPixmap::fromImage(img));
    picid++;

    if (picid == 42) {
        picid = 39;   // 循环回第一帧
    }
}

void Widget::on_stopButton_clicked()
{
    timer->stop();
}

/**
 * QTimer::singleShot —— 一次性定时器。
 *
 * 适合「延迟一段时间后只执行一次」的场景，不用自己
 * 管理 QTimer 对象的生命周期。
 *
 * 这里用的是旧版 SLOT 宏写法；Qt 5 起推荐：
 *   QTimer::singleShot(1000, this, &Widget::timeoutslot);
 * 或带 lambda：
 *   QTimer::singleShot(1000, this, [this]{ ... });
 */
void Widget::on_startButton_2_clicked()
{
    QTimer::singleShot(1000, this, SLOT(timeoutslot()));
}
