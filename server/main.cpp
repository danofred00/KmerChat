
#include <QCoreApplication>

#include "services/chatservice.h"
#include "services/dbservice.h"
#include "services/authservice.h"
#include "services/appservice.h"
#include "core/user.h"
#include "core/request.h"
#include "core/models/usermodel.h"
#include "core/message.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    // main code
    QCoreApplication a{argc, argv};

    DbService::start();
    auto db = DbService::instance();

    UserModel model {db->users()};

    AuthService::start(&model);

    ChatService::start();

    AppService::start("localhost", 1234);

    //auto user = DbService::instance()->user("Danofred");

    Request req;

    req.parse("{\"headers\":{\"type\":1}, \"content\":{\"Hello\": \"client\"}}");

    //req.addHeader("type", Request::Login);
    //req.setContent("{\"Hello\": \"client\"}");

    qDebug() << req.toJsonString() << " - content : " << req.content();

    return a.exec();
}
