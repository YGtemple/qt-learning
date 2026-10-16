#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class chat;
}

class chat : public QWidget
{
    Q_OBJECT

public:
    // socket 从外部传入（由 Widget 建立连接后移交）
    explicit chat(QTcpSocket * s, QWidget *parent = nullptr);
    ~chat();

private slots:
    void on_clearButton_clicked();
    void on_sendButton_clicked();

private:
    Ui::chat *ui;
    QTcpSocket *Socket;
};
#endif // CHAT_H
