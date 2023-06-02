#ifndef ABSTRACTDBSERVICE_H
#define ABSTRACTDBSERVICE_H

#include "../message.h"
#include "../user.h"
#include <QList>

class AbstractDbService
{

public:

    virtual ~AbstractDbService()
    { }

    virtual User user(quint64 id) = 0;

    virtual Message message(quint64 id) = 0;

    virtual QList<User> users() = 0;

    virtual QList<Message> messages() = 0;

    virtual void addUser(const User & user) = 0;

    virtual void addMessage(const Message & message) = 0;

    virtual void updateUser(const User & user) = 0;

    virtual void updateMessage(const Message & message) = 0;

private:

};

#endif // ABSTRACTDBSERVICE_H
