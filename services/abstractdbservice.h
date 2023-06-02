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

    virtual void addUser(User user) = 0;

    virtual void addMessage(Message message) = 0;

    virtual void updateUser(User user) = 0;

    virtual void updateMessage() = 0;

private:

};

#endif // ABSTRACTDBSERVICE_H
