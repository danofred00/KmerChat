#include "authservice.h"

#include <exception>

using namespace Client::Service;

AuthService::AuthService(Client::ServerSocket * ss, QObject *parent)
    : Core::Service::AbstractService{parent}
{
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


void AuthService::request(int requestType)
{
    if(mServerSocket == nullptr)
        throw std::runtime_error("The property serverSocket should'nt be set to null, please update it");

    Core::Request req;
    req.addHeader("type", requestType);
    req.setContent(mUser->toString());

    // send request
    mServerSocket->sendBinaryMessage(req.toJsonString().toLatin1());
    qDebug() << req.toJsonString();
}

void AuthService::login()
{
    request(Core::Request::Login);
}

void AuthService::signup()
{
    request(Core::Request::Register);
}

void AuthService::logout()
{
    request(Core::Request::Logout);
}

void AuthService::signout()
{
    request(Core::Request::UnRegister);
}
