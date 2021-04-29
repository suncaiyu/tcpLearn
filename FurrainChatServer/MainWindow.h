#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QSqlTableModel *tableModel;
    QTcpServer *mServer;

    QVector<QTcpSocket *> socketVector;
    QMap<QTcpSocket *, QString> userConnection;

private slots:
    void NewConnectionSlot();
    void ReceiveSocketSlot();
    void DisconnectSlot();
};
#endif // MAINWINDOW_H
