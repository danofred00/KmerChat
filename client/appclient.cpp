#include "appclient.h"

#include "core/response.h"

#include "ui/dialog.h"

using namespace Client;

AppClient::AppClient(QObject *parent)
    : serverSocket{ServerSocket()}, QObject{parent}
{
    AuthService::start();
    auth = AuthService::instance();

    // connect signals
    connect(auth, SIGNAL(loginResult(int)), this, SLOT(login(int)));
    connect(auth, SIGNAL(logoutResult(int)), this, SLOT(logout(int)));
    connect(auth, SIGNAL(signinResult(int)), this, SLOT(signin(int)));
    connect(auth, SIGNAL(signoutResult(int)), this, SLOT(signout(int)));

}

void AppClient::start(QString host, int port)
{

    serverSocket.open(QUrl(QString("ws://%1:%2").arg(host).arg(port)));

    QObject::connect(&serverSocket, &ServerSocket::connected, [&]() {

        auth->setServerSocket(&serverSocket);

        // connect ui signals
        // loginDlg signals
        QObject::connect(&loginDlg, &Login::readyLogin, this, [&](QString username, QString password) {
            User user = User::Builder().hasUsername(username).hasPassword(password).build();
            auth->login(&user);
        });
        QObject::connect(&loginDlg, SIGNAL(hasNoAccount()), this, SLOT(onHasNoAccount()));
        // signinDlg signals
        QObject::connect(&signupDlg, &Signup::ready, this, [&](QMap<QString, QVariant> user) {
            User _user = User(user);
            auth->signup(&_user);
        });
        QObject::connect(&signupDlg, SIGNAL(alreadyHasAccount()), this, SLOT(onAlreadyHasAccount()));

        // start uis
        loginDlg.show();
    });
}

void AppClient::onAlreadyHasAccount()
{
    signupDlg.hide();
    loginDlg.show();
}

void AppClient::onHasNoAccount()
{
    loginDlg.hide();
    signupDlg.show();
}

void AppClient::login(int result)
{
    if(result == Response::SUCCESS)
    {
        Response * response = serverSocket.lastResponse();
        auto user = User::fromJsonString(response->content());
        mUser = new User(user);

        qDebug() << mUser->toString();
    } else {
        qDebug() << "Invalid login";

        Dialog dlg = Dialog({tr("Invalid username or Password")});
        dlg.exec();
    }
}

void AppClient::logout(int result)
{
    if(result == Response::SUCCESS)
    {
        qDebug() << "Login Out ... ";
    } else {

        Dialog dlg = Dialog({tr("Some Error apear.")});
        dlg.exec();
    }
}

void AppClient::signin(int result)
{
    if(result == Response::SUCCESS)
    {
        Response * response = serverSocket.lastResponse();
        auto user = User::fromJsonString(response->content());
        mUser = new User(user);

        qDebug() << mUser->toString();
    } else {
        Dialog dlg = Dialog({tr("User Already Exists")});
        dlg.exec();
    }
}

void AppClient::signout(int result)
{
    if(result == Response::SUCCESS)
    {
        qDebug() << "Account Removed";
    } else {

        Dialog dlg = Dialog({tr("There's some error apear.")});
        dlg.exec();
    }
}
