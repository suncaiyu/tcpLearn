#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <ChatWidget.h>
#include <QDebug>
#include <QFileDialog>
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>
extern QString Myname;
MainWindow::MainWindow(QTcpSocket *s, QWidget *parent)
    : QMainWindow(parent), socket(s)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    QPixmap pm(":/touxiang.jpg");
    ui->toolButton->setIcon(pm.scaled(66, 66));
//    ui->toolButton->setPixmap(pm.scaled(66, 66));
    ui->label_2->setText(Myname);
    this->setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);
    ui->widget_2->setMouseTracking(true);
    ui->label->setMouseTracking(true);
}

void MainWindow::ShangxianSlot(QString name)
{
    ui->listWidget->addItem(name);
    ChatWidget *chat = new ChatWidget(socket, name);
    ui->tabWidget->addTab(chat, name);
}

void MainWindow::XiaxianSlot(QString str)
{
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        if (ui->tabWidget->tabText(i) == str) {
            QWidget *w = ui->tabWidget->widget(i);
            ui->tabWidget->removeTab(i);
            delete w;
            w = nullptr;
        }
    }
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

void MainWindow::ChatSlot(QString f, QString t, QString s)
{
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        if (ui->tabWidget->tabText(i) == f) {
            ChatWidget *cw = qobject_cast<ChatWidget *>(ui->tabWidget->widget(i));
            cw->Write(s);
//            cw->WriteList(s);
            break;
        }
    }
}

void MainWindow::on_toolButton_clicked()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("选中文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("File(*.jpg *.png *.bmp)"));
    //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList fileNames;
    if (fileDialog->exec()) {
        fileNames = fileDialog->selectedFiles();
    }
    qDebug() << fileNames;
    QPixmap pm(fileNames.first());
    ui->toolButton->setIcon(pm.scaled(66, 66));
    delete fileDialog;
    fileDialog = nullptr;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(QRect(10,10,ui->widget_2->width(), ui->widget_2->height()), 5, 5);
    painter.fillPath(path, QColor(255, 255, 255));
    PaintTitle(&painter);
    painter.setClipPath(path);
    closeRect = QRect(this->width() - 10 - 43, 10, 43, 34);
    minRect = QRect(this->width() - 10 - 43 * 2, 10, 43, 34);
    if (!closeHovered) {
        closemap = QPixmap(":/close_0.png");
    } else {
        closemap = QPixmap(":/close_1.png");
        painter.fillRect(closeRect, Qt::red);
    }
    painter.drawPixmap(closeRect, closemap);
    if (!minHovered) {
        minmap = QPixmap(":/min_0.png");
    } else {
        minmap = QPixmap(":/min_1.png");
        painter.fillRect(minRect, Qt::lightGray);
    }
    painter.drawPixmap(minRect, minmap);
    PaintShadow(&painter);
}

void MainWindow::PaintTitle(QPainter *painter)
{
    QRect title(10, 10, this->width() - 10 * 2, titleheight);
    QPainterPath path;
    path.addRoundedRect(QRect(10, 10, this->width() - 10 * 2, ui->widget_2->height()), 5, 5);
    painter->setClipPath(path);
    painter->fillRect(title, QColor(0, 0, 0, 155));
//    QPen pen = painter->pen();
    painter->setFont(QFont("楷体",14,1));
    painter->setPen(QPen(Qt::white));
//    QFont(字体，大小，线宽)
    painter->drawText(title, Qt::AlignVCenter, QStringLiteral(" 瞎聊"));
}

void MainWindow::PaintShadow(QPainter *painter)
{
    QColor color(50, 50, 50, 30);
    int SHADOW_WIDTH = 10;
    int w = ui->centralwidget->width();
    for (int i = 0; i < SHADOW_WIDTH; i++)
    {
        color.setAlpha(120 - qSqrt(i) * 40);
        painter->setPen(color);
        // 方角阴影边框;
     //   painter.drawRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2, this->height() - (SHADOW_WIDTH - i) * 2);
        // 圆角阴影边框;
        painter->drawRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, w - (SHADOW_WIDTH - i) * 2, ui->centralwidget->height() - (SHADOW_WIDTH - i) * 2, 5, 5);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (closeRect.contains(e->pos())) {
        closeHovered = true;
        minHovered = false;
    } else if (minRect.contains(e->pos())) {
        minHovered = true;
        closeHovered = false;
    } else {
        minHovered = false;
        closeHovered = false;
    }
    if (isPressed) {
        move(e->globalPos() - pressPoint);
    }
    update();
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (QRect(10, 10, ui->widget_2->width(), titleheight).contains(e->pos())) {
        pressPoint = e->pos();
        isPressed = true;
    }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (minRect.contains(e->pos()) && minRect.contains(pressPoint)) {
        this->setWindowState(Qt::WindowMinimized);
        minHovered = false;
    }
    if (closeRect.contains(e->pos()) && closeRect.contains(pressPoint)) {
        qApp->exit(0);
    }
    isPressed = false;
    update();
}
