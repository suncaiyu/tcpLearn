#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QTcpSocket>
#include "MainWindow.h"

extern QString Myname;
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_pushButton_login_clicked();

    void on_label_linkActivated(const QString &link);

private:
    Ui::Login *ui;
    QTcpSocket *socket;
    // 43   34
    QRect close;
    QRect min;
    QRect log;
    QPixmap closemap;
    QPixmap minmap;
    QPixmap logmap;
    bool minHovered = false;
    bool closeHovered = false;

    QPoint pressPoint;
    bool isPressed = false;
    void PaintShadow(QPainter *painter);

signals:
    void ShangxianSignal(QString str);
    void XiaxianSignal(QString str);
    void ChatSignal(QString from, QString to, QString str);

};

#endif // LOGIN_H
