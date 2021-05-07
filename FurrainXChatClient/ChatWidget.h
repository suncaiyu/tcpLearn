#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <qtcpsocket.h>
namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    ChatWidget(QTcpSocket *s, QString name, QWidget *parent = nullptr);
    ChatWidget();
    ~ChatWidget();
    void Write(QString str);
    void WriteList(QString str);
    Q_INVOKABLE void SendToOther(QString str);

private:
    Ui::ChatWidget *ui;
    QTcpSocket *socket;
    QString name;

signals:
    void otherMsgSignal(QString str);

};

#endif // CHATWIDGET_H
