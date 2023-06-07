#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include "core/user.h"
#include "core/models/usermodel.h"
#include "core/services/abstractservice.h"

#include <QList>

namespace Server::Service  {

using namespace Core;
using namespace Core::Model;
using namespace Core::Service;

class AuthService : public AbstractService
{
    Q_OBJECT
public:
    static void start(UserModel * model);

    static AuthService * instance();

    static QString serviceName() {
        return QString("AuthService");
    }

    bool login(const User * user);

    bool signup(const User * user);

    bool remove(const User * user);

    void logout(const User * user);

    static void stop()
    {
        qDebug() << "Closing the Service : " << serviceName();
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

    void userLogin(const User * user);

    void userLogout(const User * user);

    void newUser(const User * user);

    void removedAccount(const User * user);

private:

    AuthService(UserModel * model);
    static AuthService * _instance;

    UserModel * mUserModel;

};

} // namespace Server

#endif // AUTHSERVICE_H
