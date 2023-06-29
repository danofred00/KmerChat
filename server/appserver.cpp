#include "appserver.h"

#include "core/user.h"
#include "core/request.h"
#include "core/message.h"
#include "services/chatservice.h"
#include "services/dbservice.h"
#include "services/authservice.h"
#include "services/appservice.h"

#include "configloader.h"

#include <QDebug>
#include <QMap>

using namespace Core;
using namespace Server;
using namespace Server::Service;

AppServer::AppServer(ConfigLoader * configLoader, QObject *parent)
    : QObject{parent}, configLoader{configLoader}
{ }

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

    auto conf = configLoader->config();

    // db config
    auto db_conf = conf.value("database").toHash();
    auto db_host = db_conf.value("host").toString();
    auto db_port = db_conf.value("port", 5432).toInt();
    auto db_name = db_conf.value("name").toString();
    auto db_password = db_conf.value("password").toString();
    auto db_user = db_conf.value("username").toString();
    auto db_driver = db_conf.value("driver", "QSQLITE").toString();

    // app configs
    auto app_conf = conf.value("application").toHash();
    auto app_port = app_conf.value("port", 1234).toInt();

    qDebug() << "Starting the server at : " << app_port;

    // start all services
    DbService::start(db_host, db_port, db_user, db_password, db_name, db_driver);

    userModel = new UserModel(DbService::instance()->users());

    AuthService::start(userModel);

    ChatService::start();

    AppService::start(app_port);
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
