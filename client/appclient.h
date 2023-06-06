#ifndef APPCLIENT_H
#define APPCLIENT_H

#include <QObject>

#include "core/user.h"
#include "core/models/usermodel.h"
#include "serversocket.h"
#include "services/authservice.h"

#include "ui/login.h"
#include "ui/signup.h"

namespace Client {

using namespace Core;
using namespace Core::Model;
using namespace Client;
using namespace Client::Service;
using namespace Client::Gui;

class AppClient : public QObject
{
    Q_OBJECT
public:

    AppClient(QObject *parent = nullptr);

    ~AppClient() {
        delete mUser;
    }

    void start(QString host, int port);

signals:

    void quit();

private slots:

    void onAlreadyHasAccount();

    void onHasNoAccount();

    void login(int result);

    void logout(int result);

    void signin(int result);

    void signout(int result);

private:

    AuthService * auth;
    ServerSocket serverSocket;

    User * mUser;

    Login loginDlg;
    Signup signupDlg;
};

}

#endif // APPCLIENT_H
