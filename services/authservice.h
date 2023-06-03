#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "abstractservice.h"
#include "../user.h"
#include "../usermodel.h"

#include <QList>


class AuthService : public AbstractService
{
    Q_OBJECT
public:
    static void start(UserModel * model);

    static AuthService * instance();

    bool login(const User * user);

    void signup(const User * user);

    void remove(const quint64 & id);

    void logout(const quint64 & id);

    static void stop()
    {
        delete _instance;
    }

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

    void newUser(const quint64 & id);

    void removedAccount(const User user);

private:

    AuthService(UserModel * model);
    static AuthService * _instance;

    void removeConnection(const quint64 & id);

    UserModel * model;

};

#endif // AUTHSERVICE_H
