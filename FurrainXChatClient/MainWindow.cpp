#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
MainWindow::MainWindow(QTcpSocket *s, QWidget *parent)
    : QMainWindow(parent), socket(s)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::ShangxianSlot(QString name)
{
    ui->listWidget->addItem(name);
}

void MainWindow::XiaxianSlot(QString str)
{
    for (int i = 0; i < ui->listWidget->count(); i++) {
        if (ui->listWidget->item(i)->text() == str) {
            QListWidgetItem *item = ui->listWidget->takeItem(i);
            delete item;
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    if (socket != nullptr) {
        delete socket;
        socket = nullptr;
    }
    delete ui;
}

