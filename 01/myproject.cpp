#include "myproject.h"
#include "ui_myproject.h"

myproject::myproject(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::myproject)
{
    ui->setupUi(this);

    // ── connect 的两种写法（学习过程中的对比）──
    //
    // 旧写法：SIGNAL/SLOT 宏，运行时字符串匹配。优点是可读，
    // 缺点是写错不会编译报错，只在运行时静默失效。
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(on_commitButton_clicked()));

    // 新写法：函数指针。编译期就能检查类型，推荐使用。
    connect(ui->cancelButton, &QPushButton::clicked, this, &myproject::on_cancelButton_clicked);

    // 第三个参数为 this 时，接收者销毁后连接自动断开，不用手动 disconnect
    connect(ui->browseButton, &QPushButton::clicked, [this] {
        QMessageBox::information(this, "信息", "点击浏览");
    });
}

myproject::~myproject()
{
    delete ui;
}

/**
 * 把输入框里的字符串当作程序路径启动。
 *
 * 注意：这是学习 QProcess 的示例，直接把用户输入交给
 * QProcess::start() 没有任何校验。实际产品中需限制可执行
 * 文件范围（否则等于开放了任意命令执行）。
 *
 * 另外 myprocess 指定了 this 作为 parent，对象会随窗口
 * 一起销毁，不需要手动 delete。
 */
void myproject::on_commitButton_clicked()
{
    QString program = ui->lineEdit->text();
    QProcess *myprocess = new QProcess(this);
    myprocess->start(program);
}

void myproject::on_cancelButton_clicked()
{
    this->close();
}
