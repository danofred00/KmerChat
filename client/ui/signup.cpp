#include "signup.h"
#include "ui_signup.h"

using namespace Client::Gui;

Signup::Signup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);

    connect(ui->alreadyHasAccount, &QPushButton::clicked, [&](){
        emit alreadyHasAccount();
    });

    connect(ui->signupButton, &QPushButton::clicked, this, &Signup::signupButtonClicked);
}

Signup::~Signup()
{
    delete ui;
}

void Signup::signupButtonClicked()
{
    auto pwd = ui->inputPassword->text();
    auto cpwd = ui->inputConfirmPassword->text();

    if(pwd == cpwd) {

        qDebug() << "Matching passwords ";

        QMap<QString, QVariant> user;
        user.insert("username", QVariant(ui->inputUsername->text()));
        user.insert("name", QVariant(ui->inputName->text()));
        user.insert("email", QVariant(ui->inputEmail->text()));
        user.insert("password", QVariant(pwd));

        emit ready(user);
    } else {
        qDebug() << "Not matching passwords ";
    }
}
