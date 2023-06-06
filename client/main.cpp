
#include <QCoreApplication>

#include <QDebug>

#include "core/user.h"
#include "serversocket.h"
#include "services/authservice.h"

using namespace Client;
using namespace Client::Service;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    auto user = Core::User::Builder()
        .hasUsername("admin")
        .hasPassword("password")
        .build();

    ServerSocket socket = ServerSocket();

    socket.open(QUrl("ws://localhost:1234"));

    QObject::connect(&socket, &ServerSocket::connected, [&]() {

        AuthService::start(&socket);

        AuthService * auth = AuthService::instance();

        auth->setUser(&user);

        auth->login();

    });


    return app.exec();
}
