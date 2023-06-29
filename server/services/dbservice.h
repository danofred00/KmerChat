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
            const QString & host,
            const int port,
            const QString & username,
            const QString & password,
            const QString & dbName,
            const QString & driver);

    static void stop()
    {
        qDebug() << "Closing the Service : " << serviceName();
        delete _instance;
    }

    static QString serviceName() {
        return QString("DbService");
    }

protected:

    void createChatsTables() ;
    void createMessagesTable() ;
    void createUsersTable();

private:

    DbService(
        const QString & host,
        const int port,
        const QString & username,
        const QString & password,
        const QString & dbname,
        const QString & type,
        QObject * parent = nullptr);

    static DbService * _instance;

};

} // namespace Server

#endif // DBSERVICE_H
