#ifndef APPSERVICE_H
#define APPSERVICE_H

#include <QMap>
#include <QWebSocket>
#include <QWebSocketServer>

#include "abstractservice.h"
#include "authservice.h"
#include "chatservice.h"
#include "dbservice.h"

#define DEFAULT_SERVERNAME "KmerchatServer"

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

private:
    AppService(QString host, int port, QObject *parent = nullptr);
    static AppService * _instance;

    // private methods
    void init();
    void removeConnection(const quint64 & id);
    void addConnection(QWebSocket * s, quint64 id = 0);
    //void authClient(QWebSocket * socket);


    // other services
    AuthService * auth;
    DbService * db;
    ChatService * chat;

    // properties
    QWebSocketServer mWebsocketserver;
    Connection mClients;
    QString mHost;
    int mPort;
};

#endif // APPSERVICE_H
