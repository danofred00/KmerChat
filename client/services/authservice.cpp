#include "authservice.h"

#include <exception>

using namespace Client::Service;

AuthService::AuthService(Client::ServerSocket * ss, QObject *parent)
    : Core::Service::AbstractService{parent}
{
    if(ss != nullptr)
        setServerSocket(ss);
}

AuthService * AuthService::_instance = nullptr;


AuthService * AuthService::instance()
{
    if(_instance == nullptr)
        // usage of uninstanciate object
        throw std::runtime_error("Unable to get instance of ChatService, please run start first.");

    // if everything is okay
    return _instance;
}

void AuthService::start(Client::ServerSocket * ss)
{
    if(_instance == nullptr)
        _instance = new AuthService(ss);
}

void AuthService::stop()
{
    delete _instance;
}


void AuthService::request(Core::User * user, int requestType)
{
    if(mServerSocket == nullptr)
        throw std::runtime_error("The property serverSocket should'nt be set to null, please update it");

    Core::Request req;
    req.addHeader("type", requestType);
    req.setContent(user->toString());

    // send request
    mServerSocket->sendBinaryMessage(req.toString().toLatin1());
    qDebug() << req.toString();
}

void AuthService::setServerSocket(ServerSocket *ss)
{
    mServerSocket = ss;
    // do some connection
    QObject::connect(mServerSocket, SIGNAL(authResponse(Core::Response *)),
                     this, SLOT(authResponseReceived(Core::Response*)));
}


void AuthService::login(Core::User * user)
{
    request(user, Core::Request::Login);
}

void AuthService::signup(Core::User * user)
{
    request(user, Core::Request::Register);
}

void AuthService::logout(Core::User * user)
{
    request(user, Core::Request::Logout);
}

void AuthService::signout(Core::User * user)
{
    request(user, Core::Request::UnRegister);
}

void AuthService::authResponseReceived(Core::Response * response)
{
    auto type = response->headers().value("type");
    auto code = response->headers().value("code");

    switch (type) {
    case Core::Response::Login:
        emit loginResult(code);
        break;
    case Core::Response::Logout:
        emit logoutResult(code);
        break;
    case Core::Response::Register:
        emit signinResult(code);
        break;
    case Core::Response::UnRegister:
        emit signoutResult(code);
        break;
    default:
        break;
    }

}

