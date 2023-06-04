
#include "appservice.h"
#include "../request.h"

#include <exception>

AppService::AppService(QString host, int port, QObject *parent)
    : mHost{host}, mPort{port},
      mWebsocketserver{DEFAULT_SERVERNAME, QWebSocketServer::NonSecureMode},
      AbstractService{parent}
{
    // get instances of all avaliables services
    auth = AuthService::instance();
    chat = ChatService::instance();
    db = DbService::instance();

    // init AppService
    init();
}

AppService * AppService::_instance = nullptr;

AppService * AppService::instance()
{
    if(_instance == nullptr)
        // usage of uninstanciate object
        throw std::runtime_error("Unable to get instance of AppService, please run start first.");

    return _instance;
}

void AppService::start(QString host, int port)
{
    if(_instance == nullptr) {
        _instance = new AppService(host, port);
    }
}

void AppService::init()
{
    // run the server
    if(mWebsocketserver.listen(QHostAddress(mHost), mPort))
    {
        // connect signals
        QObject::connect(&mWebsocketserver, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    }
}

void AppService::removeConnection(const quint64 & id)
{

}

void AppService::onNewConnection()
{
    QWebSocket * socket = mWebsocketserver.nextPendingConnection();

    QObject::connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(onMessageReceived(QString)));
    QObject::connect(socket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(onBinaryMessageReceived(QByteArray)));

}

void AppService::onBinaryMessageReceived(const QByteArray & message)
{
    onMessageReceived(QString(message));
}

void AppService::onMessageReceived(const QString & message)
{
    // to do
}

void AppService::addConnection(QWebSocket * s, quint64 id)
{
    if(mClients.contains(s))
        return;

    mClients.insert(s, id);
}
//void AppService::authClient(QWebSocket * socket)
//{
//    qDebug() << socket->request().url();
//    socket->sendTextMessage("Hello From Server");
//}
