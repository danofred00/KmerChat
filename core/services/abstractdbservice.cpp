
#include "abstractdbservice.h"

#include <QSqlError>
#include <QDebug>
#include "../user.h"
#include "../message.h"
#include "../chat.h"

using namespace Core::Service;

AbstractDbService::AbstractDbService(
    const QString & host,
    const int port,
    const QString & username,
    const QString & password,
    const QString &dbname,
    const QString & type,
    QObject * parent)
    :   mHost(host),
        mPort(port),
        mUsername(username),
        mPassword(password),
        mDbType(type),
        AbstractService(parent)
{
    // debug
    qDebug() << "Starting AbstractDatabase Service ...";
}

AbstractDbService::AbstractDbService(const AbstractDbService &other)
{
    db = other.db;
    setUsername(other.username());
    setDbName(other.dbName());
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
        createUsersTable();
        createChatsTables();
        createMessagesTable();
    }
}

void AbstractDbService::init()
{
    // init the database
    db = QSqlDatabase::addDatabase(mDbType, DEFAULT_CONNECTION_NAME);

    // open the database
    auto n = mDbName.isEmpty() ? DEFAULT_DB_NAME : mDbName;
    db.setDatabaseName(n);
    db.setUserName(mUsername);
    db.setHostName(mHost);
    db.setPassword(mPassword);
    db.setPort(mPort);

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

Chat AbstractDbService::chat(quint64 id)
{
    if(db.isOpen()) {

        query.exec(QString("SELECT * FROM chats WHERE id=%1;").arg(id));
        if(query.first()) {

            Chat _chat = recordToChat(query.record());

            // get all users
            query.prepare("SELECT user_id FROM user_chat_table WHERE chat_id=?;");
            query.bindValue(0, QVariant(id));
            query.exec();

            while(query.next())
                _chat.add(query.value(0).toULongLong());
            return _chat;
        } else {
            throw std::runtime_error(QString("Chat %d not exits").arg(id).toStdString());
        }

    }
    throw std::runtime_error("Database is closed");
}

QList<User> AbstractDbService::users(QString orderBy, bool useFilter, QString filter)
{
    QList<User> users;

    QString q = "SELECT * FROM users WHERE ";
    q += (useFilter) ? "1"
                     : QString("name LIKE '%%1%' OR username LIKE '%%2%'").arg(filter, filter);
    q += QString(" ORDER BY %1 ASC;").arg(orderBy);
    query.exec(q);

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
    msg.setChatId(query.value(2).toULongLong());
    msg.setUserId(query.value(3).toULongLong());
    msg.setContent(query.value(1).toString());
    msg.setState(Message::strToMessageState(query.value(5).toString()));
    msg.setType(Message::strToMessageType(query.value(4).toString()));

    return msg;
}

Chat AbstractDbService::recordToChat(const QSqlRecord &record)
{
    Chat chat;
    chat.setId(record.value(0).toULongLong());
    chat.setName(record.value(1).toString());
    chat.setType(Chat::strToType(record.value(2).toString()));
    return chat;
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

bool AbstractDbService::deleteChat(const quint64 &id)
{
    bool result = false;

    if(db.isOpen()) {

        // delete all the messages on this chat
        result = query.exec(QString("DELETE FROM messages WHERE chat_id=%1;").arg(id));
        // delete all elements in user_chat_table
        result = query.exec(QString("DELETE FROM user_chat_table WHERE chat_id=%1;").arg(id));
        // delete the chat
        result = query.exec(QString("DELETE FROM chats WHERE id=%1;").arg(id));
        return result;
    }
    throw std::runtime_error("Database is closed");
}

void AbstractDbService::createUsersTable()
{
    // create table users
    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "   id INTEGER NOT NULL,"
               "   username VARCHAR(50) UNIQUE,"
               "   name VARCHAR(50) NOT NULL,"
               "   email VARCHAR(50) NOT NULL,"
               "   password VARCHAR(64) NOT NULL,"
               "   tel VARCHAR(20),"
               "   image VARCHAR(255),"
               "   PRIMARY KEY(id AUTOINCREMENT)"
               ");");
}

void AbstractDbService::createMessagesTable()
{
    // create table messages
    query.exec("CREATE TABLE IF NOT EXISTS messages ("
               "   id INTEGER NOT NULL,"
               "   content TEXT NOT NULL,"
               "   user_id INTEGER NOT NULL,"
               "   chat_id INTEGER NOT NULL,"
               "   type CHAR(10),"
               "   state CHAR(10),"
               "   PRIMARY KEY(id AUTOINCREMENT),"
               "   FOREIGN KEY(user_id) REFERENCES users(id),"
               "   FOREIGN KEY(chat_id) REFERENCES chats(id)"
               ");");
}

void AbstractDbService::createChatsTables()
{
    // create table chats
    query.exec("CREATE TABLE IF NOT EXISTS chats ("
               "   id INTEGER,"
               "   name VARCHAR(50),"
               "   type VARCHAR(10),"
               "   PRIMARY KEY(id AUTOINCREMENT)"
               ");");
    // create a relation between user & chat
    query.exec("CREATE TABLE IF NOT EXISTS user_chat_table ("
               "   id INTEGER,"
               "   chat_id INTEGER NOT NULL,"
               "   user_id INTEGER NOT NULL,"
               "   PRIMARY KEY(id AUTOINCREMENT),"
               "   FOREIGN KEY(chat_id) REFERENCES chats(id),"
               "   FOREIGN KEY(user_id) REFERENCES users(id)"
               ");");
}

User AbstractDbService::recordToUser(const QSqlRecord & record)
{
    return User::Builder()
        .hasId(record.value(0).toULongLong())
        .hasUsername(record.value(1).toString())
        .hasName(record.value(2).toString())
        .hasEmail(record.value(3).toString())
        .hasPassword(record.value(4).toString())
        .hasTel(record.value(5).toString())
        .hasImage(record.value(6).toString())
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

QList<Chat> AbstractDbService::chats(QString filter)
{
    QList<Chat> _chats;

    query.prepare(QString("SELECT id FROM chats WHERE 1 ORDER BY %1 ASC").arg(filter));
    query.exec();

    while(query.next()) {
        //auto id = query.value(0).toULongLong();
        _chats.append(chat(query.value(0).toULongLong()));
    }

    return _chats;
}

void AbstractDbService::add(const User & user)
{
    // prepare the request
    query.prepare("INSERT INTO users(username, name, email, password, tel, image)"
                  "  VALUES (:username, :name, :email, :password, :tel, :image);");
    // bind all values
    query.bindValue(":username", user.username().toLower());
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
    query.bindValue(":userFrom", message.chatId());
    query.bindValue(":userTo", message.userId());
    query.bindValue(":type", Message::msgTypeToStr(message.type()));
    query.bindValue(":state", Message::msgStateToStr(message.state()));
    // execution
    query.exec();

    // commit the transaction
    db.commit();
}

void AbstractDbService::add(const Chat &chat)
{
    // appends the chat
    query.prepare("INSERT INTO chats(name, type)"
                  " VALUES (:name, :type)");
    query.bindValue(":name", chat.name());
    query.bindValue(":type", Chat::typeToString(chat.type()));
    query.exec();
    // save changes
    db.commit();
}

void AbstractDbService::addToChat(const Chat &chat, const quint64 &user_id)
{
    // appends in the chat
    query.prepare("INSERT INTO user_chat_table(chat_id, user_id)"
                  " VALUES (:chat, :user)");
    query.bindValue(":chat", chat.id());
    query.bindValue(":user", QVariant((qint64) user_id));
    query.exec();
    // save changes
    db.commit();
}

void AbstractDbService::removeToChat(const Chat &chat, const quint64 &user_id)
{
    // appends in the chat
    query.prepare("DELETE FROM user_chat_table WHERE user_id=? AND chat_id=?");
    query.bindValue(0, user_id);
    query.bindValue(1, chat.id());
    query.exec();
    // save changes
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
    query.bindValue(":userFrom", message.chatId());
    query.bindValue(":userTo", message.userId());
    query.bindValue(":type", Message::msgTypeToStr(message.type()));
    query.bindValue(":state", Message::msgStateToStr(message.state()));

    // execution
    query.exec();

    // commit the transaction
    db.commit();
}

void AbstractDbService::update(const Chat &chat)
{

}
