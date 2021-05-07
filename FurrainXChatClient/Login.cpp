#include "Login.h"
#include "ui_Login.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <qmath.h>
QString Myname;
Login::Login(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    ui->widget->setMouseTracking(true);
    ui->label_4->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setFixedWidth(540 + 18);
    setFixedHeight(410 +8);
    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress("192.168.2.241"), 5748);
    connect(socket, &QTcpSocket::stateChanged, this, [this]() {
        if (ui->label_3 == nullptr) {
            return;
        }
        //更新窗口状态
        switch (socket->state()) {
        case QAbstractSocket::UnconnectedState:
            ui->label_3->setText(QStringLiteral("未连接到服务器，请检查网络连接后重试！"));
            break;
        case QAbstractSocket::HostLookupState:
            ui->label_3->setText(QStringLiteral("请稍等。"));
            break;
        case QAbstractSocket::ConnectingState:
            ui->label_3->setText(QStringLiteral("正在连接请稍等。"));
            break;
        case QAbstractSocket::ConnectedState:
            ui->label_3->setText(QStringLiteral("连接成功"));
            break;
        case QAbstractSocket::BoundState:
            ui->label_3->setText(QStringLiteral("请稍等。"));
            break;
        case QAbstractSocket::ClosingState:
            ui->label_3->setText(QStringLiteral("正在关闭连接"));
            break;
        case QAbstractSocket::ListeningState:
            ui->label_3->setText(QStringLiteral("请稍等。"));
            break;
        }
    });

    connect(socket, &QTcpSocket::readyRead, this, [this]() {
        //接受消息
        QByteArray array = socket->readAll();
        QJsonParseError error;
        QJsonDocument doc;
        QJsonValue value;
        QJsonObject obj;
        QJsonArray jsonArray;
        doc = QJsonDocument::fromJson(array, &error);
        if (!doc.isNull() && (error.error == QJsonParseError::NoError)) {
            obj = doc.object();
            value = obj.value("type");
            qDebug() << "xxx:  " << value;
            if (value.toString() == "registeredMessage") {
                value = obj.value("registeredResult");
                switch (value.toInt()) {
                case 0:
                    QMessageBox::warning(this,
                        QStringLiteral("错误信息"),
                        QStringLiteral("账号已存在"));
                    break;
                case 1:
                    QMessageBox::information(this,
                        QStringLiteral("信息"),
                        QStringLiteral("注册成功，您可以使用此账号登录。"));
                    break;
                }
            }
            else if (value.toString() == "loadMessage") {
                value = obj.value("loginResult");
                switch (value.toInt()) {
                case 0:
                    QMessageBox::warning(this,
                        QStringLiteral("错误信息"),
                        QStringLiteral("账号或密码错误"));
                    break;
                case 1:
                    Myname = obj.value("name").toString();
                    MainWindow *m = new MainWindow(socket);
                    connect(this, SIGNAL(ShangxianSignal(QString)), m, SLOT(ShangxianSlot(QString)));
                    connect(this, SIGNAL(XiaxianSignal(QString)), m, SLOT(XiaxianSlot(QString)));
                    connect(this, SIGNAL(ChatSignal(QString, QString, QString)), m, SLOT(ChatSlot(QString, QString, QString)));
                    m->show();
                    m->setWindowTitle("Furrain Chat");
                    this->hide();
                    QJsonObject::iterator it;
                    for (it = obj.begin(); it != obj.end(); ++it) {
                        qDebug() << it.key();
                        if (it.key().contains("zaixianname")) {
                            emit ShangxianSignal(it.value().toString());
                        }
                    }
                }
            }
            else if (value.toString() == "shangxian") {
                //ui->listWidget->addItem(obj.value("name").toString());
                emit ShangxianSignal(obj.value("name").toString());
            }
            else if (value.toString() == "yizaixian") {
                emit ShangxianSignal(obj.value("name").toString());
            }
            else if (value.toString() == "xiaxian") {
                emit XiaxianSignal(obj.value("name").toString());
            }
            else if (value.toString() == "chat") {
                QString from = obj.value("from").toString();
                QString to = obj.value("to").toString();
                QString str = obj.value("content").toString();
                qDebug() << "login : >>" << str;
                emit ChatSignal(from, to, str);
            }
        }
        //QList<QByteArray> list = array.split('}');
        //QByteArray array1;
        //for (int i = 0; i < list.size(); i++) {
        //    if (list[i].length() < 2) {
        //        continue;
        //    }
        //    array1 = list[i] + "}";
        //    QJsonParseError error;
        //    QJsonDocument doc;
        //    QJsonValue value;
        //    QJsonObject obj;
        //    QJsonArray jsonArray;
        //    doc = QJsonDocument::fromJson(array1, &error);
        //    if (!doc.isNull() && (error.error == QJsonParseError::NoError)) {
        //        obj = doc.object();
        //        value = obj.value("type");
        //        qDebug() << "xxx:  " << value;
        //        if (value.toString() == "registeredMessage") {
        //            value = obj.value("registeredResult");
        //            switch (value.toInt()) {
        //            case 0:
        //                QMessageBox::warning(this,
        //                    QStringLiteral("错误信息"),
        //                    QStringLiteral("账号已存在"));
        //                break;
        //            case 1:
        //                QMessageBox::information(this,
        //                    QStringLiteral("信息"),
        //                    QStringLiteral("注册成功，您可以使用此账号登录。"));
        //                break;
        //            }
        //        }
        //        else if (value.toString() == "loadMessage") {
        //            value = obj.value("loginResult");
        //            switch (value.toInt()) {
        //            case 0:
        //                QMessageBox::warning(this,
        //                    QStringLiteral("错误信息"),
        //                    QStringLiteral("账号或密码错误"));
        //                break;
        //            case 1:
        //                Myname = obj.value("name").toString();
        //                MainWindow *m = new MainWindow(socket);
        //                connect(this, SIGNAL(ShangxianSignal(QString)), m, SLOT(ShangxianSlot(QString)));
        //                connect(this, SIGNAL(XiaxianSignal(QString)), m, SLOT(XiaxianSlot(QString)));
        //                connect(this, SIGNAL(ChatSignal(QString, QString, QString)), m, SLOT(ChatSlot(QString, QString, QString)));
        //                m->show();
        //                m->setWindowTitle("Furrain Chat");
        //                this->hide();
        //            }
        //        }
        //        else if (value.toString() == "shangxian") {
        //            //ui->listWidget->addItem(obj.value("name").toString());
        //            emit ShangxianSignal(obj.value("name").toString());
        //        }
        //        else if (value.toString() == "yizaixian") {
        //            emit ShangxianSignal(obj.value("name").toString());
        //        }
        //        else if (value.toString() == "xiaxian") {
        //            emit XiaxianSignal(obj.value("name").toString());
        //        }
        //        else if (value.toString() == "chat") {
        //            QString from = obj.value("from").toString();
        //            QString to = obj.value("to").toString();
        //            QString str = obj.value("content").toString();
        //            qDebug() << "login : >>" << str;
        //            emit ChatSignal(from, to,str);
        //        }
        //    }
        //}
    });

    close = QRect(540 + 18 - 43 - 10, 9, 43, 34);
    min = QRect(540 + 18 - 10 - 43 * 2, 9, 43, 34);
    log = QRect(9, 9, 540, 158);
}

