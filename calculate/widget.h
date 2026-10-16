#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

//#include<iostream>
//using namespace std;

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
    // 数字键 0-9（由 Qt 的自动连接机制绑定到同名 .ui 控件）
    void on_oneButton_clicked();
    void on_twoButton_clicked();
    void on_threeButton_clicked();
    void on_fourButton_clicked();
    void on_fiveButton_clicked();
    void on_sixButton_clicked();
    void on_sevenButton_clicked();
    void on_eightButton_clicked();
    void on_nineButton_clicked();
    void on_zeroButton_clicked();

    // 四则运算符
    void on_addButton_clicked();
    void on_reduceButton_clicked();
    void on_multiplicationButton_clicked();
    void on_divisionButton_clicked();

    void on_equalButton_clicked();   // 等号：触发计算
    void on_clearButton_clicked();   // 清空
    void on_deleteButton_clicked();  // 退格
    void on_pictureButton_clicked(); // 贴图按钮（装饰用）

private:
    Ui::Widget *ui;
    QString express;   // 当前累积的表达式字符串

    /**
     * 计算二元表达式。
     *
     * 用 bool& ok 输出参数而非返回值传递成败：
     * 返回 0 无法区分「结果就是 0」和「计算出错」。
     */
    double calculate(const QString& expr, bool& ok);
};
#endif // WIDGET_H
