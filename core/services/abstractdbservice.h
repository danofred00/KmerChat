#ifndef ABSTRACTDBSERVICE_H
#define ABSTRACTDBSERVICE_H

#include "abstractservice.h"

#include "../user.h"
#include "../message.h"

#include <QList>

namespace Core::Service {

using namespace Core;

class AbstractDbService : public AbstractService
{
    Q_OBJECT
public:

    virtual ~AbstractDbService()
    { }

    virtual User user(quint64 id) = 0;

    virtual User user(const QString &username) = 0;

    virtual Message message(quint64 id) = 0;

    virtual QList<User> users() = 0;

    virtual QList<Message> messages() = 0;

    virtual void add(const User & user) = 0;

    virtual void add(const Message & message) = 0;

    virtual void update(const User & user) = 0;

    virtual void update(const Message & message) = 0;

    virtual bool deleteUser(const quint64 id) = 0;

    virtual bool deleteUser(const QString & username) = 0;

    virtual bool deleteMessage(const quint64 id) = 0;

private:

};

} // namespace Core::Service

#endif // ABSTRACTDBSERVICE_H
