#ifndef ABSTRACTDBSERVICE_H
#define ABSTRACTDBSERVICE_H

#include "abstractservice.h"

#include "../user.h"
#include "../message.h"

#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#define DEFAULT_DB_TYPE "QSQLITE"
#define DEFAULT_CONNECTION_NAME "KmerChat"
#define DEFAULT_DB_NAME "kmerchat.db"

namespace Core::Service {

using namespace Core;

class AbstractDbService : public AbstractService
{
    Q_OBJECT
public:

    AbstractDbService(
        const QString & host,
        const int port,
        const QString & username,
        const QString & password,
        QObject * parent = nullptr);

    AbstractDbService(const AbstractDbService &other);

    virtual ~AbstractDbService();

    virtual User user(quint64 id);

    virtual User user(const QString &username);

    virtual Message message(quint64 id);

    virtual QList<User> users(QString filter = QString("username"));

    virtual QList<Message> messages();

    virtual void add(const User & user);

    virtual void add(const Message & message);

    virtual void update(const User & user);

    virtual void update(const Message & message);

    virtual bool deleteUser(const quint64 id);

    virtual bool deleteUser(const QString & username);

    virtual bool deleteMessage(const quint64 id);

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


protected:

    void createTables();

    User recordToUser(const QSqlRecord & record);

    Message recordToMessage(const QSqlRecord & record);

private:
    // properties
    QString mHost;
    int mPort;
    QString mUsername;
    QString mPassword;
    QString mDbname;

    QSqlDatabase db;
    QSqlQuery query;

};

} // namespace Core::Service

#endif // ABSTRACTDBSERVICE_H
