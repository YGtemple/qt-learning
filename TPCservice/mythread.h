#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>

/**
 * 尝试用子线程处理客户端连接的 socket。
 *
 * ⚠ 这个写法有跨线程问题，详见 mythread.cpp 的说明。
 * 保留原样是为了记录学习过程。
 */
class Mythread : public QThread
{
    Q_OBJECT

public:
    explicit Mythread(QTcpSocket * socket, QObject *parent = nullptr);

    /**
     * 重写 run()：QThread::start() 后在新线程里执行这里。
     * 注意 run() 返回后线程就结束了——不适合需要长期
     * 响应信号的场景。
     */
    void run();

private slots:
    void clientInfoslot();

private:
    QTcpSocket * socket;
};

#endif // MYTHREAD_H
