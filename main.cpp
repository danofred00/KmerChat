
#include <QCoreApplication>

#include "message.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    // main code
    QCoreApplication a{argc, argv};

    Message msg = Message::fromJsonString(QString("{\"content\":\"Hello User 2\",\"from\":0,\"state\":\"NOTSEND\",\"to\":0,\"type\":\"TEXT\"}"));

    qDebug() << msg.toString();

    return a.exec();
}
