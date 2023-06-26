#ifndef APPSERVICE_H
#define APPSERVICE_H

#include <QMap>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QWebSocketProtocol>

#include "core/user.h"
#include "core/models/usermodel.h"
#include "core/services/abstractservice.h"

#include "authservice.h"
#include "dbservice.h"
#include "chatservice.h"

#include "../models/connectionmodel.h"

#define DEFAULT_SERVERNAME "KmerchatServer"

namespace Server::Service  {

using namespace Core;
using namespace Core::Model;
using namespace Core::Service;
using namespace Server::Model;

class AppService : public AbstractService
{
    Q_OBJECT
public:

    static AppService * instance();

    static void start(int port);

    static void stop()
    {
        qDebug() << "Closing the Service : " << serviceName();
        delete _instance;
    }

    static QString serviceName() {
        return QString("AppService");
    }

    // const ConnectionModel clients() const { return mClients; }

//    QString host() const { return mHost; }
//    void setHost(const QString &host) { mHost = host; }

    int port() const;
    void setPort(int newPort);

    bool sendToClient(const QString & msg, const quint64 &sender, const quint64 &id, const int type = 0);

signals:

    void closed(int code);

private slots:

    void onNewConnection();
    void onMessageReceived(const QString & message);
    void onBinaryMessageReceived(const QByteArray & message);

    void onUserAdded(const User * user);
    void onUserRemoved(const User * user);
    void onUserLogout(const User * user);
    void onUserLogin(const User * user);

    void onError(QWebSocketProtocol::CloseCode code);

private:
    AppService(int port, QObject *parent = nullptr);
    static AppService * _instance;

    // private methods
    void init();

    void login(User * user, QWebSocket * socket);
    void registerUser(User * user, QWebSocket * socket);
    void logout(User * user, QWebSocket * socket);
    void unregisterUser(User * user, QWebSocket * socket);
    //void authClient(QWebSocket * socket);


    // other services
    AuthService * auth;
    DbService * db;
    ChatService * chat;

    UserModel * userModel;

    // properties
    QWebSocketServer mWebsocketserver;
    ConnectionModel mClients;
    // QString mHost;
    int mPort;
};

} // namespace Server

#endif // APPSERVICE_H
