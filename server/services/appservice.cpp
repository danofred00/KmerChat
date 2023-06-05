
#include "appservice.h"
#include "core/request.h"
#include "core/response.h"

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

    // connect all signals for differents services
    QObject::connect(auth, SIGNAL(newUser(const User*)), this, SLOT(onUserAdded(const User*)));
    QObject::connect(auth, SIGNAL(removedAccount(const User*)), this, SLOT(onUserRemoved(const User*)));
    QObject::connect(auth, SIGNAL(userLogin(quint64)), this, SLOT(onUserLogin(quint64)));
    QObject::connect(auth, SIGNAL(userLogout(quint64)), this, SLOT(onUserLogout(quint64)));

    //
    userModel = auth->model();

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

void AppService::onUserAdded(const User * user)
{
    // when the user is registering, we add this to the database
    db->add(*user);
    // we gonna refresh the list
    userModel->setUsers(db->users());

    // debug string
    qDebug() << "New User Registered : " << user->name() << " | " << user->email();
}

void AppService::onUserRemoved(const User * user)
{
    // when the user is registering, we add this to the database
    db->deleteUser(user->username());
    // we gonna refresh the list
    userModel->setUsers(db->users());

    // debug string
    qDebug() << "New Removed account : " << user->name() << " | " << user->email();
}

void AppService::onUserLogout(quint64 id)
{
    qDebug() << "User Logout : user_id = " << id;
}

void AppService::onUserLogin(quint64 id)
{
    qDebug() << "New User Login : user_id = " << id;
}

void AppService::removeConnection(QWebSocket * s)
{
    if(mClients.remove(s)) {
        s->deleteLater();
    }
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

    // vars
    Request req;
    req.parse(message);
    int reqType = req.headers().value("type");

    // get the websocket who send the message
    auto _sender = static_cast<QWebSocket *>(sender());

    //qDebug() << _sender->localAddress() << " - " << _sender->localPort();

    User user = User::fromJsonString(req.content());

    // some action
    switch (reqType) {

    case Request::Login :
        // login the user
        login(&user, _sender);
        break;
    case Request::Logout:
        // logout the user
        logout(&user, _sender);
        break;
    case Request::Register :
        // register the user
        registerUser(&user, _sender);
        break;
    case Request::UnRegister:
        unregisterUser(&user, _sender);
        break;
    }
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

void AppService::login(User * user, QWebSocket * socket)
{
    Response res;
    res.addHeader("type", Response::Login);

    if(auth->login(user)) {
        auto u = userModel->user(user->username());
        auto id = u.id();

        addConnection(socket, id);
        // update header
        res.addHeader("code", Response::SUCCESS);
        res.setContent(u.toString());
    } else {
        // update header
        res.addHeader("code", Response::FAILED);
    }

    socket->sendBinaryMessage(res.toJsonString().toLatin1());
}

void AppService::logout(User *user, QWebSocket *socket)
{
    auth->logout(user->id());

    // send the logout response
    Response res;
    res.addHeader("type", Response::Logout);
    res.addHeader("code", Response::SUCCESS);
    socket->sendBinaryMessage(res.toJsonString().toLatin1());

    // remove the connection
    removeConnection(socket);
}

void AppService::registerUser(User * user, QWebSocket * socket)
{
    Response res;
    res.addHeader("type", Response::Register);

    if(auth->signup(user)) {

        auto u = userModel->user(user->username());
        auto id = u.id();
        addConnection(socket, id);
        // update header
        res.addHeader("code", Response::SUCCESS);
        res.setContent(u.toString());
    } else {
        // update header
        res.addHeader("code", Response::FAILED);
    }
    socket->sendBinaryMessage(res.toJsonString().toLatin1());
}

void AppService::unregisterUser(User * user, QWebSocket * socket)
{
    Response res;
    res.addHeader("type", Response::UnRegister);

    auto b = auth->remove(user);
    if(b) {
        res.addHeader("code", Response::SUCCESS);

    } else {
        res.addHeader("code", Response::FAILED);
    }

    socket->sendBinaryMessage(res.toJsonString().toLatin1());

    // remove the connection
    if(b)
        removeConnection(socket);
}
