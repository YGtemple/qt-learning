#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>   // 用于弹出提示对话框
//#include <QRegExp>       // 早期版本用正则解析表达式（已改为纯字符串处理）
//#include <QRegExpValidator>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //this->setMaximumSize(198,281);
    this->setMinimumSize(198,281);
    this->setWindowTitle("计算机");
    QFont f("宋体",14);
    this->setFont(f);

    // 按钮图标。
    // 注意：原代码里写的是绝对路径（指向本地截图目录），
    // 换成相对路径放到 assets/ 下，这样 clone 下来也能直接跑。
    QIcon con("assets/button_icon.jpg");
    ui->pictureButton->setIcon(con);
}

Widget::~Widget()
{
    delete ui;
}

/**
 * 纯字符串解析的二元表达式计算。
 *
 * 不支持运算符优先级：只处理第一个找到的运算符，
 * 所以 "1+2*3" 会得到 3 而不是 7。
 * 真正的表达式求值需要双栈或递归下降，这里止步于二元运算。
 */
double Widget::calculate(const QString& expr, bool& ok)
{
    ok = false;  // 初始化计算状态为失败
    double num1 = 0, num2 = 0;
    char op = '\0';
    int opIndex = -1;

    // 1. 预处理：移除所有空格，简化解析
    QString exprNoSpace = expr;
    exprNoSpace.remove(' ');

    if (exprNoSpace.isEmpty()) {
        return 0;
    }

    // 2. 查找运算符位置（+、-、*、/）
    //
    // 关键：从索引 1 开始，不是 0。
    // 因为 "-3+5" 里第一个 "-" 是负号而非运算符，
    // 从 0 开始找会把它误判成运算符，
    // 于是左边是空字符串、右边是 "3+5"，转换直接失败。
    for (int i = 1; i < exprNoSpace.length(); ++i) {
        QChar c = exprNoSpace.at(i);
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            opIndex = i;
            op = c.toLatin1();
            break;  // 只处理第一个运算符
        }
    }

    // 特殊情况：没找到运算符，但整个串能转成数字（如输入 "-123"）
    if (opIndex == -1) {
        bool convertOk;
        num1 = exprNoSpace.toDouble(&convertOk);
        if (convertOk) {
            ok = true;
            return num1;
        }
        return 0;
    }

    // 3. 分割出两个操作数的字符串
    QString num1Str = exprNoSpace.left(opIndex);
    QString num2Str = exprNoSpace.right(exprNoSpace.length() - opIndex - 1);

    // 4. 转换字符串为数字
    bool num1Ok, num2Ok;
    num1 = num1Str.toDouble(&num1Ok);
    num2 = num2Str.toDouble(&num2Ok);

    if (!num1Ok || !num2Ok) {
        return 0;  // 数字格式无效
    }

    // 5. 执行计算
    ok = true;
    switch (op) {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        if (num2 == 0) {
            ok = false;  // 除数为零
            return 0;
        }
        return num1 / num2;
    default:
        ok = false;
        return 0;
    }
}

// ============================================================
// 数字键：累加到表达式字符串并回显
// ============================================================
void Widget::on_oneButton_clicked()   { express += "1"; ui->lineEdit->setText(express); }
void Widget::on_twoButton_clicked()   { express += "2"; ui->lineEdit->setText(express); }
void Widget::on_threeButton_clicked() { express += "3"; ui->lineEdit->setText(express); }
void Widget::on_fourButton_clicked()  { express += "4"; ui->lineEdit->setText(express); }
void Widget::on_fiveButton_clicked()  { express += "5"; ui->lineEdit->setText(express); }
void Widget::on_sixButton_clicked()   { express += "6"; ui->lineEdit->setText(express); }
void Widget::on_sevenButton_clicked() { express += "7"; ui->lineEdit->setText(express); }
void Widget::on_eightButton_clicked() { express += "8"; ui->lineEdit->setText(express); }
void Widget::on_nineButton_clicked()  { express += "9"; ui->lineEdit->setText(express); }
void Widget::on_zeroButton_clicked()  { express += "0"; ui->lineEdit->setText(express); }

// ============================================================
// 运算符键
// ============================================================
void Widget::on_addButton_clicked()            { express += "+"; ui->lineEdit->setText(express); }
void Widget::on_reduceButton_clicked()         { express += "-"; ui->lineEdit->setText(express); }
void Widget::on_multiplicationButton_clicked() { express += "*"; ui->lineEdit->setText(express); }
void Widget::on_divisionButton_clicked()       { express += "/"; ui->lineEdit->setText(express); }

void Widget::on_equalButton_clicked()
{
    // 早期版本这里是控制台版的 cin/cout + while 循环，
    // 但 GUI 程序应该通过界面交互而非控制台 I/O，所以完全重写了。
    // （原实现见文末注释）

    if (express.isEmpty()) {
        return;
    }

    bool ok;
    double result = calculate(express, ok);

    if (ok) {
        // 计算成功：显示结果，并把结果作为下一次计算的起始值（支持连续计算）
        express = QString::number(result);
        ui->lineEdit->setText(express);
    } else {
        // 计算失败：根据错误类型显示不同提示
        if (express.contains("/0")) {
            QMessageBox::critical(this, "错误", "除数不能为零！");
        } else {
            QMessageBox::critical(this, "错误", "无效的表达式！\n请使用类似 '3+4' 的格式");
        }
    }
}

void Widget::on_clearButton_clicked()
{
    express.clear();
    ui->lineEdit->clear();
}

void Widget::on_deleteButton_clicked()
{
    express.chop(1);   // 退格：删掉最后一个字符
    ui->lineEdit->setText(express);
}

void Widget::on_pictureButton_clicked()
{
    // 装饰性按钮，无功能
}

// ============================================================
// 附：被替换掉的控制台版实现（学习痕迹，保留供对照）
// ============================================================
// void Widget::on_equalButton_clicked()
// {
//     cout << "欢迎使用计算器2.0" << endl;
//     cout << "请输入两个数字和一个运算符（+、-、*、/）" << endl;
//     cout << "输入'exit'退出程序" << endl;
//
//     while (true) {
//         double num1, num2;
//         char op;
//         cout << "请输入计算式（例如：3 + 4）：";
//         cin >> num1 >> op >> num2;
//         if (cin.fail()) {
//             cin.clear();
//             string input;
//             cin >> input;
//             if (input == "exit") break;
//             cout << "无效输入，请重新输入。" << endl;
//             continue;
//         }
//         switch (op) {
//         case '+': cout << "结果: " << num1 + num2 << endl; break;
//         case '-': cout << "结果: " << num1 - num2 << endl; break;
//         case '*': cout << "结果: " << num1 * num2 << endl; break;
//         case '/':
//             if (num2 != 0) cout << "结果: " << num1 / num2 << endl;
//             else           cout << "错误：除数不能为零。" << endl;
//             break;
//         default: cout << "无效的运算符，请使用 +、-、* 或 /。" << endl;
//         }
//     }
//     cout << "感谢使用计算器2.0，再见！" << endl;
// }
