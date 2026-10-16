#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>

#define TIMEOUT 1*1000   // 毫秒为单位，这里是 1 秒

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_startButton_2_clicked();   // singleShot 单次定时
    void timeoutslot();                // 定时器超时回调

private:
    Ui::Widget *ui;
    QTimer *timer;
    int picid;      // 当前显示到第几帧
};
#endif // WIDGET_H
