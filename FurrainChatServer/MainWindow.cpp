#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mServer = new QTcpServer(this);
    mServer->listen(QHostAddress::Any, 5748);
    connect(mServer, SIGNAL(newConnection()), this, SLOT(NewConnectionSlot()));
    database = QSqlDatabase::addDatabase("QSQLITE");
    //QString dsn="DRIVER={SQL SERVER};SERVER=127.0.0.1\\dms;DATABASE=ChatServer;UID=sa;PWD=dmsdms;";
    //database.setDatabaseName(dsn);
    database.setHostName("acidalia");
    database.setDatabaseName("customdb.db");
    database.setUserName("root");
    database.setPassword("123456");
    if (!database.open()) {
        QMessageBox::warning(this, QStringLiteral("错误信息"), QStringLiteral("打开数据库失败！"));
    }
    QString strsql = QString::fromLocal8Bit(
        "create table if not exists userMange(userName varchar, passWord varchar, loadState "
        "varchar, IPAddress varchar, tcpPort varchar)");
    database.exec(strsql);
    tableModel = new QSqlTableModel(this, database);
    tableModel->setTable("userMange");
    ui->tableView->setModel(tableModel);
    tableModel->select();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::NewConnectionSlot()
{
    QTcpSocket *tcpSocket = mServer->nextPendingConnection();
    socketVector.push_back(tcpSocket);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(ReceiveSocketSlot()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(DisconnectSlot()));
}

void MainWindow::ReceiveSocketSlot()
{
    QJsonObject obj;
    QJsonObject returnToClient; //回传用户的Json对象
    QJsonDocument returnToClientDoc; //回传用户的JsonDocument
    QTcpSocket *thisSocket = qobject_cast<QTcpSocket *>(sender());
    QByteArray array = thisSocket->readAll();
    QJsonParseError error;
    QJsonValue value;
    QJsonDocument doc = QJsonDocument::fromJson(array, &error);
    if (doc.isNull() || (error.error != QJsonParseError::NoError)) {
        return;
    }
    obj = doc.object();
    if (obj.value("type") == "register") {
        QString name = obj.value("name").toString();
        QSqlQuery queryUser;
        QString sql = QStringLiteral("select * from userMange where userName='%1'").arg(name);
        queryUser.exec(sql);
        if (queryUser.next()) {
            returnToClient.insert("type", "registeredMessage");
            returnToClient.insert("registeredResult", 0); //用户名已经存在
            returnToClientDoc.setObject(returnToClient);
            thisSocket->write(returnToClientDoc.toJson());
        } else {
            QString pwd = obj.value("pwd").toString();
            QHostAddress address = thisSocket->peerAddress();
            quint16 port = thisSocket->peerPort();
            returnToClient.insert("type", "registeredMessage");
            returnToClient.insert("registeredResult", 1); //注册成功
            returnToClientDoc.setObject(returnToClient);
            thisSocket->write(returnToClientDoc.toJson());
            //更新数据库
            QSqlQuery query;
            QString insert
                = QStringLiteral(
                      "insert into userMange (userName,passWord,loadState,IPAddress,tcpPort) "
                      "values('%1','%2',0,'%3','%4')")
                      .arg(name)
                      .arg(pwd)
                      .arg(address.toString())
                      .arg(QString::number(port, 10));
            query.exec(insert);
            tableModel->select();
        }
    } else if (obj.value("type") == "login") {
        //获取登录用户名密码以及端口号
        QString userName;
        QString passWord;
        value = obj.value("userName");
        userName = value.toString();
        value = obj.value("passWord");
        passWord = value.toString();
        value = obj.value("udpPort");
        int udpPort = value.toInt();
        QHostAddress address = thisSocket->peerAddress();
        quint16 port = thisSocket->peerPort();

        //检查密码用户名是否正确,是否能够登陆成功
        QString str = QStringLiteral("select * from userMange where userName='%1'").arg(userName);
        QSqlQuery query;
        query.exec(str);
        bool isOK = query.next();
        if (!isOK)//用户名不存在的情况
        {
            returnToClient.insert("type", "loadMessage");
            returnToClient.insert("loginResult", 0);   //用户名不存在！
            returnToClientDoc.setObject(returnToClient);
            thisSocket->write(returnToClientDoc.toJson());
        }
        else {
            returnToClient.insert("type", "loadMessage");
            returnToClient.insert("loginResult", 1);   //ok！
            returnToClientDoc.setObject(returnToClient);
            thisSocket->write(returnToClientDoc.toJson());
            userConnection.insert(thisSocket, userName);
            // 自己查询
            QString str11 = QStringLiteral("select * from userMange where loadState= 1");
            bool f = query.exec(str11);
            while (query.next()) {
                QString name = query.value("userName").toString();
                qDebug() << name;
                QJsonObject tongzhi;
                QJsonDocument tongzhiDoc;
                tongzhi.insert("type", "yizaixian");
                tongzhi.insert("name", name);
                tongzhiDoc.setObject(tongzhi);
                thisSocket->write(tongzhiDoc.toJson());
            }
            QString str1 = QStringLiteral("update userMange set loadState=1 where userName='%4'").
                arg(userName);
            qDebug() << str1;
            bool ff = query.exec(str1);
            tableModel->select();
            // 处理上线通知
            for (int i = 0; i < socketVector.size(); ++i) {
                if (thisSocket == socketVector[i]) {
                    continue;
                }
                QJsonObject tongzhi;
                QJsonDocument tongzhiDoc;
                tongzhi.insert("type", "shangxian");
                tongzhi.insert("name", obj.value("userName").toString());
                tongzhiDoc.setObject(tongzhi);
                socketVector[i]->write(tongzhiDoc.toJson());
            }
        }
    }
}

void MainWindow::DisconnectSlot()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    QString userName = userConnection[socket];
    if (!userName.isEmpty()) {
        QString str1 = QStringLiteral("update userMange set loadState=0 where userName='%4'").
            arg(userName);
        QSqlQuery query;
        query.exec(str1);
        tableModel->select();
        userConnection.remove(socket);
        QList<QTcpSocket *>list = userConnection.keys();
        for (int i = 0; i < list.size(); ++i) {
            if (list[i] == socket) {
                continue;
            }
            QJsonObject tongzhi;
            QJsonDocument tongzhiDoc;
            tongzhi.insert("type", "xiaxian");
            tongzhi.insert("name", userName);
            tongzhiDoc.setObject(tongzhi);
            list[i]->write(tongzhiDoc.toJson());
        }
    }
    for (int i = 0; i < socketVector.size(); ++i) {
        if (socketVector[i] == socket) {
            socketVector.erase(socketVector.begin() + i);
            socket->deleteLater();
            socket = nullptr;
            break;
        }
    }
    
}
