#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QWebSocket>

#include "core/response.h"

namespace Client{

/**
 * @brief The ServerSocket class
 */
class ServerSocket : public QWebSocket
{
    Q_OBJECT
public:
    ServerSocket(QObject *parent = nullptr);

    Core::Response * lastResponse();

signals:

    void authResponse(Core::Response * response);

    void responseReceived(Core::Response * response);

public slots:

    void onMessageReceived(QString msg);

    void onError(QAbstractSocket::SocketError error);

private:

    Core::Response response;
};

} // namespace Client

#endif // SERVERSOCKET_H
