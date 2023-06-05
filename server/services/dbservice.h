#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#include "core/message.h"
#include "core/user.h"
#include "core/services/abstractdbservice.h"

namespace Server::Service {

using namespace Core;
using namespace Core::Service;

class DbService : public AbstractDbService
{
    Q_OBJECT
public:

    // usage of singleton pattern to enable one only instance of DbService::class
    static DbService * instance();

    static void start(
            const QString & host = QString("localhost"),
            const int port = 8080,
            const QString & username = QString("admin"),
            const QString & password = QString("password"));

    static void stop()
    {
        delete _instance;
    }

private:

    using AbstractDbService::AbstractDbService;

    static DbService * _instance;

};

} // namespace Server

#endif // DBSERVICE_H
