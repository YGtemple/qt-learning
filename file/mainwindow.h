#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    // 重写事件处理器——注意不是槽函数，由 Qt 框架调用
    void keyPressEvent(QKeyEvent *k) override;
    void mousePressEvent(QMouseEvent *m) override;

private slots:
    void Nactionslot();   // 新建
    void Oactionslot();   // 打开
    void Sactionslot();   // 保存
    void Eactionslot();   // 编辑（空实现）
    void Bactionslot();   // 构建（空实现）

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
