
#include <QCoreApplication>

#include "services/chatservice.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    // main code
    QCoreApplication a{argc, argv};

    ChatService * service =  ChatService::instance();

    return a.exec();
}
