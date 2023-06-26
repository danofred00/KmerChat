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

    User * user(quint64 id);

    User * user(const QString &username);

    /**
     * @brief exists
     * Check if a user with username gave at param exists
     * @param username
     * @return index of user+1 exists and 0 in other case
     */
    quint64 exists(const QString & username);

    quint64 exists(const quint64 & id);

    User * at(const quint64 & index);

    quint64 add(User & user);

    void remove(const quint64 & id);

    void remove(const QString & username);

signals:

    void userRemoved(const quint64 & id);

    void userAdded(User * user);

protected:

    /**
     * @brief search
     * This function do the dicotomic search into a QList<User>
     * @param func The criteria, it return -1 if User.T<T, 0 if equal and 1 otherwise
     * @return the position of element to search or 0 if the element is not found
     */
    // std::function<int(const User *, const T &)> & func
    template<typename T, typename _Func>
    quint64 search(const T & criteria, _Func func);

private:

    static int searchById(User * user, quint64 id);

    static int searchByUsername(User * user, QString username);

    QList<User> mUsers;
};

} // namespace Core::Model

#endif // USERMODEL_H
