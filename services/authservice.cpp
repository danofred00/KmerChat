
#include "authservice.h"


AuthService::AuthService(UserModel * model)
    : model{model}, AbstractService{nullptr}
{
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
    const User _user = model->user(user->username());

    if(_user.password() == user->password()) {
        emit userLogin(user->id());
        return true;
    }
    return false;
}

void AuthService::signup(const User * user)
{
    if(model->exists(user->username()) == 0){
        model->add(*user);

        // get userId
        auto id = model->user(user->username()).id();
        emit newUser(id);
    }
}

void AuthService::remove(const quint64 & id)
{
    if(model->exists(id) > 0)
        model->remove(id);
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