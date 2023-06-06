#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "core/user.h"
#include "core/response.h"
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

    void login(Core::User * user);

    void signup(Core::User * user);

    void logout(Core::User * user);

    void signout(Core::User * user);

    void request(Core::User * user, int requestType);

    // Client::ServerSocket * serverSocket() const { return mServerSocket; }

    void setServerSocket(Client::ServerSocket * ss);

signals:

    void loginResult(int result);

    void logoutResult(int result);

    void signintResult(int result);

    void signoutResult(int result);

private slots:

    void authResponseReceived(Core::Response * response);

private:

    AuthService(Client::ServerSocket * ss = nullptr, QObject *parent = nullptr);

    static AuthService * _instance;

    Client::ServerSocket * mServerSocket;

};

}

#endif // AUTHSERVICE_H
