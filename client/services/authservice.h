#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "core/user.h"
#include "core/request.h"
#include "core/services/abstractservice.h"

#include "../serversocket.h"

namespace Client::Service {

class AuthService : public Core::Service::AbstractService
{
    Q_OBJECT
public:

    static void start(Client::ServerSocket * ss = nullptr);

    static void stop();

    static AuthService * instance();

    void login();

    void signup();

    void logout();

    void signout();

    void request(int requestType);

    Core::User * user() const { return mUser; }

    void setUser(Core::User * user) { mUser = user; }

    Client::ServerSocket * serverSocket() const { return mServerSocket; }

    void setServerSocket(Client::ServerSocket * ss) { mServerSocket = ss; }

private slots:

    void received(const QByteArray & message) {
        qDebug() << message;
    }

private:

    AuthService(Client::ServerSocket * ss = nullptr, QObject *parent = nullptr);

    static AuthService * _instance;

    Core::User * mUser;
    Client::ServerSocket * mServerSocket;

};

}

#endif // AUTHSERVICE_H
