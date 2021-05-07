#include "ChatMessage.h"
#include "ui_ChatMessage.h"
#include <QPainter>

ChatMessage::ChatMessage(QWidget *parent)
    : QWidget(parent)
{
    ui = new Ui::ChatMessage();
    ui->setupUi(this);
    //QFont ft;
    //ft.setPointSize(15);
    //ui->label->setFont(ft);
}

ChatMessage::~ChatMessage()
{
    delete ui;
}

void ChatMessage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (isMe) {
        painter.setBrush(QColor(0, 162, 232));
    }
    else {
        painter.setBrush(Qt::lightGray);
    }
    if (isMe)
    painter.drawRoundedRect(QRect(DISTANCE_ME, UP_PADDING, ui->label->width(), height() - UP_PADDING * 2), 2, 8);
    else
        painter.drawRoundedRect(QRect(this->width() - ui->label->width() - 10, UP_PADDING, ui->label->width(), height() - UP_PADDING * 2), 2, 8);
}

void ChatMessage::SetText(QString str, bool flag)
{
    isMe = flag;
    // zt
    int width = this->width() - DISTANCE_OTHER;
    QFontMetrics fm(ui->label->font());
    int count = 0;
    QString all;
    QString tmp;
    int perwidth = fm.width("һ");
    for (int i = 0; i < str.length(); ++i) {
        tmp += str[i];
        int tmpWidth = fm.width(tmp);
        if (tmpWidth > width - WENBEN_ZUOYOU * 2 - DISTANCE_ME) {
            all = all + tmp + "\n";
            tmp.clear();
            count++;
        }
    }
    int h = fm.height();
    if (!tmp.isEmpty()) {
        all += tmp;
        count++;
    }
    int hangjianju = 6;
    ui->label->setText(all);
    if (count == 1) {
        if (isMe)
            ui->label->setGeometry(DISTANCE_ME + WENBEN_ZUOYOU, UP_PADDING, width, h * count + UP_PADDING + DOWN_PADDING);
        else {
            ui->label->setAlignment(Qt::AlignRight);
            ui->label->setGeometry(DISTANCE_OTHER + 10, UP_PADDING, width, h * count + UP_PADDING + DOWN_PADDING);
        }
    }
    else {
        if (isMe)
        ui->label->setGeometry(DISTANCE_ME + WENBEN_ZUOYOU, UP_PADDING, width, (h + hangjianju) * count + UP_PADDING + DOWN_PADDING);
        else {
            ui->label->setAlignment(Qt::AlignRight);
            ui->label->setGeometry(DISTANCE_OTHER + 60, UP_PADDING, width, (h + hangjianju) * count + UP_PADDING + DOWN_PADDING);
        }
    }
    this->setFixedHeight((h + hangjianju) * count + UP_PADDING * 2 + DOWN_PADDING * 2);
    mHeight = (h + hangjianju) * count + UP_PADDING * 2 + DOWN_PADDING * 2;
}