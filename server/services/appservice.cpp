
#include "appservice.h"
#include "core/response.h"
#include "core/request.h"
#include <exception>

using namespace Server::Service ;

AppService::AppService(int port, QObject *parent)
    : mPort{port},
      mWebsocketserver{DEFAULT_SERVERNAME, QWebSocketServer::NonSecureMode},
      AbstractService{parent}
{
    // debug
    qDebug() << "Starting AppService ...";

    // get instances of all avaliables services
    auth = AuthService::instance();
    chat = ChatService::instance();
    db = DbService::instance();

    // connect all signals for differents services
    QObject::connect(auth, SIGNAL(newUser(const User*)), this, SLOT(onUserAdded(const User*)));
    QObject::connect(auth, SIGNAL(removedAccount(const User*)), this, SLOT(onUserRemoved(const User*)));
    QObject::connect(auth, SIGNAL(userLogin(const User*)), this, SLOT(onUserLogin(const User*)));
    QObject::connect(auth, SIGNAL(userLogout(const User*)), this, SLOT(onUserLogout(const User*)));

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

void AppService::start(int port)
{
    if(_instance == nullptr) {
        _instance = new AppService(port);
    }
}

bool AppService::sendToClient(const QString & msg, const quint64 & sender, const quint64 &id, const int type)
{
    Request req;
    req.setContentKey("message", msg);
    req.addHeader("type", type);

    // get info about sender
    auto _sender = userModel->user(sender);
    // remove sensible infos
    _sender->setPassword("");
    req.setContentKey("user", _sender->toString());

    // send message msg to all online clients
    bool sended = false;
    if(mClients.contains(id)) {
        for(auto client : mClients[id])
            client->sendTextMessage(req.toString());
        sended = true;
    }

    return sended;
}

void AppService::init()
{
    // run the server
    if(mWebsocketserver.listen(QHostAddress(QHostAddress::AnyIPv4), mPort))
    {
        // connect signals
        QObject::connect(&mWebsocketserver, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    }
    else {
        qDebug() << "Unable to run the server ... Closing connection";
        stop();
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

void AppService::onUserLogout(const User * user)
{
    qDebug() << "User Logout : user_id = " << user->id();
}

void AppService::onUserLogin(const User * user)
{
    qDebug() << "New User Login : user_id = " << user->id();
}

void AppService::onNewConnection()
{
    QWebSocket * socket = mWebsocketserver.nextPendingConnection();
    qDebug() << "New Client Connected ... Waiting for authentification (Addr : " << socket << " )";
    QObject::connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(onMessageReceived(QString)));
    QObject::connect(socket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(onBinaryMessageReceived(QByteArray)));

}

void AppService::onBinaryMessageReceived(const QByteArray & message)
{
    onMessageReceived(QString(message));
}

void AppService::onMessageReceived(const QString & message)
{
    qDebug() << "Message received : " << message;

    // vars
    Request req;
    req.parse(message);
    int reqType = req.headers().value("type");

    //

    User * user = User::fromJsonString(
                            req.contentKey("user", QVariant("")).toString());;
    Message * msg = nullptr;

    // get the websocket who send the message
    auto _sender = static_cast<QWebSocket *>(sender());

    //qDebug() << _sender->localAddress() << " - " << _sender->localPort();

    // some action
    switch (reqType) {

    case Request::Login :
        // login the user
        login(user, _sender);
        break;
    case Request::Logout:
        //user = User::fromJsonString(req.content());
        // logout the user
        logout(user, _sender);
        break;
    case Request::Register :
        //user = User::fromJsonString(req.content());
        // register the user
        registerUser(user, _sender);
        break;
    case Request::UnRegister:
        //user = User::fromJsonString(req.content());
        // unregister user
        unregisterUser(user, _sender);
        break;
    case Request::Simple:
        // the the request is a simple message, we just
        // parse the content as a message, and send it to chat service
        msg = Message::fromJsonString(req.contentKey("message").toString());
        emit chat->messageReceive(msg);
        break;
    default:
        qDebug() << " ---- UNKNOW REQUEST ----";
        qDebug() << message;
        qDebug() << " ----------- END ------------";
        break;
    }

    // free the memory allocated by
    // deleting all pointers
    if (user != nullptr) delete user;
    if (msg != nullptr) delete msg;
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

    qDebug() << "Trying to login User(" << user->username() << ", " << socket << ")";

    if(auth->login(user)) {

        // this approach is not good for me, i will change it later
        // when the AuthService login the user,
        auto u = userModel->user(user->username());
        auto id = u->id();
        // add the connection
        mClients.addConnection(socket, id);
        // update header
        res.addHeader("code", Response::SUCCESS);
        res.setContentKey("user", u->toString());

    } else {
        // update header
        res.addHeader("code", Response::FAILED);
    }

    socket->sendBinaryMessage(res.toString().toLatin1());
}

void AppService::logout(User *user, QWebSocket *socket)
{
    qDebug() << "Trying to logout User(" << user->username() << ", " << socket << ")";

    auth->logout(user);

    // send the logout response
    Response res;
    res.addHeader("type", Response::Logout);
    res.addHeader("code", Response::SUCCESS);
    socket->sendBinaryMessage(res.toString().toLatin1());

    // remove the connection
    mClients.removeConnection(socket, user->id());
}

void AppService::registerUser(User * user, QWebSocket * socket)
{
    Response res;
    res.addHeader("type", Response::Register);

    qDebug() << "Trying to register User(" << user->username() << ", " << socket << ")";

    if(auth->signup(user)) {

        auto u = userModel->user(user->username());
        auto id = u->id();

        mClients.addConnection(socket, id);
        // update header
        res.addHeader("code", Response::SUCCESS);
        res.setContentKey("user", u->toString());

    } else {
        // update header
        res.addHeader("code", Response::FAILED);
    }
    socket->sendBinaryMessage(res.toString().toLatin1());
}

void AppService::unregisterUser(User * user, QWebSocket * socket)
{
    Response res;
    res.addHeader("type", Response::UnRegister);

    qDebug() << "Trying to unregister User(" << user->username() << ", " << socket << ")";

    auto b = auth->remove(user);
    if(b) {
        res.addHeader("code", Response::SUCCESS);

    } else {
        res.addHeader("code", Response::FAILED);
    }

    socket->sendBinaryMessage(res.toString().toLatin1());

    // remove the connection
    if(b)
        mClients.removeConnection(socket, user->id());
}


void AppService::onError(QWebSocketProtocol::CloseCode code)
{
    qDebug() << code;
    emit closed(code);
}
