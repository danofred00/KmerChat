
#include "dbservice.h"

#include <QSqlError>
#include <QSqlDatabase>

using namespace Server::Service ;


DbService * DbService::instance() {
    if(_instance == nullptr)
        // usage of uninstanciate object
        throw std::runtime_error("Unable to get instance of DbService, please run start first.");

    // if everything is okay
    return _instance;
}

void DbService::start(const QString & host, const int port,
                      const QString & username, const QString & password,
                      const QString & dbName, const QString &driver)
{
    // instanciate the service of it's not already done
    if(_instance == nullptr)
    {
        _instance = new DbService(host, port, username, password, dbName, driver);
    }
}

// the unique instance of this object
DbService * DbService::_instance = nullptr;

/////////////////////////

void DbService::createUsersTable()
{
    // create table users
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "   id SERIAL,"
               "   username VARCHAR(50) UNIQUE,"
               "   name VARCHAR(50) NOT NULL,"
               "   email VARCHAR(50) NOT NULL,"
               "   password VARCHAR(64) NOT NULL,"
               "   tel VARCHAR(20),"
               "   image VARCHAR(255),"
               "   PRIMARY KEY(id)"
               ");");
}

void DbService::createMessagesTable()
{
    // create table messages
    query.exec("CREATE TABLE IF NOT EXISTS messages ("
               "   id SERIAL,"
               "   content TEXT NOT NULL,"
               "   user_id INTEGER NOT NULL,"
               "   chat_id INTEGER NOT NULL,"
               "   type CHAR(10),"
               "   state CHAR(10),"
               "   PRIMARY KEY(id),"
               "   FOREIGN KEY(user_id) REFERENCES users(id),"
               "   FOREIGN KEY(chat_id) REFERENCES chats(id)"
               ");");
}

void DbService::createChatsTables()
{
    // create table chats
    query.exec("CREATE TABLE IF NOT EXISTS chats ("
               "   id SERIAL,"
               "   name VARCHAR(50),"
               "   type VARCHAR(10),"
               "   PRIMARY KEY(id)"
               ");");
    // create a relation between user & chat
    query.exec("CREATE TABLE IF NOT EXISTS user_chat_table ("
               "   chat_id INTEGER NOT NULL,"
               "   user_id INTEGER NOT NULL,"
               "   FOREIGN KEY(chat_id) REFERENCES chats(id),"
               "   FOREIGN KEY(user_id) REFERENCES users(id),"
               "   PRIMARY KEY(chat_id, user_id)"
               ");");
}

DbService::DbService(const QString &host, const int port,
                     const QString &username, const QString &password,
                     const QString &dbname, const QString &type, QObject *parent)
    : AbstractDbService(host, port, username, password, dbname, type, parent)
{
    init();
}

