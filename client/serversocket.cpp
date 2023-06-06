#include "serversocket.h"

using namespace Client;

ServerSocket::ServerSocket(QObject *parent)
    : QWebSocket{}
{
    // init response object
    response = Core::Response();

    // connect some signals
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    connect(this, &ServerSocket::textMessageReceived, this, &ServerSocket::onMessageReceived);
    connect(this, &ServerSocket::binaryMessageReceived, this, [&](const QByteArray & msg) {
        onMessageReceived(QString(msg));
    });
}

Core::Response * ServerSocket::lastResponse()
{
    return &response;
}

void ServerSocket::onError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    close();
}

//Core::Response * ServerSocket::lastResponse()
//{
//    return response;
//}

void ServerSocket::onMessageReceived(QString msg)
{
    response.parse(msg);
    auto resType = response.headers().value("type");

    // When the response type is one of auth response types
    // we just emit authResponse even, and responseReceived otherwise
    switch (resType) {
    case Core::Response::Login:
    case Core::Response::Logout:
    case Core::Response::Register:
    case Core::Response::UnRegister:
        // emit auth response signal
        emit authResponse(&response);
        break;
    default:
        // emit responseReceive event
        emit responseReceived(&response);
        break;
    }
}
