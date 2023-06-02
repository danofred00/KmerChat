#ifndef ABSTRACTDBSERVICE_H
#define ABSTRACTDBSERVICE_H

#include "abstractservice.h"
#include "../message.h"
#include "../user.h"
#include <QList>

class AbstractDbService : public AbstractService
{

public:

    virtual ~AbstractDbService()
    { }

    virtual User user(quint64 id) = 0;

    virtual Message message(quint64 id) = 0;

    virtual QList<User> users() = 0;

    virtual QList<Message> messages() = 0;

    virtual void add(const User & user) = 0;

    virtual void add(const Message & message) = 0;

    virtual void update(const User & user) = 0;

    virtual void update(const Message & message) = 0;

    virtual bool deleteUser(const quint64 id) = 0;

    virtual bool deleteMessage(const quint64 id) = 0;

private:

};

#endif // ABSTRACTDBSERVICE_H
