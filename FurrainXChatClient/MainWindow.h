#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtcpsocket.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QTcpSocket *socket, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    int titleheight = 35;
    QPixmap minmap;
    QPixmap closemap;
    QRect minRect;
    QRect closeRect;
    bool minHovered = false;
    bool closeHovered = false;
    bool isPressed = false;
    QPoint pressPoint;

private slots:
    void ShangxianSlot(QString str);
    void XiaxianSlot(QString str);
    void ChatSlot(QString, QString, QString);
    void on_toolButton_clicked();
    void PaintShadow(QPainter *painter);
    void PaintTitle(QPainter *painter);
};
#endif // MAINWINDOW_H
