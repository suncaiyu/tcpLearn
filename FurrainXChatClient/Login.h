#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>
#include "MainWindow.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_login_clicked();

private:
    Ui::Login *ui;
    QTcpSocket *socket;

signals:
    void ShangxianSignal(QString str);
    void XiaxianSignal(QString str);
};

#endif // LOGIN_H
