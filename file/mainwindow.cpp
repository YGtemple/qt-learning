#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 菜单项绑定。第三个参数传了 this，
    // 且槽函数是成员函数——连接会随对象销毁自动断开。
    connect(ui->action_N, &QAction::triggered, this, &MainWindow::Nactionslot);
    connect(ui->action_O, &QAction::triggered, this, &MainWindow::Oactionslot);
    connect(ui->action_S, &QAction::triggered, this, &MainWindow::Sactionslot);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Nactionslot()
{
    ui->textEdit->clear();
    this->setWindowTitle("新建文本文档.txt");
}

void MainWindow::Oactionslot()
{
    // 第三个参数是默认目录，第四个是文件过滤器。
    // 这里把默认目录设成程序自己的路径、过滤器限为 *.cpp，
    // 是学习时的临时做法：
    //   - QCoreApplication::applicationFilePath() 返回的是可执行文件
    //     的完整路径（不是目录），当默认目录用并不合适，
    //     应该用 applicationDirPath()
    //   - 过滤器只给 "*.cpp" 会让用户看不到其他文件，
    //     实际应写成 "文本文件 (*.txt *.cpp *.h);;所有文件 (*)"
    QString fileName = QFileDialog::getOpenFileName(this, "请选择一个文件",
                                                    QCoreApplication::applicationFilePath(),
                                                    "*.cpp");
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择正确文件！");
    } else {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QByteArray ba = file.readAll();
        ui->textEdit->setText(ba);
        file.close();
    }
}

void MainWindow::Sactionslot()
{
    QString fileName = QFileDialog::getSaveFileName(this, "请选择一个文件",
                                                    QCoreApplication::applicationFilePath());
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择正确文件！");
    } else {
        QFile file(fileName);
        file.open(QIODevice::WriteOnly);
        QByteArray ba;
        // 显式 toUtf8()：QString 内部是 UTF-16，
        // 写入文件前要转成 UTF-8，否则中文会乱码
        ba.append(ui->textEdit->toPlainText().toUtf8());
        file.write(ba);
        file.close();
    }
}

void MainWindow::Eactionslot()
{
    // 未实现
}

void MainWindow::Bactionslot()
{
    // 未实现
}

/** 重写键盘事件：Ctrl+S 触发保存 */
void MainWindow::keyPressEvent(QKeyEvent *k)
{
    if (k->modifiers() == Qt::ControlModifier && k->key() == Qt::Key_S) {
        Sactionslot();
    }
}

/** 重写鼠标事件：打印坐标与按键类型 */
void MainWindow::mousePressEvent(QMouseEvent *m)
{
    QPoint pt = m->pos();
    qDebug() << pt;
    if (m->button() == Qt::LeftButton) {
        qDebug() << "左键被按下";
    } else if (m->button() == Qt::RightButton) {
        qDebug() << "右键被按下";
    }
}
