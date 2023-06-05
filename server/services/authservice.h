#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "core/services/abstractservice.h"
#include "core/user.h"
#include "core/models/usermodel.h"

#include <QList>


class AuthService : public AbstractService
{
    Q_OBJECT
public:
    static void start(UserModel * model);

    static AuthService * instance();

    bool login(const User * user);

    bool signup(const User * user);

    bool remove(const User * user);

    void logout(const quint64 & id);

    static void stop()
    {
        delete _instance;
    }

    UserModel * model() const { return mUserModel; }

/*
public slots:

    void updateUsers();

    void onUserLogin(const User * user);

    void onUserLogout(const User * user);

    void onUserSignup(const User * user);

    void onUserRemove(const User * user);
*/
signals:

    void userLogin(const quint64 & id);

    void userLogout(const quint64 & id);

    void newUser(const User * user);

    void removedAccount(const User * user);

private:

    AuthService(UserModel * model);
    static AuthService * _instance;

    UserModel * mUserModel;

};

#endif // AUTHSERVICE_H
