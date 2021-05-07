#include "Login.h"
#include <QQmlApplicationEngine>
#include <QApplication>
#include <ChatWidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login w;
    w.show();
    return a.exec();
}
