#ifndef USERMODEL_H
#define USERMODEL_H

#include <QObject>

#include "../user.h"

namespace Core::Model {

using namespace Core;

class UserModel : public QObject
{
    Q_OBJECT
public:
    UserModel(QList<User> users, QObject *parent = nullptr);

    QList<User> users() { return mUsers; }

    void setUsers(QList<User> users) { mUsers = users; }

    User user(quint64 id);

    User user(const QString &username);

    /**
     * @brief exists
     * Check if a user with username gave at param exists
     * @param username
     * @return index of user+1 exists and 0 in other case
     */
    quint64 exists(const QString & username);

    quint64 exists(const quint64 & id);

    void add(const User & user);

    void remove(const quint64 & id);

    void remove(const QString & username);

signals:

    void userRemoved(const quint64 & id);

    void userAdded(const User & user);

private:
    QList<User> mUsers;
};

} // namespace Core::Model

#endif // USERMODEL_H
