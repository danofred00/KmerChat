#ifndef ABSTRACTDBSERVICE_H
#define ABSTRACTDBSERVICE_H

#include "abstractservice.h"

#define DEFAULT_DB_TYPE "QSQLITE"
#define DEFAULT_CONNECTION_NAME "KmerChat"
#define DEFAULT_DB_NAME "kmerchat"

#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Core {
class User;
class Message;
class Chat;
}

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
        const QString & dbname = QString(),
        const QString & type = QString(DEFAULT_DB_TYPE),
        QObject * parent = nullptr);

    AbstractDbService(const AbstractDbService &other);

    virtual ~AbstractDbService();

    virtual User user(quint64 id);

    virtual User user(const QString &username);

    virtual Message message(quint64 id);

    virtual Chat chat(quint64 id);

    virtual QList<User> users(QString orderBy = QString("username"), bool useFilter = false, QString filter = QString());

    virtual QList<Message> messages();

    virtual QList<Chat> chats(QString filter = QString("id"));

    virtual void add(const User & user);

    virtual void add(const Message & message);

    virtual void add(const Chat & chat);

    virtual void addToChat(const Chat & chat, const quint64 &user_id);

    virtual void removeToChat(const Chat & chat, const quint64 &user_id);

    virtual void update(const User & user);

    virtual void update(const Message & message);

    virtual void update(const Chat & chat);

    virtual bool deleteUser(const quint64 id);

    virtual bool deleteUser(const QString & username);

    virtual bool deleteMessage(const quint64 id);

    virtual bool deleteChat(const quint64 & id);

    // getters
    const QString host() const { return mHost; }

    const QString dbName() const { return mDbName; }

    const int port() const { return mPort; }

    const QString username() const { return mUsername; }

    const QString password() const { return mPassword; }

    const QString driverType() const { return mDbType; }

    QSqlDatabase database() const { return db; }

    // setters
    void setHost(const QString & host) { mHost = host; }

    void setPort(const int & port) { mPort = port; }

    void setUsername(const QString &username) { mUsername = username; }

    void setPassword(const QString & password) { mPassword = password; }

    void setDbName(const QString &name) { mDbName = name; }

protected:
    // create tables
    virtual void createUsersTable();
    virtual void createMessagesTable();
    virtual void createChatsTables();

    void createTables();

    virtual void init();

    User recordToUser(const QSqlRecord & record);
    Message recordToMessage(const QSqlRecord & record);
    Chat recordToChat(const QSqlRecord & record);

    QSqlDatabase db;
    QSqlQuery query;

private:
    // properties
    QString mHost;
    int mPort;
    QString mUsername;
    QString mPassword;
    QString mDbName;
    QString mDbType;

};

} // namespace Core::Service

#endif // ABSTRACTDBSERVICE_H
