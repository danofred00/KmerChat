#ifndef APPSERVICE_H
#define APPSERVICE_H

#include <QMap>
#include <QWebSocket>
#include <QWebSocketServer>

#include "core/user.h"
#include "core/models/usermodel.h"
#include "core/services/abstractservice.h"

#include "authservice.h"
#include "dbservice.h"
#include "chatservice.h"

#define DEFAULT_SERVERNAME "KmerchatServer"

namespace Server::Service  {

using namespace Core;
using namespace Core::Model;
using namespace Core::Service;

using Connection = QMap<QWebSocket *, quint64>;

class AppService : public AbstractService
{
    Q_OBJECT
public:

    static AppService * instance();

    static void start(QString host, int port);

    static void stop(){
        delete _instance;
    }

    const Connection clients() const { return mClients; }

    QString host() const { return mHost; }
    void setHost(const QString &host) { mHost = host; }

    int port() const;
    void setPort(int newPort);

private slots:

    void onNewConnection();

    void onMessageReceived(const QString & message);

    void onBinaryMessageReceived(const QByteArray & message);

    void onUserAdded(const User * user);
    void onUserRemoved(const User * user);
    void onUserLogout(quint64 id);
    void onUserLogin(quint64 id);

private:
    AppService(QString host, int port, QObject *parent = nullptr);
    static AppService * _instance;

    // private methods
    void init();
    void removeConnection(QWebSocket * s);
    void addConnection(QWebSocket * s, quint64 id = 0);

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
    Connection mClients;
    QString mHost;
    int mPort;
};

} // namespace Server

#endif // APPSERVICE_H
