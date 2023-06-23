#include "appserver.h"

#include "core/user.h"
#include "core/request.h"
#include "core/message.h"
#include "services/chatservice.h"
#include "services/dbservice.h"
#include "services/authservice.h"
#include "services/appservice.h"

#include <QDebug>

using namespace Core;
using namespace Server;
using namespace Server::Service;

AppServer::AppServer(int port, QObject *parent)
    : port(port), QObject{parent}
{
    qDebug() << "Starting the server at : " << port;
}

AppServer::~AppServer()
{
    qDebug() << "Closing The Server ... ";
    this->close();
    delete userModel;
    qDebug() << "Server Closed";
    emit closed(0);
}

void AppServer::start()
{
    // start all services
    DbService::start();

    userModel = new UserModel(DbService::instance()->users());

    AuthService::start(userModel);

    ChatService::start();

    AppService::start(port);
    QObject::connect(AppService::instance(), &AppService::closed, [&](){
        emit closed(-1);
    });

}

void AppServer::close()
{
    AppService::stop();
    AuthService::stop();
    ChatService::stop();
    DbService::stop();
}