Login::~Login()
{
    if (socket != nullptr) {
        socket->disconnect();
        delete socket;
        socket = nullptr;
    }
    delete ui;
}

void Login::on_pushButton_login_clicked()
{
        qDebug() << "c0";
    //如果用户名或者密码为空提示用户
    if (ui->lineEdit_name->text().isEmpty() || ui->lineEdit_2_pwd->text().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误信息"), QStringLiteral("请输入用户名以及密码！"));
        return;
    }
    //构造Json数据
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QJsonObject obj;
        obj.insert("type", "login");
        obj.insert("userName", ui->lineEdit_name->text());
        obj.insert("passWord", ui->lineEdit_2_pwd->text());
        quint16 p = socket->localPort();
        obj.insert("udpPort", p);
        QJsonDocument doc(obj);

        //发送数据
        socket->write(doc.toJson());
    }
}

void Login::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPainterPath path;
    path.addRoundedRect(QRect(9, 9, 540, 400), 5, 5);
    painter.fillPath(path, QColor(255, 255, 255));
    path.clear();
    path.addRoundedRect(log, 5.0, 5.0);
    painter.save();
    painter.setClipPath(path);

    logmap = QPixmap(":/show.png");
    painter.drawPixmap(log, logmap);
    if (!closeHovered) {
        closemap = QPixmap(":/close_0.png");
    } else {
        closemap = QPixmap(":/close_1.png");
        painter.fillRect(close, Qt::red);
    }
    painter.drawPixmap(close, closemap);
    if (!minHovered) {
        minmap = QPixmap(":/min_0.png");
    } else {
        minmap = QPixmap(":/min_1.png");
        painter.fillRect(min, Qt::lightGray);
    }
    painter.drawPixmap(min, minmap);
    painter.restore();
    PaintShadow(&painter);
}

