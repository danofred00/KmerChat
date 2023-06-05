#include "appserver.h"

#include "core/user.h"
#include "core/request.h"
#include "core/message.h"
#include "services/chatservice.h"
#include "services/dbservice.h"
#include "services/authservice.h"
#include "services/appservice.h"

using namespace Core;
using namespace Server;
using namespace Server::Service;

AppServer::AppServer(QString host, int port, QObject *parent)
    : host(host), port(port), QObject{parent}
{ }

AppServer::~AppServer()
{
    delete userModel;

    this->close();
}

void AppServer::start()
{
    // start all services
    DbService::start();

    userModel = new UserModel(DbService::instance()->users());

    AuthService::start(userModel);

    ChatService::start();

    AppService::start(host, port);
}

void AppServer::close()
{
    DbService::stop();
    AuthService::stop();
    ChatService::stop();
    AppService::stop();
}
