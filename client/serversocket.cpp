#include "serversocket.h"

using namespace Client;

ServerSocket::ServerSocket(QObject *parent)
    : QWebSocket{}
{
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
}

void ServerSocket::onError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    close();
}
