#include "login.h"
#include "ui_login.h"

using namespace Client::Gui;

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setFixedSize(size());

    // connect signals
    connect(ui->loginButton, SIGNAL(clicked(bool)), this, SLOT(loginButtonClicked()));

    connect(ui->hasNoAccount, &QPushButton::clicked, [&](){
       emit hasNoAccount();
    });
}

void Login::loginButtonClicked()
{
    auto username = ui->inputUsername->text();
    auto password = ui->inputPassword->text();

    emit readyLogin(username, password);
}

Login::~Login()
{
    delete ui;
}
