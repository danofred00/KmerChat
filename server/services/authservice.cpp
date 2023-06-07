
#include "authservice.h"
#include <QDebug>

using namespace Server::Service ;

AuthService::AuthService(UserModel * model)
    : mUserModel{model}, AbstractService{nullptr}
{
    // debug
    qDebug() << "Starting AuthService ...";
}

void AuthService::start(UserModel *model)
{
    if(_instance == nullptr)
        _instance = new AuthService(model);
}

AuthService * AuthService::instance()
{
    if(_instance == nullptr)
        // usage of uninstanciate object
        throw std::runtime_error("Unable to get instance of AuthService, please run start first.");

    // if everything is okay
    return _instance;
}

AuthService * AuthService::_instance = nullptr;


bool AuthService::login(const User * user)
{
    const User * _user = mUserModel->user(user->username());

    if(_user != nullptr)
        if(_user->password() == user->password()) {
            emit userLogin(_user->id());
            return true;
        }
    return false;
}

bool AuthService::signup(const User * user)
{
    if(mUserModel->exists(user->username()) == 0){
        mUserModel->add(*user);

        // get userId
        // auto id = mUserModel->users().size() - 1;
        emit newUser(user);
        return true;
    }

    return false;
}

bool AuthService::remove(const User * user)
{
    auto username = user->username();
    // we remove the user only if is valid id and credentials
    if(mUserModel->exists(username) > 0 && user->password() == mUserModel->user(username)->password()) {
        mUserModel->remove(username);
        emit removedAccount(user);
        return true;
    }

    return false;
}

void AuthService::logout(const quint64 & id)
{
    emit userLogout(id);
}

/*
void AuthService::onUserLogin(const User & user)
{

}

void AuthService::onUserLogout(const User & user)
{

}

void AuthService::onUserSignup(const User & user)
{

}

void AuthService::onUserRemove(const User & user)
{

}
*/
