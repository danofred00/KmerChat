#include "usermodel.h"

#include "user.h"

UserModel::UserModel(QList<User> users, QObject *parent)
    : mUsers{users}, QObject{parent}
{ }

User UserModel::user(quint64 id)
{
    for(User & u : mUsers)
        if(u.id() == id)
            return u;
    return nullptr;
}

User UserModel::user(const QString &username)
{
    for(User & u : mUsers)
        if(u.username() == username)
            return u;
    return nullptr;
}


quint64 UserModel::exists(const QString & username)
{
    for(quint64 i=0; i<users().size(); i++)
        if(users().at(i).username() == username)
            return i+1;

    return 0;
}

quint64 UserModel::exists(const quint64 & id)
{
    for(quint64 i=0; i<users().size(); i++)
        if(users().at(i).id() == id)
            return i+1;

    return 0;
}

void UserModel::add(const User & user)
{
    if(exists(user.username()) == 0)
    {
        mUsers.append(user);
        emit userAdded(user);
    }
}

void UserModel::remove(const quint64 & id)
{
    auto index = exists(id);
    if(index > 0) {
        mUsers.removeAt(index-1);
        emit userRemoved(id);
    }
}

void UserModel::remove(const QString & username)
{
    auto index = exists(username);
    if(index > 0) {
        // get the id of user to remove
        auto id = mUsers.at(index-1).id();
        mUsers.removeAt(index-1);
        //
        emit userRemoved(id);
    }
}
