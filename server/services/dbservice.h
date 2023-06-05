#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#include "core/message.h"
#include "core/user.h"
#include "core/services/abstractdbservice.h"

#define DEFAULT_DB_TYPE "QSQLITE"
#define DEFAULT_CONNECTION_NAME "KmerChat"
#define DEFAULT_DB_NAME "kmerchat.db"

namespace Server {

using namespace Core;
using namespace Core::Service;

class DbService : public AbstractDbService
{
    Q_OBJECT
public:

    // usage of singleton pattern to enable one only instance of DbService::class
    static DbService * instance();

    ~DbService();

    static void start(const QString & host = QString("localhost"),
                     const int port = 8080,
                     const QString & username = QString("admin"),
                     const QString & password = QString("password")
                    );

    static void stop()
    {
        delete _instance;
    }

    User user(quint64 id) override;

    User user(const QString &username) override;

    Message message(quint64 id) override;

    QList<User> users() override;

    QList<Message> messages() override;

    void add(const User & user) override;

    void add(const Message & message) override;

    void update(const User & user) override;

    void update(const Message & message) override;

    bool deleteUser(const quint64 id) override;

    bool deleteUser(const QString & username) override;

    bool deleteMessage(const quint64 id) override;

    // getters
    const QString host() const { return mHost; }

    const QString dbname() const { return mDbname; }

    const int port() const { return mPort; }

    const QString username() const { return mUsername; }

    const QString password() const { return mPassword; }

    // setters
    void setHost(const QString & host) { mHost = host; }

    void setPort(const int & port) { mPort = port; }

    void setUsername(const QString &username) { mUsername = username; }

    void setPassword(const QString & password) { mPassword = password; }

    void setDbname(const QString &name) { mDbname = name; }

private:
    DbService(const QString & host,
              const int port,
              const QString & username,
              const QString & password);

    void createTables();

    DbService(const DbService &other);

    User recordToUser(const QSqlRecord & record);

    Message recordToMessage(const QSqlRecord & record);

    static DbService * _instance;

    // prpêrties
    QString mHost;
    int mPort;
    QString mUsername;
    QString mPassword;
    QString mDbname;

    QSqlDatabase db;
    QSqlQuery query;

};

} // namespace Server

#endif // DBSERVICE_H
