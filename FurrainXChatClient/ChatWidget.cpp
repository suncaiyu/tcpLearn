#include "ChatWidget.h"
#include "ui_ChatWidget.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <ChatMessage.h>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#pragma execution_character_set("utf-8")
extern QString Myname;
ChatWidget::ChatWidget(QTcpSocket *s, QString n, QWidget *parent) :
      QWidget(parent),socket(s),name(n),
      ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickWidget->rootContext()->setContextProperty("chat", this);
    ui->quickWidget->setSource(QUrl("qrc:/ConversationPage.qml"));
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::Write(QString str)
{
    qDebug() << "发送过里的： " <<str;
    QQuickItem *pctrlobj = ui->quickWidget->rootObject();
    if (pctrlobj) {
        QMetaObject::invokeMethod(pctrlobj,
                                  "addToSession",
                                  Qt::DirectConnection,
                                  Q_ARG(QVariant, str),
                                  Q_ARG(QVariant, false));
    }
    // 两种方式，上面类似于回调，下面是信号槽方式  下面的简单，上面操作起来很烦而且容易出现问题
    //emit otherMsgSignal(str);
    QApplication::alert(this);
}

void ChatWidget::SendToOther(QString str)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QJsonObject obj;
        obj.insert("type", "chat");
        obj.insert("from", Myname);
        obj.insert("to", name);
        obj.insert("content", str);
        QJsonDocument doc(obj);

        //发送数据
        socket->write(doc.toJson());
    }
    qDebug() << "发给别人: "<< str;
}

void ChatWidget::WriteList(QString str)
{

}
