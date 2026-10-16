#ifndef MYPROJECT_H
#define MYPROJECT_H

#include <QWidget>
#include <QProcess>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class myproject;
}
QT_END_NAMESPACE

class myproject : public QWidget
{
    Q_OBJECT

public:
    myproject(QWidget *parent = nullptr);
    ~myproject();

private slots:
    void on_commitButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::myproject *ui;
};
#endif // MYPROJECT_H
