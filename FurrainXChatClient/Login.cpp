#include "Login.h"
#include "ui_Login.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
Login::Login(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::Login)
{
    ui->setupUi(this);
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
        QList<QByteArray> list = array.split('}');
        QByteArray array1;
        for (int i = 0; i < list.size(); i++) {
            if (list[i].length() < 2) {
                continue;
            }
            array1 = list[i] + "}";
            QJsonParseError error;
            QJsonDocument doc;
            QJsonValue value;
            QJsonObject obj;
            QJsonArray jsonArray;
            doc = QJsonDocument::fromJson(array1, &error);
            if (!doc.isNull() && (error.error == QJsonParseError::NoError)) {
                obj = doc.object();
                value = obj.value("type");
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
                        MainWindow *m = new MainWindow(socket);
                        connect(this, SIGNAL(ShangxianSignal(QString)), m, SLOT(ShangxianSlot(QString)));
                        connect(this, SIGNAL(XiaxianSignal(QString)), m, SLOT(XiaxianSlot(QString)));
                        m->show();
                        this->hide();
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
            }
        }
    });
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

void Login::on_pushButton_2_clicked()
{
    if (ui->lineEdit_name->text().isEmpty() || ui->lineEdit_2_pwd->text().isEmpty()) {
        QMessageBox::warning(this,
                             QStringLiteral("错误信息"),
                             QStringLiteral("请输入用户名以及密码！"));
        return;
    }
    QString name = ui->lineEdit_name->text();
    QString pwd = ui->lineEdit_2_pwd->text();
    if (socket->state()==QAbstractSocket::ConnectedState) {
        QJsonObject obj;
        obj.insert("type","register");
        obj.insert("name",name);
        obj.insert("pwd",pwd);
        QJsonDocument doc(obj);

        //发送数据
        socket->write(doc.toJson());
    }
}

void Login::on_pushButton_login_clicked()
{

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
