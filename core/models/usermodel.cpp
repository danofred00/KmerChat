#include "usermodel.h"

using namespace Core::Model;

UserModel::UserModel(QList<User> users, QObject *parent)
    : mUsers{users}, QObject{parent}
{ }

template<typename T, typename _Func>
quint64 UserModel::search(const T & criteria, _Func func)
{
    // vars
    bool found = false;
    quint64 middle = 0, left = 0, rigth = mUsers.size()-1;

    // dico search
    do {

        middle = (left+rigth)/2;

        int code = func(&mUsers[middle], criteria);

        if(code == 0) {
            found = true;
            return middle+1;
        } else if(code == -1) {
            left = middle+1;
        } else {
            rigth = middle-1;
        }

    } while(!found && (left < rigth));

    return 0;
}

int UserModel::searchById(User * user, quint64 id)
{
    auto user_id = user->id();

    if(user_id > id)
        return 1;
    else if (user_id < id)
        return -1;
    else
        return 0;
}

int UserModel::searchByUsername(User * user, QString username)
{
    int result = QString::compare(user->username(), username, Qt::CaseInsensitive);

    if(result < 0)
        return -1;
    else if (result > 0)
        return 1;
    else
        return 0;
}

User * UserModel::user(quint64 id)
{
    auto index = exists(id);
    return at(index);
}

User * UserModel::user(const QString &username)
{

    auto index = exists(username);
    return at(index);
}

quint64 UserModel::exists(const QString & username)
{
    return search<QString>(username, searchByUsername);
}

User * UserModel::at(const quint64 & index)
{
    if(index >= mUsers.size() && index < 0)
        return nullptr;
    return &mUsers[index];
}

quint64 UserModel::exists(const quint64 & id)
{
    return search<quint64>(id, searchById);
}

void UserModel::add(const User & user)
{
    if(exists(user.username()) == 0)
    {
        mUsers.append(user);
        emit userAdded(&user);
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
