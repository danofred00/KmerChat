#include "dbservice.h"

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

User DbService::user(quint64 id)
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

Message DbService::message(quint64 id)
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

QList<User> DbService::users()
{
    QList<User> users;

    query.exec("SELECT DISTINCT * FROM users WHERE 1 ORDER BY id ASC;");

    while(query.next())
    {
        users.append(recordToUser(query.record()));
    }

    return users;
}

Message DbService::recordToMessage(const QSqlRecord & record)
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

bool DbService::deleteUser(const quint64 id)
{
    if(db.isOpen()) {

        query.prepare("DELETE FROM users WHERE id=?;");
        query.bindValue(0, id);
        return query.exec();
    }
    throw std::runtime_error("Database is closed");
}

bool DbService::deleteMessage(const quint64 id)
{
    if(db.isOpen()) {

        query.prepare("DELETE FROM messages WHERE id=?;");
        query.bindValue(0, id);
        return query.exec();
    }
    throw std::runtime_error("Database is closed");
}

User DbService::recordToUser(const QSqlRecord & record)
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

QList<Message> DbService::messages()
{
    QList<Message> messages;

    query.exec("SELECT DISTINCT * FROM messages WHERE 1 ORDER BY id ASC;");

    while(query.next())
    {
        messages.append(recordToMessage(query.record()));
    }

    return messages;
}

void DbService::add(const User & user)
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

void DbService::add(const Message & message)
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

void DbService::update(const User & user)
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

void DbService::update(const Message & message)
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