void Login::PaintShadow(QPainter *painter)
{
    //QColor color(50, 50, 50, 30);
    //int SHADOW_WIDTH = 8;
    //for (int i = 0; i < SHADOW_WIDTH; i++)
    //{
    //    QPainterPath path;
    //    color.setAlpha(120 - qSqrt(i) * 40);
    //    painter->setPen(color);
    //    // 方角阴影边框;
    // //   painter.drawRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2, this->height() - (SHADOW_WIDTH - i) * 2);
    //    // 圆角阴影边框;
    //    if (i == 5)
    //        path.addRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2, this->height() - (SHADOW_WIDTH - i) * 2);
    //    else 

    //        path.addRoundedRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2, this->height() - (SHADOW_WIDTH - i) * 2, 5, 5);
    //    painter->drawPath(path);
    //}

    QColor color(92, 93, 95, 50);
    int arr[10] = {150, 120, 80, 50, 40, 30, 20, 10, 5, 5};
    for (int i = 0; i < 10; i++) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        if (i == 5)
            path.addRect(9 - i - 1,
                         9 - i - 1,
                         this->width() - (9 - i) * 2,
                         this->height() - (9 - i) * 2);
        else
            path.addRoundedRect(9 - i - 1,
                                9 - i - 1,
                                this->width() - (9 - i) * 2,
                                this->height() - (9 - i) * 2,
                                2,
                                2);

        color.setAlpha(arr[i]);
        painter->setPen(color);
        painter->drawPath(path);
    }
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    if (min.contains(event->pos())) {
        minHovered = true;
        closeHovered = false;
    } else if (close.contains(event->pos())) {
        closeHovered = true;
        minHovered = false;
    } else {
        minHovered = false;
        closeHovered = false;
    }
    if (isPressed) {
        QPoint current = event->pos();
        move(current + pos() - pressPoint);
    }
    update();
}

void Login::mousePressEvent(QMouseEvent *event)
{
    pressPoint = event->pos();
    isPressed = true;
}

void Login::mouseReleaseEvent(QMouseEvent *event)
{
    if (min.contains(pressPoint) && min.contains(event->pos())) {
        setWindowState(Qt::WindowMinimized);
        minHovered = false;
    }
    if (close.contains(pressPoint) && close.contains(event->pos())) {
        qApp->exit(0);
    }
    isPressed = false;
    update();
}

void Login::on_label_linkActivated(const QString &link)
{
    qDebug() << "c0";
    //如果用户名或者密码为空提示用户
    if (ui->lineEdit_name->text().isEmpty() || ui->lineEdit_2_pwd->text().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("错误信息"), QStringLiteral("请输入用户名以及密码！"));
        return;
    }
    //构造Json数据
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QJsonObject obj;
        obj.insert("type", "login");
        obj.insert("userName", ui->lineEdit_name->text());
        obj.insert("passWord", ui->lineEdit_2_pwd->text());
        quint16 p = socket->localPort();
        obj.insert("udpPort", p);
        QJsonDocument doc(obj);

        //发送数据
        socket->write(doc.toJson());
    }
}
