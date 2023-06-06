#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>

namespace Ui {
class Signup;
}

namespace Client::Gui {

class Signup : public QDialog
{
    Q_OBJECT

public:
    Signup(QWidget *parent = nullptr);
    ~Signup();

signals:

    void ready(QMap<QString, QVariant> user);

    void alreadyHasAccount();

private slots:

    void signupButtonClicked();

private:
    Ui::Signup *ui;
};

} // namespace Client::Gui

#endif // SIGNUP_H
