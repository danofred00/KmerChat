#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QObject>
#include <QSqlDatabase>

#include "abstractdbservice.h"

#define DEFAULT_DB_TYPE "QSQLITE"
#define DEFAULT_CONNECTION_NAME "KmerChat"
#define DEFAULT_DB_NAME "kmerchat.db"

class DbService : public QObject, public AbstractDbService
{
    Q_OBJECT
public:

    // usage of singleton pattern to enable one only instance of DbService::class
    static DbService * instance();

    ~DbService();

    static void init(const QString & host = QString("localhost"),
                     const int port = 8080,
                     const QString & username = QString("admin"),
                     const QString & password = QString("password")
                    );

    User user(quint64 id) override;

    Message message(quint64 id) override;

    QList<User> users() override;

    QList<Message> messages() override;

    void addUser(User user) override;

    void addMessage(Message message) override;

    void updateUser(User user) override;

    void updateMessage() override;

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
              const QString & password,
              QObject *parent = nullptr);

    void createTables();

    DbService(const DbService &other);

    static DbService * _instance;

    // prpêrties
    QString mHost;
    int mPort;
    QString mUsername;
    QString mPassword;
    QString mDbname;

    QSqlDatabase db;

};

#endif // DBSERVICE_H
