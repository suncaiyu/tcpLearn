#pragma once

#include <QWidget>
namespace Ui { class ChatMessage; };

constexpr int UP_PADDING = 5;
constexpr int DOWN_PADDING = 5;
constexpr int ICON_NAME_HEIGHT = 10;
constexpr int DISTANCE_ME = 10;
constexpr int DISTANCE_OTHER = 20;
constexpr int WENBEN_ZUOYOU = 5;
enum MsgType {
    MSG_ME,
    MSG_OTHER
};
class ChatMessage : public QWidget
{
    Q_OBJECT

public:
    ChatMessage(QWidget *parent = Q_NULLPTR);
    ~ChatMessage();
    void SetText(QString text, bool flag);
    int GetHeight() { return mHeight; }
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    Ui::ChatMessage *ui;
    int mHeight = 0;
    bool isMe;
};
