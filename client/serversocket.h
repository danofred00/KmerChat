#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QWebSocket>

namespace Client{

/**
 * @brief The ServerSocket class
 */
class ServerSocket : public QWebSocket
{
    Q_OBJECT
public:
    ServerSocket(QObject *parent = nullptr);

public slots:

    void onError(QAbstractSocket::SocketError error);

};

} // namespace Client

#endif // SERVERSOCKET_H
