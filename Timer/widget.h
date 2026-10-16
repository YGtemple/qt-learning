#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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

    /**
     * 重写 timerEvent：这是 QObject 的虚函数，
     * 由 startTimer() 启动的定时器触发。
     */
    virtual void timerEvent(QTimerEvent* event) override;

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();

private:
    Ui::Widget *ui;
    int timeid;    // startTimer 返回的定时器编号
    int pixid;
};
#endif // WIDGET_H
