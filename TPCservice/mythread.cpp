#include "mythread.h"

Mythread::Mythread(QTcpSocket * s, QObject *parent)
    : QThread(parent)
{
    socket = s;
}

/**
 * 跨线程问题说明
 *
 * socket 是在主线程用 server->nextPendingConnection() 创建的，
 * 而 this（Mythread 对象）也生活在主线程——因为 new Mythread(...)
 * 是在 Widget::NewClientHandler() 里调用的。
 *
 * QObject 的线程亲和性在对象创建时就确定了，run() 虽在子线程
 * 执行，但 connect(socket, ..., this, ...) 实际上会以 this 的
 * 亲和性（主线程）为准——clientInfoslot() 最终还是在主线程
 * 事件循环里被调用，多线程没有起到分担作用。
 *
 * 另外，run() 一旦返回，线程就结束了，这里建立的信号连接
 * 无法长期存活。
 *
 * 正确做法（moveToThread 模式）：
 *
 *   // 让普通 QObject 承载业务逻辑，而不是继承 QThread
 *   auto *worker = new SocketWorker;
 *   auto *thread = new QThread(this);
 *   worker->moveToThread(thread);           // 把 worker 移到子线程
 *   connect(thread, &QThread::started, worker, &SocketWorker::setup);
 *   connect(socket, &QTcpSocket::readyRead, worker, &SocketWorker::onData);
 *   thread->start();
 *
 * 关键区别：继承 QThread 并重写 run() 只适合「一次性的循环任务」
 * （如一个跑到底的计算）；需要长期响应信号的对象应该用
 * moveToThread，让它自己跑在子线程的事件循环里。
 */
void Mythread::run()
{
    connect(socket, &QTcpSocket::readyRead, this, &Mythread::clientInfoslot);
}

void Mythread::clientInfoslot()
{
    qDebug() << socket->readAll();
}
