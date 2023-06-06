#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

namespace Client::Gui{

class Login : public QDialog
{
    Q_OBJECT

public:

    Login(QWidget *parent = nullptr);

    ~Login();

signals:

    void readyLogin(QString & username, QString & password);

    void hasNoAccount();

private slots:

    void loginButtonClicked();


private:
    Ui::Login *ui;
};

} // namespace Client::Gui

#endif // LOGIN_H
