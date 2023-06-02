#include "dbservice.h"

#include <QSqlQuery>
#include <QSqlError>

DbService::DbService(const QString & host,
                     const int port,
                     const QString & username,
                     const QString & password,
                     QObject *parent)
    : QObject(parent),
    mHost(host),
    mPort(port),
    mUsername(username),
    mPassword(password)
{
    // init the database
    db = QSqlDatabase::addDatabase(DEFAULT_DB_TYPE, DEFAULT_CONNECTION_NAME);

    // open the database
    db.setDatabaseName(DEFAULT_DB_NAME);
    db.setUserName(username);
    db.setHostName(host);
    db.setPassword(password);
    db.setPort(port);

    if(db.open()) {
        // create tables
        createTables();
        // update the instance var
        // _instance = this;
    } else {
        qDebug() << db.lastError().text();
    }
}

DbService * DbService::instance() {
    if(_instance == nullptr)
        // usage of uninstanciate object
        throw std::runtime_error("Unable to get instance of DbService, please run init first.");

    // if everything is okay
    return _instance;
}

DbService::DbService(const DbService &other)
{
    db = other.db;
    setUsername(other.username());
    setDbname(other.dbname());
    setHost(other.host());
    setPassword(other.password());
    setPort(other.port());
}

DbService::~DbService()
{
    // close database
    db.close();
    // delete _instance pointer
    // delete _instance;
}

void DbService::init(const QString & host, const int port, const QString & username, const QString & password)
{
    // instanciate the service of it's not already done
    if(_instance == nullptr)
    {
        _instance = new DbService(host, port, username, password);
    }
}

// the unique instance of this object
DbService * DbService::_instance = nullptr;

void DbService::createTables()
{
    QSqlQuery query = QSqlQuery(db);

    if(db.isOpen()) {


        // create table users
        query.exec("CREATE TABLE IF NOT EXISTS users ("
                "   id INTEGER NOT NULL,"
                "   username VARCHAR(50),"
                "   name VARCHAR(50),"
                "   email VARCHAR(50),"
                "   password VARCHAR(32),"
                "   tel VARCHAR(20),"
                "   image VARCHAR(255),"
                "   PRIMARY KEY(id AUTOINCREMENT)"
                ");");

        // create table messages
        query.exec("CREATE TABLE IF NOT EXISTS messages ("
                "   id INTEGER NOT NULL,"
                "   content TEXT NOT NULL,"
                "   userFrom INTEGER NOT NULL,"
                "   userTo INTEGER NOT NULL,"
                "   type CHAR(10),"
                "   state CHAR(10),"
                "   PRIMARY KEY(id AUTOINCREMENT),"
                "   FOREIGN KEY(userFrom) REFERENCES users(id),"
                "   FOREIGN KEY(userTo) REFERENCES users(id)"
                ");");

        // db.commit();
    }
}

User DbService::user(quint64 id)
{
    User user;

    if(db.isOpen()) {

    }

    return user;
}

Message DbService::message(quint64 id)
{
    Message message;

    return message;
}
QList<User> DbService::users()
{
    QList<User> users;

    return users;
}

QList<Message> DbService::messages()
{
    QList<Message> messages;


    return messages;
}

void DbService::addUser(User user)
{

}

void DbService::addMessage(Message message)
{

}

void DbService::updateUser(User user)
{

}

void DbService::updateMessage()
{

}
