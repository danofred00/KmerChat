
#include "abstractdbservice.h"

#include <QSqlError>
#include <QDebug>

using namespace Core::Service;

AbstractDbService::AbstractDbService(
    const QString & host,
    const int port,
    const QString & username,
    const QString & password,
    QObject * parent)
    :   mHost(host),
        mPort(port),
        mUsername(username),
        mPassword(password),
        AbstractService(parent)
{
    // debug
    qDebug() << "Starting Database Service ...";

    // init the database
    db = QSqlDatabase::addDatabase(DEFAULT_DB_TYPE, DEFAULT_CONNECTION_NAME);

    // open the database
    db.setDatabaseName(DEFAULT_DB_NAME);
    db.setUserName(username);
    db.setHostName(host);
    db.setPassword(password);
    db.setPort(port);

    if(db.open()) {
        // init query
        query = QSqlQuery(db);
        // create tables
        createTables();
        // update the instance var
        // _instance = this;
    } else {
        qDebug() << db.lastError().text();
    }
}

AbstractDbService::AbstractDbService(const AbstractDbService &other)
{
    db = other.db;
    setUsername(other.username());
    setDbname(other.dbname());
    setHost(other.host());
    setPassword(other.password());
    setPort(other.port());
}

AbstractDbService::~AbstractDbService()
{
    // close database
    db.close();
    // delete _instance pointer
    // delete _instance;
}

void AbstractDbService::createTables()
{
    if(db.isOpen()) {
        // create table users
        query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "   id INTEGER NOT NULL,"
                   "   username VARCHAR(50) UNIQUE,"
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
    }
}

User AbstractDbService::user(quint64 id)
{
    if(db.isOpen()) {

        query.prepare("SELECT * FROM users WHERE id=?;");
        query.bindValue(0, id);
        query.exec();

        if(query.first())
            return recordToUser(query.record());
        else
            throw std::runtime_error("Non exists user");
    }
    throw std::runtime_error("Database is closed");
}

User AbstractDbService::user(const QString &username)
{
    if(db.isOpen()) {

        query.prepare("SELECT * FROM users WHERE username=?;");
        query.bindValue(0, username);
        query.exec();

        if(query.first())
            return recordToUser(query.record());
        else
            throw std::runtime_error("Non exists user");
    }
    throw std::runtime_error("Database is closed");
}

Message AbstractDbService::message(quint64 id)
{
    if(db.isOpen()) {

        query.prepare("SELECT * FROM messages WHERE id=?;");
        query.bindValue(0, id);
        query.exec();

        if(query.first())
            return recordToMessage(query.record());
        else
            throw std::runtime_error("Non exists user");
    }
    throw std::runtime_error("Database is closed");
}

QList<User> AbstractDbService::users()
{
    QList<User> users;

    query.exec("SELECT DISTINCT * FROM users WHERE 1 ORDER BY id ASC;");

    while(query.next())
    {
        users.append(recordToUser(query.record()));
    }

    return users;
}

Message AbstractDbService::recordToMessage(const QSqlRecord & record)
{
    Message msg;

    msg.setId(query.value(0).toULongLong());
    msg.setUserFrom(query.value(2).toULongLong());
    msg.setUserTo(query.value(3).toULongLong());
    msg.setContent(query.value(1).toString());
    msg.setState(Message::strToMessageState(query.value(5).toString()));
    msg.setType(Message::strToMessageType(query.value(4).toString()));

    return msg;
}

bool AbstractDbService::deleteUser(const quint64 id)
{
    if(db.isOpen()) {

        query.prepare("DELETE FROM users WHERE id=?;");
        query.bindValue(0, id);
        return query.exec();
    }
    throw std::runtime_error("Database is closed");
}

bool AbstractDbService::deleteUser(const QString & username)
{
    if(db.isOpen()) {

        query.prepare("DELETE FROM users WHERE username=?;");
        query.bindValue(0, username);
        return query.exec();
    }
    throw std::runtime_error("Database is closed");
}

bool AbstractDbService::deleteMessage(const quint64 id)
{
    if(db.isOpen()) {

        query.prepare("DELETE FROM messages WHERE id=?;");
        query.bindValue(0, id);
        return query.exec();
    }
    throw std::runtime_error("Database is closed");
}

User AbstractDbService::recordToUser(const QSqlRecord & record)
{
    return User::Builder()
        .hasId(query.value(0).toULongLong())
        .hasUsername(query.value(1).toString())
        .hasName(query.value(2).toString())
        .hasEmail(query.value(3).toString())
        .hasPassword(query.value(4).toString())
        .hasTel(query.value(5).toString())
        .hasImage(query.value(6).toString())
        .build();
}

QList<Message> AbstractDbService::messages()
{
    QList<Message> messages;

    query.exec("SELECT DISTINCT * FROM messages WHERE 1 ORDER BY id ASC;");

    while(query.next())
    {
        messages.append(recordToMessage(query.record()));
    }

    return messages;
}

void AbstractDbService::add(const User & user)
{
    // prepare the request
    query.prepare("INSERT INTO users(username, name, email, password, tel, image)"
                  "  VALUES (:username, :name, :email, :password, :tel, :image);");
    // bind all values
    query.bindValue(":username", user.username());
    query.bindValue(":name", user.name());
    query.bindValue(":password", user.password());
    query.bindValue(":tel", user.tel());
    query.bindValue(":image", user.image());
    query.bindValue(":email", user.email());
    // execution
    query.exec();

    // commit the transaction
    db.commit();
}

void AbstractDbService::add(const Message & message)
{
    // prepare the request
    query.prepare("INSERT INTO messages(content, userFrom, userTo, type, state)"
                  "  VALUES (:content, :userFrom, :userTo, :type, :state);");
    // bind all values
    query.bindValue(":content", message.content());
    query.bindValue(":userFrom", message.from());
    query.bindValue(":userTo", message.to());
    query.bindValue(":type", Message::msgTypeToStr(message.type()));
    query.bindValue(":state", Message::msgStateToStr(message.state()));
    // execution
    query.exec();

    // commit the transaction
    db.commit();
}

void AbstractDbService::update(const User & user)
{
    QSqlQuery query(db);
    // prepare the request
    query.prepare("UPDATE users "
                  "SET username=:username, name=:name, email=:email, password=:password, tel=:tel, image=:image "
                  "WHERE id=:id;");
    // bind all values
    query.bindValue(":username", user.username());
    query.bindValue(":name", user.name());
    query.bindValue(":password", user.password());
    query.bindValue(":tel", user.tel());
    query.bindValue(":email", user.email());
    query.bindValue(":image", user.image());
    query.bindValue(":id", user.id());
    // execution
    query.exec();

    // commit the transaction
    db.commit();
}

void AbstractDbService::update(const Message & message)
{
    // prepare the request
    query.prepare("UPATE messages "
                  "SET content=:content, userFrom=:userFrom, userTo=:userTo, type=:type, state=:state "
                  "WHERE id=:id;");
    // bind all values
    query.bindValue(":id", message.id());
    query.bindValue(":content", message.content());
    query.bindValue(":userFrom", message.from());
    query.bindValue(":userTo", message.to());
    query.bindValue(":type", Message::msgTypeToStr(message.type()));
    query.bindValue(":state", Message::msgStateToStr(message.state()));

    // execution
    query.exec();

    // commit the transaction
    db.commit();
}
